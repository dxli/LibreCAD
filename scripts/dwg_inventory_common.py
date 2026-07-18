#!/usr/bin/env python3
# File: dwg_inventory_common.py

# /*
#  * ********************************************************************************
#  * This file is part of the LibreCAD project, a 2D CAD program
#  *
#  * Copyright (C) 2025 LibreCAD.org
#  * Copyright (C) 2025 Dongxu Li (github.com/dxli)
#  *
#  * This program is free software; you can redistribute it and/or
#  * modify it under the terms of the GNU General Public License
#  * as published by the Free Software Foundation; either version 2
#  * of the License, or (at your option) any later version.
#  *
#  * This program is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with this program; if not, write to the Free Software
#  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
#  * USA.
#  * ********************************************************************************
#  */

"""Shared helpers for libdxfrw DWG/DXF inventory scripts."""

from __future__ import annotations

import difflib
import re
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class VersionInfo:
    enum_name: str
    code: str
    release: str
    order: int


def repo_root_from_script(script_path: str) -> Path:
    return Path(script_path).resolve().parents[1]


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except OSError as exc:
        raise SystemExit(f"error: cannot read {path}: {exc}") from exc


def markdown_cell(value: object) -> str:
    text = str(value).replace("\n", " ").replace("|", "\\|")
    return text if text else "-"


def write_or_check(output: Path, text: str, check: bool, label: str) -> int:
    if not text.endswith("\n"):
        text += "\n"
    if check:
        current = output.read_text(encoding="utf-8") if output.exists() else ""
        if current != text:
            sys.stderr.write(f"error: {label} is stale: {output}\n")
            diff = difflib.unified_diff(
                current.splitlines(),
                text.splitlines(),
                fromfile=str(output),
                tofile=f"{output} (generated)",
                lineterm="",
            )
            for line in list(diff)[:200]:
                sys.stderr.write(line + "\n")
            sys.stderr.write(f"refresh with: {Path(sys.argv[0]).as_posix()}\n")
            return 1
        return 0
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(text, encoding="utf-8")
    print(f"wrote {output}")
    return 0


def extract_function(text: str, signature: str) -> str:
    start = text.find(signature)
    if start < 0:
        return ""
    body_start = text.find("{", start)
    if body_start < 0:
        return ""
    depth = 0
    for index in range(body_start, len(text)):
        ch = text[index]
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0:
                return text[start : index + 1]
    return text[start:]


def extract_cpp_cases(block: str) -> set[int]:
    return {int(number) for number in re.findall(r"\bcase\s+(\d+)\s*:", block)}


def extract_cpp_string_names(block: str) -> set[str]:
    names = set(
        re.findall(
            r"\b(?:nextentity|nextobject|rn|recName|className)\s*==\s*\"([^\"]+)\"",
            block,
        )
    )
    names.update(re.findall(r"\{\s*\"([A-Za-z0-9_:]+)\"\s*,\s*\"AcDb", block))
    names.update(re.findall(r"writeString\(\s*0\s*,\s*\"([A-Za-z0-9_:]+)\"", block))
    return names


def extract_dwg_typed_names(block: str) -> set[str]:
    """Find custom DWG class names whose branch emits a typed callback.

    Fixed object IDs use ``case`` labels, while post-R13 custom classes are
    selected from ``recName``/``className``.  A name alone is insufficient: the
    raw fallback names a class too, but emits only ``addUnsupportedObject``.
    Restrict this surface to nearby branches that call ``intfa.add...``.
    """
    lines = block.splitlines()
    typed: set[str] = set()
    name_pattern = re.compile(r"\b(?:recName|className)\s*==\s*\"([^\"]+)\"")
    for index, line in enumerate(lines):
        names = name_pattern.findall(line)
        if not names:
            continue
        if any("intfa.add" in candidate for candidate in lines[index : index + 20]):
            typed.update(names)
    return typed


