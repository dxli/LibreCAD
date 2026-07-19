/******************************************************************************
 *
 * Project:  LibreCAD — ESRI Shapefile Import Plugin
 * Purpose:  Import ESRI Shapefile (.shp) geometry and DBF attributes
 *           into LibreCAD drawings.
 *
 * Copyright (C) 2011-2013 Rallaz (original importshp plugin)
 * Copyright (C) 2026 LibreCAD contributors (restored and modernized)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * shapelib (bundled in shapelib/) is available under MIT or LGPL-2.0-or-later.
 *
 ******************************************************************************/

#include "importshp.h"

#include <array>
#include <vector>

/* ---------------------------------------------------------------------------
 * Plugin Interface
 * --------------------------------------------------------------------------- */

PluginCapabilities ImportShp::getCapabilities() const {
    PluginCapabilities caps;
    caps.menuEntryPoints
        << PluginMenuLocation("plugins_menu", tr("ESRI Shapefile"),
            tr("Import ESRI Shapefile (.shp)"));
    return caps;
}

QString ImportShp::name() const {
    return tr("ESRI Shapefile Import");
}

void ImportShp::execComm(Document_Interface* doc, QWidget* parent,
                         QString /*cmd*/) {
    dibSHP dlg(parent);
    dlg.currDoc = doc;
    dlg.exec();
}

/* ---------------------------------------------------------------------------
 * Dialog Construction
 * --------------------------------------------------------------------------- */

/**
 * @brief Helper: create a "Current / From data:" radio pair with combo.
 */
static QGroupBox* makeAttrGroup(const QString& title, QComboBox*& combo,
                                QRadioButton*& radioCurrent,
                                QRadioButton*& radioFromData) {
    QGroupBox* box = new QGroupBox(title);
    radioCurrent = new QRadioButton(QObject::tr("Current"));
    radioFromData = new QRadioButton(QObject::tr("From data:"));
    combo = new QComboBox();
    QHBoxLayout* lo = new QHBoxLayout;
    lo->addWidget(radioCurrent);
    lo->addWidget(radioFromData);
    lo->addWidget(combo);
    lo->addStretch(1);
    box->setLayout(lo);
    radioCurrent->setChecked(true);
    return box;
}

dibSHP::dibSHP(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr("Import ESRI Shapefile"));

    // ---- File selection ----
    fileButton = new QPushButton(tr("File..."));
    fileEdit = new QLineEdit();
    QHBoxLayout* loFile = new QHBoxLayout;
    loFile->addWidget(fileButton);
    loFile->addWidget(fileEdit);
    loFile->setSizeConstraint(QLayout::SetFixedSize);

    // ---- File type label ----
    QLabel* formatLabel = new QLabel(tr("File type:"));
    formatType = new QLabel(tr("Unknown"));
    QHBoxLayout* loFormat = new QHBoxLayout;
    loFormat->addWidget(formatLabel);
    loFormat->addWidget(formatType);
    loFormat->addStretch();

    // ---- Attribute groups (layer, color, linetype, width) ----
    layBox = makeAttrGroup(tr("Layer"), layerData,
                           radioLayerCurrent, radioLayerFromData);
    colBox = makeAttrGroup(tr("Color"), colorData,
                           radioColorCurrent, radioColorFromData);
    ltypeBox = makeAttrGroup(tr("Line type"), ltypeData,
                             radioLtypeCurrent, radioLtypeFromData);
    lwidthBox = makeAttrGroup(tr("Width"), lwidthData,
                              radioLwidthCurrent, radioLwidthFromData);

    // ---- Point label section ----
    pointBox = new QGroupBox(tr("Point"));
    radioPointAsPoint = new QRadioButton(tr("as Point"));
    radioPointAsLabel = new QRadioButton(tr("as Label:"));
    pointData = new QComboBox();
    QVBoxLayout* pointLayout = new QVBoxLayout;
    pointLayout->addWidget(radioPointAsPoint);
    pointLayout->addWidget(radioPointAsLabel);
    pointLayout->addWidget(pointData);
    pointLayout->addStretch(1);
    pointBox->setLayout(pointLayout);
    radioPointAsPoint->setChecked(true);
    pointBox->setDisabled(true);  // disabled until file loaded

    // ---- Accept / Cancel ----
    acceptButton = new QPushButton(tr("Accept"));
    cancelButton = new QPushButton(tr("Cancel"));
    QHBoxLayout* loAccept = new QHBoxLayout;
    loAccept->addStretch();
    loAccept->addWidget(acceptButton);
    loAccept->addWidget(cancelButton);

    // ---- Main layout ----
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(loFile);
    mainLayout->addLayout(loFormat);
    mainLayout->addWidget(layBox);
    mainLayout->addWidget(colBox);
    mainLayout->addWidget(ltypeBox);
    mainLayout->addWidget(lwidthBox);
    mainLayout->addWidget(pointBox);
    mainLayout->addLayout(loAccept);
    setLayout(mainLayout);

    // ---- Connections (Qt6 function-pointer syntax) ----
    connect(cancelButton, &QPushButton::clicked, this, &dibSHP::reject);
    connect(acceptButton, &QPushButton::clicked, this, &dibSHP::checkAccept);
    connect(fileButton, &QPushButton::clicked, this, &dibSHP::getFile);
    connect(fileEdit, &QLineEdit::editingFinished, this, &dibSHP::updateFile);

    // ---- Settings ----
    readSettings();
    updateFile();
}

