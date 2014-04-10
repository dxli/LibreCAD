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
#include "qg_floweroptions.h"

#include "rs_settings.h"

/*
 *  Constructs a QG_FlowerOptions as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_FlowerOptions::QG_FlowerOptions(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_FlowerOptions::~QG_FlowerOptions()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}



/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_FlowerOptions::languageChange()
{
    retranslateUi(this);
}



void QG_FlowerOptions::destroy() {
    RS_SETTINGS->beginGroup("/Draw");
    RS_SETTINGS->writeEntry("/FlowerPetal", lePetal->text());
    RS_SETTINGS->endGroup();
}



void QG_FlowerOptions::setAction(RS_ActionInterface* a, bool update) {
    if (a!=NULL && a->rtti()==RS2::ActionDrawFlower) {
        action = (RS_ActionDrawFlower*)a;

        QString sr;
        if (update) {
            sr = QString("%1").arg(action->getPetal());
        } else {
            RS_SETTINGS->beginGroup("/Draw");
            sr = RS_SETTINGS->readEntry("/FlowerPetal", "6");
            RS_SETTINGS->endGroup();
            action->setPetal(sr.toInt());
        }
        lePetal->setText(sr);
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "QG_FlowerOptions::setAction: wrong action type");
        action = NULL;
    }

}



void QG_FlowerOptions::updatePetal(const QString& r) {
    if (action!=NULL) {
        action->setPetal(RS_Math::eval(r));
    }
}
