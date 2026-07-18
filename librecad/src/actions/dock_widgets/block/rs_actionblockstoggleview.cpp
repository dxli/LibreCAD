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

#include "rs_actionblockstoggleview.h"

#include <QList>

#include "rs_block.h"
#include "rs_debug.h"
#include "rs_graphic.h"

class RS_BlockList;

RS_ActionBlocksToggleView::RS_ActionBlocksToggleView(LC_ActionContext *actionContext)
        :RS_ActionInterface("Toggle Block Visibility",actionContext, RS2::ActionBlocksToggleView) {}


void RS_ActionBlocksToggleView::trigger() {
    RS_DEBUG->print("toggle block");
    if (m_graphic != nullptr) {
        RS_BlockList* blockList = m_graphic->getBlockList();
        QList<RS_Block*> blocksToToggle;
        // Collect first so a multi-selection has one derived-INSERT refresh.
        for (const auto block: *blockList) {
            if (block != nullptr && block->isVisibleInBlockList() && block->isSelectedInBlockList()) {
                blocksToToggle.append(block);
            }
        }
        if (blocksToToggle.isEmpty())
            blocksToToggle.append(m_graphic->getActiveBlock());
        m_graphic->toggleBlocks(blocksToToggle);
    }
    redrawDrawing();
    finish();
}

void RS_ActionBlocksToggleView::init(const int status) {
    RS_ActionInterface::init(status);
    trigger();
}
