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
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
**********************************************************************/

/**
 * PR-2a: AcDb:AcDsPrototype_1b typed DataStorage index tests.
 *
 * - short-read / truncated section
 * - absurd segmentIndexEntryCount hardMax clamp
 * - dynblock_point.dwg fixture: records.size() > 0
 */

#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include "drw_datastorage.h"
#include "drw_header.h"
#include "drw_interface.h"
#include "drw_objects.h"
#include "lc_dwgadvancedmetadata.h"
#include "libdwgr.h"

namespace {

class StubInterface : public DRW_Interface {
public:
  void addHeader(const DRW_Header *) override {}
  void addLType(const DRW_LType &) override {}
  void addLayer(const DRW_Layer &) override {}
  void addDimStyle(const DRW_Dimstyle &) override {}
  void addVport(const DRW_Vport &) override {}
  void addTextStyle(const DRW_Textstyle &) override {}
  void addAppId(const DRW_AppId &) override {}
  void addBlock(const DRW_Block &) override {}
  void setBlock(const int) override {}
  void endBlock() override {}
  void addPoint(const DRW_Point &) override {}
  void addLine(const DRW_Line &) override {}
  void addRay(const DRW_Ray &) override {}
  void addXline(const DRW_Xline &) override {}
  void addArc(const DRW_Arc &) override {}
  void addCircle(const DRW_Circle &) override {}
  void addEllipse(const DRW_Ellipse &) override {}
  void addLWPolyline(const DRW_LWPolyline &) override {}
  void addPolyline(const DRW_Polyline &) override {}
  void addSpline(const DRW_Spline *) override {}
  void addKnot(const DRW_Entity &) override {}
  void addInsert(const DRW_Insert &) override {}
  void addTrace(const DRW_Trace &) override {}
  void add3dFace(const DRW_3Dface &) override {}
  void addSolid(const DRW_Solid &) override {}
  void addMText(const DRW_MText &) override {}
  void addText(const DRW_Text &) override {}
  void addDimAlign(const DRW_DimAligned *) override {}
  void addDimLinear(const DRW_DimLinear *) override {}
  void addDimRadial(const DRW_DimRadial *) override {}
  void addDimDiametric(const DRW_DimDiametric *) override {}
  void addDimAngular(const DRW_DimAngular *) override {}
  void addDimAngular3P(const DRW_DimAngular3p *) override {}
  void addDimArc(const DRW_DimArc *) override {}
  void addDimOrdinate(const DRW_DimOrdinate *) override {}
  void addLeader(const DRW_Leader *) override {}
  void addHatch(const DRW_Hatch *) override {}
  void addViewport(const DRW_Viewport &) override {}
  void addImage(const DRW_Image *) override {}
  void addWipeout(const DRW_Wipeout *) override {}
  void addMLeader(const DRW_MLeader *) override {}
  void addMLeaderStyle(const DRW_MLeaderStyle *) override {}
  void linkImage(const DRW_ImageDef *) override {}
  void addComment(const char *) override {}
  void addPlotSettings(const DRW_PlotSettings *) override {}
  void writeHeader(DRW_Header &) override {}
  void writeBlocks() override {}
  void writeBlockRecords() override {}
  void writeEntities() override {}
  void writeLTypes() override {}
  void writeLayers() override {}
  void writeTextstyles() override {}
  void writeVports() override {}
  void writeDimstyles() override {}
  void writeObjects() override {}
  void writeAppId() override {}

  std::vector<DRW_DataStorageSection> m_storages;
  std::vector<DRW_RawDwgSection> m_rawSections;
  void addDataStorage(const DRW_DataStorageSection &s) override {
    m_storages.push_back(s);
  }
  void addRawDwgSection(const DRW_RawDwgSection &s) override {
    m_rawSections.push_back(s);
  }
};

void writeU32(std::vector<std::uint8_t> &buf, std::size_t off, std::uint32_t v) {
  if (off + 4 > buf.size())
    buf.resize(off + 4);
  buf[off] = static_cast<std::uint8_t>(v & 0xff);
  buf[off + 1] = static_cast<std::uint8_t>((v >> 8) & 0xff);
  buf[off + 2] = static_cast<std::uint8_t>((v >> 16) & 0xff);
  buf[off + 3] = static_cast<std::uint8_t>((v >> 24) & 0xff);
}

void writeI32(std::vector<std::uint8_t> &buf, std::size_t off, std::int32_t v) {
  writeU32(buf, off, static_cast<std::uint32_t>(v));
}

} // namespace

