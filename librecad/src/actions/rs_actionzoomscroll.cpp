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

#include "rs_actionzoomscroll.h"

#include "rs_graphicview.h"
#include "rs_commandevent.h"
#include "rs_dialogfactory.h"


RS_ActionZoomScroll::RS_ActionZoomScroll(RS2::Direction direction,
        RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        const RS_Vector& point)
        :RS_ActionInterface("Zoom scroll", container, graphicView) {

    this->direction = direction;
    this->point = point;
}



void RS_ActionZoomScroll::trigger() {
    graphicView->zoomScroll(point);
    finish(false);
}



void RS_ActionZoomScroll::init(int status) {
    RS_ActionInterface::init(status);
    setStatus(SetPoint);
}



void RS_ActionZoomScroll::updateMouseButtonHints()
{
    switch (getStatus()) {
    case SetPoint:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Specify point to view at"),
                                            tr("Cancel"),
                                            false);
        break;

    default:
        RS_DIALOGFACTORY->restoreMouseWidget();//restore previous mouse button hints
    }
}


void RS_ActionZoomScroll::mouseMoveEvent(QMouseEvent* e) {
    //    RS_DEBUG->print("RS_ActionDrawLine::mouseMoveEvent begin");

    //    RS_DEBUG->print("RS_ActionDrawLine::mouseMoveEvent: snap point");
    snapPoint(e);

    //    RS_DEBUG->print("RS_ActionDrawLine::mouseMoveEvent end");
}



void RS_ActionZoomScroll::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()==Qt::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (e->button()==Qt::RightButton) {
        init(getStatus()-1);
    }
}

void RS_ActionZoomScroll::coordinateEvent(RS_CoordinateEvent* e) {

    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();
   
    switch (getStatus()) {
    case SetPoint:
        point = mouse;
        trigger();
        RS_DIALOGFACTORY->restoreMouseWidget();
        break;
    default:
        break;
    }
}



void RS_ActionZoomScroll::commandEvent(RS_CommandEvent* e) {
    RS_DEBUG->print(RS_Debug::D_WARNING,"cmd");
    QString cmd = e->getCommand().toLower();
    if(checkCommand("help",cmd)) {
        if(RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             +getAvailableCommands().join(", "));
         }
        return;
    }
    trigger();
}

// EOF
