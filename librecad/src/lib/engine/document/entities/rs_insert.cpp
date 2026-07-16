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

#include<iostream>

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

class RS_Circle;
class RS_Arc;

namespace {

// Minimum scaling factor allowed
constexpr double MIN_Scale_Factor = 1.0e-6;
constexpr double kExtrusionPlaneTol = 1.0e-6;
constexpr double kInsertAngleCompoundTol = 1.0e-9;
constexpr double kBlockAbsCoordSkip = 100000.0;

bool vectorHasWcsCoords(const RS_Vector &v) {
    return std::abs(v.x) > kBlockAbsCoordSkip || std::abs(v.y) > kBlockAbsCoordSkip;
}

// Nested INSERT ips that lie far outside the parent block's own leaf envelope
// are almost always absolute WCS grips (chicun sofa/chair assemblies). Treating
// them as parent-local offsets flings small symbols across model space.
constexpr double kNestedIpOutsideMargin = 10000.0;

bool computeParentLeafEnvelope(RS_Block *blk, RS_Vector &outMin,
                               RS_Vector &outMax) {
    if (blk == nullptr)
        return false;
    bool any = false;
    RS_Vector mn(false);
    RS_Vector mx(false);
    for (RS_Entity *e : *blk) {
        if (e == nullptr || e->isContainer())
            continue;
        e->calculateBorders();
        const RS_Vector emin = e->getMin();
        const RS_Vector emax = e->getMax();
        if (!emin.valid || !emax.valid)
            continue;
        if (!any) {
            mn = emin;
            mx = emax;
            any = true;
        } else {
            mn = RS_Vector::minimum(mn, emin);
            mx = RS_Vector::maximum(mx, emax);
        }
    }
    if (!any)
        return false;
    outMin = mn;
    outMax = mx;
    return true;
}

RS_Vector sanitizeNestedInsertIp(const RS_Vector &ip, const RS_Vector &leafMin,
                                 const RS_Vector &leafMax, bool haveLeaves) {
    if (!ip.valid)
        return ip;
    RS_Vector r = ip;
    if (haveLeaves) {
        if (ip.x < leafMin.x - kNestedIpOutsideMargin
                || ip.x > leafMax.x + kNestedIpOutsideMargin)
            r.x = 0.0;
        if (ip.y < leafMin.y - kNestedIpOutsideMargin
                || ip.y > leafMax.y + kNestedIpOutsideMargin)
            r.y = 0.0;
        return r;
    }
    // Parent is insert-only (no direct leaves): still drop huge absolute
    // components that cannot be parent-local for a compact assembly.
    constexpr double kAbsNestedIp = 20000.0;
    if (std::abs(ip.x) > kAbsNestedIp)
        r.x = 0.0;
    if (std::abs(ip.y) > kAbsNestedIp)
        r.y = 0.0;
    return r;
}

bool insertEntityReachesNegativeX(const RS_Entity *entity) {
    if (entity == nullptr)
        return false;
    const RS_Vector minPt = entity->getMin();
    return minPt.valid && minPt.x < -kExtrusionPlaneTol;
}

// Apply INSERT OCS mirroring before the block->WCS insert chain.
void applyInsertOcsMirror(RS_Entity *entity, const RS_Vector &extrusion,
                          const RS_Vector &scale) {
    if (entity == nullptr)
        return;
    const bool negExtrusion = extrusion.z < -kExtrusionPlaneTol;
    const bool negZScale = scale.z < -kExtrusionPlaneTol;
    if (negExtrusion && negZScale) {
        // Extrusion (0,0,-1) with negative z-scale folds any geometry that
        // reaches block -X onto +X (large-radius arcs centered on +X included).
        if (insertEntityReachesNegativeX(entity)) {
            entity->mirror(RS_Vector(0.0, 0.0), RS_Vector(0.0, 1.0));
        }
        return;
    }
    if (negExtrusion) {
        entity->mirror(RS_Vector(0.0, 0.0), RS_Vector(1.0, 0.0));
    }
    if (negZScale) {
        entity->mirror(RS_Vector(0.0, 0.0), RS_Vector(0.0, 1.0));
    }
}

// Apply one INSERT's block->parent transform to leaf geometry only.
// RS_Insert::move/scale/rotate must not be used on pre-expanded nested
// shells — they call update() and destroy transferred children.
void applyInsertTransformToEntity(RS_Entity *entity, const RS_Vector &arrayOffset,
                                  const RS_Vector &blockBase,
                                  const RS_Vector &insertionPoint,
                                  const RS_Vector &scaleFactor, double angle,
                                  const RS_Vector &extrusion, bool wcsEmbedded) {
    if (entity == nullptr)
        return;
    applyInsertOcsMirror(entity, extrusion, scaleFactor);
    entity->move(arrayOffset);
    if (!wcsEmbedded) {
        entity->move(blockBase * (-1.0));
        entity->scale(insertionPoint, scaleFactor);
        entity->rotate(insertionPoint, angle);
    }
}

void reanchorNestedWcsEntity(RS_Entity *entity,
                             const RS_Vector &nestedInsertIp) {
    if (entity == nullptr)
        return;
    entity->calculateBorders();
    const RS_Vector center = (entity->getMin() + entity->getMax()) * 0.5;
    if (!center.valid)
        return;
    entity->move(nestedInsertIp - center);
}

void applyParentInsertTransform(RS_Entity *entity, const RS_Vector &arrayOffset,
                                const RS_Vector &blockBase,
                                const RS_Vector &insertionPoint,
                                const RS_Vector &scaleFactor, double angle,
                                const RS_Vector &extrusion, bool previewUpdate,
                                bool wcsEmbeddedChild, bool parentBlockWcs) {
    if (entity == nullptr)
        return;
    if (entity->rtti() == RS2::EntityInsert) {
        auto *ins = static_cast<RS_Insert *>(entity);
        for (RS_Entity *sub : *ins) {
            applyParentInsertTransform(sub, arrayOffset, blockBase, insertionPoint,
                                       scaleFactor, angle, extrusion, previewUpdate,
                                       wcsEmbeddedChild, parentBlockWcs);
        }
        ins->calculateBorders();
        return;
    }
    entity->setUpdateEnabled(false);
    // Apply full INSERT transform to all leaves, including WCS wipeouts.
    // Skipping scale/rotate for wipeouts left chicun "hanging light 1" /
    // pd.light wipeouts at entity+ip while siblings used scale around ip.
    applyInsertTransformToEntity(entity, arrayOffset, blockBase, insertionPoint,
                                 scaleFactor, angle, extrusion,
                                 /*wcsEmbedded=*/false);
    entity->setUpdateEnabled(true);
    if (!previewUpdate) {
        entity->update();
    }
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
    // Empty inserts (or expands that collapse to a point at the world origin)
    // used to inherit EntityContainer's corrupt-data fallback of (0,0), which
    // pinned zoomAuto to the origin even when the insertion grip was re-based
    // (chicun fdfd/ddegh/ghnah). Prefer the insertion point, or leave borders
    // invalid for a truly empty insert so adjustBorders skips it.
    const bool originPin =
        std::abs(minV.x) < 1.0e-9 && std::abs(maxV.x) < 1.0e-9
        && std::abs(minV.y) < 1.0e-9 && std::abs(maxV.y) < 1.0e-9;
    if (count() == 0) {
        resetBorders();
        return;
    }
    if (originPin && m_data.insertionPoint.valid) {
        minV = m_data.insertionPoint;
        maxV = m_data.insertionPoint;
    }
}

void RS_Insert::update() {

    RS_DEBUG->print("RS_Insert::update");
    RS_DEBUG->print("RS_Insert::update: name: %s", m_data.name.toLatin1().data());
    //        RS_DEBUG->print("RS_Insert::update: insertionPoint: %f/%f",
    //                data.insertionPoint.x, data.insertionPoint.y);

    if (updateEnabled==false) {
        return;
    }

    clear();

    RS_Block* blk = getBlockForInsert();
    if (blk == nullptr) {
        RS_DEBUG->print("RS_Insert::update: Block is nullptr");
        return;
    }
    // Font letter INSERTs (blockSource = font letterList) must never mutate
    // the shared letter geometry. prepareForInsertExpansion re-centers WCS
    // document blocks; applying it to font chars corrupts the global font
    // cache and has crashed DWG import in RS_Text::update → letter expand
    // (SIGSEGV in layer pen QString while resolving getPen()).
    const bool fontLetterInsert = (m_data.blockSource != nullptr)
            || (blk->rtti() == RS2::EntityFontChar);
    if (!fontLetterInsert)
        blk->prepareForInsertExpansion();

    if (isUndone()) {
        RS_DEBUG->print("RS_Insert::update: Insert is in undo list");
        return;
    }

    if (std::abs(m_data.scaleFactor.x)<MIN_Scale_Factor || std::abs(m_data.scaleFactor.y)<MIN_Scale_Factor) {
        RS_DEBUG->print("RS_Insert::update: scale factor is 0");
        return;
    }

    RS_DEBUG->print("RS_Insert::update: cols: %d, rows: %d",
                    m_data.cols, m_data.rows);
    RS_DEBUG->print("RS_Insert::update: block has %d entities",
                    blk->count());
    // Pen used when expanding block members. Document inserts resolve ByLayer
    // via getPen(). Font letter inserts intentionally have null layer +
    // invalid pen (RS_Text::update); resolving ByLayer walks the text parent
    // and can crash if layer state is mid-import. Keep unresolved pens.
    RS_Pen expansionPen = getPen(false);
    if (fontLetterInsert) {
        if (!expansionPen.isValid() && parent != nullptr)
            expansionPen = parent->getPen(false);
    } else {
        expansionPen = getPen(true);
    }
    const bool parentBlockWcs =
        fontLetterInsert ? false : blk->hasWcsEmbeddedGeometry();
    RS_Vector parentLeafMin(false);
    RS_Vector parentLeafMax(false);
    const bool haveParentLeaves =
        computeParentLeafEnvelope(blk, parentLeafMin, parentLeafMax);
        for(auto* e: *blk){
            for (int c=0; c<m_data.cols; ++c) {
//            RS_DEBUG->print("RS_Insert::update: col %d", c);
                for (int r=0; r<m_data.rows; ++r) {
//                i_en_counts++;
//                RS_DEBUG->print("RS_Insert::update: row %d", r);
                    // fixme - sand - this is quick fix for #2177 - yet it's necessary to check why undone entity is in block?
                    if (e->isUndone()) {
                        continue;
                    }
//                                RS_DEBUG->print("RS_Insert::update: cloning entity");

                    RS_Vector arrayOffset = m_data.insertionPoint;
                    if (std::abs(m_data.scaleFactor.x)>MIN_Scale_Factor &&
                            std::abs(m_data.scaleFactor.y)>MIN_Scale_Factor) {
                        arrayOffset += RS_Vector(m_data.spacing.x/m_data.scaleFactor.x*c,
                                                 m_data.spacing.y/m_data.scaleFactor.y*r);
                    }

                    if (e->rtti() == RS2::EntityInsert) {
                        const auto* childIns = static_cast<const RS_Insert*>(e);
                        RS_Block *childBlk = childIns->getBlockForInsert();
                        const bool childWcs = childBlk != nullptr
                                && childBlk->hasWcsEmbeddedGeometry();
                        const bool parentRotates =
                            std::abs(m_data.angle) >= kInsertAngleCompoundTol;
                        // Zero nested IP components that sit far outside the
                        // parent leaf envelope (WCS grips mis-tagged as local).
                        RS_InsertData childData = childIns->getData();
                        const RS_Vector rawNestedIp = childData.insertionPoint;
                        childData.insertionPoint = sanitizeNestedInsertIp(
                            childData.insertionPoint, parentLeafMin, parentLeafMax,
                            haveParentLeaves);
                        const bool nestedIpSanitized =
                            childData.insertionPoint.x != rawNestedIp.x
                            || childData.insertionPoint.y != rawNestedIp.y;

                        // WCS-in-block children nested inside a local parent (CUSH,
                        // FLOOWER1, LNG-13, ch00a) must expand first: compound
                        // INSERT data carries WCS absolute grips that misplace
                        // arcs/wipeouts. WCS-in-WCS wipeout children still need
                        // expand when the parent rotates or hosts wipeouts.
                        // Also expand when the nested IP was sanitized from abs
                        // grips (ch00a/cush1 under local assemblies).
                        const bool needNestedExpand = parentRotates
                                || (childWcs && childBlk != nullptr
                                    && !parentBlockWcs)
                                || (childWcs && childBlk != nullptr
                                    && parentBlockWcs
                                    && childBlk->hasWipeoutEntities())
                                || (nestedIpSanitized && childWcs);
                        RS_Layer *childLayer = e->getLayer();

                        if (!needNestedExpand) {
                            auto* ne = new RS_Insert(this, childData);
                            ne->setOwner(true);
                            ne->setUpdateEnabled(false);
                            if (childLayer != nullptr && childLayer->getName() == "0")
                                ne->setLayer(getLayer());
                            ne->setVisible(getFlag(RS2::FlagVisible));
                            ne->move(arrayOffset);
                            ne->move(blk->getBasePoint() * (-1.0));
                            ne->scale(m_data.insertionPoint, m_data.scaleFactor);
                            ne->rotate(m_data.insertionPoint, m_data.angle);
                            ne->setSelected(isSelected());
                            ne->setPen(updatePen(ne->getPen(false), expansionPen));
                            ne->setUpdateEnabled(true);
                            if (m_data.updateMode != RS2::PreviewUpdate) {
                                ne->update();
                            }
                            appendEntity(ne);
                            continue;
                        }

                        // Parent rotates or WCS wipeout child: expand first,
                        // then transform leaves. Compounding fails when rotation
                        // and negative scale combine (chicun 015/A$C446327FF).
                        auto* childExpand = new RS_Insert(this, childData);
                        childExpand->setOwner(true);
                        childExpand->setUpdateEnabled(false);
                        if (childLayer != nullptr && childLayer->getName() == "0")
                            childExpand->setLayer(getLayer());
                        childExpand->setVisible(getFlag(RS2::FlagVisible));
                        childExpand->setSelected(isSelected());
                        childExpand->setPen(updatePen(childExpand->getPen(false), expansionPen));
                        childExpand->setUpdateEnabled(true);
                        childExpand->update();

                        for (RS_Entity* gc : *childExpand) {
                            if (gc->getLayer() != nullptr
                                    && gc->getLayer()->getName() == "0")
                                gc->setLayer(getLayer());
                            gc->setVisible(getFlag(RS2::FlagVisible));
                            gc->setSelected(isSelected());
                            gc->setPen(updatePen(gc->getPen(false), expansionPen));

                            if (childWcs && !parentBlockWcs) {
                                RS_Vector nestedTarget =
                                    childData.insertionPoint;
                                // FLOOWER1/CUSH/chicun: WCS child INSERT grips stored
                                // as absolute coords inside a local parent block.
                                if (vectorHasWcsCoords(nestedTarget)
                                        || nestedIpSanitized)
                                    nestedTarget = RS_Vector(0., 0.);
                                reanchorNestedWcsEntity(gc, nestedTarget);
                            }

                            applyParentInsertTransform(
                                gc, arrayOffset, blk->getBasePoint(),
                                m_data.insertionPoint, m_data.scaleFactor,
                                m_data.angle, m_data.extrusion,
                                m_data.updateMode == RS2::PreviewUpdate,
                                childWcs, parentBlockWcs);
                        }
                        childExpand->calculateBorders();
                        appendEntity(childExpand);
                        continue;
                    }

                    RS_Entity* ne = nullptr;
                    if ( (m_data.scaleFactor.x - m_data.scaleFactor.y)>MIN_Scale_Factor) {
                        if (e->rtti()== RS2::EntityArc) {
                            auto a= static_cast<RS_Arc*>(e);
                            ne = new RS_Ellipse{this,
                            {a->getCenter(), {a->getRadius(), 0.},
                                    1, a->getAngle1(), a->getAngle2(),
                                    a->isReversed()}};
                            ne->setLayer(e->getLayer());
                            ne->setPen(e->getPen(false));
                        } else if (e->rtti()== RS2::EntityCircle) {
                            auto a= static_cast<RS_Circle*>(e);
                            ne = new RS_Ellipse{this,
                            { a->getCenter(), {a->getRadius(), 0.}, 1, 0., 2.*M_PI, false}};
                            ne->setLayer(e->getLayer());
                            ne->setPen(e->getPen(false));
                        } else {
                            ne = e->clone();
                        }
                    } else {
                        ne = e->clone();
                    }
                    ne->setUpdateEnabled(false);
                // if entity layer are 0 set to insert layer to allow "1 layer control" bug ID #3602152
                    RS_Layer *l= ne->getLayer();//special fontchar block don't have
                    if (l != nullptr  && ne->getLayer()->getName() == "0")
                    ne->setLayer(getLayer());
                    ne->setParent(this);
                    ne->setVisible(getFlag(RS2::FlagVisible));

                    // Full transform for every leaf (including wipeouts in WCS
                    // blocks). See applyParentInsertTransform above.
                    applyInsertTransformToEntity(
                        ne, arrayOffset, blk->getBasePoint(),
                        m_data.insertionPoint, m_data.scaleFactor, m_data.angle,
                        m_data.extrusion, /*wcsEmbedded=*/false);

                   // RS_DEBUG->print(RS_Debug::D_ERROR, "ne: angle: %lg\n", data.angle);
                // Select:
                    ne->setSelected(isSelected());

                // individual entities can be on indiv. layers
                    RS_Pen tmpPen = updatePen(ne->getPen(false), expansionPen);
                // now that we've evaluated all flags, let's strip them:
                // TODO: strip all flags (width, line type)
                //tmpPen.setColor(tmpPen.getColor().stripFlags());
                    ne->setPen(tmpPen);

                    ne->setUpdateEnabled(true);

                // insert must be updated even in preview mode
                    if (m_data.updateMode != RS2::PreviewUpdate
                            || ne->rtti() == RS2::EntityInsert) {
                        //RS_DEBUG->print("RS_Insert::update: updating new entity");
                        ne->update();
                    }

//                                RS_DEBUG->print("RS_Insert::update: adding new entity");
                    appendEntity(ne);
//                std::cout<<"done # of entity: "<<i_en_counts<<std::endl;
                }
            }
        }
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

RS_Vector RS_Insert::getNearestRef(const RS_Vector& coord,
									 double* dist) const{
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
