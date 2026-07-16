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

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


#include "rs_block.h"

#include "lc_import_repair_flags.h"
#include "rs_blocklist.h"
#include "rs_graphic.h"
#include "rs_insert.h"
#include "rs_line.h"

RS_BlockData::RS_BlockData(const QString& name, const RS_Vector& basePoint, const bool frozen) : name(name), basePoint(basePoint),
    frozen(frozen) {
}

bool RS_BlockData::isValid() const {
    return !name.isEmpty() && basePoint.valid;
}

/**
 * @param parent The graphic this block belongs to.
 * @param blockData
 */
RS_Block::RS_Block(RS_EntityContainer* parent, const RS_BlockData& blockData)
    : RS_Document(parent), m_data(blockData) {
    setPen({RS_Color(128, 128, 128), RS2::Width01, RS2::SolidLine});
}

RS_Entity* RS_Block::clone() const {
    const auto blk = new RS_Block(getParent(), RS_BlockData(m_data));
    blk->setGraphicView(getGraphicView()); // fixme - remove this dependency

    if (isOwner()) {
        const auto entitylist = getEntityList();
        for (const RS_Entity* entity : entitylist) {
            if (entity != nullptr) {
                blk->push_back(entity->clone());
            }
        }
    }
    else {
        const auto entitylist = getEntityList();
        for (RS_Entity* entity : entitylist) {
            if (entity != nullptr) {
                blk->push_back(entity);
            }
        }
    }
    blk->detach();
    return blk;
}

RS_LayerList* RS_Block::getLayerList() {
    RS_Graphic* g = getGraphic();
    return (g != nullptr) ? g->getLayerList() : nullptr;
}

RS_BlockList* RS_Block::getBlockList() {
    RS_Graphic* g = getGraphic();
    return (g != nullptr) ? g->getBlockList() : nullptr;
}

LC_DimStylesList* RS_Block::getDimStyleList() {
    RS_Graphic* g = getGraphic();
    return (g != nullptr) ? g->getDimStyleList() : nullptr;
}

LC_TextStyleList* RS_Block::getTextStyleList() {
    RS_Graphic* g = getGraphic();
    return (g != nullptr) ? g->getTextStyleList() : nullptr;
}

// bool RS_Block::save(bool isAutoSave) {
//     RS_Graphic* g = getGraphic();
//     if (g) {
//         return g->save(isAutoSave);
//     } else {
//         return false;
//     }
// }

// bool RS_Block::saveAs(const QString& filename, RS2::FormatType type, bool force) {
//     RS_Graphic* g = getGraphic();
//     if (g) {
//         return g->saveAs(filename, type, force);
//     } else {
//         return false;
//     }
// }

bool RS_Block::isVisible() const {
    if (!getFlag(RS2::FlagVisible)) {
        return false;
    }

    if (isDeleted()) {
        return false;
    }
    return true;
}

/**
 * Sets the parent documents modified status to 'm'.
 */
void RS_Block::setModified(const bool m) {
    RS_Graphic* p = getGraphic();
    if (p != nullptr) {
        p->setModified(m);
    }
    m_modified = m;
}

/**
 * Sets the visibility of the Block in block list
 *
 * @param v true: visible, false: invisible
 */
void RS_Block::visibleInBlockList(const bool v) const {
    m_data.visibleInBlockList = v;
}

/**
 * Returns the visibility of the Block in block list
 */
bool RS_Block::isVisibleInBlockList() const {
    return m_data.visibleInBlockList;
}

/**
 * Sets selection state of the block in block list
 *
 * @param v true: selected, false: deselected
 */
void RS_Block::selectedInBlockList(const bool v) const {
    m_data.selectedInBlockList = v;
}

/**
 * Returns selection state of the block in block list
 */
bool RS_Block::isSelectedInBlockList() const {
    return m_data.selectedInBlockList;
}

/**
 * Block may contain inserts of other blocks.
 * Find name of the nested block that contain the insert
 * of specified block.
 *
 * @param bName name of the block the nested insert references to
 *
 * @return block name chain to the block that contain searched insert
 */
QStringList RS_Block::findNestedInsert(const QString& bName) {
    QStringList bnChain;

    for (RS_Entity* e : *this) {
        if (e->rtti() == RS2::EntityInsert) {
            const auto i = static_cast<RS_Insert*>(e);
            QString iName = i->getName();
            if (iName == bName) {
                bnChain << m_data.name;
                break;
            }
            RS_BlockList* bList = getBlockList();
            if (bList != nullptr) {
                RS_Block* nestedBlock = bList->find(iName);
                if (nestedBlock != nullptr) {
                    QStringList nestedChain;
                    nestedChain = nestedBlock->findNestedInsert(bName);
                    if (!nestedChain.empty()) {
                        bnChain << m_data.name;
                        bnChain << nestedChain;
                        break;
                    }
                }
            }
        }
    }

    return bnChain;
}

