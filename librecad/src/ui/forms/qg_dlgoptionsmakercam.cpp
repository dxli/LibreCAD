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

#include "qg_dlgoptionsmakercam.h"

#include "rs_settings.h"

QG_DlgOptionsMakerCam::QG_DlgOptionsMakerCam(QWidget* parent, bool modal, Qt::WindowFlags fl) : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    loadSettings();
}

QG_DlgOptionsMakerCam::~QG_DlgOptionsMakerCam()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_DlgOptionsMakerCam::languageChange()
{
    retranslateUi(this);
}

void QG_DlgOptionsMakerCam::validate() {

    saveSettings();

    accept();
}

void QG_DlgOptionsMakerCam::cancel() {
    reject();
}

void QG_DlgOptionsMakerCam::loadSettings() {

    RS_SETTINGS->beginGroup("/ExportMakerCam");

    updateCheckbox(checkInvisibleLayers, "ExportInvisibleLayers", 0);
    updateCheckbox(checkConstructionLayers, "ExportConstructionLayers", 0);
    updateCheckbox(checkBlocksInline, "WriteBlocksInline", 1);
    updateCheckbox(checkEllipsesToPaths, "ConvertEllipsesToPaths", 0);

    RS_SETTINGS->endGroup();
}

void QG_DlgOptionsMakerCam::updateCheckbox(QCheckBox* checkbox, QString name, int defaultValue) {

    checkbox->setChecked(RS_SETTINGS->readNumEntry("/" + name, defaultValue) ? true : false);
}

void QG_DlgOptionsMakerCam::saveSettings() {

    RS_SETTINGS->beginGroup("/ExportMakerCam");

    saveBoolean("ExportInvisibleLayers", checkInvisibleLayers);
    saveBoolean("ExportConstructionLayers", checkConstructionLayers);
    saveBoolean("WriteBlocksInline", checkBlocksInline);
    saveBoolean("ConvertEllipsesToPaths", checkEllipsesToPaths);

    RS_SETTINGS->endGroup();
}

void QG_DlgOptionsMakerCam::saveBoolean(QString name, QCheckBox* checkbox) {

    RS_SETTINGS->writeEntry("/" + name, checkbox->isChecked() ? 1 : 0);
}
