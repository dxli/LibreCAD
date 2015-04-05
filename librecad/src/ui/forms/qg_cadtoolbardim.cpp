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
#include "qg_cadtoolbardim.h"
#include "qg_cadtoolbar.h"
#include "qg_actionhandler.h"

/*
 *  Constructs a QG_CadToolBarDim as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_CadToolBarDim::QG_CadToolBarDim(QG_CadToolBar* parent, Qt::WindowFlags fl)
	:LC_CadToolBarInterface(parent, fl)
{
	setupUi(this);
}


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_CadToolBarDim::languageChange()
{
    retranslateUi(this);
}

void QG_CadToolBarDim::drawDimAligned() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimAligned();
    }
}

void QG_CadToolBarDim::drawDimLinear() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimLinear();
    }
}

void QG_CadToolBarDim::drawDimLinearHor() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimLinearHor();
    }
}

void QG_CadToolBarDim::drawDimLinearVer() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimLinearVer();
    }
}

void QG_CadToolBarDim::drawDimRadial() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimRadial();
    }
}

void QG_CadToolBarDim::drawDimDiametric() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimDiametric();
    }
}

void QG_CadToolBarDim::drawDimAngular() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimAngular();
    }
}

void QG_CadToolBarDim::drawDimLeader() {
	if (cadToolBar!=nullptr && actionHandler!=nullptr) {
        actionHandler->slotDimLeader();
    }
}

//void QG_CadToolBarDim::back() {
//    if (cadToolBar!=nullptr) {
//        cadToolBar->back();
//    }
//}
//restore action from checked button
void QG_CadToolBarDim::restoreAction()
{
	if(actionHandler==nullptr) return;
    if ( bAligned ->isChecked() ) {
        actionHandler->slotDimAligned();
        return;
    }
    if ( bLinear ->isChecked() ) {
        actionHandler->slotDimLinear();
        return;
    }
    if ( bLinearHor ->isChecked() ) {
        actionHandler->slotDimLinearHor();
        return;
    }
    if ( bLinearVer ->isChecked() ) {
        actionHandler->slotDimLinearVer();
        return;
    }
    if ( bRadial ->isChecked() ) {
        actionHandler->slotDimRadial();
        return;
    }
    if ( bDiametric ->isChecked() ) {
        actionHandler->slotDimDiametric();
        return;
    }
    if ( bAngular ->isChecked() ) {
        actionHandler->slotDimAngular();
        return;
    }
    if ( bLeader ->isChecked() ) {
        actionHandler->slotDimLeader();
        return;
    }
    //clear all action
    bHidden->setChecked(true);
    RS_ActionInterface* currentAction =actionHandler->getCurrentAction();
	if(currentAction != nullptr) {
        currentAction->finish(false); //finish the action, but do not update toolBar
    }
}
void QG_CadToolBarDim::resetToolBar()
{
    bHidden->setChecked(true);
}

void QG_CadToolBarDim::on_bBack_clicked()
{
	finishCurrentAction(true);
    parentTB->showPreviousToolBar();
}

void  QG_CadToolBarDim::showCadToolBar(RS2::ActionType actionType){
    switch(actionType){
    case RS2::ActionDimAligned:
        bAligned->setChecked(true);
        return;
    case RS2::ActionDimLinear:
        bLinear->setChecked(true);
        return;
    case RS2::ActionDimLinearVer:
        bLinearVer->setChecked(true);
        return;
    case RS2::ActionDimLinearHor:
        bLinearHor->setChecked(true);
        return;
    case RS2::ActionDimRadial:
        bRadial->setChecked(true);
        return;
    case RS2::ActionDimDiametric:
        bDiametric->setChecked(true);
        return;
    case RS2::ActionDimAngular:
        bAngular->setChecked(true);
        return;
    case RS2::ActionDimLeader:
        bLeader->setChecked(true);
        return;
    default:
        bHidden->setChecked(true);
        return;
    }
}
