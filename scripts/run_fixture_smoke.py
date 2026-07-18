#!/usr/bin/env python3
# File: run_fixture_smoke.py

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

"""Run smoke audits for fixtures listed in the libdxfrw fixture manifest."""

from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path
from typing import Any

from dwg_inventory_common import repo_root_from_script


DEFAULT_MANIFEST = Path("tests/fixtures/fixture_manifest.json")


def load_manifest(path: Path) -> dict[str, Any]:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except OSError as exc:
        raise SystemExit(f"error: cannot read {path}: {exc}") from exc
    except json.JSONDecodeError as exc:
        raise SystemExit(f"error: invalid JSON in {path}: {exc}") from exc
    if not isinstance(data, dict) or not isinstance(data.get("fixtures"), list):
        raise SystemExit("error: manifest root must contain a fixtures list")
    return data


def selected_fixtures(repo: Path, manifest: dict[str, Any], default_only: bool) -> list[dict[str, Any]]:
    fixtures: list[dict[str, Any]] = []
    for index, fixture in enumerate(manifest.get("fixtures", [])):
        if not isinstance(fixture, dict):
            raise SystemExit(f"error: fixtures[{index}] is not an object")
        if default_only and fixture.get("defaultEnabled") is not True:
            continue
        raw_path = fixture.get("path")
        if not isinstance(raw_path, str) or not raw_path:
            raise SystemExit(f"error: {fixture.get('id', index)}: missing path")
        path = Path(raw_path)
        if path.is_absolute() or raw_path.startswith("${"):
            if fixture.get("defaultEnabled") is True:
                raise SystemExit(f"error: {fixture.get('id', index)}: default fixture path must be source-relative")
            continue
        full_path = repo / path
        if not full_path.is_file():
            if fixture.get("defaultEnabled") is True:
                raise SystemExit(f"error: {fixture.get('id', index)}: missing default fixture: {raw_path}")
            continue
        copy = dict(fixture)
        copy["_fullPath"] = str(full_path)
        fixtures.append(copy)
    return fixtures


def audit_binary(explicit: str | None, env_name: str) -> str:
    if explicit:
        return explicit
    return os.environ.get(env_name, "")


def run_audit(tool: str, files: list[Path], allow_missing_tool: bool) -> tuple[dict[str, Any], int]:
    if not files:
        return {"status": "skipped", "diagnostics": ["no files for this format"]}, 0
    if not tool:
        status = "skipped" if allow_missing_tool else "error"
        code = 0 if allow_missing_tool else 2
        return {"status": status, "diagnostics": ["audit helper not configured"]}, code
    command = [tool, "--json", *(str(path) for path in files)]
    try:
        proc = subprocess.run(command, capture_output=True, text=True, timeout=300)
    except OSError as exc:
        return {"status": "error", "command": command, "diagnostics": [str(exc)]}, 2
    try:
        payload = json.loads(proc.stdout) if proc.stdout.strip() else {}
    except json.JSONDecodeError as exc:
        return {
            "status": "failed",
            "command": command,
            "exitCode": proc.returncode,
            "diagnostics": [f"invalid audit JSON: {exc}"],
            "stderr": proc.stderr[-4000:],
        }, 1
    failures = [
        str(item.get("fixture"))
        for item in payload.get("files", [])
        if isinstance(item, dict) and item.get("readOk") is False
    ]
    status = "passed" if proc.returncode == 0 and not failures else "failed"
    result = {
        "status": status,
        "command": command,
        "exitCode": proc.returncode,
        "files": len(files),
        "failedFixtures": failures,
        "stderr": proc.stderr[-4000:],
    }
    return result, 0 if status == "passed" else 1


def print_text(results: dict[str, Any]) -> None:
    print(f"fixture smoke: {results['status']} ({results['selectedFixtures']} selected)")
    for name, result in results["audits"].items():
        print(f"{name}: {result['status']}")
        for diagnostic in result.get("diagnostics", []):
            print(f"  {diagnostic}")
        for fixture in result.get("failedFixtures", []):
            print(f"  failed: {fixture}")


def main(argv: list[str]) -> int:
    repo = repo_root_from_script(__file__)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", type=Path, default=repo)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--default-only", action="store_true", help="smoke only default-enabled fixtures")
    parser.add_argument("--dwg-audit", help="path to built dwg_audit helper")
    parser.add_argument("--dxf-audit", help="path to built dxf_audit helper")
    parser.add_argument(
        "--allow-missing-audit-tools",
        action="store_true",
        help="skip selected fixtures instead of failing when audit helpers are absent",
    )
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args(argv)

    repo = args.repo_root.resolve()
    manifest_path = args.manifest if args.manifest.is_absolute() else repo / args.manifest
    fixtures = selected_fixtures(repo, load_manifest(manifest_path), args.default_only)
    dwg_files = [Path(item["_fullPath"]) for item in fixtures if item.get("format") == "DWG"]
    dxf_files = [Path(item["_fullPath"]) for item in fixtures if item.get("format") == "DXF"]
    dwg_result, dwg_status = run_audit(
        audit_binary(args.dwg_audit, "DWG_AUDIT"),
        dwg_files,
        args.allow_missing_audit_tools,
    )
    dxf_result, dxf_status = run_audit(
        audit_binary(args.dxf_audit, "DXF_AUDIT"),
        dxf_files,
        args.allow_missing_audit_tools,
    )
    exit_code = 1 if dwg_status == 1 or dxf_status == 1 else 2 if dwg_status == 2 or dxf_status == 2 else 0
    status = "passed" if exit_code == 0 else "failed"
    payload = {
        "schema": 1,
        "tool": "run_fixture_smoke",
        "status": status,
        "selectedFixtures": len(fixtures),
        "audits": {
            "DWG": dwg_result,
            "DXF": dxf_result,
        },
    }
    if args.json:
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        print_text(payload)
    return exit_code


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