namespace {

constexpr double kBlockBaseNormalizeMinDist = 10000.0;
constexpr double kBlockAbsCoordSkip = 100000.0;

bool blockEnvelopeHasWcsCoords(const RS_Vector &min, const RS_Vector &max) {
    if (!min.valid || !max.valid)
        return false;
    const auto isAbsCoord = [](double v) {
        return std::abs(v) > kBlockAbsCoordSkip;
    };
    return isAbsCoord(min.x) || isAbsCoord(min.y) || isAbsCoord(max.x)
            || isAbsCoord(max.y);
}

void normalizeBlockGeometryToBase(RS_Block *block) {
    if (block == nullptr || block->count() == 0)
        return;

    block->calculateBorders();
    const RS_Vector min = block->getMin();
    const RS_Vector max = block->getMax();
    if (!min.valid || !max.valid)
        return;

    const RS_Vector base = block->getBasePoint();

    if (blockEnvelopeHasWcsCoords(min, max))
        return;

    if (base.x >= min.x && base.x <= max.x && base.y >= min.y
            && base.y <= max.y) {
        return;
    }

    const RS_Vector center = (min + max) * 0.5;
    const double centerDist = center.distanceTo(base);
    if (centerDist < kBlockBaseNormalizeMinDist)
        return;

    // Re-center whenever the leaf/envelope centroid sits far from basePoint.
    // Nested INSERT ips alone must not block this: chicun "hms" has compact
    // geometry near (95k,58k) (just under the WCS absolute threshold) plus
    // far nested WCS children — the old insert-distance skip left that
    // geometry unmoved so top-level INSERT flings it across model space.
    const RS_Vector offset = base - center;
    for (RS_Entity *e : *block) {
        if (e != nullptr)
            e->move(offset);
    }
    block->calculateBorders();
}

// Pull sparse absolute outliers into the local majority cluster.
// Mixed blocks (chicun A$C5DC07332): ~local furniture plus a few huge elliptic
// arcs stored tens/hundreds of k units away. Without re-anchor, INSERT places
// those arcs on the model envelope and inflates zoomAuto bbox several×.
// Fully WCS blocks (11ma, hanging light) keep a WCS majority and are skipped.
constexpr double kLocalClusterRadius = 50000.0;
constexpr double kOutlierFractionMax = 0.20;
constexpr size_t kMinLeavesForOutlierFix = 10;

void reanchorSparseWcsOutliers(RS_Block *block) {
    if (block == nullptr || block->count() == 0)
        return;

    struct Leaf {
        RS_Entity *e = nullptr;
        RS_Vector center;
    };
    std::vector<Leaf> leaves;
    leaves.reserve(static_cast<size_t>(block->count()));
    for (RS_Entity *e : *block) {
        if (e == nullptr || e->isContainer())
            continue;
        e->calculateBorders();
        const RS_Vector mn = e->getMin();
        const RS_Vector mx = e->getMax();
        if (!mn.valid || !mx.valid)
            continue;
        leaves.push_back({e, (mn + mx) * 0.5});
    }
    if (leaves.size() < 5)
        return;

    // Approximate median via nth_element on x then y of middle band.
    std::vector<double> xs;
    std::vector<double> ys;
    xs.reserve(leaves.size());
    ys.reserve(leaves.size());
    for (const Leaf &l : leaves) {
        xs.push_back(l.center.x);
        ys.push_back(l.center.y);
    }
    const size_t mid = leaves.size() / 2;
    std::nth_element(xs.begin(), xs.begin() + static_cast<long>(mid), xs.end());
    std::nth_element(ys.begin(), ys.begin() + static_cast<long>(mid), ys.end());
    const RS_Vector median(xs[mid], ys[mid]);

    std::vector<Leaf *> local;
    std::vector<Leaf *> outliers;
    local.reserve(leaves.size());
    outliers.reserve(8);
    for (Leaf &l : leaves) {
        if (l.center.distanceTo(median) <= kLocalClusterRadius)
            local.push_back(&l);
        else
            outliers.push_back(&l);
    }
    // Extreme-axis outliers (pd.light ellipses at y≈-170k vs core at y≈0):
    // pull anything more than 100k from the median into the core, as long as
    // at least half the leaves remain as core.
    constexpr double kExtremeAxisDist = 100000.0;
    for (Leaf &l : leaves) {
        const bool extreme =
            std::abs(l.center.x - median.x) > kExtremeAxisDist
            || std::abs(l.center.y - median.y) > kExtremeAxisDist;
        if (extreme
                && std::find(outliers.begin(), outliers.end(), &l)
                    == outliers.end())
            outliers.push_back(&l);
    }
    // Rebuild local excluding outliers.
    local.clear();
    for (Leaf &l : leaves) {
        if (std::find(outliers.begin(), outliers.end(), &l) == outliers.end())
            local.push_back(&l);
    }

    if (outliers.empty() || local.empty())
        return;
    if (static_cast<double>(local.size()) < 0.5 * leaves.size())
        return;

    RS_Vector localCentroid(0., 0.);
    for (const Leaf *l : local)
        localCentroid += l->center;
    localCentroid /= static_cast<double>(local.size());

    for (Leaf *l : outliers) {
        const RS_Vector delta = localCentroid - l->center;
        l->e->move(delta);
    }
    block->calculateBorders();
}

// Compact WCS symbols used only as nested inserts (ch00a, cush1, CUSH) store
// geometry at absolute sheet coords. Top-level WCS inserts (11ma, hanging light)
// encode placement as abs_content+ip and must keep absolute coords.
// Re-center nested-only compact WCS blocks onto basePoint so nested expand
// treats them as local furniture near the parent insert.
constexpr double kCompactWcsMaxSpan = 25000.0;

bool blockUsedAsTopLevelModelInsert(RS_Block *block) {
    if (block == nullptr)
        return false;
    RS_Graphic *g = block->getGraphic();
    if (g == nullptr)
        return false;
    const QString name = block->getName();
    for (RS_Entity *e : *g) {
        if (e == nullptr || e->rtti() != RS2::EntityInsert)
            continue;
        if (static_cast<RS_Insert *>(e)->getName() == name)
            return true;
    }
    return false;
}

// Wipeouts that dwarf the rest of a local block (PANDANT LAMP-2 north,
// TABLE500-PL south) inflate the insert envelope. Re-center those wipeouts
// onto the non-wipeout leaf centroid so they track the real symbol.
constexpr double kWipeoutOutlierDist = 5000.0;
constexpr double kWipeoutSpanFactor = 5.0;

void reanchorOversizedWipeouts(RS_Block *block) {
    if (block == nullptr || block->count() == 0)
        return;

    RS_Vector nwMin(false);
    RS_Vector nwMax(false);
    RS_Vector nwSum(0., 0.);
    int nwCount = 0;
    std::vector<RS_Entity *> wipes;
    wipes.reserve(4);

    for (RS_Entity *e : *block) {
        if (e == nullptr || e->isContainer())
            continue;
        e->calculateBorders();
        const RS_Vector mn = e->getMin();
        const RS_Vector mx = e->getMax();
        if (!mn.valid || !mx.valid)
            continue;
        if (e->rtti() == RS2::EntityWipeout) {
            wipes.push_back(e);
            continue;
        }
        if (nwCount == 0) {
            nwMin = mn;
            nwMax = mx;
        } else {
            nwMin = RS_Vector::minimum(nwMin, mn);
            nwMax = RS_Vector::maximum(nwMax, mx);
        }
        nwSum += (mn + mx) * 0.5;
        ++nwCount;
    }
    if (wipes.empty())
        return;

    // Prefer non-wipe leaf core; if the block is wipeout + nested inserts only
    // (A$C7BC56EFF), fall back to basePoint with a small target span.
    const RS_Vector base = block->getBasePoint();
    const RS_Vector nwCentroid =
        nwCount > 0 ? (nwSum / static_cast<double>(nwCount)) : base;
    const double nwSpan =
        nwCount > 0 ? std::max(nwMax.x - nwMin.x, nwMax.y - nwMin.y) : 0.0;
    bool moved = false;

    for (RS_Entity *w : wipes) {
        w->calculateBorders();
        const RS_Vector mn = w->getMin();
        const RS_Vector mx = w->getMax();
        if (!mn.valid || !mx.valid)
            continue;
        const RS_Vector wCtr = (mn + mx) * 0.5;
        const double wSpan = std::max(mx.x - mn.x, mx.y - mn.y);
        const double dist = wCtr.distanceTo(nwCentroid);
        const bool spansHuge =
            (nwSpan > 1.0 && wSpan > nwSpan * kWipeoutSpanFactor)
            || (nwCount == 0 && wSpan > 5000.0);
        const bool farFromCore = dist > kWipeoutOutlierDist;
        const bool extendsFar =
            nwCount > 0
            && (mn.x < nwMin.x - kWipeoutOutlierDist
                || mn.y < nwMin.y - kWipeoutOutlierDist
                || mx.x > nwMax.x + kWipeoutOutlierDist
                || mx.y > nwMax.y + kWipeoutOutlierDist);
        if (!spansHuge && !farFromCore && !extendsFar)
            continue;
        // Re-center onto non-wipe core (or base).
        w->move(nwCentroid - wCtr);
        // Shrink if still enormous relative to the symbol.
        w->calculateBorders();
        const RS_Vector mn2 = w->getMin();
        const RS_Vector mx2 = w->getMax();
        const double wSpan2 = std::max(mx2.x - mn2.x, mx2.y - mn2.y);
        const double targetSpan = std::max(nwSpan * 1.25, 500.0);
        if (wSpan2 > targetSpan * kWipeoutSpanFactor && wSpan2 > 1.0) {
            const double factor = targetSpan / wSpan2;
            w->scale(nwCentroid, RS_Vector(factor, factor));
        }
        moved = true;
    }
    if (moved)
        block->calculateBorders();
}

void recenterCompactNestedOnlyWcsBlock(RS_Block *block) {
    if (block == nullptr || block->count() == 0)
        return;
    block->calculateBorders();
    const RS_Vector min = block->getMin();
    const RS_Vector max = block->getMax();
    if (!min.valid || !max.valid)
        return;
    if (!blockEnvelopeHasWcsCoords(min, max))
        return;
    const double spanX = max.x - min.x;
    const double spanY = max.y - min.y;
    if (spanX > kCompactWcsMaxSpan || spanY > kCompactWcsMaxSpan)
        return;
    if (blockUsedAsTopLevelModelInsert(block))
        return;

    const RS_Vector base = block->getBasePoint();
    const RS_Vector center = (min + max) * 0.5;
    if (center.distanceTo(base) < kBlockBaseNormalizeMinDist)
        return;

    const RS_Vector offset = base - center;
    for (RS_Entity *e : *block) {
        if (e != nullptr)
            e->move(offset);
    }
    // Nested insert grips must move with the geometry.
    block->calculateBorders();
}

} // namespace