def normalize_name(name: str) -> str:
    name = name.strip().upper()
    name = name.removeprefix("DWG_TYPE_")
    replacements = {
        "FACE3D": "3DFACE",
        "_3DFACE": "3DFACE",
        "SOLID3D": "3DSOLID",
        "_3DSOLID": "3DSOLID",
        "DIMENSION_ANG_2_LN": "DIMENSION_ANG2LN",
        "DIMENSION_ANG_3_PT": "DIMENSION_ANG3PT",
        "BLOCK_HEADER": "BLOCK_RECORD",
        "BLOCK_CONTROL_OBJ": "BLOCK_CONTROL",
        "LAYER_CONTROL_OBJ": "LAYER_CONTROL",
        "STYLE_CONTROL_OBJ": "STYLE_CONTROL",
        "LTYPE_CONTROL_OBJ": "LTYPE_CONTROL",
        "VIEW_CONTROL_OBJ": "VIEW_CONTROL",
        "UCS_CONTROL_OBJ": "UCS_CONTROL",
        "VPORT_CONTROL_OBJ": "VPORT_CONTROL",
        "APPID_CONTROL_OBJ": "APPID_CONTROL",
        "DIMSTYLE_CONTROL_OBJ": "DIMSTYLE_CONTROL",
        "VP_ENT_HDR_CTRL_OBJ": "VX_CONTROL",
        "VP_ENT_HDR": "VX_TABLE_RECORD",
        "PLACEHOLDER": "ACDBPLACEHOLDER",
        "PROXY_ENTITY": "ACAD_PROXY_ENTITY",
        "PROXY_OBJECT": "ACAD_PROXY_OBJECT",
        "TABLE": "ACAD_TABLE",
        "MLEADER": "MULTILEADER",
        "PDFREFERENCE": "PDFUNDERLAY",
        "DGNREFERENCE": "DGNUNDERLAY",
        "DWFREFERENCE": "DWFUNDERLAY",
        "DICTIONARYWDFLT": "ACDBDICTIONARYWDFLT",
    }
    return replacements.get(name, name)


def candidate_names(name: str) -> set[str]:
    normalized = normalize_name(name)
    out = {normalized}
    if normalized.startswith("ACDB"):
        out.add(normalized[4:])
    else:
        out.add("ACDB" + normalized)
    extra = {
        "3DFACE": {"FACE3D", "_3DFACE"},
        "3DSOLID": {"SOLID3D", "_3DSOLID"},
        "MULTILEADER": {"MLEADER"},
        "ACDBDICTIONARYWDFLT": {"DICTIONARYWDFLT"},
        "ACDBPLACEHOLDER": {"PLACEHOLDER"},
        "ACAD_TABLE": {"TABLE"},
        "PDFUNDERLAY": {"PDFREFERENCE"},
        "DGNUNDERLAY": {"DGNREFERENCE"},
        "DWFUNDERLAY": {"DWFREFERENCE"},
    }
    out.update(extra.get(normalized, set()))
    return out


def normalized_set(names: set[str]) -> set[str]:
    out: set[str] = set()
    for name in names:
        out.update(candidate_names(name))
    return out


def parse_versions(repo: Path) -> list[VersionInfo]:
    header = read_text(repo / "libraries/libdxfrw/src/drw_base.h")
    enum_match = re.search(r"enum\s+Version\s*\{(.*?)\n\s*\};", header, re.S)
    map_match = re.search(r"dwgVersionStrings\s*\{(.*?)\n\s*\};", header, re.S)
    if not enum_match or not map_match:
        raise SystemExit("error: cannot parse DRW::Version definitions")

    release_by_enum: dict[str, str] = {}
    order_by_enum: dict[str, int] = {}
    order = 0
    for raw_line in enum_match.group(1).splitlines():
        line = raw_line.strip()
        match = re.match(r"([A-Z0-9_]+)\s*,\s*//!<\s*(.*?)\s*$", line)
        if not match:
            continue
        enum_name, release = match.groups()
        order_by_enum[enum_name] = order
        release_by_enum[enum_name] = release.rstrip(".")
        order += 1

    versions: list[VersionInfo] = []
    for code, enum_name in re.findall(r"\{\s*\"([^\"]+)\"\s*,\s*DRW::([A-Z0-9_]+)\s*\}", map_match.group(1)):
        versions.append(
            VersionInfo(
                enum_name=enum_name,
                code=code,
                release=release_by_enum.get(enum_name, "unknown"),
                order=order_by_enum.get(enum_name, 999),
            )
        )
    return sorted(versions, key=lambda item: item.order)


