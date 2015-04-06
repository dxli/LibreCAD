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
#include "qg_cadtoolbarselect.h"
#include "qg_cadtoolbar.h"
#include "rs_actionselect.h"
#include "qg_actionhandler.h"

/*
 *  Constructs a QG_CadToolBarSelect as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_CadToolBarSelect::QG_CadToolBarSelect(QG_CadToolBar* parent, Qt::WindowFlags fl)
	:LC_CadToolBarInterface(parent, fl)
	,nextAction(-1)
	,selectAction(nullptr)
{
	setupUi(this);
}


/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_CadToolBarSelect::languageChange()
{
    retranslateUi(this);
}

void QG_CadToolBarSelect::selectSingle() {
	if (actionHandler) {
       if(actionHandler->getCurrentAction()->rtti() != RS2::ActionSelect){
        actionHandler->slotSelectSingle();
       }
    }
}

void QG_CadToolBarSelect::selectContour() {
	if (actionHandler) {
        actionHandler->slotSelectContour();
    }
}

void QG_CadToolBarSelect::deselectAll() {
	if (actionHandler) {
        actionHandler->slotDeselectAll();
    }
}

void QG_CadToolBarSelect::selectAll() {
	if (actionHandler) {
        actionHandler->slotSelectAll();
    }
}

void QG_CadToolBarSelect::selectWindow() {
	if (actionHandler) {
        actionHandler->slotSelectWindow();
    }
}

void QG_CadToolBarSelect::deselectWindow() {
	if (actionHandler) {
        actionHandler->slotDeselectWindow();
    }
}

void QG_CadToolBarSelect::selectIntersected() {
	if (actionHandler) {
        actionHandler->slotSelectIntersected();
    }
}

void QG_CadToolBarSelect::deselectIntersected() {
	if (actionHandler) {
        actionHandler->slotDeselectIntersected();
    }
}

void QG_CadToolBarSelect::selectInvert() {
	if (actionHandler) {
        actionHandler->slotSelectInvert();
    }
}

void QG_CadToolBarSelect::selectLayer() {
	if (actionHandler) {
        actionHandler->slotSelectLayer();
    }
}

void QG_CadToolBarSelect::setSelectAction(RS_ActionInterface* selectAction) {
    this->selectAction = selectAction;
}

void QG_CadToolBarSelect::setNextAction(int nextAction) {
    this->nextAction = nextAction;
    if (nextAction==-1) {
        bDoit->hide();
    } else {
        bDoit->show();
    }
}

void QG_CadToolBarSelect::runNextAction() {
	if (selectAction) {
        if(selectAction->rtti() == RS2::ActionSelect){
            //refuse to run next action if no entity is selected, to avoid segfault by action upon empty selection
            //issue#235
            if( static_cast<RS_ActionSelect*>(selectAction)->countSelected()==0) return;
        }
        selectAction->finish();
		selectAction = nullptr;
    }
    if (nextAction!=-1) {
        actionHandler->killSelectActions();
        actionHandler->setCurrentAction((RS2::ActionType)nextAction);
    }
}

void QG_CadToolBarSelect::on_bBack_clicked()
{
	finishCurrentAction(true);
	if(cadToolBar){
		cadToolBar->showPreviousToolBar(true);
    }
}
