#ifndef LC_CADTOOLBARINTERFACE_H
#define LC_CADTOOLBARINTERFACE_H

#include <QWidget>
#include "rs.h"

class QG_CadToolBar;
class QG_ActionHandler;
class RS_ActionInterface;

class LC_CadToolBarInterface: public QWidget
{
public:
	LC_CadToolBarInterface() = delete;
	LC_CadToolBarInterface(QG_CadToolBar* _parentTB, Qt::WindowFlags fl = 0);
	virtual ~LC_CadToolBarInterface()=default;
	virtual void setCadToolBar( QG_CadToolBar * tb );
	void finishCurrentAction(bool resetToolBar=false); //clear current action

	virtual void resetToolBar() {}
	virtual void runNextAction() {}
	virtual void restoreAction() {} //restore action from checked button
	virtual void showCadToolBar(RS2::ActionType /*actionType*/) {}
	virtual void setSelectAction( RS_ActionInterface * /*selectAction*/ ) {}
	virtual void setNextAction( int /*nextAction*/ ) {}

public slots:
	virtual void back();
	virtual void mousePressEvent( QMouseEvent * e );
//	virtual void contextMenuEvent( QContextMenuEvent * e );

protected:
	QG_CadToolBar* cadToolBar;
	QG_CadToolBar* parentTB;
	QG_ActionHandler* actionHandler;

};

#endif // LC_CADTOOLBARINTERFACE_H
