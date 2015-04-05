#include "lc_cadtoolbarinterface.h"
#include "qg_cadtoolbar.h"

LC_CadToolBarInterface::LC_CadToolBarInterface(QG_CadToolBar* _parentTB, Qt::WindowFlags fl):
	QWidget(_parentTB, fl)
	,parentTB(_parentTB)
  ,actionHandler(nullptr)
{
}


void LC_CadToolBarInterface::setCadToolBar(QG_CadToolBar* tb) {
	if (tb) {
		cadToolBar = tb;
		actionHandler = tb->getActionHandler();
	} else {
		RS_DEBUG->print(RS_Debug::D_ERROR,
						"QG_CadToolBarArcs::setCadToolBar(): No valid toolbar set.");
	}
}


void LC_CadToolBarInterface::mousePressEvent(QMouseEvent* e) {
	if (e->button()==Qt::RightButton && cadToolBar) {
		cadToolBar->showPreviousToolBar(true);
		e->accept();
	}
}

//void LC_CadToolBarInterface::contextMenuEvent(QContextMenuEvent *e) {
//	e->accept();
//}

void LC_CadToolBarInterface::back()
{
	if (cadToolBar) {
		cadToolBar->showPreviousToolBar(true);
	}
}
