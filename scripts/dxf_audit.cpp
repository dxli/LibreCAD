// DXF read-coverage audit harness (build-and-run, not part of the test suite).
// Reads each .dxf argument through libdxfrw's dxfRW (ASCII + binary) and prints
// either the legacy pipe-separated summary or a stable JSON audit document with:
//   --json FILE...
//
// Build (from repo root):
//   clang++ -std=c++17 -O1 -I libraries/libdxfrw/src \
//     scripts/dxf_audit.cpp \
//     libraries/libdxfrw/src/*.cpp libraries/libdxfrw/src/intern/*.cpp \
//     -o /tmp/dxf_audit
//   /tmp/dxf_audit --json <files...>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "drw_base.h"
#include "drw_interface.h"
#include "drw_objects.h"
#include "libdxfrw.h"

namespace {
class CountIface : public DRW_Interface {
public:
    long m_ents = 0, m_objs = 0;
    std::map<std::string, long> m_entityTypes;
    std::map<std::string, long> m_objectTypes;
    std::map<std::string, long> m_callbacks;
    std::map<std::string, int> m_rawEnt;  // unmodeled ENTITIES (by type name)
    std::map<std::string, int> m_rawObj;  // unmodeled OBJECTS (by type name)

    void entity(const char* name, const char* callback) {
        ++m_ents;
        ++m_entityTypes[name];
        ++m_callbacks[callback];
    }

    void object(const char* name, const char* callback) {
        ++m_objs;
        ++m_objectTypes[name];
        ++m_callbacks[callback];
    }

