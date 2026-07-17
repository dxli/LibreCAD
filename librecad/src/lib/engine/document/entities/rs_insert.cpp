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

constexpr double kIdentityScale = 1.0;
constexpr double kZero = 0.0;
constexpr double kNegativeIdentityScale = -kIdentityScale;
const double kHalfTurnRadians = std::acos(kNegativeIdentityScale);
const double kFullTurnRadians = kHalfTurnRadians + kHalfTurnRadians;

const RS_Vector& transformOrigin() {
    static const RS_Vector origin(kZero, kZero);
    return origin;
}

const QString& layerZeroName() {
    static const QString name = QStringLiteral("0");
    return name;
}

enum class LC_InsertTransformStatus {
    Ok,
    InvalidFields,
    UnsupportedNormal,
    NonFiniteResult
};

const char* insertTransformStatusName(LC_InsertTransformStatus status) {
    switch (status) {
    case LC_InsertTransformStatus::Ok:
        return "ok";
    case LC_InsertTransformStatus::InvalidFields:
        return "invalid fields";
    case LC_InsertTransformStatus::UnsupportedNormal:
        return "unsupported normal";
    case LC_InsertTransformStatus::NonFiniteResult:
        return "non-finite result";
    }
    return "unknown status";
}

// A 2D affine map used solely while expanding INSERTs.  The source fields are
// the BLOCK base point, INSERT insertion/scale/rotation, MINSERT spacing, and
// the only two OCS normals representable by LibreCAD's 2D entity model (+Z/-Z).
// General affine shear and non-planar OCS are rejected before an expansion is
// committed; approximating either would silently change drawing geometry.
struct LC_InsertTransform {
    double a = kIdentityScale;
    double b = kZero;
    double c = kZero;
    double d = kIdentityScale;
    double tx = kZero;
    double ty = kZero;

    bool isFinite() const {
        return std::isfinite(a) && std::isfinite(b) && std::isfinite(c)
               && std::isfinite(d) && std::isfinite(tx) && std::isfinite(ty);
    }

    RS_Vector mapVector(const RS_Vector& vector) const {
        return RS_Vector(a * vector.x + c * vector.y,
                         b * vector.x + d * vector.y, vector.z);
    }

    RS_Vector mapLeafPoint(const RS_Vector& point) const {
        RS_Vector result = mapVector(point);
        result.move(RS_Vector(tx, ty));
        return result;
    }

    static bool compose(const LC_InsertTransform& parent,
                        const LC_InsertTransform& child,
                        LC_InsertTransform& result) {
        result.a = parent.a * child.a + parent.c * child.b;
        result.b = parent.b * child.a + parent.d * child.b;
        result.c = parent.a * child.c + parent.c * child.d;
        result.d = parent.b * child.c + parent.d * child.d;
        result.tx = parent.a * child.tx + parent.c * child.ty + parent.tx;
        result.ty = parent.b * child.tx + parent.d * child.ty + parent.ty;
        return result.isFinite();
    }

    static RS_Vector mapArrayOffset(const RS_Vector& spacing, int column, int row,
                                    double cosAngle, double sinAngle,
                                    double ocsXAxis) {
        const RS_Vector arrayOffset(spacing.x * column, spacing.y * row);
        // MINSERT spacing is rotated into OCS but is not affected by INSERT
        // scale.  The base point is part of the scaled block-local geometry.
        return RS_Vector(ocsXAxis * (cosAngle * arrayOffset.x
                                     - sinAngle * arrayOffset.y),
                         sinAngle * arrayOffset.x + cosAngle * arrayOffset.y);
    }

