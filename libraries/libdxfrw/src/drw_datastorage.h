/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2026 LibreCAD (librecad.org)                                **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#ifndef DRW_DATASTORAGE_H
#define DRW_DATASTORAGE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "drw_base.h"

//! Constants for AcDb:AcDsPrototype_1b (DataStorage) binary layout.
//! Ported from dwg-parser DwgDataStorageReader (TS + C++).
namespace DRW_DataStorageConst {
constexpr std::uint32_t HEADER_SIZE = 56;
constexpr std::uint32_t SEGMENT_INDEX_ENTRY_SIZE = 12;
constexpr std::uint32_t SEGMENT_HEADER_SIZE = 48;
constexpr std::uint32_t DATA_INDEX_ENTRY_SIZE = 12;
constexpr std::uint32_t DATA_RECORD_HEADER_SIZE = 20;
//! Hard ceiling on declared index counts (malicious/absurd inputs).
constexpr std::uint32_t HARD_MAX_ENTRIES = 1000000u;
//! Retain record payload bytes only when the full section is within this size.
constexpr std::size_t PAYLOAD_BLOB_SECTION_CAP = 8u * 1024u * 1024u;
} // namespace DRW_DataStorageConst

enum class DRW_DataStorageSegmentKind {
    Unknown = 0,
    SegmentIndex,
    DataIndex,
    Data,
    SchemaIndex,
    SchemaData,
    Search,
    Blob,
    PreviousSave,
    FreeSpace,
    Schema
};

struct DRW_DataStorageDiagnostic {
    std::string code;
    std::string message;
    std::uint64_t handle = 0;
    std::uint64_t offset = 0;
    bool hasHandle = false;
    bool hasOffset = false;
};

struct DRW_DataStorageSegment {
    std::uint32_t index = 0;
    std::uint64_t offset = 0;
    std::uint32_t size = 0;
    std::uint64_t payloadOffset = 0;
    std::uint32_t payloadByteLength = 0;
    std::uint16_t signature = 0;
    UTF8STRING name;
    std::int32_t segmentIndex = -1;
    std::uint32_t revision = 0;
    bool hasRevision = false;
    std::uint32_t systemDataAlignmentOffset = 0;
    std::uint32_t objectDataAlignmentOffset = 0;
    DRW_DataStorageSegmentKind kind = DRW_DataStorageSegmentKind::Unknown;
    bool isKnownKind = false;
};

struct DRW_DataStorageIndexEntry {
    std::uint32_t segmentIndex = 0;
    std::uint32_t localOffset = 0;
    std::uint32_t schemaIndex = 0;
};

struct DRW_DataStorageRecord {
    std::uint64_t handle = 0;
    UTF8STRING handleKey;
    bool isHandleSafe = true;
    std::uint32_t segmentIndex = 0;
    std::uint32_t localOffset = 0;
    std::uint32_t schemaIndex = 0;
    std::uint32_t entrySize = 0;
    std::uint32_t recordLocalOffset = 0;
    std::uint64_t recordOffset = 0;
    std::uint64_t dataOffset = 0;
    std::uint32_t dataByteLength = 0;
    //! Optional payload copy (empty when section exceeds blob retention cap).
    std::vector<std::uint8_t> payload;
};

//! Typed index of AcDb:AcDsPrototype_1b. Read-only; no ACIS linkage (PR-2a).
struct DRW_DataStorageSection {
    UTF8STRING m_name = "AcDb:AcDsPrototype_1b";
    DRW::Version m_version = DRW::UNKNOWNV;
    std::uint32_t signature = 0;
    std::int32_t headerSize = 0;
    std::uint32_t version = 0;
    std::uint32_t revision = 0;
    std::uint32_t segmentIndexOffset = 0;
    std::uint32_t segmentIndexEntryCount = 0;
    std::int32_t schemaIndexSegmentIndex = -1;
    std::int32_t dataIndexSegmentIndex = -1;
    std::int32_t searchSegmentIndex = -1;
    std::int32_t previousSaveIndex = -1;
    std::uint32_t fileSize = 0;
    std::size_t sectionByteLength = 0;
    bool payloadsRetained = false;
    std::vector<DRW_DataStorageSegment> segments;
    std::vector<DRW_DataStorageIndexEntry> dataIndexEntries;
    std::vector<DRW_DataStorageRecord> records;
    std::vector<DRW_DataStorageDiagnostic> diagnostics;
    //! True when the input was shorter than HEADER_SIZE or otherwise unusable.
    bool parseFailed = false;
};

//! Parse a raw AcDb:AcDsPrototype_1b section buffer into a typed index.
//! Never throws. Malicious counts are capped; short reads produce diagnostics.
DRW_DataStorageSection DRW_parseDataStorage(
    const std::uint8_t* data,
    std::size_t size,
    DRW::Version version = DRW::UNKNOWNV);

inline DRW_DataStorageSection DRW_parseDataStorage(
    const std::vector<std::uint8_t>& data,
    DRW::Version version = DRW::UNKNOWNV) {
    return DRW_parseDataStorage(data.data(), data.size(), version);
}

#endif // DRW_DATASTORAGE_H
