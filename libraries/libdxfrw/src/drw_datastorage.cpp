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

#include "drw_datastorage.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>

#include "intern/drw_reserve.h"

namespace {

using namespace DRW_DataStorageConst;

class ByteReader {
public:
    ByteReader(const std::uint8_t* data, std::size_t size)
        : m_data(data)
        , m_size(size) {}

    std::size_t length() const { return m_size; }

    bool has(std::uint64_t offset, std::uint64_t size) const {
        if (m_data == nullptr)
            return false;
        if (offset > m_size)
            return false;
        return size <= (m_size - static_cast<std::size_t>(offset));
    }

    std::uint16_t u16(std::uint64_t offset) const {
        const auto i = static_cast<std::size_t>(offset);
        return static_cast<std::uint16_t>(m_data[i])
            | (static_cast<std::uint16_t>(m_data[i + 1]) << 8);
    }

    std::uint32_t u32(std::uint64_t offset) const {
        const auto i = static_cast<std::size_t>(offset);
        return static_cast<std::uint32_t>(m_data[i])
            | (static_cast<std::uint32_t>(m_data[i + 1]) << 8)
            | (static_cast<std::uint32_t>(m_data[i + 2]) << 16)
            | (static_cast<std::uint32_t>(m_data[i + 3]) << 24);
    }

    std::int32_t i32(std::uint64_t offset) const {
        return static_cast<std::int32_t>(u32(offset));
    }

    std::uint64_t u64(std::uint64_t offset) const {
        return static_cast<std::uint64_t>(u32(offset))
            | (static_cast<std::uint64_t>(u32(offset + 4)) << 32);
    }

    UTF8STRING ascii(std::uint64_t offset, std::uint64_t size) const {
        if (!has(offset, size))
            return {};
        UTF8STRING value;
        value.reserve(static_cast<std::size_t>(std::min<std::uint64_t>(size, 16)));
        const auto start = static_cast<std::size_t>(offset);
        for (std::uint64_t i = 0; i < size; ++i) {
            const std::uint8_t ch = m_data[start + static_cast<std::size_t>(i)];
            if (ch == 0)
                break;
            value.push_back(static_cast<char>(ch));
        }
        return value;
    }

    std::vector<std::uint8_t> bytes(std::uint64_t offset, std::uint64_t size) const {
        if (!has(offset, size))
            return {};
        const auto first = m_data + static_cast<std::size_t>(offset);
        return std::vector<std::uint8_t>(first, first + static_cast<std::size_t>(size));
    }

private:
    const std::uint8_t* m_data;
    std::size_t m_size;
};

void pushDiag(DRW_DataStorageSection& section,
              std::string code,
              std::string message,
              std::uint64_t handle = 0,
              bool hasHandle = false,
              std::uint64_t offset = 0,
              bool hasOffset = false) {
    DRW_DataStorageDiagnostic d;
    d.code = std::move(code);
    d.message = std::move(message);
    d.handle = handle;
    d.hasHandle = hasHandle;
    d.offset = offset;
    d.hasOffset = hasOffset;
    section.diagnostics.push_back(std::move(d));
}

UTF8STRING handleKey(std::uint64_t value) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%llX",
                  static_cast<unsigned long long>(value));
    return UTF8STRING(buf);
}

std::uint64_t roundUpTo16(std::uint64_t n) {
    return (n + 15u) & ~std::uint64_t{15};
}

std::string toLower(const UTF8STRING& name) {
    std::string out;
    out.reserve(name.size());
    for (unsigned char ch : name)
        out.push_back(static_cast<char>(std::tolower(ch)));
    return out;
}

