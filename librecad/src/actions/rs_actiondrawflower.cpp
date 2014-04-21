/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2014 Priyanka Kapoor (anjalicool.kapoor444@gmail.com)
**
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
**********************************************************************/

#include "rs_actiondrawflower.h"

#include <QAction>
#include "rs_dialogfactory.h"
#include "rs_graphicview.h"
#include "rs_commandevent.h"



RS_ActionDrawFlower::RS_ActionDrawFlower(RS_EntityContainer& container,RS_GraphicView& graphicView)
    :RS_PreviewActionInterface("Draw Flowers", container, graphicView) {
        reset();
}



RS_ActionDrawFlower::~RS_ActionDrawFlower() {}


QAction* RS_ActionDrawFlower::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {

    QAction* action = new QAction(tr("Flower"), NULL);
    action->setIcon(QIcon(":/extui/flower.png"));
    return action;
}




void RS_ActionDrawFlower::reset() {
    data = RS_CircleData(RS_Vector(false), 0.0);
}



void RS_ActionDrawFlower::init(int status) {
    RS_PreviewActionInterface::init(status);

    reset();
}



void RS_ActionDrawFlower::trigger() {
    RS_PreviewActionInterface::trigger();
    QList<RS_AtomicEntity*> lists;
    RS_Circle* flower= new RS_Circle(container, data);
    lists<<flower;
    int i = petal-1;
    double offset=2.*data.radius*sin(M_PI/petal);

    while (i--){
        flower = new RS_Circle(*flower);
        if(i==petal-1)
               flower->move(RS_Vector(data.radius, 0.));
        else
               flower->move(RS_Vector(((petal -i)*2 + 0.5*petal - 3.)*M_PI/petal)*offset);
        lists<<flower;
    }

    for(RS_AtomicEntity* c: lists){
        c->setLayerToActive();
        c->setPenToActive();
        container->addEntity(c);
    }

    if (document!=NULL) {
        document->startUndoCycle();
        for(RS_AtomicEntity* c: lists){
            document->addUndoable(c);
        }
        document->endUndoCycle();
     }

     // upd. undo list:
     graphicView->redraw(RS2::RedrawDrawing);
     graphicView->moveRelativeZero(data.center);

     setStatus(SetCenter);
     reset();
}



void RS_ActionDrawFlower::mouseMoveEvent(QMouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawFlower::mouseMoveEvent begin");

    RS_Vector mouse = snapPoint(e);
    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        break;

    case SetRadius:
        if (data.center.valid) {
            data.radius = data.center.distanceTo(mouse);
            deletePreview();
            preview->addEntity(new RS_Circle(preview,
                                             data));
            drawPreview();
        }
        break;
    }

    RS_DEBUG->print("RS_ActionDrawFlower::mouseMoveEvent end");
}



void RS_ActionDrawFlower::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button()==Qt::LeftButton) {
        RS_CoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    } else if (e->button()==Qt::RightButton) {
        deletePreview();
        init(getStatus()-1);
    }
}



void RS_ActionDrawFlower::coordinateEvent(RS_CoordinateEvent* e) {
    if (e==NULL) {
        return;
    }

    RS_Vector mouse = e->getCoordinate();

    switch (getStatus()) {
    case SetCenter:
        data.center = mouse;
        graphicView->moveRelativeZero(mouse);
        setStatus(SetRadius);
        break;

    case SetRadius:
        if (data.center.valid) {
            graphicView->moveRelativeZero(mouse);
            data.radius = data.center.distanceTo(mouse);
            trigger();
        }
        setStatus(SetCenter);
        break;

    default:
        break;
    }
}



void RS_ActionDrawFlower::commandEvent(RS_CommandEvent* e) {
    QString c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {

    case SetCenter:
        if (checkCommand("radius", c)) {
            deletePreview();
            setStatus(SetRadius);
        }
        break;

    case SetRadius: {
        bool ok;
        double r = RS_Math::eval(c, &ok);
        if (ok==true) {
            data.radius = r;
        } else {
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->commandMessage(
                        tr("Not a valid expression"));
            }
          }
        setStatus(SetPetal);
        }
        break;

    case SetPetal:{
        bool ok;
        int r = RS_Math::eval(c, &ok);
        if (ok==true) {
            petal=r;
        }else {
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->commandMessage(
                            tr("Not a valid expression"));
            }
         }
        trigger();
        setStatus(SetCenter);
        }
        break;

    default:
        break;
    }
}



QStringList RS_ActionDrawFlower::getAvailableCommands() {
    QStringList cmd;
    return cmd;
}



int RS_ActionDrawFlower::getPetal() {
    return petal;
}



void RS_ActionDrawFlower::setPetal(int p) {
    petal=p;
}




void RS_ActionDrawFlower::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetCenter:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify center"), tr("Cancel"));
        }
        break;
    case SetRadius:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget(tr("Specify radius"), tr("Back"));
        }
        break;
    case SetPetal:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget(
                        tr("Specify petal"),tr("Back"));
        }
        break;
    default:
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->updateMouseWidget("", "");
        }
        break;
    }
}



void RS_ActionDrawFlower::showOptions() {
    RS_ActionInterface::showOptions();
    RS_DIALOGFACTORY->requestOptions(this,true);
}



void RS_ActionDrawFlower::hideOptions() {
    RS_ActionInterface::hideOptions();
    RS_DIALOGFACTORY->requestOptions(this,false);
}



void RS_ActionDrawFlower::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawFlower::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        if (isFinished()) {
            RS_DIALOGFACTORY->resetToolBar();
        }
    }
}

//EOF