    void addHeader(const DRW_Header*) override {}
    void addLType(const DRW_LType&) override { object("LTYPE", "addLType"); }
    void addLayer(const DRW_Layer&) override { object("LAYER", "addLayer"); }
    void addDimStyle(const DRW_Dimstyle&) override { object("DIMSTYLE", "addDimStyle"); }
    void addVport(const DRW_Vport&) override { object("VPORT", "addVport"); }
    void addTextStyle(const DRW_Textstyle&) override { object("STYLE", "addTextStyle"); }
    void addAppId(const DRW_AppId&) override { object("APPID", "addAppId"); }
    void addBlock(const DRW_Block&) override { object("BLOCK", "addBlock"); }
    void setBlock(const int) override {}
    void endBlock() override {}
    void addPoint(const DRW_Point&) override { entity("POINT", "addPoint"); }
    void addLine(const DRW_Line&) override { entity("LINE", "addLine"); }
    void addRay(const DRW_Ray&) override { entity("RAY", "addRay"); }
    void addXline(const DRW_Xline&) override { entity("XLINE", "addXline"); }
    void addCircle(const DRW_Circle&) override { entity("CIRCLE", "addCircle"); }
    void addArc(const DRW_Arc&) override { entity("ARC", "addArc"); }
    void addEllipse(const DRW_Ellipse&) override { entity("ELLIPSE", "addEllipse"); }
    void addLWPolyline(const DRW_LWPolyline&) override { entity("LWPOLYLINE", "addLWPolyline"); }
    void addPolyline(const DRW_Polyline&) override { entity("POLYLINE", "addPolyline"); }
    void addSpline(const DRW_Spline*) override { entity("SPLINE", "addSpline"); }
    void addKnot(const DRW_Entity&) override {}
    void addInsert(const DRW_Insert&) override { entity("INSERT", "addInsert"); }
    void addTrace(const DRW_Trace&) override { entity("TRACE", "addTrace"); }
    void add3dFace(const DRW_3Dface&) override { entity("3DFACE", "add3dFace"); }
    void addSolid(const DRW_Solid&) override { entity("SOLID", "addSolid"); }
    void addMText(const DRW_MText&) override { entity("MTEXT", "addMText"); }
    void addText(const DRW_Text&) override { entity("TEXT", "addText"); }
    void addDimAlign(const DRW_DimAligned*) override { entity("DIMENSION_ALIGNED", "addDimAlign"); }
    void addDimLinear(const DRW_DimLinear*) override { entity("DIMENSION_LINEAR", "addDimLinear"); }
    void addDimRadial(const DRW_DimRadial*) override { entity("DIMENSION_RADIAL", "addDimRadial"); }
    void addDimDiametric(const DRW_DimDiametric*) override { entity("DIMENSION_DIAMETRIC", "addDimDiametric"); }
    void addDimAngular(const DRW_DimAngular*) override { entity("DIMENSION_ANGULAR", "addDimAngular"); }
    void addDimAngular3P(const DRW_DimAngular3p*) override { entity("DIMENSION_ANGULAR3P", "addDimAngular3P"); }
    void addDimArc(const DRW_DimArc*) override { entity("ARC_DIMENSION", "addDimArc"); }
    void addDimOrdinate(const DRW_DimOrdinate*) override { entity("DIMENSION_ORDINATE", "addDimOrdinate"); }
    void addLeader(const DRW_Leader*) override { entity("LEADER", "addLeader"); }
    void addHatch(const DRW_Hatch*) override { entity("HATCH", "addHatch"); }
    void addViewport(const DRW_Viewport&) override { entity("VIEWPORT", "addViewport"); }
    void addImage(const DRW_Image*) override { entity("IMAGE", "addImage"); }
    void linkImage(const DRW_ImageDef*) override { object("IMAGEDEF", "linkImage"); }
    void addComment(const char*) override {}
    void addPlotSettings(const DRW_PlotSettings*) override { object("PLOTSETTINGS", "addPlotSettings"); }
    void addRawDxfEntity(const DRW_RawDxfObject& d) override { ++m_rawEnt[d.name]; }
    void addRawDxfObject(const DRW_RawDxfObject& d) override { ++m_rawObj[d.name]; }
    void writeHeader(DRW_Header&) override {}
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
};

std::string joinMap(const std::map<std::string, int>& m) {
    std::string s;
    for (const auto& kv : m) {
        if (!s.empty()) s += ",";
        s += kv.first + ":" + std::to_string(kv.second);
    }
    return s.empty() ? "-" : s;
}

std::string jsonEscape(const std::string& value) {
    std::ostringstream out;
    for (unsigned char ch : value) {
        switch (ch) {
        case '"': out << "\\\""; break;
        case '\\': out << "\\\\"; break;
        case '\b': out << "\\b"; break;
        case '\f': out << "\\f"; break;
        case '\n': out << "\\n"; break;
        case '\r': out << "\\r"; break;
        case '\t': out << "\\t"; break;
        default:
            if (ch < 0x20) {
                out << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(ch)
                    << std::dec << std::setfill(' ');
            } else {
                out << static_cast<char>(ch);
            }
        }
    }
    return out.str();
}

template <typename Value>
void printJsonMap(std::ostream& out, const std::map<std::string, Value>& values) {
    out << "{";
    bool first = true;
    for (const auto& kv : values) {
        if (!first) out << ",";
        first = false;
        out << "\"" << jsonEscape(kv.first) << "\":" << kv.second;
    }
    out << "}";
}

std::string versionCode(DRW::Version version) {
    for (const auto& kv : DRW::dwgVersionStrings) {
        if (kv.second == version) {
            return kv.first;
        }
    }
    return "UNKNOWN";
}

struct AuditResult {
    std::string path;
    DRW::Version version = DRW::UNKNOWNV;
    DRW::error error = DRW::BAD_NONE;
    bool ok = false;
    CountIface counts;
};

AuditResult auditFile(const char* path) {
    AuditResult result;
    result.path = path;
    dxfRW r(path);
    try {
        result.ok = r.read(&result.counts, /*ext=*/true);
    } catch (...) {
        result.ok = false;
    }
    result.version = r.getVersion();
    result.error = r.getError();
    return result;
}

void printJsonFile(std::ostream& out, const AuditResult& result) {
    out << "{";
    out << "\"fixture\":\"" << jsonEscape(result.path) << "\",";
    out << "\"format\":\"DXF\",";
    out << "\"version\":\"" << jsonEscape(versionCode(result.version)) << "\",";
    out << "\"versionEnum\":" << static_cast<int>(result.version) << ",";
    out << "\"supportLevel\":\"audit-only\",";
    out << "\"dwgTsParityLevel\":\"not-evaluated\",";
    out << "\"dwgTsParityDelta\":\"not-evaluated\",";
    out << "\"writerMode\":\"not-evaluated\",";
    out << "\"writerDelta\":\"not-evaluated\",";
    out << "\"readOk\":" << (result.ok ? "true" : "false") << ",";
    out << "\"error\":" << static_cast<unsigned>(result.error) << ",";
    out << "\"counters\":{";
    out << "\"entityCallbacks\":" << result.counts.m_ents << ",";
    out << "\"objectCallbacks\":" << result.counts.m_objs << ",";
    out << "\"rawEntityTypes\":" << result.counts.m_rawEnt.size() << ",";
    out << "\"rawObjectTypes\":" << result.counts.m_rawObj.size();
    out << "},";
    out << "\"entities\":";
    printJsonMap(out, result.counts.m_entityTypes);
    out << ",\"objects\":";
    printJsonMap(out, result.counts.m_objectTypes);
    out << ",\"callbacks\":";
    printJsonMap(out, result.counts.m_callbacks);
    out << ",\"classes\":[],";
    out << "\"rawEntities\":";
    printJsonMap(out, result.counts.m_rawEnt);
    out << ",\"rawObjects\":";
    printJsonMap(out, result.counts.m_rawObj);
    out << ",\"dataSections\":[],";
    out << "\"diagnostics\":[";
    bool firstDiag = true;
    auto addDiag = [&](const char* severity, const char* code, int count) {
        if (count == 0) return;
        if (!firstDiag) out << ",";
        firstDiag = false;
        out << "{\"severity\":\"" << severity << "\",\"code\":\"" << code << "\",\"count\":" << count << "}";
    };
    for (const auto& kv : result.counts.m_rawEnt) {
        addDiag("info", ("raw-entity-" + kv.first).c_str(), kv.second);
    }
    for (const auto& kv : result.counts.m_rawObj) {
        addDiag("info", ("raw-object-" + kv.first).c_str(), kv.second);
    }
    if (!result.ok) {
        if (!firstDiag) out << ",";
        out << "{\"severity\":\"error\",\"code\":\"read-failed\",\"count\":1}";
    }
    out << "]";
    out << "}";
}
} // namespace

int main(int argc, char** argv) {
    bool json = false;
    std::vector<const char*> paths;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--json") {
            json = true;
        } else {
            paths.push_back(argv[i]);
        }
    }

    if (json) {
        std::cout << "{\"schema\":1,\"tool\":\"dxf_audit\",\"format\":\"DXF\",\"files\":[";
        for (size_t i = 0; i < paths.size(); ++i) {
            if (i != 0) std::cout << ",";
            printJsonFile(std::cout, auditFile(paths[i]));
        }
        std::cout << "]}\n";
        return 0;
    }

    for (const char* path : paths) {
        AuditResult result = auditFile(path);
        std::printf("%s|ok=%d|err=0x%X|ents=%ld|objs=%ld|rawEnt=[%s]|rawObj=[%s]\n",
                    path, result.ok ? 1 : 0, static_cast<unsigned>(result.error),
                    result.counts.m_ents, result.counts.m_objs,
                    joinMap(result.counts.m_rawEnt).c_str(), joinMap(result.counts.m_rawObj).c_str());
        std::fflush(stdout);
    }
    return 0;
}