DRW_DataStorageSegmentKind classifyKind(const UTF8STRING& name) {
    const std::string n = toLower(name);
    if (n == "segidx")
        return DRW_DataStorageSegmentKind::SegmentIndex;
    if (n == "datidx" || n == "dindex")
        return DRW_DataStorageSegmentKind::DataIndex;
    if (n == "_data_" || n == "data")
        return DRW_DataStorageSegmentKind::Data;
    if (n == "schidx")
        return DRW_DataStorageSegmentKind::SchemaIndex;
    if (n == "schdat")
        return DRW_DataStorageSegmentKind::SchemaData;
    if (n == "search")
        return DRW_DataStorageSegmentKind::Search;
    if (n.rfind("blob", 0) == 0)
        return DRW_DataStorageSegmentKind::Blob;
    if (n == "prvsav")
        return DRW_DataStorageSegmentKind::PreviousSave;
    if (n == "freesp")
        return DRW_DataStorageSegmentKind::FreeSpace;
    if (n == "schema")
        return DRW_DataStorageSegmentKind::Schema;
    return DRW_DataStorageSegmentKind::Unknown;
}

bool isDataSegment(const DRW_DataStorageSegment& segment) {
    return segment.kind == DRW_DataStorageSegmentKind::Data
        || segment.kind == DRW_DataStorageSegmentKind::Blob;
}

std::uint32_t capCount(std::uint32_t declared,
                       std::uint64_t remainingBytes,
                       std::uint32_t entrySize,
                       DRW_DataStorageSection& section,
                       const char* what) {
    if (entrySize == 0)
        return 0;
    const std::uint64_t byRemaining = remainingBytes / entrySize;
    std::uint32_t capped = declared;
    if (byRemaining < capped)
        capped = static_cast<std::uint32_t>(byRemaining);
    if (capped > HARD_MAX_ENTRIES)
        capped = HARD_MAX_ENTRIES;
    if (capped < declared) {
        pushDiag(section,
                 "datastorage-count-capped",
                 std::string("DataStorage ") + what + " count capped from "
                     + std::to_string(declared) + " to " + std::to_string(capped));
    }
    return capped;
}

void readSegmentIndex(const ByteReader& r,
                      std::uint32_t offset,
                      std::uint32_t count,
                      DRW_DataStorageSection& section) {
    const std::uint64_t entriesBase =
        static_cast<std::uint64_t>(offset) + SEGMENT_HEADER_SIZE;
    const std::uint64_t remaining =
        entriesBase < r.length() ? (r.length() - entriesBase) : 0;
    const std::uint32_t maxCount =
        capCount(count, remaining, SEGMENT_INDEX_ENTRY_SIZE, section,
                 "segmentIndexEntryCount");

    if (!DRW::reserve(section.segments, static_cast<int>(maxCount))) {
        pushDiag(section, "datastorage-segment-index-reserve-failed",
                 "DataStorage segment index reserve failed");
        return;
    }

    std::unordered_map<std::int32_t, std::uint32_t> seenHeaderIndexes;
    for (std::uint32_t i = 0; i < maxCount; ++i) {
        const std::uint64_t entryOffset =
            entriesBase + static_cast<std::uint64_t>(i) * SEGMENT_INDEX_ENTRY_SIZE;
        if (!r.has(entryOffset, SEGMENT_INDEX_ENTRY_SIZE)) {
            pushDiag(section, "datastorage-segment-index-truncated",
                     "DataStorage segment index entry " + std::to_string(i)
                         + " is outside the section bounds",
                     0, false, entryOffset, true);
            break;
        }

        DRW_DataStorageSegment segment;
        segment.index = i;
        segment.offset = r.u64(entryOffset);
        segment.size = r.u32(entryOffset + 8);
        segment.payloadOffset = segment.offset + SEGMENT_HEADER_SIZE;
        segment.payloadByteLength =
            segment.size >= SEGMENT_HEADER_SIZE
                ? segment.size - SEGMENT_HEADER_SIZE
                : 0;

        if (segment.offset + segment.size > r.length()) {
            pushDiag(section, "datastorage-segment-size-out-of-bounds",
                     "DataStorage segment " + std::to_string(i)
                         + " extends beyond the section bounds",
                     0, false, segment.offset, true);
        }

        if (segment.offset == 0) {
            // Empty placeholder slot — keep dense array, skip header.
            section.segments.push_back(std::move(segment));
            continue;
        }

        if (r.has(segment.offset, SEGMENT_HEADER_SIZE)) {
            const std::int32_t headerSegmentIndex = r.i32(segment.offset + 8);
            segment.signature = r.u16(segment.offset);
            segment.name = r.ascii(segment.offset + 2, 6);
            segment.segmentIndex = headerSegmentIndex;
            segment.revision = r.u32(segment.offset + 24);
            segment.hasRevision = true;
            segment.systemDataAlignmentOffset = r.u32(segment.offset + 32);
            segment.objectDataAlignmentOffset = r.u32(segment.offset + 36);
            segment.kind = classifyKind(segment.name);
            segment.isKnownKind =
                segment.kind != DRW_DataStorageSegmentKind::Unknown;

            const auto previous = seenHeaderIndexes.find(headerSegmentIndex);
            if (previous != seenHeaderIndexes.end()) {
                pushDiag(section, "datastorage-duplicate-segment-index",
                         "DataStorage segment header index "
                             + std::to_string(headerSegmentIndex)
                             + " is used by segments "
                             + std::to_string(previous->second) + " and "
                             + std::to_string(i),
                         0, false, segment.offset, true);
            } else {
                seenHeaderIndexes.emplace(headerSegmentIndex, i);
            }
        } else {
            pushDiag(section, "datastorage-segment-header-missing",
                     "DataStorage segment " + std::to_string(i)
                         + " header is outside the section bounds",
                     0, false, segment.offset, true);
        }

        section.segments.push_back(std::move(segment));
    }
}