/* ---------------------------------------------------------------------------
 * File Selection
 * --------------------------------------------------------------------------- */

void dibSHP::getFile() {
    if (const QString& fileName = QFileDialog::getOpenFileName(
            this, tr("Select file"),
            fileEdit->text(),
            tr("ESRI Shapefiles (*.shp)"));
        !fileName.isEmpty()) {
        fileEdit->setText(fileName);
        updateFile();
    }
}

/* ---------------------------------------------------------------------------
 * File Preview — populate combo boxes with DBF field names
 * --------------------------------------------------------------------------- */

void dibSHP::updateFile() {
    const QString& fileName = fileEdit->text();
    if (fileName.isEmpty()) {
        formatType->setText(tr("Unknown"));
        pointBox->setDisabled(true);
        return;
    }

    QFileInfo fi(fileName);
    if (fi.suffix().toLower() != "shp") {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("The file %1 does not have the .shp extension"))
                .arg(fileName));
        return;
    }

    QString file = fi.canonicalFilePath();
    if (file.isEmpty()) {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("The file %1 does not exist")).arg(fileName));
        return;
    }

    int numEnt, shapeType;

    // Use QFile::encodeName() instead of deprecated toLocal8Bit()
    const QByteArray filePath = QFile::encodeName(file);
    ScopedSHPHandle sh{SHPOpen(filePath.constData(), "rb")};
    if (!sh.sh) {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("Cannot open shapefile: %1")).arg(file));
        return;
    }
    SHPGetInfo(sh.sh, &numEnt, &shapeType, nullptr, nullptr);

    // Update format type label — use switch-case for non-sequential SHP types
    switch (shapeType) {
    case SHPT_NULL:       formatType->setText(tr("Unknown")); break;
    case SHPT_POINT:      formatType->setText(tr("Point")); break;
    case SHPT_POINTM:     formatType->setText(tr("Point+Measure")); break;
    case SHPT_POINTZ:     formatType->setText(tr("3D Point")); break;
    case SHPT_MULTIPOINT: formatType->setText(tr("Multi Point")); break;
    case SHPT_MULTIPOINTM:formatType->setText(tr("Multi Point+Measure")); break;
    case SHPT_MULTIPOINTZ:formatType->setText(tr("3D Multi Point")); break;
    case SHPT_ARC:        formatType->setText(tr("Arc")); break;
    case SHPT_ARCM:       formatType->setText(tr("Arc+Measure")); break;
    case SHPT_ARCZ:       formatType->setText(tr("3D Arc")); break;
    case SHPT_POLYGON:    formatType->setText(tr("Polygon")); break;
    case SHPT_POLYGONM:   formatType->setText(tr("Polygon+Measure")); break;
    case SHPT_POLYGONZ:   formatType->setText(tr("3D Polygon")); break;
    case SHPT_MULTIPATCH: formatType->setText(tr("Multipatch")); break;
    default:              formatType->setText(tr("Unknown")); break;
    }

    // Enable/disable point label section based on geometry type
    const bool isPointType = (shapeType == SHPT_POINT ||
                              shapeType == SHPT_POINTZ ||
                              shapeType == SHPT_POINTM ||
                              shapeType == SHPT_MULTIPOINT ||
                              shapeType == SHPT_MULTIPOINTZ ||
                              shapeType == SHPT_MULTIPOINTM);
    pointBox->setEnabled(isPointType);

    // Populate combo boxes with DBF field names
    ScopedDBFHandle dh{DBFOpen(filePath.constData(), "rb")};
    if (!dh.dh) {
        formatType->setText(tr("Unknown"));
        return;
    }

    const int numFields = DBFGetFieldCount(dh.dh);
    QStringList fieldNames;
    fieldNames.reserve(numFields);
    for (int i = 0; i < numFields; ++i) {
        std::array<char, 12> fieldName{};
        int fieldWidth{};
        int fieldDecimals{};
        DBFGetFieldInfo(dh.dh, i, fieldName.data(), &fieldWidth, &fieldDecimals);
        if (!fieldName.empty()) {
            fieldNames << QString::fromUtf8(fieldName.data());
        }
    }
    fieldNames.sort();

    layerData->clear(); layerData->addItems(fieldNames);
    colorData->clear(); colorData->addItems(fieldNames);
    ltypeData->clear(); ltypeData->addItems(fieldNames);
    lwidthData->clear(); lwidthData->addItems(fieldNames);
    pointData->clear(); pointData->addItems(fieldNames);
}

