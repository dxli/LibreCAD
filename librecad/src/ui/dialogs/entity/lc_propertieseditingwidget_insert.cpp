/*
 * ********************************************************************************
 * This file is part of the LibreCAD project, a 2D CAD program
 *
 * Copyright (C) 2025 LibreCAD.org
 * Copyright (C) 2025 sand1024
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * ********************************************************************************
 */

#include "lc_propertieseditingwidget_insert.h"

#include <cmath>

#include <QSpinBox>

#include "rs_insert.h"
#include "rs_math.h"
#include "ui_lc_propertieseditingwidget_insert.h"

#define ALLOW_PICK_SCALE false

namespace {
// A factor a reference can use: zero and infinity empty it, a negative sign would mirror it.
bool isScaleFactor(const double value) {
    return std::isfinite(value) && value > 0.;
}
}

LC_PropertiesEditingWidgetInsert::LC_PropertiesEditingWidgetInsert(QWidget *parent)
    : LC_EntityPropertiesEditorWidget(parent)
    , ui(new Ui::LC_InsertPropertiesEditingWidget){
    ui->setupUi(this);
    connect(ui->leInsertionPointX, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onInsertionPointEditingFinished);
    connect(ui->leInsertionPointY, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onInsertionPointEditingFinished);
    connect(ui->leScaleX, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onScaleEditingFinishedChanged);
    connect(ui->leScaleY, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onScaleEditingFinishedChanged);
    connect(ui->leAngle, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onAngleEditingFinished);
    ui->sbRows->setRange(1, RS_InsertData::MaxArrayCount);
    ui->sbCols->setRange(1, RS_InsertData::MaxArrayCount);
    connect(ui->sbRows, &QSpinBox::valueChanged, this, &LC_PropertiesEditingWidgetInsert::onRowsValueChanged);
    connect(ui->sbCols, &QSpinBox::valueChanged, this, &LC_PropertiesEditingWidgetInsert::onColsValueChanged);
    connect(ui->leColSpacing, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onSpacingEditingFinished);
    connect(ui->leRowSpacing, &QLineEdit::editingFinished, this, &LC_PropertiesEditingWidgetInsert::onSpacingEditingFinished);
    if (!ALLOW_PICK_SCALE) {
        ui->tbPIckScaleX->setVisible(false);
        ui->tbPIckScaleY->setVisible(false);
    }
}

LC_PropertiesEditingWidgetInsert::~LC_PropertiesEditingWidgetInsert(){
    delete ui;
}

void LC_PropertiesEditingWidgetInsert::onInsertionPointEditingFinished() const {
    const RS_Vector current = m_entity->getInsertionPoint();
    const RS_Vector point = toWCS(ui->leInsertionPointX, ui->leInsertionPointY, current);
    // An entry that is no finite number (1/0) is not taken: infinity and NaN would empty the reference.
    if (std::isfinite(point.x) && std::isfinite(point.y)) {
        m_entity->setInsertionPoint(point);
    }
    else {
        toUI(current, ui->leInsertionPointX, ui->leInsertionPointY);
    }
}

void LC_PropertiesEditingWidgetInsert::onScaleEditingFinishedChanged() const {
    const RS_Vector current = m_entity->getScale();
    RS_Vector scale = toWCSRaw(ui->leScaleX,ui->leScaleY, current);
    // Only a positive factor is taken: any other entry is ignored, as an unusable count is.
    // A mirrored reference keeps its sign until a positive factor is entered.
    if (!isScaleFactor(scale.x)) {
        scale.x = current.x;
    }
    if (!isScaleFactor(scale.y)) {
        scale.y = current.y;
    }
    m_entity->setScale(scale);
    // The fields show what the reference holds: an entry it did not take must not stay.
    toUIRaw(m_entity->getScale(), ui->leScaleX, ui->leScaleY);
}

void LC_PropertiesEditingWidgetInsert::onAngleEditingFinished() const {
    const double angle = toWCSAngle(ui->leAngle, m_entity->getAngle());
    if (std::isfinite(angle)) {
        m_entity->setAngle(angle);
    }
    else {
        toUIAngleDeg(m_entity->getAngle(), ui->leAngle);
    }
}

void LC_PropertiesEditingWidgetInsert::onRowsValueChanged(const int value) const {
    m_entity->setRows(value);
}

void LC_PropertiesEditingWidgetInsert::onColsValueChanged(const int value) const {
    m_entity->setCols(value);
}

void LC_PropertiesEditingWidgetInsert::onSpacingEditingFinished() const {
    const RS_Vector current = m_entity->getSpacing();
    RS_Vector spacing(toWCSValue(ui->leColSpacing, current.x), toWCSValue(ui->leRowSpacing, current.y));
    // An entry that is no finite number is not taken, nor is text that is no number:
    // the fields show what the reference holds.
    if (!std::isfinite(spacing.x)) {
        spacing.x = current.x;
    }
    if (!std::isfinite(spacing.y)) {
        spacing.y = current.y;
    }
    m_entity->setSpacing(spacing);
    toUIValue(m_entity->getSpacing().x, ui->leColSpacing);
    toUIValue(m_entity->getSpacing().y, ui->leRowSpacing);
}

void LC_PropertiesEditingWidgetInsert::setupInteractiveInputWidgets() {
    pickPointSetup(ui->wPickInsertionPoint, "insert", ui->leInsertionPointX, ui->leInsertionPointY);
    if (!ALLOW_PICK_SCALE) {
        pickDistanceSetup(ui->tbPIckScaleX,  "scaleX",ui->leScaleX);
        pickDistanceSetup(ui->tbPIckScaleY, "scaleY",ui->leScaleY);
    }
    pickAngleSetup(ui->tbPickAngle, "angle",ui->leAngle);
    pickDistanceSetup(ui->tbPickRowSpacing,  "rowSpacing",ui->leRowSpacing);
    pickDistanceSetup(ui->tbPickColumnSpacing,  "colSpacing",ui->leColSpacing);
}

void LC_PropertiesEditingWidgetInsert::setEntity(RS_Entity* entity) {
    m_entity = static_cast<RS_Insert*>(entity);

    toUI(m_entity->getInsertionPoint(), ui->leInsertionPointX, ui->leInsertionPointY);
    toUIRaw(m_entity->getScale(), ui->leScaleX, ui->leScaleY);
    toUIAngleDeg(m_entity->getAngle(), ui->leAngle);

    ui->sbRows->setValue(m_entity->getRows());
    ui->sbCols->setValue(m_entity->getCols());
    // A field leaving the focus stores what it shows: setNum's six digits would round the spacing,
    // so use the precision of the other fields.
    toUIValue(m_entity->getSpacing().y, ui->leRowSpacing);
    toUIValue(m_entity->getSpacing().x, ui->leColSpacing);
}
