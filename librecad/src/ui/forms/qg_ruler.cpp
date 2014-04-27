#include "qg_ruler.h"
#include <QPixmap>
#include <cassert>


QG_Ruler::QG_Ruler(RS2::Direction dir):
    m_qPosition(0,0,0,0)
    ,m_pPixmap(NULL)
  ,m_eDirection(dir)
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
    if(rect==m_qPosition) return;
    if(m_pPixmap)
        delete m_pPixmap;
    m_qPosition=rect;
    m_pPixmap=new QPixmap(rect.width(), rect.height());
    m_pPixmap->fill(Qt::lightGray);
}

QRect QG_Ruler::rect() const
{
    return m_qPosition;
}

QPixmap* QG_Ruler::pixmap() const
{
    return m_pPixmap;
}