/* ---------------------------------------------------------------------------
 * Settings Persistence
 * --------------------------------------------------------------------------- */

void dibSHP::readSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "LibreCAD", "importshp");
    const QPoint pos = settings.value("pos", QPoint{200, 200}).toPoint();
    const QSize size = settings.value("size", QSize{325, 425}).toSize();
    fileEdit->setText(settings.value("lastfile").toString());
    resize(size);
    move(pos);
}

void dibSHP::writeSettings() const {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "LibreCAD", "importshp");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("lastfile", fileEdit->text());
}

/* ---------------------------------------------------------------------------
 * Accept / Cancel
 * --------------------------------------------------------------------------- */

void dibSHP::checkAccept() {
    writeSettings();
    accept();
}

/* ---------------------------------------------------------------------------
 * Import Orchestration
 * --------------------------------------------------------------------------- */

void dibSHP::procesFile(Document_Interface* doc) {
    currDoc = doc;

    // Save current layer and restore at end
    currentLayer = currDoc->getCurrentLayer();

    QString file = fileEdit->text();
    QFileInfo fi(file);
    if (!fi.exists()) {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("The file %1 does not exist")).arg(file));
        currDoc = nullptr;
        return;
    }

    file = fi.canonicalFilePath();
    if (file.isEmpty()) {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("The file %1 does not exist")).arg(file));
        currDoc = nullptr;
        return;
    }

    // Open SHP and DBF with RAII
    const QByteArray filePath = QFile::encodeName(file);
    ScopedSHPHandle sh{SHPOpen(filePath.constData(), "rb")};
    if (!sh.sh) {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("Cannot open shapefile: %1")).arg(file));
        currDoc = nullptr;
        return;
    }

    int numEnt, shapeType;
    SHPGetInfo(sh.sh, &numEnt, &shapeType, nullptr, nullptr);

    ScopedDBFHandle dh{DBFOpen(filePath.constData(), "rb")};
    if (!dh.dh) {
        QMessageBox::critical(this, tr("Shapefile"),
            QString(tr("Cannot open DBF: %1")).arg(file));
        currDoc = nullptr;
        return;
    }

    // Resolve field indices from DBF using generic helper
    resolveFieldIndex(radioLayerFromData, layerData, dh.dh, layerF);
    resolveFieldIndex(radioColorFromData, colorData, dh.dh, colorF);
    resolveFieldIndex(radioLtypeFromData, ltypeData, dh.dh, ltypeF);
    resolveFieldIndex(radioLwidthFromData, lwidthData, dh.dh, lwidthF);
    resolveFieldIndex(radioPointAsLabel, pointData, dh.dh, pointF);

    // Iterate over all shape records
    for (int recordIndex = 0; recordIndex < numEnt; ++recordIndex) {
        sObject = SHPReadObject(sh.sh, recordIndex);
        if (sObject) {
            switch (sObject->nSHPType) {
            case SHPT_POINT:
            case SHPT_POINTM:
            case SHPT_POINTZ:
                readPoint(dh.dh, recordIndex);
                break;
            case SHPT_MULTIPOINT:
            case SHPT_MULTIPOINTM:
            case SHPT_MULTIPOINTZ:
                readMultiPoint(dh.dh, recordIndex);
                break;
            case SHPT_ARC:
            case SHPT_ARCM:
            case SHPT_ARCZ:
                readPolyline(dh.dh, recordIndex);
                break;
            case SHPT_POLYGON:
            case SHPT_POLYGONM:
            case SHPT_POLYGONZ:
                readPolygon(dh.dh, recordIndex);
                break;
            case SHPT_MULTIPATCH:
                readMultiPoint(dh.dh, recordIndex);
                break;
            case SHPT_NULL:
            default:
                // Null shapes and unknown types are silently skipped
                break;
            }
            SHPDestroyObject(sObject);
            sObject = nullptr;
        }
    }

    // Restore original layer
    currDoc->setLayer(currentLayer);
    currDoc = nullptr;
}

/* ---------------------------------------------------------------------------
 * Geometry Readers
 * --------------------------------------------------------------------------- */

/**
 * @brief Read a POINT (or POINTZ, POINTM) shape as a CAD point.
 *
 * If a point label field is configured, creates both a POINT entity and
 * an optional text label from the DBF attribute.
 */
