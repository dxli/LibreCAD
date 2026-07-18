#!/usr/bin/env python3
# File: run_external_oracles.py

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

"""Run optional libdxfrw DWG/DXF external-oracle checks from a JSON config."""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path
from typing import Any

from dwg_inventory_common import repo_root_from_script


DEFAULT_CONFIG = Path("tests/fixtures/oracles.json")
FATAL_DWGREAD = re.compile(r"Failed to decode|^ERROR 0x|Assertion failed", re.MULTILINE)


def load_json(path: Path) -> dict[str, Any]:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except OSError as exc:
        raise SystemExit(f"error: cannot read {path}: {exc}") from exc
    except json.JSONDecodeError as exc:
        raise SystemExit(f"error: invalid JSON in {path}: {exc}") from exc
    if not isinstance(data, dict):
        raise SystemExit(f"error: oracle config root must be an object: {path}")
    return data


def mode(config: dict[str, Any], name: str) -> str:
    entry = config.get(name, {})
    if not isinstance(entry, dict):
        return "optional"
    return str(entry.get("mode", "optional"))


def path_from_env(entry: dict[str, Any], fallback_env: str | None = None) -> str:
    env_name = str(entry.get("env", "") or "")
    if env_name and os.environ.get(env_name):
        return os.environ[env_name]
    if fallback_env and os.environ.get(fallback_env):
        return os.environ[fallback_env]
    configured = str(entry.get("path", "") or "")
    if configured:
        return configured
    return ""


def finish_missing(result: dict[str, Any], required: bool, reason: str) -> int:
    result["status"] = "missing-required" if required else "skipped"
    result["diagnostics"].append(reason)
    return 2 if required else 0


def run_command(result: dict[str, Any], command: list[str], cwd: Path | None = None) -> int:
    result["command"] = command
    try:
        proc = subprocess.run(command, cwd=cwd, capture_output=True, text=True, timeout=300)
    except OSError as exc:
        result["status"] = "error"
        result["diagnostics"].append(str(exc))
        return 2
    except subprocess.TimeoutExpired as exc:
        result["status"] = "failed"
        result["diagnostics"].append(f"timeout after {exc.timeout}s")
        return 1
    result["exitCode"] = proc.returncode
    result["stdout"] = proc.stdout[-4000:]
    result["stderr"] = proc.stderr[-4000:]
    result["status"] = "passed" if proc.returncode == 0 else "failed"
    return 0 if proc.returncode == 0 else 1


def list_files(corpus: Path, suffix: str) -> list[Path]:
    if not corpus.is_dir():
        return []
    return sorted(path for path in corpus.rglob(f"*{suffix}") if path.is_file())


def run_dwgts(repo: Path, config: dict[str, Any], corpus: Path, quiet: bool) -> tuple[dict[str, Any], int]:
    entry = config.get("dwgts", {})
    entry = entry if isinstance(entry, dict) else {}
    required = mode(config, "dwgts") == "required"
    result = {"name": "dwgts", "mode": mode(config, "dwgts"), "status": "not-run", "diagnostics": []}
    dwgts_path = path_from_env(entry, "DWGTS_CLI")
    if not dwgts_path:
        return result, finish_missing(result, required, "DWGTS_CLI is not configured")
    candidate = Path(dwgts_path).expanduser()
    if candidate.name == "cad-to-json.cjs":
        candidate = candidate.parents[2]
    if not (candidate / "dist/cli/cad-to-json.cjs").is_file():
        return result, finish_missing(result, required, f"dwgTs CLI not built under {candidate}")
    dwg_files = list_files(corpus, ".dwg")
    if not dwg_files:
        result["status"] = "skipped"
        result["diagnostics"].append(f"no .dwg files under {corpus}")
        return result, 0
    command = [sys.executable, str(repo / "scripts/dwgts_oracle.py"), str(corpus), "--dwgts", str(candidate)]
    if quiet:
        command.append("--quiet")
    return result, run_command(result, command, cwd=repo)


def run_ezdxf(repo: Path, config: dict[str, Any], corpus: Path, quiet: bool) -> tuple[dict[str, Any], int]:
    entry = config.get("ezdxf", {})
    entry = entry if isinstance(entry, dict) else {}
    required = mode(config, "ezdxf") == "required"
    result = {"name": "ezdxf", "mode": mode(config, "ezdxf"), "status": "not-run", "diagnostics": []}
    if not list_files(corpus, ".dxf"):
        result["status"] = "skipped"
        result["diagnostics"].append(f"no .dxf files under {corpus}")
        return result, 0
    python = str(entry.get("python", "") or sys.executable)
    if not shutil.which(python) and not Path(python).exists():
        return result, finish_missing(result, required, f"python executable not found: {python}")
    command = [python, str(repo / "scripts/ezdxf_audit.py"), str(corpus)]
    if quiet:
        command.append("--quiet")
    return result, run_command(result, command, cwd=repo)