    static LC_InsertTransformStatus fromInsert(const RS_InsertData& data,
                                                const RS_Vector& base, int column,
                                                int row, LC_InsertTransform& result) {
        const double sx = data.scaleFactor.x;
        const double sy = data.scaleFactor.y;
        if (column < 0 || row < 0 || !std::isfinite(sx) || !std::isfinite(sy)
            || sx == kZero || sy == kZero || !std::isfinite(data.angle)
            || !data.insertionPoint.valid || !base.valid
            || !std::isfinite(data.insertionPoint.x) || !std::isfinite(data.insertionPoint.y)
            || !std::isfinite(base.x) || !std::isfinite(base.y)
            || !std::isfinite(data.spacing.x) || !std::isfinite(data.spacing.y)) {
            return LC_InsertTransformStatus::InvalidFields;
        }
        // DXF INSERT is OCS.  A 2D document can faithfully represent only the
        // two axial normals.  The -Z arbitrary-axis basis maps local X to -X.
        if (!std::isfinite(data.extrusion.x) || !std::isfinite(data.extrusion.y)
            || !std::isfinite(data.extrusion.z) || data.extrusion.x != kZero
            || data.extrusion.y != kZero || data.extrusion.z == kZero) {
            return LC_InsertTransformStatus::UnsupportedNormal;
        }
        const double cosAngle = std::cos(data.angle);
        const double sinAngle = std::sin(data.angle);
        const double ocsXAxis = data.extrusion.z < kZero ? kNegativeIdentityScale
                                                        : kIdentityScale;
        // INSERT points are in OCS.  For the two axial normals a 2D engine can
        // represent, the arbitrary-axis frame is F(+Z)=I and F(-Z)=diag(-1,1).
        // The columns below are F * rotation(angle) * scale(x,y).
        result.a = ocsXAxis * cosAngle * sx;
        result.b = sinAngle * sx;
        result.c = -ocsXAxis * sinAngle * sy;
        result.d = cosAngle * sy;
        const RS_Vector rotatedArrayOffset = mapArrayOffset(data.spacing, column, row,
                                                             cosAngle, sinAngle, ocsXAxis);
        const RS_Vector mappedBase = result.mapVector(base);
        result.tx = ocsXAxis * data.insertionPoint.x + rotatedArrayOffset.x
                    - mappedBase.x;
        result.ty = data.insertionPoint.y + rotatedArrayOffset.y - mappedBase.y;
        if (!result.isFinite() || !std::isfinite(rotatedArrayOffset.x)
            || !std::isfinite(rotatedArrayOffset.y)) {
            return LC_InsertTransformStatus::NonFiniteResult;
        }
        return LC_InsertTransformStatus::Ok;
    }

    // This decomposition is the capability boundary.  Native LibreCAD entity
    // transforms support translation, rotation, scaling and reflection, but
    // not a general affine shear.  The epsilon only compares computed values;
    // it never selects geometry based on drawing-unit magnitude.
    bool decompose(double& sx, double& sy, double& angle) const {
        const double col0 = std::hypot(a, b);
        const double col1 = std::hypot(c, d);
        const double dot = a * c + b * d;
        // The dot product is the sum of the two products below. Its rounding
        // error scales with those operands, not with a drawing-unit constant.
        const double tolerance = std::numeric_limits<double>::epsilon()
                                 * (std::abs(a * c) + std::abs(b * d));
        if (!std::isfinite(col0) || !std::isfinite(col1) || col0 == kZero
            || col1 == kZero || std::abs(dot) > tolerance) {
            return false;
        }
        sx = col0;
        sy = (a * d - b * c) / col0;
        angle = std::atan2(b, a);
        return std::isfinite(sy) && sy != kZero && std::isfinite(angle);
    }
};

enum class InsertTransformCapability {
    NativeOrthogonal,
    CircleToEllipse,
    ArcToEllipse,
    NestedInsert,
    Unsupported
};

