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
#include <QDebug>
#include "qg_ruler.h"
#include "qg_graphicview.h"
#include "rs_grid.h"
#include "rs_graphic.h"
#include "rs_units.h"
#include "rs_debug.h"


QG_Ruler::QG_Ruler(QG_GraphicView* view, RS2::Direction dir):
    m_qPosition(0,0,0,0)
    ,m_pPixmap(NULL)
  ,m_eDirection(dir)
  ,m_pView(view)
  ,m_fRulerLength(0.)
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
        m_fRulerLength=h - barSize;
        rect=QRect(0, barSize, barSize, m_fRulerLength);
        break;
    case RS2::Up:
        m_fRulerLength=w - barSize;
        rect=QRect(barSize, 0, m_fRulerLength, barSize);
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
    double majorDiv=(m_eDirection==RS2::Up)?grid->getCellVector().x:grid->getCellVector().y;

//    const std::vector<double>& metaGrid=(m_eDirection==RS2::Up)?grid->getMetaX():grid->getMetaY();

    QPainter painter(m_pPixmap);
    if(m_eDirection==RS2::Left){
        // transform by switching x <-> y
        QTransform transform(0.,1.,0.,1.,0.,0.,0.,0.,1.);
        painter.setTransform(transform);
    }
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    double (RS_GraphicView::*toGui)(double)=(m_eDirection==RS2::Up)?(&RS_GraphicView::toGuiX):(&RS_GraphicView::toGuiY);
    double (RS_GraphicView::*toGraph)(int)=(m_eDirection==RS2::Up)?(&RS_GraphicView::toGraphX):(&RS_GraphicView::toGraphY);

    double dx=(m_eDirection==RS2::Up)?majorDiv:-majorDiv;
    // find out unit:
    RS2::Unit unit = RS2::None;
//    RS2::LinearFormat format = RS2::Decimal;
    RS_Graphic* graphic=m_pView->getGraphic();
    if (graphic!=NULL) {
        unit = graphic->getUnit();
//        format = graphic->getLinearFormat();
    }
    qDebug()<<"dx="<<dx<<" "<< RS_Units::unitToString(unit);
    int depth=1;
    double dxGui;
    do{
        double x0=  (m_pView->*toGraph)(m_fRulerWidth);
        x0 -= remainder(x0, dx)+dx;
        double xGui;
        int rulerSize=(int) (m_fRulerWidth/(0.75*depth));
        do{
            xGui = (m_pView->*toGui)(x0) - m_fRulerWidth;
            painter.drawLine(xGui, m_fRulerWidth-rulerSize, xGui, m_fRulerWidth);
            x0 += dx;
        } while (xGui< m_fRulerLength);
        dxGui=fabs((m_pView->*toGui)(x0 +dx) - (m_pView->*toGui)(x0));
        if(depth&0x1)
            dx *= 0.5;
        else
            dx *=0.2;
        ++depth;
    }while(dxGui>=8.);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawLine(0, m_fRulerWidth-1, m_fRulerWidth, m_fRulerWidth-1);

    painter.end();
}

double QG_Ruler::subDivision(const double& dx, const RS2::Unit unit)
{

}





