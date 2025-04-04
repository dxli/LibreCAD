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

#include "lc_quickinfowidget.h"
#include "qc_applicationwindow.h"
#include "rs_actionmodifyentity.h"
#include "rs_debug.h"
#include "rs_dialogfactory.h"
#include "rs_dialogfactoryinterface.h"
#include "rs_graphicview.h"

RS_ActionModifyEntity::RS_ActionModifyEntity(RS_EntityContainer& container,
        RS_GraphicView& graphicView, bool changeCursor)
		:RS_PreviewActionInterface("Modify Entity", container, graphicView)
		,en(nullptr){
	  actionType=RS2::ActionModifyEntity;
   modifyCursor = changeCursor;
}

void RS_ActionModifyEntity::setDisplaySelected(bool highlighted){
    if (en != nullptr) {
        en->setSelected(highlighted);
    }
}

void RS_ActionModifyEntity::doTrigger() {
    if (en != nullptr) {
        std::unique_ptr<RS_Entity> clone{en->clone()};
        bool selected = en->isSelected();
        // RAII style: restore the highlighted status
        std::shared_ptr<bool> scopedFlag(&selected, [this](bool* pointer) {
            if (pointer != nullptr && en->isSelected() != *pointer) {
                setDisplaySelected(*pointer);
            }});
        // Always show the entity being edited as "Selected"
        setDisplaySelected(true);

        unsigned long originalEntityId = en->getId();

        graphicView->setForcedActionKillAllowed(false);
        if (RS_DIALOGFACTORY->requestModifyEntityDialog(clone.get(), viewport)) {
            container->addEntity(clone.get());

            en->setSelected(false);
            clone->setSelected(false);

            if (document) {
                undoCycleReplace(en, clone.get());
            }

            unsigned long cloneEntityId = clone->getId();

            // hm... probably there is a better way to notify (signal, broadcasting etc) without direct dependency?
            LC_QuickInfoWidget *entityInfoWidget = QC_ApplicationWindow::getAppWindow()->getEntityInfoWidget();
            if (entityInfoWidget != nullptr){
                entityInfoWidget->onEntityPropertiesEdited(originalEntityId, cloneEntityId);
            }

            clone.release();
        }
        graphicView->setForcedActionKillAllowed(true);
    } else {
        RS_DEBUG->print("RS_ActionModifyEntity::trigger: Entity is NULL\n");
    }
}

void RS_ActionModifyEntity::onMouseMoveEvent([[maybe_unused]]int status, LC_MouseEvent *e) {
    RS_Entity* entity = catchAndDescribe(e);
    if (entity != nullptr){
        highlightHoverWithRefPoints(entity, true);
    }
}

void RS_ActionModifyEntity::onMouseLeftButtonRelease([[maybe_unused]]int status, LC_MouseEvent *e) {
    en = catchEntityByEvent(e);
    if (en != nullptr) {
        trigger();
    }
}

void RS_ActionModifyEntity::onMouseRightButtonRelease(int status, [[maybe_unused]]LC_MouseEvent *e) {
    initPrevious(status);
}

RS2::CursorType RS_ActionModifyEntity::doGetMouseCursor([[maybe_unused]] int status){
    if (modifyCursor) {
        return RS2::SelectCursor;
    }
    else{
        return RS2::NoCursorChange;
    }
}

void RS_ActionModifyEntity::updateMouseButtonHints() {
    updateMouseWidgetTRCancel(tr("Click on entity to modify"));
}