bool isZeroPlaceholderDataIndexEntry(
    const DRW_DataStorageIndexEntry& entry,
    const std::vector<DRW_DataStorageSegment>& segments) {
    if (entry.segmentIndex != 0 || entry.localOffset != 0 || entry.schemaIndex != 0)
        return false;
    return segments.empty() || !isDataSegment(segments.front());
}

void readDataIndex(const ByteReader& r, DRW_DataStorageSection& section) {
    if (section.dataIndexSegmentIndex < 0
        || static_cast<std::size_t>(section.dataIndexSegmentIndex)
               >= section.segments.size()) {
        pushDiag(section, "datastorage-data-index-missing",
                 "DataStorage data-index segment "
                     + std::to_string(section.dataIndexSegmentIndex)
                     + " is not present");
        return;
    }

    const DRW_DataStorageSegment& segment =
        section.segments[static_cast<std::size_t>(section.dataIndexSegmentIndex)];
    const std::uint64_t offset = segment.offset + SEGMENT_HEADER_SIZE;
    if (!r.has(offset, 8)) {
        pushDiag(section, "datastorage-data-index-truncated",
                 "DataStorage data-index header is outside the section bounds",
                 0, false, offset, true);
        return;
    }

    const std::int32_t rawCount = r.i32(offset);
    if (rawCount <= 0)
        return;

    const std::uint64_t entriesBase = offset + 8;
    const std::uint64_t remaining =
        entriesBase < r.length() ? (r.length() - entriesBase) : 0;
    const std::uint32_t maxCount =
        capCount(static_cast<std::uint32_t>(rawCount), remaining,
                 DATA_INDEX_ENTRY_SIZE, section, "dataIndexEntryCount");

    if (!DRW::reserve(section.dataIndexEntries, static_cast<int>(maxCount))) {
        pushDiag(section, "datastorage-data-index-reserve-failed",
                 "DataStorage data index reserve failed");
        return;
    }

    std::unordered_map<std::string, std::uint32_t> seen;
    for (std::uint32_t i = 0; i < maxCount; ++i) {
        const std::uint64_t entryOffset =
            entriesBase + static_cast<std::uint64_t>(i) * DATA_INDEX_ENTRY_SIZE;
        if (!r.has(entryOffset, DATA_INDEX_ENTRY_SIZE)) {
            pushDiag(section, "datastorage-data-index-entry-truncated",
                     "DataStorage data-index entry " + std::to_string(i)
                         + " is outside the section bounds",
                     0, false, entryOffset, true);
            break;
        }

        DRW_DataStorageIndexEntry entry;
        entry.segmentIndex = r.u32(entryOffset);
        entry.localOffset = r.u32(entryOffset + 4);
        entry.schemaIndex = r.u32(entryOffset + 8);

        const std::string key = std::to_string(entry.segmentIndex) + ":"
            + std::to_string(entry.localOffset);
        const auto previous = seen.find(key);
        if (previous != seen.end()) {
            if (isZeroPlaceholderDataIndexEntry(entry, section.segments))
                continue;
            pushDiag(section, "datastorage-duplicate-data-index-entry",
                     "DataStorage data-index entries "
                         + std::to_string(previous->second) + " and "
                         + std::to_string(i) + " point to the same offset",
                     0, false, entryOffset, true);
            continue;
        }
        seen.emplace(key, i);
        section.dataIndexEntries.push_back(entry);
    }
}