void dibSHP::readPoint(DBFHandle dh, int recordIndex) {
    readAttributes(dh, recordIndex);

    const qreal x = static_cast<qreal>(sObject->padfX[0]);
    const qreal y = static_cast<qreal>(sObject->padfY[0]);

    // Optional text label from DBF attribute
    if (pointF >= 0) {
        const char* labelText = DBFReadStringAttribute(dh, recordIndex, pointF);
        if (labelText) {
            const QString text = QString::fromUtf8(labelText);
            if (!text.isEmpty()) {
                QPointF pt(x, y);
                currDoc->addText(text, QString{}, &pt, 2.0, 0.0,
                                 DPI::HAlignLeft, DPI::VAlignMiddle);
            }
        }
    }

    // Point entity
    QPointF pt(x, y);
    currDoc->addPoint(&pt);
}

/**
 * @brief Read an ARC (or ARCZ, ARCM) shape as open polylines.
 *
 * Each part of the multi-part arc becomes a separate POLYLINE entity.
 * Parts with fewer than 3 vertices are skipped.
 *
 * Uses processParts() for shared ring/part iteration logic.
 */
void dibSHP::readPolyline(DBFHandle dh, int recordIndex) {
    processParts(dh, recordIndex, /*closed=*/false, /*minVerts=*/3);
}

/**
 * @brief Read a POLYGON (or POLYGONZ, POLYGONM) shape.
 *
 * Each ring becomes a separate closed POLYLINE. LibreCAD doesn't support
 * true polygon holes, so all rings are drawn as closed polylines.
 *
 * Uses processParts() for shared ring/part iteration logic.
 */
void dibSHP::readPolygon(DBFHandle dh, int recordIndex) {
    processParts(dh, recordIndex, /*closed=*/true, /*minVerts=*/3);
}

/**
 * @brief Read a MULTIPOINT (or MULTIPOINTZ, MULTIPOINTM) or MULTIPATCH shape.
 *
 * Each vertex in the shape becomes an individual POINT entity.
 */
void dibSHP::readMultiPoint(DBFHandle dh, int recordIndex) {
    readAttributes(dh, recordIndex);

    for (int j = 0; j < sObject->nVertices; ++j) {
        QPointF pt(static_cast<qreal>(sObject->padfX[j]),
                   static_cast<qreal>(sObject->padfY[j]));
        currDoc->addPoint(&pt);
    }
}

/**
 * @brief Process shape parts or rings into polylines.
 *
 * Shared implementation for readPolyline() and readPolygon().
 * Each part/ring becomes a separate polyline entity.
 *
 * @param dh        DBF handle for attribute reading.
 * @param recordIndex  Current shape record index.
 * @param closed    true for polygon rings (closed), false for arcs (open).
 * @param minVerts  Minimum vertices per part to create an entity (default 3).
 */
void dibSHP::processParts(DBFHandle dh, int recordIndex,
                          bool closed, int minVerts) {
    readAttributes(dh, recordIndex);

    for (int partIdx = 0; partIdx < sObject->nParts; ++partIdx) {
        const int partStart = sObject->panPartStart[partIdx];
        const int partEnd = (partIdx + 1 < sObject->nParts)
            ? sObject->panPartStart[partIdx + 1]
            : sObject->nVertices;

        const int vertexCount = partEnd - partStart;
        if (vertexCount < minVerts) {
            continue;
        }

        std::vector<Plug_VertexData> vertices;
        vertices.reserve(static_cast<size_t>(vertexCount));

        for (int j = partStart; j < partEnd; ++j) {
            vertices.emplace_back(
                QPointF(static_cast<qreal>(sObject->padfX[j]),
                        static_cast<qreal>(sObject->padfY[j])),
                0.0
            );
        }

        currDoc->addPolyline(vertices, closed);
    }
}

/* ---------------------------------------------------------------------------
 * Attribute Reading
 * --------------------------------------------------------------------------- */

/**
 * @brief Read layer from DBF field.
 *
 * Only layer is applied (setLayer). Color/linetype/width are parsed
 * but not applied — the Document_Interface API doesn't expose per-entity
 * styling through the direct drawing commands.
 */
void dibSHP::readAttributes(DBFHandle dh, int recordIndex) {
    // Layer — this is the only attribute actually applied
    if (layerF >= 0) {
        const char* layerStr = DBFReadStringAttribute(dh, recordIndex, layerF);
        if (layerStr) {
            currDoc->setLayer(QString::fromUtf8(layerStr));
        }
    }
    // Color, linetype, width are parsed but not applied
    // (Document_Interface direct API has no per-entity styling)
    (void)colorF;
    (void)ltypeF;
    (void)lwidthF;
}
