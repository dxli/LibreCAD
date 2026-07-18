#!/usr/bin/env python3
# File: audit_diff.py

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

"""Compare two libdxfrw audit JSON snapshots by stable fixture identity."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any


COUNT_FIELDS = {
    "entityCallbacks": "entity callback count",
    "objectCallbacks": "object callback count",
    "entityParseFailures": "DWG entity parse failures",
    "objectParseFailures": "DWG object parse failures",
    "decodedProxyPrimitives": "decoded proxy primitives",
    "rawEntityTypes": "DXF raw entity type count",
    "rawObjectTypes": "DXF raw object type count",
}


def load_json(path: Path) -> dict[str, Any]:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except OSError as exc:
        raise SystemExit(f"error: cannot read {path}: {exc}") from exc
    except json.JSONDecodeError as exc:
        raise SystemExit(f"error: invalid JSON in {path}: {exc}") from exc
    if not isinstance(data, dict):
        raise SystemExit(f"error: audit JSON root must be an object: {path}")
    return data


def audit_files(data: dict[str, Any]) -> list[dict[str, Any]]:
    files = data.get("files")
    if isinstance(files, list):
        return [item for item in files if isinstance(item, dict)]
    if "fixture" in data:
        return [data]
    return []


def by_identity(data: dict[str, Any]) -> dict[str, dict[str, Any]]:
    out: dict[str, dict[str, Any]] = {}
    for item in audit_files(data):
        key = str(item.get("fixture", ""))
        if not key:
            key = str(item.get("id", f"index:{len(out)}"))
        out[key] = item
    return out


def numeric(value: Any) -> int:
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    return 0


def mapping(value: Any) -> dict[str, int]:
    if not isinstance(value, dict):
        return {}
    return {str(k): numeric(v) for k, v in value.items()}


def diff_mapping(
    diffs: list[dict[str, Any]],
    fixture: str,
    field: str,
    before: dict[str, int],
    after: dict[str, int],
) -> None:
    for key in sorted(set(before) | set(after)):
        old = before.get(key, 0)
        new = after.get(key, 0)
        if old == new:
            continue
        severity = "critical" if new < old else "warning"
        diffs.append(
            {
                "severity": severity,
                "fixture": fixture,
                "field": f"{field}.{key}",
                "before": old,
                "after": new,
                "message": f"{field} count changed for {key}: {old} -> {new}",
            }
        )


def compare_fixture(fixture: str, before: dict[str, Any], after: dict[str, Any]) -> list[dict[str, Any]]:
    diffs: list[dict[str, Any]] = []
    if bool(before.get("readOk")) and not bool(after.get("readOk")):
        diffs.append(
            {
                "severity": "critical",
                "fixture": fixture,
                "field": "readOk",
                "before": True,
                "after": False,
                "message": "read regressed from success to failure",
            }
        )
    elif not bool(before.get("readOk")) and bool(after.get("readOk")):
        diffs.append(
            {
                "severity": "info",
                "fixture": fixture,
                "field": "readOk",
                "before": False,
                "after": True,
                "message": "read improved from failure to success",
            }
        )

    before_counts = mapping(before.get("counters"))
    after_counts = mapping(after.get("counters"))
    for field, label in COUNT_FIELDS.items():
        old = before_counts.get(field, 0)
        new = after_counts.get(field, 0)
        if old == new:
            continue
        regressed = field.endswith("Failures") and new > old
        lost_coverage = field in {"entityCallbacks", "objectCallbacks", "decodedProxyPrimitives"} and new < old
        severity = "critical" if regressed or lost_coverage else "warning"
        diffs.append(
            {
                "severity": severity,
                "fixture": fixture,
                "field": f"counters.{field}",
                "before": old,
                "after": new,
                "message": f"{label} changed: {old} -> {new}",
            }
        )

    for field in ("entities", "objects", "rawEntities", "rawObjects", "callbacks"):
        diff_mapping(diffs, fixture, field, mapping(before.get(field)), mapping(after.get(field)))
    return diffs


def compare(before_data: dict[str, Any], after_data: dict[str, Any]) -> dict[str, Any]:
    before = by_identity(before_data)
    after = by_identity(after_data)
    diffs: list[dict[str, Any]] = []
    for fixture in sorted(set(before) - set(after)):
        diffs.append(
            {
                "severity": "critical",
                "fixture": fixture,
                "field": "files",
                "before": "present",
                "after": "missing",
                "message": "fixture disappeared from audit output",
            }
        )
    for fixture in sorted(set(after) - set(before)):
        diffs.append(
            {
                "severity": "info",
                "fixture": fixture,
                "field": "files",
                "before": "missing",
                "after": "present",
                "message": "fixture added to audit output",
            }
        )
    for fixture in sorted(set(before) & set(after)):
        diffs.extend(compare_fixture(fixture, before[fixture], after[fixture]))

    severities = {str(item.get("severity")) for item in diffs}
    if "critical" in severities:
        failure_level = "critical"
    elif "warning" in severities:
        failure_level = "warning"
    elif "info" in severities:
        failure_level = "info"
    else:
        failure_level = "none"
    return {
        "schema": 1,
        "tool": "audit_diff",
        "failureLevel": failure_level,
        "summary": {
            "beforeFiles": len(before),
            "afterFiles": len(after),
            "differences": len(diffs),
            "critical": sum(1 for item in diffs if item.get("severity") == "critical"),
            "warning": sum(1 for item in diffs if item.get("severity") == "warning"),
            "info": sum(1 for item in diffs if item.get("severity") == "info"),
        },
        "differences": diffs,
    }


def print_text(result: dict[str, Any]) -> None:
    summary = result["summary"]
    print(
        "audit diff: "
        f"{result['failureLevel']} "
        f"({summary['critical']} critical, {summary['warning']} warning, {summary['info']} info)"
    )
    for item in result["differences"]:
        print(
            f"{item['severity'].upper():8} {item['fixture']} {item['field']}: "
            f"{item['before']} -> {item['after']}  {item['message']}"
        )


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("before", type=Path)
    parser.add_argument("after", type=Path)
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    parser.add_argument("--fail-on-warning", action="store_true")
    args = parser.parse_args(argv)

    result = compare(load_json(args.before), load_json(args.after))
    if args.json:
        print(json.dumps(result, indent=2, sort_keys=True))
    else:
        print_text(result)
    if result["failureLevel"] == "critical":
        return 1
    if args.fail_on_warning and result["failureLevel"] == "warning":
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
