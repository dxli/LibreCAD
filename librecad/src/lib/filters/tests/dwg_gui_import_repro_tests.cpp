/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2026 LibreCAD (librecad.org)
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
**********************************************************************/

/**
 * GUI-import repro for usa_dollar100_front.dwg (AC1021, 30 MB, 41364 3D
 * polylines / 1.16M vertices). The libdxfrw READ is clean (0 parse failures),
 * but the full RS_FilterDXFRW::fileImport -> RS_Graphic build path (what the
 * GUI uses) reportedly fails/crashes. This dev-local test drives exactly that
 * path so the failure can be reproduced and located.
 */

#include <catch2/catch_test_macros.hpp>

#include <QApplication>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "rs_debug.h"
#include "rs_entitycontainer.h"
#include "rs_filterdxfrw.h"
#include "rs_graphic.h"
#include "rs_block.h"
#include "rs_layer.h"
#include "rs_settings.h"

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("GUI import of usa_dollar100_front.dwg completes", "[.dwg6_gui_import]") {
  const char *home = std::getenv("HOME");
  if (!home) {
    SUCCEED("HOME not set; skipping");
    return;
  }
  const std::string path =
      std::string(home) + "/doc/dwg6/usa_dollar100_front.dwg";
  if (!std::filesystem::is_regular_file(path)) {
    SUCCEED("usa_dollar100_front.dwg not present; skipping");
    return;
  }

  static int qargc = 1;
  static char qarg0[] = "librecad_tests";
  static char *qargv[] = {qarg0, nullptr};
  static QApplication *qapp = [] {
    auto *existing = qobject_cast<QApplication*>(QCoreApplication::instance());
    return existing ? existing : new QApplication(qargc, qargv);
  }();
  static bool settingsReady = [] {
    QApplication::setOrganizationName("LibreCAD");
    QApplication::setApplicationName("LibreCAD-tests");
    RS_Settings::init("LibreCAD", "LibreCAD-tests");
    return true;
  }();
  (void)qapp;
  (void)settingsReady;

  // Measure the true GUI cost at the default (non-verbose) log level -- the
  // test singleton otherwise inits at D_DEBUGGING, which floods a per-entity
  // vfprintf+fflush over 1.16M entities and dwarfs the real work.
  RS_DEBUG->setLevel(RS_Debug::D_NOTHING);

  RS_Graphic graphic;
  RS_FilterDXFRW filter;
  const bool imported =
      filter.fileImport(graphic, QString::fromStdString(path), RS2::FormatDWG);
  REQUIRE(imported);

  int top = 0;
  int polylines = 0;
  for (auto *e : graphic) {
    if (!e)
      continue;
    ++top;
    if (e->rtti() == RS2::EntityPolyline)
      ++polylines;
  }
  INFO("top-level entities: " << top << " polylines: " << polylines);
  // dwgread oracle: 41364 3D polylines + 4 LWPOLYLINE. The import must build
  // them all -- and (the point of this test) it must do so in seconds, not the
  // ~10 minutes it took before RS_Polyline::addVertex stopped calling
  // endPolyline() (a full O(N) calculateBorders) on every vertex (O(N^2)).
  CHECK(polylines >= 41368);
}

