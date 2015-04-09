#include <QMouseEvent>
#include <QGridLayout>
#include <QActionGroup>
#include <QIcon>
#include <QToolButton>
#include <QToolBar>
#include "lc_cadtoolbarinterface.h"
#include "qg_cadtoolbar.h"
#include "qg_actionhandler.h"
#include "rs_debug.h"

LC_CadToolBarInterface::LC_CadToolBarInterface(QG_CadToolBar* _parentTB, Qt::WindowFlags fl):
	QWidget(_parentTB, fl)
	,cadToolBar(_parentTB)
  ,actionHandler(nullptr)
  ,m_pButtonBack(new QAction(QIcon(":/extui/back.png"), "Back", this))
  ,m_pButtonForward(new QAction(QIcon(":/extui/forward.png"), "Back", this))
  ,m_pHidden(new QAction("ActionHidden", this))
  ,m_pGrid0(new QToolBar(this))
  ,m_pGrid1(new QToolBar(this))
  ,m_pActionGroup(new QActionGroup(this))
{
	for(auto p: {m_pGrid0, m_pGrid1}){
		p->setFloatable(false);
		p->setMovable(false);
		p->setOrientation(Qt::Vertical);
	}
	m_pActionGroup->setExclusive(true);
	m_pHidden->setCheckable(true);
	m_pHidden->setChecked(true);
	m_pActionGroup->addAction(m_pHidden);
}

void LC_CadToolBarInterface::setActionHandler(QG_ActionHandler* ah)
{
	actionHandler=ah;
}

void LC_CadToolBarInterface::finishCurrentAction(bool resetToolBar)
{
	if(actionHandler==nullptr) return;
	RS_ActionInterface* currentAction =actionHandler->getCurrentAction();
	if(currentAction) {
		currentAction->finish(resetToolBar); //finish the action, but do not update toolBar
	}
}


void LC_CadToolBarInterface::killSelectActions()
{
	if(actionHandler==nullptr) return;
	actionHandler->killSelectActions();
}

void LC_CadToolBarInterface::killAllActions()
{
	if(actionHandler==nullptr) return;
	actionHandler->killAllActions();
}

void LC_CadToolBarInterface::mousePressEvent(QMouseEvent* e) {
	if (e->button()==Qt::RightButton && cadToolBar) {
		finishCurrentAction(true);
		cadToolBar->showPreviousToolBar(true);
		e->accept();
	}
}

void LC_CadToolBarInterface::back()
{
	if (cadToolBar) {
		cadToolBar->showPreviousToolBar(true);
	}
}


void LC_CadToolBarInterface::addSubAction(QAction*const action, bool addGroup)
{
	qDebug()<<"LC_CadToolBarInterface::addSubAction(): begin";
	if(actions0>actions1){
		m_pGrid1->addAction(action);
		++actions1;
	}else{
		m_pGrid0->addAction(action);
		++actions0;
	}

	if(addGroup) m_pActionGroup->addAction(action);
//	qDebug()<<"m_pGrid->count()="<<m_pGrid->count();
	qDebug()<<"LC_CadToolBarInterface::addSubAction(): end";

}


void LC_CadToolBarInterface::addSubActions(const std::vector<QAction*>& actions, bool addGroup)
{
	qDebug()<<"LC_CadToolBarInterface::addSubActions(): begin";

	for(auto p: actions){
		this->addSubAction(p, addGroup);
	}
	qDebug()<<"LC_CadToolBarInterface::addSubActions(): end";
}
