/****************************************************************************
**
 * Draw circle, given its radius and two points on circle

Copyright (C) 2014 Dongxu Li (dongxuli2011 at gmail.com)

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
**********************************************************************/
#include <QPixmap>
#include <QPainter>
#include <cassert>
#include "qg_ruler.h"
#include "qg_graphicview.h"
#include "rs_grid.h"
#include "rs_debug.h"


QG_Ruler::QG_Ruler(QG_GraphicView* view, RS2::Direction dir):
    m_qPosition(0,0,0,0)
    ,m_pPixmap(NULL)
  ,m_eDirection(dir)
  ,m_pView(view)
{
}

QG_Ruler::~QG_Ruler()
{
    if(m_pPixmap)
        delete m_pPixmap;
    m_pPixmap=0;
}


void QG_Ruler::setDirection(RS2::Direction dir)
{
    m_eDirection=dir;
}

void QG_Ruler::setViewSize(int w, int h, int barSize)
{
    QRect rect;
    switch(m_eDirection){
    case RS2::Left:
        rect=QRect(0, barSize, barSize, h - barSize);
        break;
    case RS2::Up:
        rect=QRect(barSize, 0,  w-barSize, barSize);
        break;
    default:
        //only support ruler at left and Up
        assert(false);
    }
    m_fRulerWidth=barSize;
    if(rect==m_qPosition) return;
    if(m_pPixmap)
        delete m_pPixmap;
    m_qPosition=rect;
    m_pPixmap=new QPixmap(rect.width(), rect.height());
    m_pPixmap->fill(Qt::white);
}

QRect QG_Ruler::rect() const
{
    return m_qPosition;
}

QPixmap* QG_Ruler::pixmap() const
{
    return m_pPixmap;
}

void QG_Ruler::updateZoom()
{

    if(m_pPixmap==NULL) return;
    m_pPixmap->fill(Qt::white);
    RS_Grid* grid=m_pView->getGrid();
    if(grid==NULL) return;

    const std::vector<double>& metaGrid=(m_eDirection==RS2::Up)?grid->getMetaX():grid->getMetaY();

    QPainter painter(m_pPixmap);
    if(m_eDirection==RS2::Left){
        QTransform transform(0.,1.,0.,1.,0.,0.,0.,0.,1.);
        painter.setTransform(transform);
    }
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    double (RS_GraphicView::*f)(double)=(m_eDirection==RS2::Up)?(&RS_GraphicView::toGuiX):(&RS_GraphicView::toGuiY);

    for(const double& x: metaGrid){
        const double xGui=(m_pView->*f)(x)-m_fRulerWidth+0.5;
        painter.drawLine(xGui,0,xGui,m_fRulerWidth);
    }
    painter.end();
}



