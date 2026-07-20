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
 * Phase-0 safety-net tests for the vendored shapelib 1.6.3.
 *
 * These tests pin the observable behavior of the shapelib C API over the
 * 31-file corpus under test_data/shp/ *before* Phase 1 renames the .c
 * translation units to .cpp and switches them to C++17 compilation.  The
 * conversion is thereby provably behavior-preserving: any assertion here that
 * regresses points at a behavioral drift introduced by the .cpp rename.
 *
 * These tests are strictly READ-ONLY: they never call SHPRestoreSHX,
 * SHPOpenLLEx(bRestoreSHX=1), SHPCreate, SHPWrite*, DBFCreate, or DBFWrite*.
 * The corpus is a fixed reference set; the "hostile" fixtures are supposed
 * to fail to open (that is their whole point), and shapelib's null-return on
 * them is exactly what is being pinned.
 *
 * A handful of corpus fixtures were shipped without a well-formed .shx
 * (either missing or truncated to a bare header).  With the read-only
 * SHPOpen path, shapelib returns null on those without writing to disk.
 * The tests below pin that behavior for each such fixture explicitly.
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

// Vendored shapelib 1.6.3 headers (currently under plugins/importshp/shapelib;
// Phase 1 relocates them to libraries/shapelib/src, but the API is unchanged).
#include "shapefil.h"

namespace {

std::string corpusPath(std::string_view relative) {
    std::filesystem::path p = LIBRECAD_SOURCE_DIR;
    p /= "test_data/shp";
    p /= std::string(relative);
    return p.string();
}

// RAII wrappers so REQUIRE failures don't leak file handles.
struct ScopedSHP {
    SHPHandle h{nullptr};
    ~ScopedSHP() { if (h) SHPClose(h); }
    ScopedSHP() = default;
    explicit ScopedSHP(SHPHandle handle) : h(handle) {}
    ScopedSHP(const ScopedSHP&) = delete;
    ScopedSHP& operator=(const ScopedSHP&) = delete;
    operator SHPHandle() const { return h; }
};

struct ScopedDBF {
    DBFHandle h{nullptr};
    ~ScopedDBF() { if (h) DBFClose(h); }
    ScopedDBF() = default;
    explicit ScopedDBF(DBFHandle handle) : h(handle) {}
    ScopedDBF(const ScopedDBF&) = delete;
    ScopedDBF& operator=(const ScopedDBF&) = delete;
    operator DBFHandle() const { return h; }
};

struct ScopedShape {
    SHPObject* o{nullptr};
    ~ScopedShape() { if (o) SHPDestroyObject(o); }
    ScopedShape() = default;
    explicit ScopedShape(SHPObject* obj) : o(obj) {}
    ScopedShape(const ScopedShape&) = delete;
    ScopedShape& operator=(const ScopedShape&) = delete;
    SHPObject* operator->() const { return o; }
    operator SHPObject*() const { return o; }
};

} // namespace