def local_reader_surfaces(repo: Path) -> dict[str, set[str] | set[int] | bool]:
    libdxfrw = read_text(repo / "libraries/libdxfrw/src/libdxfrw.cpp")
    dwg_reader = read_text(repo / "libraries/libdxfrw/src/intern/dwgreader.cpp")

    entity_fn = extract_function(dwg_reader, "bool dwgReader::readDwgEntity")
    object_fn = extract_function(dwg_reader, "bool dwgReader::readDwgObject")
    dxf_entities_fn = extract_function(libdxfrw, "bool dxfRW::processEntities")
    dxf_objects_fn = extract_function(libdxfrw, "bool dxfRW::processObjects")

    table_records = {
        "APPID",
        "BLOCK",
        "BLOCK_RECORD",
        "DIMSTYLE",
        "ENDBLK",
        "LAYER",
        "LTYPE",
        "STYLE",
        "UCS",
        "VIEW",
        "VPORT",
    }
    dwg_table_object_ids = {
        0x30,
        0x31,
        0x32,
        0x33,
        0x34,
        0x35,
        0x38,
        0x39,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x40,
        0x41,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
    }
    # These fixed types are selected through named dwgType enum labels in the
    # reader, so extract_cpp_cases() cannot see their numeric values. Keep the
    # inventory aligned with intern/dwgutil.h rather than reporting typed
    # ATTRIB/ATTDEF and child entities as absent.
    dwg_child_entity_ids = {0x02, 0x03, 0x04, 0x05, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E}

    class_names = normalized_set(
        set(re.findall(r"\{\s*\"([A-Za-z0-9_:]+)\"\s*,\s*\"AcDb", libdxfrw))
    )

    return {
        "dwg_entity_cases": extract_cpp_cases(entity_fn) | dwg_child_entity_ids,
        "dwg_object_cases": extract_cpp_cases(object_fn) | dwg_table_object_ids,
        "dwg_typed_names": normalized_set(extract_dwg_typed_names(entity_fn)
                                            | extract_dwg_typed_names(object_fn)),
        "dwg_names": normalized_set(extract_cpp_string_names(entity_fn) | extract_cpp_string_names(object_fn)),
        "dxf_entities": normalized_set(extract_cpp_string_names(dxf_entities_fn) | table_records),
        "dxf_objects": normalized_set(extract_cpp_string_names(dxf_objects_fn)),
        "class_names": class_names,
        "has_dxf_raw_entity": "addRawDxfEntity" in libdxfrw,
        "has_dxf_raw_object": "addRawDxfObject" in libdxfrw,
        "has_dxf_classes": "bool dxfRW::processClasses" in libdxfrw,
    }


def family_for(name: str) -> str:
    upper = normalize_name(name)
    if upper.startswith("ACDS") or "ACDSPROTOTYPE" in upper:
        return "data-storage/classes"
    if "DIM" in upper or "MLEADER" in upper or upper in {"TEXT", "MTEXT", "ACAD_TABLE", "TABLESTYLE", "FIELD", "FIELDLIST"}:
        return "annotation/context"
    if "POINTCLOUD" in upper or "NAVISWORKS" in upper:
        return "point-cloud/model-reference"
    if any(token in upper for token in ("UNDERLAY", "IMAGE", "WIPEOUT", "RASTER", "PDF", "DGN", "DWF")):
        return "raster-underlay"
    if any(token in upper for token in ("SURFACE", "ACSH", "3DSOLID", "BODY", "REGION", "MESH", "ACIS")):
        return "3D/modeler"
    if upper.startswith("ASSOC") or "PARAMETER" in upper or "ACTION" in upper or "GRIP" in upper:
        return "parametric/dynamic-block"
    if any(token in upper for token in ("MATERIAL", "VISUAL", "RENDER", "BACKGROUND", "SUN", "LIGHT")):
        return "render/material"
    if upper.startswith("GEO") or "MAP" in upper:
        return "geospatial"
    if "PROXY" in upper or "OLE" in upper:
        return "proxy/embedded"
    if upper in {"DICTIONARY", "XRECORD", "GROUP", "LAYOUT", "STYLE", "LAYER", "LTYPE", "DIMSTYLE", "VPORT", "VIEW", "UCS", "APPID", "BLOCK_RECORD"} or upper.endswith("_CONTROL"):
        return "database/table"
    if upper.startswith("ACAM") or upper.startswith("ACA") or upper.startswith("AEC"):
        return "vertical/proprietary"
    return "core-geometry"
