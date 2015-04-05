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
#ifndef QG_CADTOOLBAR_H
#define QG_CADTOOLBAR_H

#include <QWidget>
#include <vector>
#include "rs.h"
#include "ui_qg_cadtoolbar.h"

class QG_ActionHandler;
class LC_CadToolBarInterface;
class RS_ActionInterface;

class QG_CadToolBar : public QWidget, public Ui::QG_CadToolBar
{
    Q_OBJECT

public:
    QG_CadToolBar(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0);
	~QG_CadToolBar() = default;

    virtual QG_ActionHandler * getActionHandler();

public slots:
    virtual void back();
    virtual void forceNext();
    virtual void mouseReleaseEvent( QMouseEvent * e );
    virtual void contextMenuEvent( QContextMenuEvent * e );
    virtual void createSubToolBars( QG_ActionHandler * ah );
    /** show the toolbar by id
      * if restoreAction is true, also, start the action specified by the checked button of the toolbar
      **/
    virtual void showToolBar(RS2::ToolBarId id, bool restoreAction = true );
    virtual void showToolBarMain();
    virtual void showToolBarLines();
    virtual void showToolBarArcs();
    virtual void showToolBarEllipses();
    virtual void showToolBarSplines();
    virtual void showToolBarPolylines();
    virtual void showToolBarCircles();
    virtual void showToolBarInfo();
    virtual void showToolBarModify();
    virtual void showToolBarDim();
    virtual void showToolBarSelect();
    virtual void showToolBarSelect( RS_ActionInterface * selectAction, int nextAction );
    virtual void showPreviousToolBar(bool cleanup = true);
    virtual void showCadToolBar(RS2::ActionType actionType, bool cleanup=false);
    virtual void resetToolBar();

signals:
    void signalBack();
    void signalNext();

protected:
	LC_CadToolBarInterface* tbSplines;
	LC_CadToolBarInterface* tbInfo;
    QG_ActionHandler* actionHandler;
	std::vector<RS2::ToolBarId> toolbarIDs;
	std::vector<LC_CadToolBarInterface*> toolbars;
    //    RS2::ToolBarId previousID;
    //    RS2::ToolBarId savedID;
	LC_CadToolBarInterface* tbMain;
	LC_CadToolBarInterface* tbDim;
	LC_CadToolBarInterface* tbLines;
    //    QG_CadToolBarPoints* tbPoints;
	LC_CadToolBarInterface* tbEllipses;
	LC_CadToolBarInterface* tbArcs;
	LC_CadToolBarInterface* tbModify;
	LC_CadToolBarInterface* tbCircles;
    //    QG_SnapToolBar* tbSnap;
	LC_CadToolBarInterface* tbSelect;
	LC_CadToolBarInterface* tbPolylines;

protected slots:
    virtual void languageChange();
    void hideSubToolBars();
    void showSubToolBar();

private:
    void init();

};

#endif // QG_CADTOOLBAR_H