// ---------------------------------------------------------------------------
// Happy-path SHP reads: fixture opens, header info matches inventory, first
// record decodes to the expected type/vertex/part counts.
// ---------------------------------------------------------------------------

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPOpen: basic 2D POINT corpus", "[shp][shapelib][basic]") {
    ScopedSHP h{SHPOpen(corpusPath("points.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_POINT);
    CHECK(n == 9);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_POINT);
    CHECK(rec->nParts == 0);
    CHECK(rec->nVertices == 1);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPOpen: ARC (polylines) corpus", "[shp][shapelib][basic]") {
    ScopedSHP h{SHPOpen(corpusPath("polylines.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_ARC);
    CHECK(n == 2);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_ARC);
    CHECK(rec->nParts == 1);
    CHECK(rec->nVertices == 5);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPOpen: POLYGON corpus", "[shp][shapelib][basic]") {
    ScopedSHP h{SHPOpen(corpusPath("polygons.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_POLYGON);
    CHECK(n == 3);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_POLYGON);
    CHECK(rec->nParts == 1);
    CHECK(rec->nVertices == 5);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPOpen: MULTIPOINT corpus", "[shp][shapelib][basic]") {
    ScopedSHP h{SHPOpen(corpusPath("multipoints.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_MULTIPOINT);
    CHECK(n == 2);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_MULTIPOINT);
    CHECK(rec->nParts == 0);
    CHECK(rec->nVertices == 5);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPOpen: multi-part POINT (bc_hospitals)",
          "[shp][shapelib][basic]") {
    ScopedSHP h{SHPOpen(corpusPath("multi_part/bc_hospitals.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_POINT);
    CHECK(n == 44);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_POINT);
    CHECK(rec->nVertices == 1);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPOpen: MULTIPOINTZ (multi_part/multipoint)",
          "[shp][shapelib][basic]") {
    ScopedSHP h{SHPOpen(corpusPath("multi_part/multipoint.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_MULTIPOINTZ);
    CHECK(n == 312);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_MULTIPOINTZ);
    // First MULTIPOINTZ record here holds a single vertex.
    CHECK(rec->nVertices == 1);
    // Z values should be populated for the *Z type.
    REQUIRE(rec->padfZ != nullptr);
}

// ---------------------------------------------------------------------------
// Multi-part POLYGON: world_borders has records with multiple rings.  Rec 0
// is a two-ring polygon (nParts=2 at panPartStart=[0,4], nVerts=8).
// ---------------------------------------------------------------------------

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib multi-part: world_borders record 0 has 2 parts",
          "[shp][shapelib][multipart]") {
    ScopedSHP h{SHPOpen(corpusPath("world_borders.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_POLYGON);
    CHECK(n == 246);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_POLYGON);
    CHECK(rec->nParts == 2);
    CHECK(rec->nVertices == 8);
    REQUIRE(rec->panPartStart != nullptr);
    CHECK(rec->panPartStart[0] == 0);
    CHECK(rec->panPartStart[1] == 4);
    // Part starts must be monotonically increasing and all < nVertices.
    for (int i = 1; i < rec->nParts; ++i)
        CHECK(rec->panPartStart[i] > rec->panPartStart[i - 1]);
    for (int i = 0; i < rec->nParts; ++i)
        CHECK(rec->panPartStart[i] < rec->nVertices);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib multi-part: TM_WORLD_BORDERS-0.2 record 0 has 2 parts",
          "[shp][shapelib][multipart]") {
    ScopedSHP h{SHPOpen(
        corpusPath("real_world/TM_WORLD_BORDERS-0.2.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_POLYGON);
    CHECK(n == 246);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nParts == 2);
    CHECK(rec->nVertices == 48);
    REQUIRE(rec->panPartStart != nullptr);
    CHECK(rec->panPartStart[0] == 0);
    CHECK(rec->panPartStart[1] == 23);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib large POLYGON: Chi-SDOH 791 records",
          "[shp][shapelib][large]") {
    ScopedSHP h{SHPOpen(corpusPath("real_world/Chi-SDOH.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(t == SHPT_POLYGON);
    CHECK(n == 791);

    ScopedShape rec{SHPReadObject(h, 0)};
    REQUIRE(rec.o != nullptr);
    CHECK(rec->nSHPType == SHPT_POLYGON);
    CHECK(rec->nParts == 1);
    CHECK(rec->nVertices == 311);
    // Sweep every record: no null, no crash.
    for (int i = 1; i < n; ++i) {
        ScopedShape r{SHPReadObject(h, i)};
        REQUIRE(r.o != nullptr);
        CHECK(r->nVertices >= 3);
    }
}

// ---------------------------------------------------------------------------
// DBF attribute reads
// ---------------------------------------------------------------------------

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: points.dbf field metadata", "[shp][shapelib][dbf]") {
    ScopedDBF h{DBFOpen(corpusPath("points.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 9);
    CHECK(DBFGetFieldCount(h) == 1);

    char name[16] = {};
    int width = 0, decimals = 0;
    DBFFieldType ft = DBFGetFieldInfo(h, 0, name, &width, &decimals);
    CHECK(std::string(name) == "FID");
    // Width 11 > 10 → shapelib returns FTDouble even with decimals=0.  This
    // is the classification rule in dbfopen.c and is what the C++17 rename
    // in Phase 1 must preserve.
    CHECK(ft == FTDouble);
    CHECK(width == 11);
    CHECK(decimals == 0);
    // DBFReadIntegerAttribute rounds the underlying double for FTDouble fields.
    CHECK(DBFReadIntegerAttribute(h, 0, 0) == 0);
    CHECK(DBFReadIntegerAttribute(h, 8, 0) == 8);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: bc_hospitals string fields",
          "[shp][shapelib][dbf]") {
    ScopedDBF h{DBFOpen(corpusPath("multi_part/bc_hospitals.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 44);
    CHECK(DBFGetFieldCount(h) == 3);

    // Field 0: ID (integer)
    char name[16] = {};
    int width = 0, decimals = 0;
    DBFFieldType ft = DBFGetFieldInfo(h, 0, name, &width, &decimals);
    CHECK(std::string(name) == "ID");
    CHECK(ft == FTInteger);

    // Field 1: AUTHORITY (string)
    ft = DBFGetFieldInfo(h, 1, name, &width, &decimals);
    CHECK(std::string(name) == "AUTHORITY");
    CHECK(ft == FTString);
    const char* auth0 = DBFReadStringAttribute(h, 0, 1);
    REQUIRE(auth0 != nullptr);
    CHECK(std::string(auth0) == "Fraser");

    // Field 2: NAME (string)
    ft = DBFGetFieldInfo(h, 2, name, &width, &decimals);
    CHECK(std::string(name) == "NAME");
    CHECK(ft == FTString);
    const char* name0 = DBFReadStringAttribute(h, 0, 2);
    REQUIRE(name0 != nullptr);
    CHECK(std::string(name0) == "Chilliwack General Hospital");
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: UTF-8 codepage round-trips raw bytes",
          "[shp][shapelib][dbf][codepage]") {
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/utf8-property.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 1);
    CHECK(DBFGetFieldCount(h) == 1);

    const char* cp = DBFGetCodePage(h);
    REQUIRE(cp != nullptr);
    CHECK(std::string(cp) == "UTF-8");

    const char* s = DBFReadStringAttribute(h, 0, 0);
    REQUIRE(s != nullptr);
    // "ηελλο ςορλδ" as UTF-8 (from probe output).
    const unsigned char expected[] = {
        0xce, 0xb7, 0xce, 0xb5, 0xce, 0xbb, 0xce, 0xbb, 0xce, 0xbf, 0x20,
        0xcf, 0x82, 0xce, 0xbf, 0xcf, 0x81, 0xce, 0xbb, 0xce, 0xb4, 0x00};
    CHECK(std::memcmp(s, expected, sizeof(expected)) == 0);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: Latin-1 (LDID/87) raw bytes preserved",
          "[shp][shapelib][dbf][codepage]") {
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/latin1-property.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 1);
    CHECK(DBFGetFieldCount(h) == 1);

    const char* cp = DBFGetCodePage(h);
    REQUIRE(cp != nullptr);
    // "LDID/87" is the ISO-8859-1 LDID (SHAPEFILE C++ code page marker).
    CHECK(std::string(cp) == "LDID/87");

    const char* s = DBFReadStringAttribute(h, 0, 0);
    REQUIRE(s != nullptr);
    // "México" in Latin-1: 4d e9 78 69 63 6f.
    const unsigned char expected[] = {0x4d, 0xe9, 0x78, 0x69, 0x63, 0x6f, 0x00};
    CHECK(std::memcmp(s, expected, sizeof(expected)) == 0);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: FTLogical (boolean-property)",
          "[shp][shapelib][dbf][types]") {
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/boolean-property.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 9);
    CHECK(DBFGetFieldCount(h) == 1);

    char name[16] = {};
    int width = 0, decimals = 0;
    DBFFieldType ft = DBFGetFieldInfo(h, 0, name, &width, &decimals);
    CHECK(ft == FTLogical);
    CHECK(width == 1);

    // Logical values are returned as a "T"/"F"/"?" string.
    const char* v = DBFReadLogicalAttribute(h, 0, 0);
    REQUIRE(v != nullptr);
    // At minimum the field must decode without a crash and return one byte.
    CHECK(std::strlen(v) >= 1);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: FTDate (date-property) round-trips first record",
          "[shp][shapelib][dbf][types]") {
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/date-property.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 3);
    CHECK(DBFGetFieldCount(h) == 1);

    char name[16] = {};
    int width = 0, decimals = 0;
    DBFFieldType ft = DBFGetFieldInfo(h, 0, name, &width, &decimals);
    CHECK(ft == FTDate);
    CHECK(std::string(name) == "date");

    SHPDate d = DBFReadDateAttribute(h, 0, 0);
    CHECK(d.year == 2013);
    CHECK(d.month == 1);
    CHECK(d.day == 2);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: FTDouble (number-property) reads",
          "[shp][shapelib][dbf][types]") {
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/number-property.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 3);
    CHECK(DBFGetFieldCount(h) == 1);

    char name[16] = {};
    int width = 0, decimals = 0;
    DBFFieldType ft = DBFGetFieldInfo(h, 0, name, &width, &decimals);
    CHECK(ft == FTDouble);
    // Reading a numeric attribute (regardless of value) must not crash;
    // pinning DBFReadDoubleAttribute's shape.
    (void)DBFReadDoubleAttribute(h, 0, 0);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: DBFIsAttributeNULL on number-null-property",
          "[shp][shapelib][dbf][types]") {
    // Note: this fixture ships without .shx (SHP-only test cases skip this
    // one), but the .dbf is a complete, well-formed DBF we can open on its own.
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/number-null-property.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 4);
    CHECK(DBFGetFieldCount(h) == 3);

    // From probe: field 0='name' (string), 1='fid' (string), 2='avg_temp'
    // (double).  Verify at least one record has a NULL avg_temp attribute.
    bool foundNull = false;
    for (int i = 0; i < DBFGetRecordCount(h); ++i) {
        if (DBFIsAttributeNULL(h, i, 2)) {
            foundNull = true;
            break;
        }
    }
    CHECK(foundNull);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib DBF: mixed-properties has heterogeneous field types",
          "[shp][shapelib][dbf][types]") {
    ScopedDBF h{DBFOpen(
        corpusPath("real_world/mixed-properties.dbf").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    CHECK(DBFGetRecordCount(h) == 3);
    CHECK(DBFGetFieldCount(h) == 2);

    char name[16] = {};
    int width = 0, decimals = 0;
    DBFFieldType ft0 = DBFGetFieldInfo(h, 0, name, &width, &decimals);
    CHECK(std::string(name) == "foo");
    CHECK(ft0 == FTDouble);

    DBFFieldType ft1 = DBFGetFieldInfo(h, 1, name, &width, &decimals);
    CHECK(std::string(name) == "bar");
    CHECK(ft1 == FTString);
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib edge: real_world/empty.shp is a valid header, 0 records",
          "[shp][shapelib][edge]") {
    ScopedSHP h{SHPOpen(corpusPath("real_world/empty.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(n == 0);
    CHECK(t == SHPT_ARC);
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib edge: real_world/null.shp opens (POINT, 9 records)",
          "[shp][shapelib][edge]") {
    ScopedSHP h{SHPOpen(corpusPath("real_world/null.shp").c_str(), "rb")};
    REQUIRE(h.h != nullptr);
    int n = 0, t = 0;
    double lo[4], hi[4];
    SHPGetInfo(h, &n, &t, lo, hi);
    CHECK(n == 9);
    CHECK(t == SHPT_POINT);
    // Sweep records: no null, no crash.
    for (int i = 0; i < n; ++i) {
        ScopedShape r{SHPReadObject(h, i)};
        REQUIRE(r.o != nullptr);
        // Every vertex count must be bounded (guard against garbage).
        CHECK(r->nVertices >= 0);
        CHECK(r->nVertices <= 1'000'000);
    }
}

// ---------------------------------------------------------------------------
// Corrupt / missing .shx: SHPOpen (read-only path) MUST return null without
// crashing.  These tests pin the observable behavior on fixtures that ship
// with truncated or absent .shx sidecar files.  Regenerating the .shx to
// "fix" any of these would defeat the point of the fixture.
// ---------------------------------------------------------------------------

namespace {
struct FixtureName {
    const char* label;
    const char* path;
};
const FixtureName kSHXCorruptOrMissing[] = {
    // .shx exists but is truncated to a 48/56-byte header stub.
    {"pointz",               "pointz.shp"},
    {"null_shape",           "null_shape.shp"},
    {"multi_part_polyline",  "multi_part_polyline.shp"},
    {"malformed_dbf",        "malformed_dbf.shp"},
    {"malformed_truncated",  "malformed_truncated.shp"},
    // .shx absent entirely.
    {"missing_shx",          "missing_shx.shp"},
    {"z_m_types/pointm",     "z_m_types/pointm.shp"},
    {"z_m_types/polylinem",  "z_m_types/polylinem.shp"},
    {"z_m_types/polygonm",   "z_m_types/polygonm.shp"},
    {"z_m_types/multipointm","z_m_types/multipointm.shp"},
    {"real_world/singleton", "real_world/singleton.shp"},
    {"real_world/ignore-properties",  "real_world/ignore-properties.shp"},
    {"real_world/number-null-property","real_world/number-null-property.shp"},
};
} // namespace

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib hostile/edge: SHPOpen returns null on corrupt/missing .shx",
          "[shp][shapelib][edge][hostile]") {
    for (const auto& fx : kSHXCorruptOrMissing) {
        INFO("fixture: " << fx.label);
        ScopedSHP h{SHPOpen(corpusPath(fx.path).c_str(), "rb")};
        // The whole point of these fixtures is that read-only SHPOpen
        // rejects them.  Pin that behavior — regressing to "opens
        // successfully" or "crashes" would both be behavioral drifts
        // introduced by the C++17 conversion in Phase 1.
        CHECK(h.h == nullptr);
    }
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib hostile: malformed_dbf.dbf either opens safely or nulls",
          "[shp][shapelib][hostile]") {
    // DBFOpen on a malformed .dbf must either return null OR return a handle
    // whose subsequent reads return null/degraded strings — but never crash.
    ScopedDBF h{DBFOpen(corpusPath("malformed_dbf.dbf").c_str(), "rb")};
    if (h.h != nullptr) {
        int recs = DBFGetRecordCount(h);
        int fields = DBFGetFieldCount(h);
        // Guard against garbage counts — a corrupt header must not lead to
        // preposterous record/field counts that a caller would loop over.
        CHECK(recs >= 0);
        CHECK(recs < 1'000'000);
        CHECK(fields >= 0);
        CHECK(fields < 10'000);
        // Every field/record read must complete without crashing.
        for (int f = 0; f < fields; ++f) {
            char name[16] = {};
            int w = 0, d = 0;
            (void)DBFGetFieldInfo(h, f, name, &w, &d);
            if (recs > 0) {
                (void)DBFReadStringAttribute(h, 0, f);
                (void)DBFIsAttributeNULL(h, 0, f);
            }
        }
    }
    // If DBFOpen returned null, that is equally acceptable: shapelib
    // gracefully rejected the malformed file.
}

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib hostile: malformed_truncated.dbf either opens safely or nulls",
          "[shp][shapelib][hostile]") {
    ScopedDBF h{DBFOpen(corpusPath("malformed_truncated.dbf").c_str(), "rb")};
    if (h.h != nullptr) {
        int recs = DBFGetRecordCount(h);
        int fields = DBFGetFieldCount(h);
        CHECK(recs >= 0);
        CHECK(recs < 1'000'000);
        CHECK(fields >= 0);
        CHECK(fields < 10'000);
        for (int f = 0; f < fields; ++f) {
            char name[16] = {};
            int w = 0, d = 0;
            (void)DBFGetFieldInfo(h, f, name, &w, &d);
        }
    }
}

// ---------------------------------------------------------------------------
// Sanity: shapelib type-name lookup (SHPTypeName) covers every SHPT_* value
// used above, and does not return null.  Pins the small helper API.
// ---------------------------------------------------------------------------

// NOLINTNEXTLINE(readability-identifier-naming)
TEST_CASE("shapelib SHPTypeName: covers all shape-type constants",
          "[shp][shapelib][api]") {
    const int types[] = {
        SHPT_NULL,    SHPT_POINT,     SHPT_ARC,        SHPT_POLYGON,
        SHPT_MULTIPOINT, SHPT_POINTZ, SHPT_ARCZ,       SHPT_POLYGONZ,
        SHPT_MULTIPOINTZ, SHPT_POINTM, SHPT_ARCM,      SHPT_POLYGONM,
        SHPT_MULTIPOINTM, SHPT_MULTIPATCH};
    for (int t : types) {
        INFO("type " << t);
        const char* n = SHPTypeName(t);
        REQUIRE(n != nullptr);
        CHECK(std::strlen(n) > 0);
    }
}