std::uint32_t countPackedRecordHeaders(const ByteReader& r,
                                       std::uint64_t segmentDataOffset,
                                       std::uint64_t segmentEndOffset) {
    std::uint32_t n = 0;
    for (;;) {
        const std::uint64_t slot =
            segmentDataOffset + static_cast<std::uint64_t>(n) * DATA_RECORD_HEADER_SIZE;
        if (slot + DATA_RECORD_HEADER_SIZE > segmentEndOffset)
            break;
        if (!r.has(slot, 4) || r.u32(slot) != DATA_RECORD_HEADER_SIZE)
            break;
        ++n;
        if (n > HARD_MAX_ENTRIES)
            break;
    }
    return n;
}

void readDataRecords(const ByteReader& r,
                     DRW_DataStorageSection& section,
                     bool retainPayloads) {
    std::unordered_map<std::uint64_t, std::uint64_t> dataRegionBaseByOffset;
    const std::uint32_t maxRecords =
        capCount(static_cast<std::uint32_t>(section.dataIndexEntries.size()),
                 r.length(), DATA_RECORD_HEADER_SIZE, section, "records");

    if (!DRW::reserve(section.records, static_cast<int>(maxRecords))) {
        pushDiag(section, "datastorage-records-reserve-failed",
                 "DataStorage records reserve failed");
        return;
    }

    std::uint32_t produced = 0;
    for (const DRW_DataStorageIndexEntry& entry : section.dataIndexEntries) {
        if (produced >= maxRecords)
            break;
        if (static_cast<std::size_t>(entry.segmentIndex) >= section.segments.size()) {
            pushDiag(section, "datastorage-record-segment-missing",
                     "DataStorage record references missing segment "
                         + std::to_string(entry.segmentIndex));
            continue;
        }
        const DRW_DataStorageSegment& segment =
            section.segments[entry.segmentIndex];
        if (!isDataSegment(segment))
            continue;

        const std::uint64_t segmentDataOffset = segment.offset + SEGMENT_HEADER_SIZE;
        const std::uint64_t recordHeaderOffset =
            segmentDataOffset + entry.localOffset;
        if (!r.has(recordHeaderOffset, DATA_RECORD_HEADER_SIZE)) {
            pushDiag(section, "datastorage-record-header-truncated",
                     "DataStorage record header at local offset "
                         + std::to_string(entry.localOffset)
                         + " is outside the section bounds",
                     0, false, recordHeaderOffset, true);
            continue;
        }

        const std::uint32_t entrySize = r.u32(recordHeaderOffset);
        const std::uint64_t handle = r.u64(recordHeaderOffset + 8);
        const std::uint32_t recordLocalOffset = r.u32(recordHeaderOffset + 16);

        std::uint64_t dataRegionBase = 0;
        const auto cached = dataRegionBaseByOffset.find(segment.offset);
        if (cached != dataRegionBaseByOffset.end()) {
            dataRegionBase = cached->second;
        } else {
            const std::uint32_t headerCount = countPackedRecordHeaders(
                r, segmentDataOffset, segment.offset + segment.size);
            dataRegionBase = roundUpTo16(
                segmentDataOffset
                + static_cast<std::uint64_t>(headerCount) * DATA_RECORD_HEADER_SIZE);
            dataRegionBaseByOffset.emplace(segment.offset, dataRegionBase);
        }

        const std::uint64_t recordOffset = dataRegionBase + recordLocalOffset;
        if (entrySize < DATA_RECORD_HEADER_SIZE || !r.has(recordOffset, 4)) {
            pushDiag(section, "datastorage-record-entry-invalid",
                     "DataStorage record entry for handle " + handleKey(handle)
                         + " has an invalid header or payload offset",
                     handle, true, recordHeaderOffset, true);
            continue;
        }

        const std::uint32_t dataSize = r.u32(recordOffset);
        const std::uint64_t dataOffset = recordOffset + 4;
        if (!r.has(dataOffset, dataSize)) {
            pushDiag(section, "datastorage-record-payload-truncated",
                     "DataStorage payload for handle " + handleKey(handle)
                         + " is outside the section bounds",
                     handle, true, dataOffset, true);
            continue;
        }
        if (dataOffset + dataSize > segment.offset + segment.size) {
            pushDiag(section, "datastorage-record-payload-out-of-segment",
                     "DataStorage payload for handle " + handleKey(handle)
                         + " extends beyond its segment bounds",
                     handle, true, dataOffset, true);
            continue;
        }

        DRW_DataStorageRecord record;
        record.handle = handle;
        record.handleKey = handleKey(handle);
        record.isHandleSafe = handle <= 9007199254740991ull;
        record.segmentIndex = entry.segmentIndex;
        record.localOffset = entry.localOffset;
        record.schemaIndex = entry.schemaIndex;
        record.entrySize = entrySize;
        record.recordLocalOffset = recordLocalOffset;
        record.recordOffset = recordOffset;
        record.dataOffset = dataOffset;
        record.dataByteLength = dataSize;
        if (retainPayloads)
            record.payload = r.bytes(dataOffset, dataSize);

        section.records.push_back(std::move(record));
        ++produced;
    }
}

} // namespace

