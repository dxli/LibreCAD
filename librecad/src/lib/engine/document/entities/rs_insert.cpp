/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "rs_insert.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

#include "rs_arc.h"
#include "rs_block.h"
#include "rs_circle.h"
#include "rs_color.h"
#include "rs_debug.h"
#include "rs_ellipse.h"
#include "rs_graphic.h"
#include "rs_layer.h"
#include "rs_math.h"
#include "rs_pen.h"

namespace {

// A 2D affine map used solely while expanding INSERTs.  The source fields are
// the BLOCK base point, INSERT insertion/scale/rotation, MINSERT spacing, and
// the only two OCS normals representable by LibreCAD's 2D entity model (+Z/-Z).
// General affine shear and non-planar OCS are rejected before an expansion is
// committed; approximating either would silently change drawing geometry.
struct LC_InsertTransform {
    double a = 1.0;
    double b = 0.0;
    double c = 0.0;
    double d = 1.0;
    double tx = 0.0;
    double ty = 0.0;

    RS_Vector mapPoint(const RS_Vector& point) const {
        return RS_Vector(a * point.x + c * point.y + tx,
                         b * point.x + d * point.y + ty, point.z);
    }

    static LC_InsertTransform compose(const LC_InsertTransform& parent,
                                      const LC_InsertTransform& child) {
        LC_InsertTransform result;
        result.a = parent.a * child.a + parent.c * child.b;
        result.b = parent.b * child.a + parent.d * child.b;
        result.c = parent.a * child.c + parent.c * child.d;
        result.d = parent.b * child.c + parent.d * child.d;
        result.tx = parent.a * child.tx + parent.c * child.ty + parent.tx;
        result.ty = parent.b * child.tx + parent.d * child.ty + parent.ty;
        return result;
    }

    static bool fromInsert(const RS_InsertData& data, const RS_Vector& base,
                           int column, int row, LC_InsertTransform& result) {
        const double sx = data.scaleFactor.x;
        const double sy = data.scaleFactor.y;
        if (!std::isfinite(sx) || !std::isfinite(sy) || sx == 0.0 || sy == 0.0
            || !std::isfinite(data.angle) || !data.insertionPoint.valid
            || !base.valid) {
            return false;
        }
        // DXF INSERT is OCS.  A 2D document can faithfully represent only the
        // two axial normals.  The -Z arbitrary-axis basis maps local X to -X.
        if (!std::isfinite(data.extrusion.x) || !std::isfinite(data.extrusion.y)
            || !std::isfinite(data.extrusion.z) || data.extrusion.x != 0.0
            || data.extrusion.y != 0.0 || data.extrusion.z == 0.0) {
            return false;
        }
        const double cosAngle = std::cos(data.angle);
        const double sinAngle = std::sin(data.angle);
        result.a = cosAngle * sx;
        result.b = sinAngle * sx;
        result.c = -sinAngle * sy;
        result.d = cosAngle * sy;
        if (data.extrusion.z < 0.0) {
            result.a = -result.a;
            result.b = -result.b;
        }
        const RS_Vector arrayOffset(data.spacing.x * column,
                                    data.spacing.y * row);
        result.tx = data.insertionPoint.x + cosAngle * arrayOffset.x
                    - sinAngle * arrayOffset.y - result.a * base.x
                    - result.c * base.y;
        result.ty = data.insertionPoint.y + sinAngle * arrayOffset.x
                    + cosAngle * arrayOffset.y - result.b * base.x
                    - result.d * base.y;
        return std::isfinite(result.tx) && std::isfinite(result.ty);
    }