InsertTransformCapability transformCapability(const RS_Entity& entity) {
    switch (entity.rtti()) {
    case RS2::EntityCircle:
        return InsertTransformCapability::CircleToEllipse;
    case RS2::EntityArc:
        return InsertTransformCapability::ArcToEllipse;
    case RS2::EntityInsert:
        return InsertTransformCapability::NestedInsert;
    // These entity families currently own a two-dimensional scale/rotate/move
    // implementation. Each remains explicitly listed so a new EntityType is
    // rejected until its INSERT-transform semantics are reviewed.
    case RS2::EntityPoint:
    case RS2::EntityLine:
    case RS2::EntityPolyline:
    case RS2::EntityEllipse:
    case RS2::EntityHyperbola:
    case RS2::EntitySolid:
    case RS2::EntityConstructionLine:
    case RS2::EntityMText:
    case RS2::EntityText:
    case RS2::EntityDimAligned:
    case RS2::EntityDimLinear:
    case RS2::EntityDimRadial:
    case RS2::EntityDimDiametric:
    case RS2::EntityDimAngular:
    case RS2::EntityDimArc:
    case RS2::EntityDimOrdinate:
    case RS2::EntityTolerance:
    case RS2::EntityDimLeader:
    case RS2::EntityHatch:
    case RS2::EntityImage:
    case RS2::EntityWipeout:
    case RS2::EntityMLeader:
    case RS2::EntitySpline:
    case RS2::EntitySplinePoints:
    case RS2::EntityParabola:
        return InsertTransformCapability::NativeOrthogonal;
    case RS2::EntityUnknown:
    case RS2::EntityContainer:
    case RS2::EntityBlock:
    case RS2::EntityFontChar:
    case RS2::EntityGraphic:
    case RS2::EntityVertex:
    case RS2::EntityOverlayBox:
    case RS2::EntityPreview:
    case RS2::EntityPattern:
    case RS2::EntityOverlayLine:
    case RS2::EntityRefPoint:
    case RS2::EntityRefLine:
    case RS2::EntityRefConstructionLine:
    case RS2::EntityRefArc:
    case RS2::EntityRefCircle:
    case RS2::EntityRefEllipse:
    case RS2::EntitySnapMark:
    case RS2::EntitySnapLine:
    case RS2::EntitySnapArc:
    case RS2::EntitySnapCircle:
    case RS2::EntitySnapConstructionLine:
    case RS2::EntityDimArrowBlock:
        return InsertTransformCapability::Unsupported;
    }
    return InsertTransformCapability::Unsupported;
}

bool isUniformScale(double sx, double sy) {
    const double tolerance = std::numeric_limits<double>::epsilon()
                             * std::max({kIdentityScale, std::abs(sx), std::abs(sy)});
    return std::abs(std::abs(sx) - std::abs(sy)) <= tolerance;
}

std::unique_ptr<RS_Entity> cloneForTransform(const RS_Entity& source,
                                              double sx, double sy) {
    switch (transformCapability(source)) {
    case InsertTransformCapability::CircleToEllipse:
        if (!isUniformScale(sx, sy)) {
            const auto& circle = static_cast<const RS_Circle&>(source);
            return std::make_unique<RS_Ellipse>(nullptr,
                RS_EllipseData{circle.getCenter(), RS_Vector(circle.getRadius(), kZero),
                               kIdentityScale, kZero, kFullTurnRadians, false});
        }
        break;
    case InsertTransformCapability::ArcToEllipse:
        if (!isUniformScale(sx, sy)) {
            const auto& arc = static_cast<const RS_Arc&>(source);
            return std::make_unique<RS_Ellipse>(nullptr,
                RS_EllipseData{arc.getCenter(), RS_Vector(arc.getRadius(), kZero),
                               kIdentityScale, arc.getAngle1(), arc.getAngle2(),
                               arc.isReversed()});
        }
        break;
    case InsertTransformCapability::NestedInsert:
    case InsertTransformCapability::Unsupported:
        return nullptr;
    case InsertTransformCapability::NativeOrthogonal:
        break;
    }
    return std::unique_ptr<RS_Entity>(source.clone());
}

void copyExpansionProperties(const RS_Entity& source, RS_Entity& target) {
    target.setPen(source.getPen(false));
    target.setLayer(source.getLayer());
    target.setVisible(source.getFlag(RS2::FlagVisible));
}

bool applyTransform(RS_Entity& entity, const LC_InsertTransform& transform) {
    double sx = kZero;
    double sy = kZero;
    double angle = kZero;
    if (!transform.decompose(sx, sy, angle))
        return false;
    const bool reversesOrientation = sy < kZero;
    entity.setUpdateEnabled(false);
    // The decomposition carries the determinant sign in sy.  Route that
    // reflection through the entity's mirror contract rather than hoping each
    // scale() implementation updates directional state (arc sweeps, hatch
    // angles, text alignment, image frames, and similar metadata) for a
    // negative component scale.
    entity.scale(transformOrigin(), RS_Vector(sx, std::abs(sy)));
    if (reversesOrientation)
        entity.mirror(transformOrigin(), RS_Vector(kIdentityScale, kZero));
    entity.rotate(transformOrigin(), angle);
    entity.move(RS_Vector(transform.tx, transform.ty));
    entity.setUpdateEnabled(true);
    return true;
}