DRW_DataStorageSection DRW_parseDataStorage(const std::uint8_t* data,
                                            std::size_t size,
                                            DRW::Version version) {
    DRW_DataStorageSection section;
    section.m_version = version;
    section.sectionByteLength = size;

    if (data == nullptr || size < HEADER_SIZE) {
        section.parseFailed = true;
        pushDiag(section, "datastorage-section-too-small",
                 "DataStorage section is too small");
        return section;
    }

    ByteReader r(data, size);
    section.signature = r.u32(0);
    section.headerSize = r.i32(4);
    section.version = r.u32(12);
    section.revision = r.u32(20);
    section.segmentIndexOffset = r.u32(24);
    section.segmentIndexEntryCount = r.u32(32);
    section.schemaIndexSegmentIndex = r.i32(36);
    section.dataIndexSegmentIndex = r.i32(40);
    section.searchSegmentIndex = r.i32(44);
    section.previousSaveIndex = r.i32(48);
    section.fileSize = r.u32(52);

    if (static_cast<std::size_t>(section.fileSize) != size) {
        pushDiag(section, "datastorage-file-size-mismatch",
                 "DataStorage header file size " + std::to_string(section.fileSize)
                     + " does not match section byte length "
                     + std::to_string(size));
    }

    section.payloadsRetained = size <= PAYLOAD_BLOB_SECTION_CAP;

    readSegmentIndex(r, section.segmentIndexOffset,
                     section.segmentIndexEntryCount, section);
    readDataIndex(r, section);
    readDataRecords(r, section, section.payloadsRetained);
    return section;
}
