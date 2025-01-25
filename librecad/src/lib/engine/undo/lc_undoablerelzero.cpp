/*******************************************************************************
 *
 This file is part of the LibreCAD project, a 2D CAD program

 Copyright (C) 2024 LibreCAD.org
 Copyright (C) 2024 sand1024

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
 ******************************************************************************/

#include "lc_undoablerelzero.h"

LC_UndoableRelZero::LC_UndoableRelZero(RS_GraphicView *view, const RS_Vector &mFrom, const RS_Vector &mTo):graphicView(view), m_From(mFrom), m_To(mTo) {}

void LC_UndoableRelZero::undoStateChanged(bool undone) {
    if (graphicView != nullptr) {
        if (undone) {
            graphicView->moveRelativeZero(m_From);
        } else {
            graphicView->moveRelativeZero(m_To);
        }
    }
}