RS_Vector scaleComponents(const RS_Vector& vector, const RS_Vector& factor) {
    // INSERT expansion is 2D. Match RS_Vector::scale's established contract:
    // preserve Z metadata when callers provide the usual two-component factor.
    return RS_Vector(vector.x * factor.x, vector.y * factor.y, vector.z);
}

// update the entity pen according to the blockPen
RS_Pen updatePen(RS_Pen pen, const RS_Pen& blockPen) {
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

enum class LC_InsertExpansionWorkKind {
    ExpandArrayCell,
    VisitBlock,
    ProcessEntity
};

// The stack models recursive INSERT expansion without consuming the C++ call
// stack.  A VisitBlock item resumes at one source entity at a time, keeping
// auxiliary memory proportional to nesting rather than block entity count.
struct LC_InsertExpansionWork {
    LC_InsertExpansionWorkKind kind = LC_InsertExpansionWorkKind::ExpandArrayCell;
    const RS_Block* block = nullptr;
    const RS_Entity* entity = nullptr;
    LC_InsertTransform transform;
    RS_Pen blockPen;
    RS_Layer* inheritedLayer = nullptr;
    bool inheritedVisibility = true;
    RS_InsertData insertData;
    int column = 0;
    int row = 0;
    std::size_t entityIndex = 0U;
};

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
    update(RS_InsertExpansionBudget{});
}

