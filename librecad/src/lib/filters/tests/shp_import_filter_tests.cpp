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

/**
 * Phase-2d smoke test for RS_FilterSHP.  Full corpus matrix lands in Phase 4.
 * Only the minimal happy-path assertion is here: opening a well-formed POINT
 * shapefile produces the expected native entity count.
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <filesystem>

#include <QCoreApplication>
#include <QString>

#include "lc_containertraverser.h"
#include "rs.h"
#include "rs_filtershp.h"
#include "rs_graphic.h"
#include "rs_point.h"
#include "rs_settings.h"

namespace {

QString corpusPath(const char* relative) {
    return QString(LIBRECAD_SOURCE_DIR) + "/test_data/shp/" + relative;
}

// RS_Graphic's ctor + LC_GROUP-driven RS_Settings reads dereference null
// without a QCoreApplication context — mirror the bootstrap that
// dwg_header_app_vars_tests.cpp / rs_graphic_layouts_tests.cpp use.
void ensureQtContext() {
    static int qargc = 1;
    static char qarg0[] = "librecad_tests";
    static char* qargv[] = {qarg0, nullptr};
    static QCoreApplication* qapp = QCoreApplication::instance()
        ? QCoreApplication::instance()
        : new QCoreApplication(qargc, qargv);
    (void)qapp;
    static bool settingsReady = [] {
        QCoreApplication::setOrganizationName("LibreCAD");
        QCoreApplication::setApplicationName("LibreCAD-tests");
        RS_Settings::init("LibreCAD", "LibreCAD-tests");
        return true;
    }();
    (void)settingsReady;
}

} // namespace

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("RS_FilterSHP: imports a basic POINT shapefile into native RS_Points",
          "[shp][filter][smoke]") {
    ensureQtContext();
    const QString path = corpusPath("points.shp");
    REQUIRE(std::filesystem::is_regular_file(path.toStdString()));

    RS_Graphic graphic;
    RS_FilterSHP filter;

    REQUIRE(filter.canImport(path, RS2::FormatSHP));
    REQUIRE_FALSE(filter.canExport(path, RS2::FormatSHP));

    const bool ok = filter.fileImport(graphic, path, RS2::FormatSHP);
    REQUIRE(ok);

    // Corpus inventory: points.shp has 9 records, all SHPT_POINT.  Filter
    // emits exactly one RS_Point per record — no MText (label field absent
    // from points.dbf; only FID is present).
    int nPoints = 0;
    for (RS_Entity* e :
         lc::LC_ContainerTraverser{graphic, RS2::ResolveAll}.entities()) {
        if (e && e->rtti() == RS2::EntityPoint) ++nPoints;
    }
    CHECK(nPoints == 9);
}
