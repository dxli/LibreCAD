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
#ifndef QG_CADTOOLBARPOINTS_H
#define QG_CADTOOLBARPOINTS_H

class QG_CadToolBar;
class QG_ActionHandler;

#include "lc_cadtoolbarinterface.h"
#include "ui_qg_cadtoolbarpoints.h"

class QG_CadToolBarPoints : public QWidget, public Ui::QG_CadToolBarPoints, public LC_CadToolBarInterface
{
    Q_OBJECT

public:
	QG_CadToolBarPoints(QG_CadToolBar* parent = 0, Qt::WindowFlags fl = 0);
	~QG_CadToolBarPoints() = default;

public slots:
    virtual void mousePressEvent( QMouseEvent * e );
	virtual void contextMenuEvent( QContextMenuEvent * e );
	virtual void drawPoint();

protected slots:
    virtual void languageChange();

};

#endif // QG_CADTOOLBARPOINTS_H