    // This decomposition is the capability boundary.  Native LibreCAD entity
    // transforms support translation, rotation, scaling and reflection, but
    // not a general affine shear.  The epsilon only compares computed values;
    // it never selects geometry based on drawing-unit magnitude.
    bool decompose(double& sx, double& sy, double& angle) const {
        const double col0 = std::hypot(a, b);
        const double col1 = std::hypot(c, d);
        const double dot = a * c + b * d;
        const double tolerance = std::numeric_limits<double>::epsilon()
                                 * 64.0 * std::max(1.0, col0 * col1);
        if (!std::isfinite(col0) || !std::isfinite(col1) || col0 == 0.0
            || col1 == 0.0 || std::abs(dot) > tolerance) {
            return false;
        }
        sx = col0;
        sy = (a * d - b * c) / col0;
        angle = std::atan2(b, a);
        return std::isfinite(sy) && sy != 0.0 && std::isfinite(angle);
    }
};

enum class InsertTransformCapability {
    NativeOrthogonal,
    CircleToEllipse,
    ArcToEllipse,
    NestedInsert
};

InsertTransformCapability transformCapability(const RS_Entity& entity) {
    switch (entity.rtti()) {
    case RS2::EntityCircle:
        return InsertTransformCapability::CircleToEllipse;
    case RS2::EntityArc:
        return InsertTransformCapability::ArcToEllipse;
    case RS2::EntityInsert:
        return InsertTransformCapability::NestedInsert;
    default:
        // Lines, polylines, splines, ellipses, hatches, text/attributes,
        // dimensions, images and WIPEOUTs implement the native orthogonal
        // transform API.  A sheared map is rejected by decompose() above.
        return InsertTransformCapability::NativeOrthogonal;
    }
}

bool isUniformScale(double sx, double sy) {
    const double tolerance = std::numeric_limits<double>::epsilon() * 64.0
                             * std::max({1.0, std::abs(sx), std::abs(sy)});
    return std::abs(std::abs(sx) - std::abs(sy)) <= tolerance;
}

std::unique_ptr<RS_Entity> cloneForTransform(const RS_Entity& source,
                                              double sx, double sy) {
    switch (transformCapability(source)) {
    case InsertTransformCapability::CircleToEllipse:
        if (!isUniformScale(sx, sy)) {
            const auto& circle = static_cast<const RS_Circle&>(source);
            return std::make_unique<RS_Ellipse>(nullptr,
                RS_EllipseData{circle.getCenter(), RS_Vector(circle.getRadius(), 0.0),
                               1.0, 0.0, 2.0 * M_PI, false});
        }
        break;
    case InsertTransformCapability::ArcToEllipse:
        if (!isUniformScale(sx, sy)) {
            const auto& arc = static_cast<const RS_Arc&>(source);
            return std::make_unique<RS_Ellipse>(nullptr,
                RS_EllipseData{arc.getCenter(), RS_Vector(arc.getRadius(), 0.0),
                               1.0, arc.getAngle1(), arc.getAngle2(), arc.isReversed()});
        }
        break;
    case InsertTransformCapability::NestedInsert:
        return nullptr;
    case InsertTransformCapability::NativeOrthogonal:
        break;
    }
    return std::unique_ptr<RS_Entity>(source.clone());
}

bool applyTransform(RS_Entity& entity, const LC_InsertTransform& transform) {
    double sx = 0.0;
    double sy = 0.0;
    double angle = 0.0;
    if (!transform.decompose(sx, sy, angle))
        return false;
    entity.setUpdateEnabled(false);
    entity.scale(RS_Vector(0.0, 0.0), RS_Vector(sx, sy));
    entity.rotate(RS_Vector(0.0, 0.0), angle);
    entity.move(RS_Vector(transform.tx, transform.ty));
    entity.setUpdateEnabled(true);
    return true;
}

// update the entity pen according to the blockPen
RS_Pen updatePen(RS_Pen&& pen, const RS_Pen& blockPen) {
    // color from block (free floating):
    if (pen.getColor() == RS_Color(RS2::FlagByBlock)) {
        pen.setColor(blockPen.getColor());
    }

    // line width from block (free floating):
    if (pen.getWidth() == RS2::WidthByBlock) {
        pen.setWidth(blockPen.getWidth());
    }

    // line type from block (free floating):
    if (pen.getLineType() == RS2::LineByBlock) {
        pen.setLineType(blockPen.getLineType());
    }

    return pen;
}

}
RS_InsertData::RS_InsertData(const QString& _name,
							 RS_Vector _insertionPoint,
							 RS_Vector _scaleFactor,
							 double _angle,
							 int _cols, int _rows, RS_Vector _spacing,
							 RS_BlockList* _blockSource ,
							 RS2::UpdateMode _updateMode ):
	name(_name)
  ,insertionPoint(_insertionPoint)
  ,scaleFactor(_scaleFactor)
  ,angle(_angle)
  ,cols(_cols)
  ,rows(_rows)
  ,spacing(_spacing)
  ,blockSource(_blockSource)
  ,updateMode(_updateMode){
}

