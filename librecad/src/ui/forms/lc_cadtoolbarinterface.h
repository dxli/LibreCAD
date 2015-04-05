#ifndef LC_CADTOOLBARINTERFACE_H
#define LC_CADTOOLBARINTERFACE_H

#include "rs.h"

class QG_CadToolBar;
class QG_ActionHandler;

class LC_CadToolBarInterface
{
public:
	LC_CadToolBarInterface() = delete;
	LC_CadToolBarInterface(QG_CadToolBar* _parentTB);
	~LC_CadToolBarInterface()=default;
	virtual void restoreAction() {} //restore action from checked button
	virtual void setCadToolBar( QG_CadToolBar * tb );
	virtual void showCadToolBar(RS2::ActionType /*actionType*/) {}
	virtual void back();

protected:
	QG_CadToolBar* cadToolBar;
	QG_CadToolBar* parentTB;
	QG_ActionHandler* actionHandler;

};

#endif // LC_CADTOOLBARINTERFACE_H