TEST_CASE("DataStorage short-read below header size",
          "[cross-read][datastorage][short-read]") {
  std::vector<std::uint8_t> tiny(16, 0);
  const DRW_DataStorageSection section =
      DRW_parseDataStorage(tiny.data(), tiny.size());
  REQUIRE(section.parseFailed);
  REQUIRE(section.records.empty());
  REQUIRE_FALSE(section.diagnostics.empty());
  bool found = false;
  for (const auto &d : section.diagnostics) {
    if (d.code == "datastorage-section-too-small")
      found = true;
  }
  REQUIRE(found);
}

TEST_CASE("DataStorage null pointer is short-read",
          "[cross-read][datastorage][short-read]") {
  const DRW_DataStorageSection section = DRW_parseDataStorage(nullptr, 0);
  REQUIRE(section.parseFailed);
  REQUIRE(section.records.empty());
}

TEST_CASE("DataStorage absurd segmentIndexEntryCount is capped",
          "[cross-read][datastorage][absurd-count]") {
  // Minimal 56-byte header + claim absurd segment index count.
  std::vector<std::uint8_t> buf(DRW_DataStorageConst::HEADER_SIZE, 0);
  // segmentIndexOffset = HEADER_SIZE (entries would start after a 48-byte
  // segidx segment header, which is outside this tiny buffer).
  writeU32(buf, 24, DRW_DataStorageConst::HEADER_SIZE);
  writeU32(buf, 32, 0xFFFFFFFFu); // absurd count
  writeI32(buf, 40, -1);          // no data index segment
  writeU32(buf, 52, static_cast<std::uint32_t>(buf.size()));

  const DRW_DataStorageSection section =
      DRW_parseDataStorage(buf.data(), buf.size());
  REQUIRE_FALSE(section.parseFailed);
  // Must not allocate millions of entries.
  REQUIRE(section.segments.size()
          <= DRW_DataStorageConst::HARD_MAX_ENTRIES);
  // Truncation / cap diagnostics expected when remaining space is small.
  bool cappedOrTruncated = false;
  for (const auto &d : section.diagnostics) {
    if (d.code == "datastorage-count-capped"
        || d.code == "datastorage-segment-index-truncated") {
      cappedOrTruncated = true;
    }
  }
  REQUIRE(cappedOrTruncated);
}

TEST_CASE("DataStorage metadata store retains handle list",
          "[cross-read][datastorage]") {
  LC_DwgAdvancedMetadata meta;
  DRW_DataStorageSection section;
  section.m_name = "AcDb:AcDsPrototype_1b";
  section.fileSize = 100;
  DRW_DataStorageRecord rec;
  rec.handle = 0xABCDEF;
  rec.dataByteLength = 16;
  section.records.push_back(rec);
  meta.addDataStorage(section);
  REQUIRE(meta.dataStorages().size() == 1);
  REQUIRE(meta.dataStorages().front().recordCount == 1);
  REQUIRE(meta.dataStorages().front().recordHandles.front() == 0xABCDEF);
}

TEST_CASE("DWG dynblock_point has non-empty DataStorage records",
          "[cross-read][datastorage][fixture]") {
  const std::string path =
      std::string(LIBRECAD_TEST_DIR) + "/dynblock_point.dwg";
  if (!std::filesystem::is_regular_file(path)) {
    SUCCEED("dynblock_point.dwg fixture not found; skipping");
    return;
  }

  StubInterface cap;
  dwgR reader(path.c_str());
  const bool ok = reader.read(&cap, /*ext=*/true);
  REQUIRE(ok);
  REQUIRE(reader.getError() == DRW::BAD_NONE);

  // Raw section should still be delivered for replay consumers.
  bool hasRawPrototype = false;
  for (const auto &s : cap.m_rawSections) {
    if (s.m_name.find("AcDs") != std::string::npos
        || s.m_name.find("AcDb:AcDs") != std::string::npos
        || s.m_name == "AcDb:AcDsPrototype_1b") {
      hasRawPrototype = true;
      REQUIRE_FALSE(s.m_data.empty());
    }
  }
  REQUIRE(hasRawPrototype);

  REQUIRE_FALSE(cap.m_storages.empty());
  const DRW_DataStorageSection &storage = cap.m_storages.front();
  REQUIRE_FALSE(storage.parseFailed);
  REQUIRE(storage.records.size() > 0);
  // Handles should be stable non-zero for real records.
  size_t nonZeroHandles = 0;
  for (const auto &r : storage.records) {
    if (r.handle != 0)
      ++nonZeroHandles;
  }
  REQUIRE(nonZeroHandles > 0);
}