void RS_Insert::update(const RS_InsertExpansionBudget& budget) {
    RS_DEBUG->print("RS_Insert::update");
    RS_DEBUG->print("RS_Insert::update: name: %s", m_data.name.toLatin1().data());
    if (!m_updateEnabled) {
        return;
    }
    RS_Block* blk = getBlockForInsert();
    if (blk == nullptr) {
        RS_DEBUG->print("RS_Insert::update: Block is nullptr");
        clear();
        calculateBorders();
        return;
    }
    if (isDeleted()) {
        RS_DEBUG->print("RS_Insert::update: Insert is in undo list");
        clear();
        calculateBorders();
        return;
    }
    const bool fontLetterInsert = blk->rtti() == RS2::EntityFontChar
                                  || m_data.blockSource != nullptr;
    RS_Pen expansionPen = fontLetterInsert ? getPen(false) : getPen(true);
    if (fontLetterInsert && !expansionPen.isValid() && m_parent != nullptr)
        expansionPen = m_parent->getPen(false);

    std::vector<std::unique_ptr<RS_Entity>> expanded;
    std::vector<const RS_Block*> activeBlocks;
    std::vector<LC_InsertExpansionWork> work;
    const auto arrayFitsBudget = [&budget](const RS_InsertData& data) {
        if (data.cols <= 0 || data.rows <= 0)
            return false;
        const auto columns = static_cast<std::size_t>(data.cols);
        const auto rows = static_cast<std::size_t>(data.rows);
        // Check before multiplication so an untrusted MINSERT grid cannot
        // overflow and bypass the traversal limit.
        return columns <= budget.maxArrayInstances / rows;
    };
    const auto appendLeaf = [&](const RS_Entity& source,
                                const LC_InsertTransform& transform,
                                const RS_Pen& blockPen,
                                RS_Layer* inheritedLayer,
                                bool inheritedVisibility) -> bool {
        if (expanded.size() >= budget.maxDerivedEntities)
            return false;
        double sx = kZero;
        double sy = kZero;
        double angle = kZero;
        if (!transform.decompose(sx, sy, angle))
            return false;
        auto clone = cloneForTransform(source, sx, sy);
        if (clone)
            copyExpansionProperties(source, *clone);
        if (!clone || !applyTransform(*clone, transform))
            return false;
        RS_Layer* layer = clone->getLayer();
        if (layerNameEquals(layer, layerZeroName()))
            clone->setLayer(inheritedLayer);
        else if (layer != nullptr && validatedLayer(layer) == nullptr)
            clone->setLayer(nullptr);
        clone->setParent(this);
        clone->setVisible(source.getFlag(RS2::FlagVisible)
                          && inheritedVisibility);
        clone->setSelectionFlag(false);
        clone->setPen(updatePen(clone->getPen(false), blockPen));
        if (m_data.updateMode != RS2::PreviewUpdate)
            clone->update();
        expanded.push_back(std::move(clone));
        return true;
    };

    const auto queueArrayCell = [&](const RS_Block& sourceBlock,
                                    const RS_InsertData& data,
                                    const LC_InsertTransform& parentTransform,
                                    const RS_Pen& blockPen,
                                    RS_Layer* inheritedLayer,
                                    bool inheritedVisibility,
                                    int column, int row) {
        LC_InsertExpansionWork item;
        item.kind = LC_InsertExpansionWorkKind::ExpandArrayCell;
        item.block = &sourceBlock;
        item.transform = parentTransform;
        item.blockPen = blockPen;
        item.inheritedLayer = inheritedLayer;
        item.inheritedVisibility = inheritedVisibility;
        item.insertData = data;
        item.column = column;
        item.row = row;
        work.push_back(std::move(item));
    };

    std::size_t arrayInstances = 0U;
    bool success = budget.isValid() && arrayFitsBudget(m_data);
    if (success) {
        queueArrayCell(*blk, m_data, LC_InsertTransform{}, expansionPen, getLayer(),
                       getFlag(RS2::FlagVisible) && !blk->isFrozen(), 0, 0);
    }
    while (success && !work.empty()) {
        LC_InsertExpansionWork item = std::move(work.back());
        work.pop_back();

        switch (item.kind) {
        case LC_InsertExpansionWorkKind::ExpandArrayCell: {
            LC_InsertTransform child;
            if (item.block == nullptr || !arrayFitsBudget(item.insertData)
                || item.column < 0 || item.row < 0
                || item.column >= item.insertData.cols || item.row >= item.insertData.rows) {
                success = false;
                break;
            }
            if (arrayInstances >= budget.maxArrayInstances) {
                RS_DEBUG->print(RS_Debug::D_ERROR,
                                "RS_Insert::update: array instance limit exceeded");
                success = false;
                break;
            }
            ++arrayInstances;
            const LC_InsertTransformStatus transformStatus =
                LC_InsertTransform::fromInsert(item.insertData, item.block->getBasePoint(),
                                                item.column, item.row, child);
            if (transformStatus != LC_InsertTransformStatus::Ok) {
                RS_DEBUG->print(RS_Debug::D_ERROR,
                                "RS_Insert::update: transform rejected: %s",
                                insertTransformStatusName(transformStatus));
                success = false;
                break;
            }

            int nextColumn = item.column;
            int nextRow = item.row + 1;
            if (nextRow >= item.insertData.rows) {
                nextRow = 0;
                ++nextColumn;
            }
            if (nextColumn < item.insertData.cols) {
                queueArrayCell(*item.block, item.insertData, item.transform,
                               item.blockPen, item.inheritedLayer,
                               item.inheritedVisibility, nextColumn, nextRow);
            }

            LC_InsertExpansionWork enterBlock;
            enterBlock.kind = LC_InsertExpansionWorkKind::VisitBlock;
            enterBlock.block = item.block;
            if (!LC_InsertTransform::compose(item.transform, child,
                                              enterBlock.transform)) {
                RS_DEBUG->print(RS_Debug::D_ERROR,
                                "RS_Insert::update: transform composition overflow");
                success = false;
                break;
            }
            enterBlock.blockPen = item.blockPen;
            enterBlock.inheritedLayer = item.inheritedLayer;
            enterBlock.inheritedVisibility = item.inheritedVisibility;
            work.push_back(std::move(enterBlock));
            break;
        }
        case LC_InsertExpansionWorkKind::VisitBlock: {
            if (item.block == nullptr) {
                success = false;
                break;
            }
            if (item.entityIndex == 0) {
                if (std::find(activeBlocks.cbegin(), activeBlocks.cend(), item.block)
                    != activeBlocks.cend()) {
                    RS_DEBUG->print(RS_Debug::D_ERROR,
                                    "RS_Insert::update: recursive block reference rejected");
                    success = false;
                    break;
                }
                if (activeBlocks.size() >= budget.maxNestingDepth) {
                    RS_DEBUG->print(RS_Debug::D_ERROR,
                                    "RS_Insert::update: nesting depth exceeds expansion limit");
                    success = false;
                    break;
                }
                activeBlocks.push_back(item.block);
            }
            const std::size_t entityCount = item.block->count();
            if (item.entityIndex >= entityCount) {
                if (activeBlocks.empty() || activeBlocks.back() != item.block) {
                    success = false;
                } else {
                    activeBlocks.pop_back();
                }
                break;
            }

            LC_InsertExpansionWork continueBlock = item;
            ++continueBlock.entityIndex;
            work.push_back(std::move(continueBlock));

            LC_InsertExpansionWork processEntity;
            processEntity.kind = LC_InsertExpansionWorkKind::ProcessEntity;
            if (item.entityIndex > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
                success = false;
                break;
            }
            processEntity.entity = item.block->entityAt(static_cast<int>(item.entityIndex));
            processEntity.transform = item.transform;
            processEntity.blockPen = item.blockPen;
            processEntity.inheritedLayer = item.inheritedLayer;
            processEntity.inheritedVisibility = item.inheritedVisibility;
            work.push_back(std::move(processEntity));
            break;
        }
        case LC_InsertExpansionWorkKind::ProcessEntity:
            if (item.entity == nullptr || item.entity->isDeleted())
                break;
            if (item.entity->rtti() != RS2::EntityInsert) {
                success = appendLeaf(*item.entity, item.transform, item.blockPen,
                                     item.inheritedLayer, item.inheritedVisibility);
                break;
            }

            {
                const auto& nested = static_cast<const RS_Insert&>(*item.entity);
                RS_Block* nestedBlock = nested.getBlockForInsert();
                const RS_InsertData nestedData = nested.getData();
                if (nestedBlock == nullptr || !arrayFitsBudget(nestedData)) {
                    success = false;
                    break;
                }
                RS_Layer* nestedLayer = nested.getLayer();
                if (layerNameEquals(nestedLayer, layerZeroName()))
                    nestedLayer = item.inheritedLayer;
                const bool nestedVisibility = item.inheritedVisibility
                                              && nested.getFlag(RS2::FlagVisible)
                                              && !nestedBlock->isFrozen();
                queueArrayCell(*nestedBlock, nestedData, item.transform,
                               updatePen(nested.getPen(false), item.blockPen), nestedLayer,
                               nestedVisibility, 0, 0);
            }
            break;
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
    RS_BlockList* blkList = nullptr;

    if (!m_data.blockSource) {
        if (getGraphic()) {
            blkList = getGraphic()->getBlockList();
        }
    } else {
        blkList = m_data.blockSource;
    }

    if (blkList == nullptr) {
        invalidateBlockCache();
        return nullptr;
    }
    if (m_blockList == blkList && m_blockListGeneration == blkList->generation()) {
        return m_block;
    }
    m_block = blkList->find(m_data.name);
    m_blockList = blkList;
    m_blockListGeneration = blkList->generation();
    return m_block;
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
    m_data.scaleFactor = scaleComponents(m_data.scaleFactor, factor);
    m_data.spacing = scaleComponents(m_data.spacing, factor);
    RS_DEBUG->print("RS_Insert::scale2: insertionPoint: %f/%f",
                    m_data.insertionPoint.x, m_data.insertionPoint.y);
    update();

}

void RS_Insert::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    m_data.insertionPoint.mirror(axisPoint1, axisPoint2);
    RS_Vector axisDirection = axisPoint2 - axisPoint1;
    RS_Vector direction = RS_Vector::polar(kIdentityScale, m_data.angle);
    direction.mirror(transformOrigin(), axisDirection);
    m_data.angle = RS_Math::correctAngle(direction.angle() - kHalfTurnRadians);
    m_data.scaleFactor.x *= -1;
    update();
}

std::ostream& operator << (std::ostream& os, const RS_Insert& i) {
    os << " Insert: " << i.getData() << std::endl;
    return os;
}
