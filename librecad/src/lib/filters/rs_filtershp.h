/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2026 LibreCAD (librecad.org)
** Copyright (C) 2026 Dongxu Li (github.com/dxli)
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
**********************************************************************/

#ifndef RS_FILTERSHP_H
#define RS_FILTERSHP_H

#include <QString>

#include "rs.h"
#include "rs_filterinterface.h"

class RS_Graphic;

/**
 * ESRI Shapefile (SHP) import filter — read-only.
 *
 * File-format background
 * ----------------------
 * A "shapefile" is actually a set of three files sharing a basename:
 *   .shp — geometry records
 *   .shx — record-index sidecar (SHPOpen requires it or is offered
 *          SHPOpenLLEx with bRestoreSHX; this filter uses the read-only path)
 *   .dbf — dBase-III attribute table (one row per .shp record)
 * plus optional companions: .prj (CRS), .cpg (DBF codepage marker).
 *
 * Design decisions
 * ----------------
 *   - **Import only.** canExport() always returns false; there is no writer.
 *     SHP write support is an explicit non-goal of the plan.
 *   - **Native entities** — RS_Point / RS_Polyline (open for ARC, closed for
 *     POLYGON) / RS_MText for DBF-derived labels.  Full per-entity RS_Pen
 *     (color, linetype, width) — the capability the retired importshp plugin
 *     could not deliver via Document_Interface.
 *   - **Z preserved** — POINTZ / ARCZ / POLYGONZ / MULTIPOINTZ / POINTM /
 *     ARCM / POLYGONM / MULTIPOINTM records store their Z (or M interpreted
 *     as Z) in RS_Vector; 2D views ignore it, but the data survives.
 *   - **DBF-driven layers + styling** — auto-detected fields (LAYER / COLOR
 *     / LINETYPE / WIDTH / LABEL family) map to RS_Layer + per-entity RS_Pen.
 *     Codepage-aware string decoding (UTF-8 / LDID/xx).
 *   - **Missing .dbf** — geometry-only import lands on layer "0", ByLayer pen.
 *   - **Malformed / corrupt / missing-.shx** — shapelib returns null; filter
 *     logs a warning and either returns false (open failed outright) or
 *     partial import (some records readable).  Never crashes: pinned by the
 *     Phase-0 corpus tests over the hostile fixtures.
 *
 * Data flow (matches the plan section 1):
 *   File→Open → QG_FileDialog(*.shp) → LC_DocumentsStorage::loadGraphic →
 *   RS_FileIO::fileImport → RS_FilterSHP::fileImport
 */
class RS_FilterSHP : public RS_FilterInterface {
public:
    RS_FilterSHP() = default;
    ~RS_FilterSHP() override = default;

    /** @return true iff @p t is RS2::FormatSHP. */
    bool canImport(const QString& /*fileName*/,
                   const RS2::FormatType t) const override {
        return t == RS2::FormatSHP;
    }

    /** SHP export is out of scope. Always false. */
    bool canExport(const QString& /*fileName*/,
                   const RS2::FormatType /*t*/) const override {
        return false;
    }

    bool fileImport(RS_Graphic& g, const QString& file,
                    RS2::FormatType type) override;

    /** SHP export is out of scope. Always false. */
    bool fileExport(RS_Graphic& /*g*/, const QString& /*file*/,
                    RS2::FormatType /*type*/) override {
        return false;
    }

    QString lastError() const override { return m_lastError; }

    /** Factory hook registered in RS_FileIO::getFilters(). */
    static RS_FilterInterface* createFilter() { return new RS_FilterSHP(); }

private:
    /** Human-readable diagnostic set by fileImport() on failure/degradation. */
    QString m_lastError;
};

#endif // RS_FILTERSHP_H
