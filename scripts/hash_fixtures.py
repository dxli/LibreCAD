#!/usr/bin/env python3
# File: hash_fixtures.py

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

"""Validate or refresh SHA256 values in the libdxfrw fixture manifest."""

from __future__ import annotations

import argparse
import hashlib
import json
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
    if not isinstance(data, dict):
        raise SystemExit("error: manifest root must be an object")
    if not isinstance(data.get("fixtures"), list):
        raise SystemExit("error: manifest fixtures must be a list")
    return data


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def fixture_path(repo: Path, fixture: dict[str, Any]) -> Path | None:
    raw_path = fixture.get("path")
    if not isinstance(raw_path, str) or not raw_path:
        return None
    path = Path(raw_path)
    if path.is_absolute() or raw_path.startswith("${"):
        return None
    return repo / path


def update_hashes(repo: Path, data: dict[str, Any], check: bool) -> tuple[list[str], int]:
    messages: list[str] = []
    changed = 0
    for index, fixture in enumerate(data.get("fixtures", [])):
        if not isinstance(fixture, dict):
            messages.append(f"error: fixtures[{index}] is not an object")
            continue
        fixture_id = str(fixture.get("id", f"fixtures[{index}]"))
        path = fixture_path(repo, fixture)
        if path is None:
            if fixture.get("defaultEnabled") is True:
                messages.append(f"error: {fixture_id}: default fixture path is not source-relative")
            continue
        if not path.is_file():
            if fixture.get("defaultEnabled") is True:
                messages.append(f"error: {fixture_id}: missing default fixture: {fixture.get('path')}")
            else:
                messages.append(f"skip: {fixture_id}: file is absent: {fixture.get('path')}")
            continue
        actual = sha256_file(path)
        current = fixture.get("sha256")
        if current == actual:
            messages.append(f"ok: {fixture_id}: {actual}")
            continue
        if check:
            messages.append(f"error: {fixture_id}: sha256 mismatch: {current!r} != {actual}")
        else:
            fixture["sha256"] = actual
            changed += 1
            messages.append(f"updated: {fixture_id}: {actual}")
    return messages, changed


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo-root", type=Path, default=repo_root_from_script(__file__))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--check", action="store_true", help="fail when hashes are stale")
    args = parser.parse_args(argv)

    repo = args.repo_root.resolve()
    manifest = args.manifest if args.manifest.is_absolute() else repo / args.manifest
    data = load_manifest(manifest)
    messages, changed = update_hashes(repo, data, args.check)
    for message in messages:
        print(message)
    errors = [message for message in messages if message.startswith("error:")]
    if errors:
        return 1
    if changed and not args.check:
        manifest.write_text(json.dumps(data, indent=2, sort_keys=False) + "\n", encoding="utf-8")
        print(f"wrote {manifest}")
    if not data.get("fixtures"):
        print("ok: no fixtures registered")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