RS_InsertData::RS_InsertData(const RS_InsertData &other):
   name(other.name)
  ,insertionPoint(other.insertionPoint)
  ,scaleFactor(other.scaleFactor)
  ,extrusion(other.extrusion)
  ,angle(other.angle)
  ,cols(other.cols)
  ,rows(other.rows)
  ,spacing(other.spacing)
  ,blockSource(other.blockSource)
  ,updateMode(other.updateMode){
}

std::ostream& operator <<(std::ostream& os,
                          const RS_InsertData& d) {
    os << "(" << d.name.toLatin1().data() << ")";
    return os;
}

/**
 * @param parent The graphic this m_block belongs to.
 */
RS_Insert::RS_Insert(RS_EntityContainer* parent,
                     const RS_InsertData& d)
    : RS_EntityContainer(parent)
      , m_data(d) {
    if (m_data.updateMode != RS2::NoUpdate) {
        RS_Insert::update();
        //calculateBorders();
    }
}

RS_Entity* RS_Insert::clone() const{
	auto i = new RS_Insert(*this);
	i->setOwner(isOwner());
	i->detach();
	return i;
}

/**
 * Updates the entity buffer of this insert entity. This method
 * needs to be called whenever the block this insert is based on changes.
 */
void RS_Insert::calculateBorders() {
    RS_EntityContainer::calculateBorders();
    if (count() == 0) {
        m_minV = RS_Vector(false);
        m_maxV = RS_Vector(false);
    }
}

