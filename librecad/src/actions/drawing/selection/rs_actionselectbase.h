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

#ifndef RS_ACTIONSELECTBASE_H
#define RS_ACTIONSELECTBASE_H

#include "rs_previewactioninterface.h"

/**
 * This class is the base class to all select actions.
 *
 * @author Andrew Mustun
 */
class RS_ActionSelectBase:public RS_PreviewActionInterface {
Q_OBJECT

public:
    RS_ActionSelectBase(
        const char *name,
        RS_EntityContainer &container,
        RS_GraphicView &graphicView,
        QList<RS2::EntityType> entityTypeList = {});

    void keyReleaseEvent(QKeyEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
protected:
    RS2::CursorType doGetMouseCursor(int status) override;
    RS_Entity *entityToSelect = nullptr;
    const QList<RS2::EntityType> catchForSelectionEntityTypes;
    virtual bool isEntityAllowedToSelect([[maybe_unused]]RS_Entity *ent) const { return true; };
    void selectEntity();
    RS_Entity *selectionMouseMove(QMouseEvent *event);
    virtual void selectionFinishedByKey(QKeyEvent *e, bool escape) = 0;
    virtual bool isShowRefPointsOnHighlight();
    void deselectAll();
};

#endif
