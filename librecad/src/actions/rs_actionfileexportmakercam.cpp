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

#include "rs_actionfileexportmakercam.h"

#include <iostream>
#include <fstream>

#include <QAction>

#include "rs_dialogfactory.h"
#include "rs_graphic.h"
#include "rs_makercamsvg.h"
#include "rs_settings.h"

RS_ActionFileExportMakerCam::RS_ActionFileExportMakerCam(RS_EntityContainer& container,
                                                         RS_GraphicView& graphicView)
    : RS_ActionInterface("Export as MakerCAM SVG...", container, graphicView) {}

QAction* RS_ActionFileExportMakerCam::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {

    QAction* action = new QAction(tr("Export as &MakerCAM SVG..."), NULL);	
//	action->setIcon(QIcon(":/ui/blockadd.png"));
    return action;
}

void RS_ActionFileExportMakerCam::init(int status) {

    RS_ActionInterface::init(status);
    trigger();
}

void RS_ActionFileExportMakerCam::trigger() {

    RS_DEBUG->print("RS_ActionFileExportMakerCam::trigger()");

    if (graphic != NULL) {

        RS_DIALOGFACTORY->requestOptionsMakerCamDialog();

        QString filename = RS_DIALOGFACTORY->requestFileSaveAsDialog(tr("Export as"), 
                                                                     "", 
                                                                     "Scalable Vector Graphics (*.svg)");

        if (filename != "") {
        
            RS_SETTINGS->beginGroup("/ExportMakerCam");
        
            RS_MakerCamSVG* generator = new RS_MakerCamSVG((bool)RS_SETTINGS->readNumEntry("/ExportInvisibleLayers"),
                                                           (bool)RS_SETTINGS->readNumEntry("/ExportConstructionLayers"),
                                                           (bool)RS_SETTINGS->readNumEntry("/WriteBlocksInline"),
                                                           (bool)RS_SETTINGS->readNumEntry("/ConvertEllipsesToPaths"));

            RS_SETTINGS->endGroup();
            
            if (generator->generate(graphic)) {
            
                std::ofstream file;
                file.open(filename.toStdString());
                file << generator->resultAsString();
                file.close();
            }
            
            delete generator;
            generator = NULL;
        }

    }
    
    finish(false);
}