void RS_Insert::update() {
    RS_DEBUG->print("RS_Insert::update");
    RS_DEBUG->print("RS_Insert::update: name: %s", m_data.name.toLatin1().data());
    if (!m_updateEnabled) {
        return;
    }
    RS_Block* blk = getBlockForInsert();
    if (blk == nullptr) {
        RS_DEBUG->print("RS_Insert::update: Block is nullptr");
        clear();
        return;
    }
    if (isDeleted()) {
        RS_DEBUG->print("RS_Insert::update: Insert is in undo list");
        clear();
        return;
    }
    const bool fontLetterInsert = blk->rtti() == RS2::EntityFontChar
                                  || m_data.blockSource != nullptr;
    RS_Pen expansionPen = fontLetterInsert ? getPen(false) : getPen(true);
    if (fontLetterInsert && !expansionPen.isValid() && m_parent != nullptr)
        expansionPen = m_parent->getPen(false);

    std::vector<std::unique_ptr<RS_Entity>> expanded;
    std::vector<const RS_Block*> activeBlocks;
    const auto appendLeaf = [&](const RS_Entity& source,
                                const LC_InsertTransform& transform) -> bool {
        double sx = 0.0;
        double sy = 0.0;
        double angle = 0.0;
        if (!transform.decompose(sx, sy, angle))
            return false;
        auto clone = cloneForTransform(source, sx, sy);
        if (!clone || !applyTransform(*clone, transform))
            return false;
        RS_Layer* layer = clone->getLayer();
        if (layerNameEquals(layer, QStringLiteral("0")))
            clone->setLayer(getLayer());
        else if (layer != nullptr && validatedLayer(layer) == nullptr)
            clone->setLayer(nullptr);
        clone->setParent(this);
        clone->setVisible(getFlag(RS2::FlagVisible));
        clone->setSelectionFlag(false);
        clone->setPen(updatePen(clone->getPen(false), expansionPen));
        if (m_data.updateMode != RS2::PreviewUpdate)
            clone->update();
        expanded.push_back(std::move(clone));
        return true;
    };

    const auto expandBlock = [&](auto&& self, const RS_Block& sourceBlock,
                                 const LC_InsertTransform& transform) -> bool {
        if (std::find(activeBlocks.cbegin(), activeBlocks.cend(), &sourceBlock)
            != activeBlocks.cend()) {
            RS_DEBUG->print(RS_Debug::D_ERROR,
                            "RS_Insert::update: recursive block reference rejected");
            return false;
        }
        activeBlocks.push_back(&sourceBlock);
        for (const RS_Entity* source : sourceBlock) {
            if (source == nullptr || source->isDeleted())
                continue;
            if (source->rtti() == RS2::EntityInsert) {
                const auto& nested = static_cast<const RS_Insert&>(*source);
                RS_Block* nestedBlock = nested.getBlockForInsert();
                if (nestedBlock == nullptr) {
                    activeBlocks.pop_back();
                    return false;
                }
                const RS_InsertData nestedData = nested.getData();
                for (int column = 0; column < nestedData.cols; ++column) {
                    for (int row = 0; row < nestedData.rows; ++row) {
                        LC_InsertTransform child;
                        if (!LC_InsertTransform::fromInsert(nestedData,
                                                            nestedBlock->getBasePoint(),
                                                            column, row, child)
                            || !self(self, *nestedBlock,
                                     LC_InsertTransform::compose(transform, child))) {
                            activeBlocks.pop_back();
                            return false;
                        }
                    }
                }
                continue;
            }
            if (!appendLeaf(*source, transform)) {
                activeBlocks.pop_back();
                return false;
            }
        }
        activeBlocks.pop_back();
        return true;
    };

    bool success = true;
    for (int column = 0; success && column < m_data.cols; ++column) {
        for (int row = 0; success && row < m_data.rows; ++row) {
            LC_InsertTransform transform;
            success = LC_InsertTransform::fromInsert(m_data, blk->getBasePoint(),
                                                      column, row, transform)
                      && expandBlock(expandBlock, *blk, transform);
        }
    }

    // Commit only a fully expanded tree.  Failed cycles, unsupported non-planar
    // OCS, and nonrepresentable shear leave this INSERT empty, never partial.
    clear();
    if (!success) {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "RS_Insert::update: expansion rejected without partial children");
        calculateBorders();
        return;
    }
    const bool oldAutoUpdateBorders = getAutoUpdateBorders();
    setAutoUpdateBorders(false);
    for (auto& entity : expanded)
        appendEntity(entity.release());
    setAutoUpdateBorders(oldAutoUpdateBorders);
    calculateBorders();
    RS_DEBUG->print("RS_Insert::update: OK");
}

/**
 * @return Pointer to the m_block associated with this Insert or
 *   nullptr if the m_block couldn't be found. Blocks are requested
 *   from the blockSource if one was supplied and otherwise from
 *   the closest parent graphic.
 */
RS_Block* RS_Insert::getBlockForInsert() const{
    if (m_block != nullptr) {
        return m_block;
    }

    RS_BlockList* blkList = nullptr;

    if (!m_data.blockSource) {
        if (getGraphic()) {
            blkList = getGraphic()->getBlockList();
        }
    } else {
        blkList = m_data.blockSource;
    }

    RS_Block* blk = nullptr;
    if (blkList != nullptr) {
        blk = blkList->find(m_data.name);
    }

    m_block = blk;

    return blk;
}

