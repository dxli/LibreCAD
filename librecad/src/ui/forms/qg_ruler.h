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
#ifndef QG_RULER_H
#define QG_RULER_H

#include "rs.h"

class QPixmap;
class QRect;
class QG_GraphicView;


class QG_Ruler
{
public:
    QG_Ruler(QG_GraphicView* view, RS2::Direction dir=RS2::Up);
    ~QG_Ruler();
    QRect rect() const;
    void setViewSize(int w, int h, int barSize);
    QPixmap* pixmap() const;
    void updateZoom();

    QRect m_qPosition;
    QPixmap* m_pPixmap;
    RS2::Direction m_eDirection;
private:
    void setDirection(RS2::Direction dir);
    QG_GraphicView*const m_pView;
    qreal m_fRulerWidth;
    qreal m_fRulerLength;


};

#endif // QG_RULER_H
