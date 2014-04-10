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
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/
#ifndef RS_ACTIONDRAWFLOWER_H
#define RS_ACTIONDRAWFLOWER_H


#include "rs_previewactioninterface.h"
#include "rs_circle.h"

/**
* This action class can handle user events to draw
* flower with a given center, radius and number of petals.
*
* @author Priyanka Kapoor(anjalicool.kapoor444@gmail.com)
*/
class RS_ActionDrawFlower : public RS_PreviewActionInterface {
   
public:
    /**
     * Action States.
    */
    enum Status {
        SetCenter, /**< Setting the center point. */
        SetRadius, /**< Setting the radius. */
        SetPetal /**< Setting the petal. */
    };

public:
    RS_ActionDrawFlower(RS_EntityContainer& container,
                        RS_GraphicView& graphicView);
    ~RS_ActionDrawFlower();

    static QAction* createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/);

    virtual RS2::ActionType rtti() {
        return RS2::ActionDrawFlower;
    }

    void reset();
    void setPetal(int p);
    int getPetal();
    virtual void init(int status=0);

    virtual void trigger();

    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

    virtual void coordinateEvent(RS_CoordinateEvent* e);
    virtual void commandEvent(RS_CommandEvent* e);
    virtual QStringList getAvailableCommands();

    virtual void hideOptions();
    virtual void showOptions();

    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
  
    virtual void updateToolBar();

protected:
    /**
     * Circle data defined so far.
    */
    RS_CircleData data;
    int petal=6;
};

#endif // RS_ACTIONDRAWFLOWER_H
