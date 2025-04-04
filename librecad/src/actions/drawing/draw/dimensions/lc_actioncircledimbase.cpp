/*******************************************************************************
 *
 This file is part of the LibreCAD project, a 2D CAD program

 Copyright (C) 2024 LibreCAD.org
 Copyright (C) 2024 sand1024

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 ******************************************************************************/
#include "lc_actioncircledimbase.h"
#include "qg_dimoptions.h"
#include "rs_debug.h"
#include "rs_graphicview.h"
#include "rs_math.h"
#include "rs_preview.h"


LC_ActionCircleDimBase::LC_ActionCircleDimBase(const char* name, RS_EntityContainer &container, RS_GraphicView &graphicView, RS2::ActionType type)
  : RS_ActionDimension(name, container,  graphicView)
    , entity(nullptr)
    , lastStatus(SetEntity)
    , pos(std::make_unique<RS_Vector>()){
    actionType = type;
}

LC_ActionCircleDimBase::~LC_ActionCircleDimBase() = default;

void LC_ActionCircleDimBase::doTrigger() {
    if (entity != nullptr) {
        preparePreview(entity, *pos, alternateAngle);
        auto *newEntity = createDim(container);

        setPenAndLayerToActive(newEntity);
        newEntity->update();
        undoCycleAdd(newEntity);
        alternateAngle = false;
        RS_Snapper::finish();
    } else {
        RS_DEBUG->print("RS_ActionDimDiametric::trigger: Entity is nullptr\n");
    }
}

void LC_ActionCircleDimBase::onMouseMoveEvent(int status, LC_MouseEvent *e) {
    RS_Vector snap = e->snapPoint;
    switch (status) {
        case SetEntity: {
            RS_Entity *en = catchEntityByEvent(e, RS2::ResolveAll);
            if (en != nullptr) {
                if (isArc(en) || isCircle(en)) {
                    highlightHover(en);
                    moveRelativeZero(en->getCenter());
                    if (previewShowsFullDimension) {
                        RS_Vector pointOnCircle = preparePreview(en, snap, e->isControl);
                        auto *d = createDim(preview.get());
                        d->update();
                        previewEntity(d);
                        previewRefSelectablePoint(pointOnCircle);
                    }
                }
            }
            break;
        }
        case SetPos: {
            if (entity != nullptr) {
                highlightSelected(entity);
                *pos = getSnapAngleAwarePoint(e, entity->getCenter(), snap, true);
                RS_Vector pointOnCircle = preparePreview(entity, *pos, false);

                auto *d = createDim(preview.get());
                m_currentAngle = entity->getCenter().angleTo(pointOnCircle);
                ucsBasisAngleDegrees = toUCSBasisAngleDegrees(m_currentAngle);
                updateOptionsUI(QG_DimOptions::UI_UPDATE_CIRCLE_ANGLE);
                d->update();
                previewEntity(d);
                previewRefSelectablePoint(pointOnCircle);
            }
            break;
        }
        default:
            break;
    }
}

void LC_ActionCircleDimBase::onMouseLeftButtonRelease(int status, LC_MouseEvent *e) {
    switch (status) {
        case SetEntity: {
            RS_Entity *en = catchEntityByEvent(e, RS2::ResolveAll);
            if (en != nullptr) {
                if (isArc(en) || isCircle(en)) {
                    entity = en;
                    const RS_Vector &center = en->getCenter();
                    moveRelativeZero(center);
                    if (!isAngleIsFree()){
                        alternateAngle = e->isControl;
                        if (!pos->valid){
                            *pos = e->snapPoint;
                        }
                        trigger();
                        reset();
                    }
                    else {
                        setStatus(SetPos);
                    }
                } else {
                    commandMessage(tr("Not a circle or arc entity"));
                }
            }
            break;
        }
        case SetPos: {
            RS_Vector snap = e->snapPoint;
            snap = getSnapAngleAwarePoint(e, entity->getCenter(), snap);
            fireCoordinateEvent(snap);
            break;
        }
        default:
            break;
    }
}

void LC_ActionCircleDimBase::onMouseRightButtonRelease(int status, [[maybe_unused]] LC_MouseEvent *e) {
    deletePreview();
    initPrevious(status);
}

void LC_ActionCircleDimBase::onCoordinateEvent(int status, [[maybe_unused]] bool isZero, const RS_Vector &coord) {
    switch (status) {
        case SetPos: {
            *pos = coord;
            trigger();
            reset();
            setStatus(SetEntity);
            break;
        }
        default:
            break;
    }
}

bool LC_ActionCircleDimBase::doProcessCommand(int status, const QString &c) {
    // fixme - check whether the code is duplicated with other dim actions
    bool accept = false;
    // setting new text label:
    if (status == SetText) {
        setText(c);
        updateOptions();
        enableCoordinateInput();
        setStatus(lastStatus);
        accept = true;
    } else if (checkCommand("text", c)) { // command: text
        lastStatus = (Status) status;
        disableCoordinateInput();
        setStatus(SetText);
        accept = true;
    } else if (status == SetPos) {// setting angle
        double angle;
        bool ok = parseToUCSBasisAngle(c, angle);
        if (ok) {
            accept = true;
            ucsBasisAngleDegrees = angle;
            m_currentAngle = toWorldAngleFromUCSBasisDegrees(angle);
            pos->setPolar(1.0, m_currentAngle);
            *pos += data->definitionPoint;
            updateOptionsUI(QG_DimOptions::UI_UPDATE_CIRCLE_ANGLE);
            trigger();
            reset();
            setStatus(SetEntity);
        } else {
            commandMessage(tr("Not a valid expression"));
        }
    }
    return accept;
}

QStringList LC_ActionCircleDimBase::getAvailableCommands() {
    QStringList cmd;

    switch (getStatus()) {
        case SetEntity:
        case SetPos:
            cmd += command("text");
            break;
        default:
            break;
    }
    return cmd;
}

void LC_ActionCircleDimBase::updateMouseButtonHints() {
    switch (getStatus()) {
        case SetEntity:
            updateMouseWidgetTRCancel(tr("Select arc or circle entity"), angleIsFree ? MOD_NONE : MOD_CTRL(tr("Free angle")));
            break;
        case SetPos:
            updateMouseWidgetTRCancel(tr("Specify dimension line position or enter angle:"), MOD_SHIFT_ANGLE_SNAP);
            break;
        case SetText:
            updateMouseWidget(tr("Enter dimension text:"));
            break;
        default:
            updateMouseWidget();
            break;
    }
}

double LC_ActionCircleDimBase::getUcsAngleDegrees() const {
    return ucsBasisAngleDegrees;
}

void LC_ActionCircleDimBase::setUcsAngleDegrees(double ucsRelAngleDegrees) {
    ucsBasisAngleDegrees = ucsRelAngleDegrees;
    m_currentAngle = toWorldAngleFromUCSBasisDegrees(ucsRelAngleDegrees);
}

bool LC_ActionCircleDimBase::isAngleIsFree() const {
    return angleIsFree;
}

void LC_ActionCircleDimBase::setAngleIsFree(bool angleIsFree) {
    this->angleIsFree = angleIsFree;
}

double LC_ActionCircleDimBase::getCurrentAngle() {
    double angleDeg = toUCSBasisAngleDegrees(m_currentAngle);
    return angleDeg;
}
