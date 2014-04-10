/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2014 Priyanka Kapoor (anjalicool.kapoor444@gmail.com)
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
#ifndef QG_FLOWEROPTIONS_H
#define QG_FLOWEROPTIONS_H

#include "rs_actiondrawflower.h"
#include "ui_qg_floweroptions.h"

class QG_FlowerOptions : public QWidget, public Ui::QG_FlowerOptions
{
    Q_OBJECT

public:
    QG_FlowerOptions(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~QG_FlowerOptions();

public slots:
    virtual void setAction( RS_ActionInterface * a, bool update );
    
protected:
    RS_ActionDrawFlower* action;

protected slots:
    virtual void languageChange();
    virtual void updatePetal( const QString & r );

private:
    void destroy();

};

#endif // QG_FLOWEROPTIONS_H