/**
 * Is this insert visible? (re-implementation from RS_Entity)
 *
 * @return true Only if the entity and the block and the layer it is on
 * are visible.
 * The Layer might also be nullptr. In that case the layer visibility
 * is ignored.
 * The Block might also be nullptr. In that case the block visibility
 * is ignored.
 */
bool RS_Insert::isVisible() const{
    RS_Block* blk = getBlockForInsert();
    if (blk != nullptr) {
        if (blk->isFrozen()) {
            return false;
        }
    }

    return RS_Entity::isVisible();
}

RS_VectorSolutions RS_Insert::getRefPoints() const{
    return RS_VectorSolutions{m_data.insertionPoint};
}

RS_Vector RS_Insert::doGetNearestRef(const RS_Vector& coord, double* dist) const {
    return getRefPoints().getClosest(coord, dist);
}

void RS_Insert::move(const RS_Vector& offset) {
    RS_DEBUG->print("RS_Insert::move: offset: %f/%f", offset.x, offset.y);
    RS_DEBUG->print("RS_Insert::move1: insertionPoint: %f/%f", m_data.insertionPoint.x, m_data.insertionPoint.y);
    m_data.insertionPoint.move(offset);
    RS_DEBUG->print("RS_Insert::move2: insertionPoint: %f/%f", m_data.insertionPoint.x, m_data.insertionPoint.y);
    update();
}

void RS_Insert::rotate(const RS_Vector& center, double angle) {
    RS_DEBUG->print("RS_Insert::rotate1: insertionPoint: %f/%f / center: %f/%f",
                    m_data.insertionPoint.x, m_data.insertionPoint.y,
                    center.x, center.y);
    m_data.insertionPoint.rotate(center, angle);
    m_data.angle = RS_Math::correctAngle(m_data.angle + angle);
    RS_DEBUG->print("RS_Insert::rotate2: insertionPoint: %f/%f", m_data.insertionPoint.x, m_data.insertionPoint.y);
    update();
}

void RS_Insert::rotate(const RS_Vector& center, const RS_Vector& angleVector) {
    RS_DEBUG->print("RS_Insert::rotate1: insertionPoint: %f/%f "
                    "/ center: %f/%f",
                    m_data.insertionPoint.x, m_data.insertionPoint.y,
                    center.x, center.y);
    m_data.insertionPoint.rotate(center, angleVector);
    m_data.angle = RS_Math::correctAngle(m_data.angle + angleVector.angle());
    RS_DEBUG->print("RS_Insert::rotate2: insertionPoint: %f/%f",
                    m_data.insertionPoint.x, m_data.insertionPoint.y);
    update();
}

void RS_Insert::scale(const RS_Vector& center, const RS_Vector& factor) {
    RS_DEBUG->print("RS_Insert::scale1: insertionPoint: %f/%f",
                    m_data.insertionPoint.x, m_data.insertionPoint.y);
    m_data.insertionPoint.scale(center, factor);
    m_data.scaleFactor.scale(RS_Vector(0.0, 0.0), factor);
    m_data.spacing.scale(RS_Vector(0.0, 0.0), factor);
    RS_DEBUG->print("RS_Insert::scale2: insertionPoint: %f/%f",
                    m_data.insertionPoint.x, m_data.insertionPoint.y);
    update();

}

void RS_Insert::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    m_data.insertionPoint.mirror(axisPoint1, axisPoint2);
    RS_Vector vec = RS_Vector::polar(1.0, m_data.angle);
    vec.mirror(RS_Vector(0.0, 0.0), axisPoint2 - axisPoint1);
    m_data.angle = RS_Math::correctAngle(vec.angle() - M_PI);
    m_data.scaleFactor.x *= -1;
    update();
}

std::ostream& operator << (std::ostream& os, const RS_Insert& i) {
    os << " Insert: " << i.getData() << std::endl;
    return os;
}
