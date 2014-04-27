#ifndef QG_RULER_H
#define QG_RULER_H

#include "rs.h"

class QPixmap;
class QRect;


class QG_Ruler
{
public:
    QG_Ruler(RS2::Direction dir=RS2::Up);
    ~QG_Ruler();
    QRect rect() const;
    void setViewSize(int w, int h, int barSize);
    QPixmap* pixmap() const;

    QRect m_qPosition;
    QPixmap* m_pPixmap;
    RS2::Direction m_eDirection;
private:
    void setDirection(RS2::Direction dir);
};

#endif // QG_RULER_H