TEST_CASE("GUI import of makeall-plus.dwg counts entities", "[.dwg_makeall_gui]") {
  const char *home = std::getenv("HOME");
  if (!home) {
    SUCCEED("HOME not set; skipping");
    return;
  }
  const std::string path =
      std::string(home) + "/doc/dwg3/makeall-plus.dwg";
  if (!std::filesystem::is_regular_file(path)) {
    SUCCEED("makeall-plus.dwg not present; skipping");
    return;
  }

  static int qargc = 1;
  static char qarg0[] = "librecad_tests";
  static char *qargv[] = {qarg0, nullptr};
  static QApplication *qapp = [] {
    auto *existing = qobject_cast<QApplication*>(QCoreApplication::instance());
    return existing ? existing : new QApplication(qargc, qargv);
  }();
  static bool settingsReady = [] {
    QApplication::setOrganizationName("LibreCAD");
    QApplication::setApplicationName("LibreCAD-tests");
    RS_Settings::init("LibreCAD", "LibreCAD-tests");
    return true;
  }();
  (void)qapp;
  (void)settingsReady;

  RS_DEBUG->setLevel(RS_Debug::D_NOTHING);

  RS_Graphic graphic;
  RS_FilterDXFRW filter;
  const bool imported =
      filter.fileImport(graphic, QString::fromStdString(path), RS2::FormatDWG);
  REQUIRE(imported);

  int nTop = 0;
  int nBlocks = 0;
  int nBlockEnts = 0;
  for (auto *e : graphic) {
    if (e) ++nTop;
  }
  nBlocks = graphic.countBlocks();
  for (unsigned i = 0; i < nBlocks; ++i) {
    auto* b = graphic.blockAt(i);
    if (b) {
      for (auto *e : *b) {
        if (e) ++nBlockEnts;
      }
    }
  }
  INFO("top-level entities: " << nTop << " blocks: " << nBlocks << " block entities: " << nBlockEnts);
  std::cout << "\n=== makeall-plus.dwg GUI import ===\n";
  std::cout << "top-level: " << nTop << "\n";
  std::cout << "blocks: " << nBlocks << "\n";
  std::cout << "entities in blocks: " << nBlockEnts << "\n";
  CHECK(nTop > 0);
}

TEST_CASE("GUI import of visualization_condominium.dwg counts entities", "[.dwg_condo]") {
  const char *home = std::getenv("HOME");
  if (!home) {
    SUCCEED("HOME not set; skipping");
    return;
  }
  const std::string path =
      std::string(home) + "/doc/dwg/visualization_-_condominium_with_skylight.dwg";
  if (!std::filesystem::is_regular_file(path)) {
    SUCCEED("condo dwg not present; skipping");
    return;
  }

  static int qargc = 1;
  static char qarg0[] = "librecad_tests";
  static char *qargv[] = {qarg0, nullptr};
  static QApplication *qapp = [] {
    auto *existing = qobject_cast<QApplication*>(QCoreApplication::instance());
    return existing ? existing : new QApplication(qargc, qargv);
  }();
  static bool settingsReady = [] {
    QApplication::setOrganizationName("LibreCAD");
    QApplication::setApplicationName("LibreCAD-tests");
    RS_Settings::init("LibreCAD", "LibreCAD-tests");
    return true;
  }();
  (void)qapp;
  (void)settingsReady;

  RS_DEBUG->setLevel(RS_Debug::D_NOTHING);

  RS_Graphic graphic;
  RS_FilterDXFRW filter;
  const bool imported =
      filter.fileImport(graphic, QString::fromStdString(path), RS2::FormatDWG);
  std::cout << "\n=== condo dwg GUI import ===\n";
  std::cout << "imported: " << imported << "\n";
  int nTop = 0, nBlocks = 0, nBlockEnts = 0;
  for (auto *e : graphic) if (e) ++nTop;
  nBlocks = graphic.countBlocks();
  std::cout << "top-level: " << nTop << "\n";
  std::cout << "blocks: " << nBlocks << "\n";
  for (unsigned i = 0; i < nBlocks; ++i) {
    auto* b = graphic.blockAt(i);
    if (b) {
      int cnt = 0;
      for (auto *e : *b) if (e) ++cnt;
      std::cout << "  Block '" << b->getName().toStdString() << "': " << cnt << " entities\n";
      nBlockEnts += cnt;
    }
  }
  std::cout << "entities in blocks: " << nBlockEnts << "\n";

  std::cout << "\nlayers:\n";
  for (unsigned i = 0; i < graphic.countLayers(); ++i) {
    auto* l = graphic.layerAt(i);
    if (l) {
      std::cout << "  '" << l->getName().toStdString() << "' "
                << (l->isFrozen() ? "FROZEN" : "visible") << "\n";
    }
  }

  std::cout << "\ntop-level entity rtti types:\n";
  std::map<int, int> topTypes;
  for (auto *e : graphic) {
    if (!e) continue;
    topTypes[e->rtti()]++;
  }
  for (const auto& [t, c] : topTypes) std::cout << "  rtti=" << t << ": " << c << "\n";
  REQUIRE(imported);
}