void RS_Block::prepareForInsertExpansion() {
    if (m_preparedForInsert >= 0)
        return;

    // Fidelity path: skip mutators; still mark "touched" so we do not re-check.
    if (!LC_ImportRepairFlags::repairBlockDefs()) {
        m_preparedForInsert = 0;
        return;
    }

    normalizeBlockGeometryToBase(this);
    reanchorSparseWcsOutliers(this);
    reanchorOversizedWipeouts(this);
    recenterCompactNestedOnlyWcsBlock(this);
    m_preparedForInsert = 1;
    // Flags after re-center so nested-only compact WCS becomes local.
    m_wcsEmbeddedGeometry = -1;
    (void)hasWcsEmbeddedGeometry();
    (void)hasWipeoutEntities();

    // Import/open: flag host graphic so load path can mark dirty after markSaved.
    if (RS_Graphic *g = getGraphic())
        g->setImportGeometryMutated(true);
}

bool RS_Block::hasWcsEmbeddedGeometry() {
    if (m_wcsEmbeddedGeometry >= 0)
        return m_wcsEmbeddedGeometry != 0;

    calculateBorders();
    const bool wcs = blockEnvelopeHasWcsCoords(getMin(), getMax());
    m_wcsEmbeddedGeometry = wcs ? 1 : 0;
    return wcs;
}

bool RS_Block::hasWipeoutEntities() const {
    if (m_hasWipeoutEntities >= 0)
        return m_hasWipeoutEntities != 0;

    bool found = false;
    for (RS_Entity *e : *this) {
        if (e != nullptr && e->rtti() == RS2::EntityWipeout) {
            found = true;
            break;
        }
    }
    m_hasWipeoutEntities = found ? 1 : 0;
    return found;
}

void RS_Block::addByBlockLine(const RS_Vector& start, const RS_Vector& end) {
    addByBlockEntity(new RS_Line(start, end));
}

void RS_Block::addByBlockEntity(RS_Entity* entity) {
    const RS_Pen byBlockPen(RS2::FlagByBlock, RS2::WidthByBlock, RS2::LineByBlock);
    entity->setPen(byBlockPen);
    addEntity(entity);
}

std::ostream& operator <<(std::ostream& os, const RS_Block& b) {
    os << " name: " << b.getName().toLatin1().data() << "\n";
    auto asContainer = static_cast<const RS_EntityContainer&>(b);
    os << " entities: " << asContainer << "\n";
    return os;
}