def run_libredwg(config: dict[str, Any], corpus: Path) -> tuple[dict[str, Any], int]:
    entry = config.get("libredwg", {})
    entry = entry if isinstance(entry, dict) else {}
    required = mode(config, "libredwg") == "required"
    result = {"name": "libredwg", "mode": mode(config, "libredwg"), "status": "not-run", "diagnostics": []}
    dwgread = path_from_env(entry, "DWGREAD")
    if not dwgread:
        return result, finish_missing(result, required, "LIBREDWG_DWGREAD/DWGREAD is not configured")
    dwgread_path = Path(dwgread).expanduser()
    if not dwgread_path.is_file() or not os.access(dwgread_path, os.X_OK):
        return result, finish_missing(result, required, f"dwgread is not executable: {dwgread_path}")
    dwg_files = list_files(corpus, ".dwg")
    if not dwg_files:
        result["status"] = "skipped"
        result["diagnostics"].append(f"no .dwg files under {corpus}")
        return result, 0

    failures: list[str] = []
    result["command"] = [str(dwgread_path), "<fixture>"]
    result["checkedFiles"] = len(dwg_files)
    for path in dwg_files:
        proc = subprocess.run([str(dwgread_path), str(path)], capture_output=True, text=True, timeout=120)
        output = proc.stdout + proc.stderr
        if proc.returncode != 0 or FATAL_DWGREAD.search(output):
            failures.append(f"{path}: exit={proc.returncode}")
    if failures:
        result["status"] = "failed"
        result["diagnostics"].extend(failures[:20])
        return result, 1
    result["status"] = "passed"
    return result, 0


def run_oda(repo: Path, config: dict[str, Any], corpus: Path) -> tuple[dict[str, Any], int]:
    entry = config.get("oda", {})
    entry = entry if isinstance(entry, dict) else {}
    required = mode(config, "oda") == "required"
    result = {"name": "oda", "mode": mode(config, "oda"), "status": "not-run", "diagnostics": []}
    oda = path_from_env(entry, "ODAFC")
    if not oda:
        oda = path_from_env(entry, "ODA_FILE_CONVERTER")
    if not oda:
        return result, finish_missing(result, required, "ODA_FILE_CONVERTER/ODAFC is not configured")
    oda_path = Path(oda).expanduser()
    if not oda_path.is_file() or not os.access(oda_path, os.X_OK):
        return result, finish_missing(result, required, f"ODA File Converter is not executable: {oda_path}")
    if not list_files(corpus, ".dwg") and not list_files(corpus, ".dxf"):
        result["status"] = "skipped"
        result["diagnostics"].append(f"no DWG/DXF files under {corpus}")
        return result, 0
    output = repo / "tmp/oracle-oda"
    command = [str(repo / "scripts/oda-validate.sh"), str(corpus), str(output)]
    env = os.environ.copy()
    env["ODAFC"] = str(oda_path)
    result["command"] = command
    try:
        proc = subprocess.run(command, cwd=repo, env=env, capture_output=True, text=True, timeout=300)
    except OSError as exc:
        result["status"] = "error"
        result["diagnostics"].append(str(exc))
        return result, 2
    result["exitCode"] = proc.returncode
    result["stdout"] = proc.stdout[-4000:]
    result["stderr"] = proc.stderr[-4000:]
    result["status"] = "passed" if proc.returncode == 0 else "failed"
    return result, 0 if proc.returncode == 0 else 1


def print_text(results: list[dict[str, Any]]) -> None:
    for result in results:
        line = f"{result['name']}: {result['status']} ({result['mode']})"
        if result.get("checkedFiles") is not None:
            line += f" files={result['checkedFiles']}"
        print(line)
        for diagnostic in result.get("diagnostics", []):
            print(f"  {diagnostic}")


def main(argv: list[str]) -> int:
    repo = repo_root_from_script(__file__)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path, default=DEFAULT_CONFIG)
    parser.add_argument("--corpus", type=Path, default=Path("tests/fixtures/corpus"))
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--quiet", action="store_true")
    parser.add_argument("--only", choices=["dwgts", "libredwg", "ezdxf", "oda"], action="append")
    args = parser.parse_args(argv)

    config_path = args.config if args.config.is_absolute() else repo / args.config
    corpus = args.corpus if args.corpus.is_absolute() else repo / args.corpus
    config = load_json(config_path)
    selected = set(args.only or ["dwgts", "libredwg", "ezdxf", "oda"])

    results: list[dict[str, Any]] = []
    exit_code = 0
    runners = {
        "dwgts": lambda: run_dwgts(repo, config, corpus, args.quiet),
        "libredwg": lambda: run_libredwg(config, corpus),
        "ezdxf": lambda: run_ezdxf(repo, config, corpus, args.quiet),
        "oda": lambda: run_oda(repo, config, corpus),
    }
    for name in ("dwgts", "libredwg", "ezdxf", "oda"):
        if name not in selected:
            continue
        result, status = runners[name]()
        results.append(result)
        if status == 1:
            exit_code = 1
        elif status == 2 and exit_code == 0:
            exit_code = 2

    payload = {"schema": 1, "tool": "run_external_oracles", "corpus": str(corpus), "results": results}
    if args.json:
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        print_text(results)
    return exit_code


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
