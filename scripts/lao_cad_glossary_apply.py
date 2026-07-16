#!/usr/bin/env python3
"""Apply authoritative Lao CAD glossary to LibreCAD .ts translation files.

Edits <translation> text only; preserves XML structure, locations, and
placeholders. Run from repo root:

  python3 scripts/lao_cad_glossary_apply.py librecad/ts/librecad_lo.ts
  python3 scripts/lao_cad_glossary_apply.py plugins/ts/plugins_lo.ts
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

# Exact English source → preferred Lao (menu labels, snap names, core nouns).
EXACT_SOURCE: dict[str, str] = {
    # Core CAD nouns
    "Layer": "ເລເຢີ",
    "Layers": "ເລເຢີ",
    "layer": "ເລເຢີ",
    "layers": "ເລເຢີ",
    "Entity": "ອົງປະກອບ",
    "Entities": "ອົງປະກອບ",
    "entity": "ອົງປະກອບ",
    "entities": "ອົງປະກອບ",
    "Block": "ບລັອກ",
    "Blocks": "ບລັອກ",
    "Dimension": "ມິຕິ",
    "Dimensions": "ມິຕິ",
    "Polyline": "ເສັ້ນຕໍ່ເນື່ອງ",
    "polyline": "ເສັ້ນຕໍ່ເນື່ອງ",
    "Spline": "ເສັ້ນໂຄ້ງສະປຼາຍ",
    "spline": "ເສັ້ນໂຄ້ງສະປຼາຍ",
    "Hatch": "ລວດລາຍ",
    "hatch": "ລວດລາຍ",
    "Insert": "ແຊກ",
    "Snap": "ເກາະຈັບ",
    "Grid": "ຕາຕະລາງ",
    "Viewport": "ວິວພອດ",
    "UCS": "UCS",
    # Edit history
    "Undo": "ຍົກເລີກ",
    "Redo": "ເຮັດຄືນ",
    "&Undo": "&ຍົກເລີກ",
    "&Redo": "&ເຮັດຄືນ",
    # Coordinate / snap semantics
    "Absolute": "ແທ້ຈິງ",
    "Relative": "ສຳພັນ",
    "Center": "ໃຈກາງ",
    "Endpoint": "ຈຸດປາຍ",
    "Middle": "ຈຸດກາງ",
    "Intersection": "ຈຸດຕັດ",
    "Nearest": "ໃກ້ທີ່ສຸດ",
    "Perpendicular": "ຕັ້ງສາກ",
    "Tangent": "ສຳຜັດ",
    "Node": "ໂຫນດ",
    "On Entity": "ບນອົງປະກອບ",
    "On entity": "ບນອົງປະກອບ",
    # Common UI
    "File": "ໄຟລ໌",
    "Edit": "ແກ້ໄຂ",
    "View": "ມຸມມອງ",
    "Draw": "ແຕ້ມ",
    "Modify": "ປັບປ່ຽນ",
    "Tools": "ເຄື່ອງມື",
    "Help": "ຊ່ວຍເຫຼືອ",
    "Options": "ຕົວເລືອກ",
    "Preferences": "ການຕັ້ງຄ່າ",
    "Save": "ບັນທຶກ",
    "Open": "ເປີດ",
    "Close": "ປິດ",
    "Cancel": "ຍົກເລີກ",
    "Delete": "ລຶບ",
    "Copy": "ຄັດລອກ",
    "Paste": "ວາງ",
    "Cut": "ຕັດ",
    "Select": "ເລືອກ",
    "Line": "ເສັ້ນ",
    "Circle": "ວົງມົນ",
    "Arc": "ເສັ້ນໂຄ້ງ",
    "Ellipse": "ວົງລີ",
    "Point": "ຈຸດ",
    "Text": "ຂໍ້ຄວາມ",
    "Radius": "ລັດສະໝີ",
    "Diameter": "ເສັ້ນຜ່ານສູນກາງ",
    "Angle": "ມຸມ",
    "Length": "ຄວາມຍາວ",
    "Distance": "ໄລຍະຫ່າງ",
    "Scale": "ມາດຕາສ່ວນ",
    "Move": "ຍ້າຍ",
    "Rotate": "ໝຸນ",
    "Mirror": "ສະທ້ອນ",
    "Offset": "ອອບເຊັດ",
    "Trim": "ຕັດຂອບ",
    "Extend": "ຂະຫຍາຍ",
    "Explode": "ແຍກ",
    "Export": "ສົ່ງອອກ",
    "Import": "ນຳເຂົ້າ",
    "Print": "ພິມ",
    "Zoom In": "ຊູມເຂົ້າ",
    "Zoom Out": "ຊູມອອກ",
    "Auto Zoom": "ຊູມອັດຕະໂນມັດ",
    "Zoom Window": "ຊູມໜ້າຕ່າງ",
    "Coordinates": "ພິກັດ",
    "Library": "ຄັງສະໝຸດ",
    "Attributes": "ຄຸນລັກສະນະ",
    "Properties": "ຄຸນສົມບັດ",
    "Layers": "ເລເຢີ",
    "Blocks": "ບລັອກ",
    # plugins
    "Align": "ຈັດວາງ",
    "Layer": "ເລເຢີ",
    "POLYLINE": "ເສັ້ນຕໍ່ເນື່ອງ",
    "SPLINE": "ເສັ້ນໂຄ້ງສະປຼາຍ",
    "SPLINEPOINTS": "ຈຸດເສັ້ນໂຄ້ງສະປຼາຍ",
    "HATCH": "ລວດລາຍ",
    "Empty Entity": "ອົງປະກອບຫວ່າງ",
}

# Source-prefix / contains rules applied when exact miss (source, preferred).
# Only used when source equals key case-insensitively after strip.
EXACT_SOURCE_CI: dict[str, str] = {k.lower(): v for k, v in EXACT_SOURCE.items()}

# Ordered phrase replacements inside existing Lao translations (longest first).
# Applied after exact-source overrides.
PHRASE_REPLACEMENTS: list[tuple[str, str]] = [
    # Undo family → ຍົກເລີກ
    ("ເລີກເຮັດການແຕ້ມ", "ຍົກເລີກການແຕ້ມ"),
    ("ເລີກເຮັດສຳລັບ", "ຍົກເລີກສຳລັບ"),
    ("ເລີກເຮັດຈຸດ", "ຍົກເລີກຈຸດ"),
    ("ເລີກເຮັດ", "ຍົກເລີກ"),
    ("ບໍ່ສາມາດເລີກເຮັດ", "ບໍ່ສາມາດຍົກເລີກ"),
    # Layer variants → ເລເຢີ
    ("ຊັ້ນວຽກຂອງວັດຖຸ", "ເລເຢີຂອງອົງປະກອບ"),
    ("ຊັ້ນວຽກ", "ເລເຢີ"),
    ("ຊັ້ນວາງ", "ເລເຢີ"),
    ("ຊັ້ນງານ", "ເລເຢີ"),
    ("ເລຢີ", "ເລເຢີ"),
    ("ເລເຍີ", "ເລເຢີ"),
    ("ເລເຢີ (Layer)", "ເລເຢີ"),
    ("ຊັ້ນ (Layers)", "ເລເຢີ"),
    ("ບລັອກ (Blocks)", "ບລັອກ"),
    ("ຂະໜາດ (Dimensions)", "ມິຕິ"),
    # Entity / object loanwords
    ("ອອບເຈັກ", "ອົງປະກອບ"),
    ("ວັດຖຸ", "ອົງປະກອບ"),
    # Dimension menu sense: bare ຂະໜາດ for Dimension tools often wrong;
    # only replace common dimension-line phrases carefully via exact sources.
    # Spline transliteration unify
    ("ສະພລາຍ", "ສະປຼາຍ"),
    ("ສະປາຍ", "ສະປຼາຍ"),
    ("ເສັ້ນໂຄ້ງສະປຼາຍສະປຼາຍ", "ເສັ້ນໂຄ້ງສະປຼາຍ"),  # accidental double
    # Relative typo
    ("ສຳພັດ", "ສຳພັນ"),
    # Center line misused for center point
    ("ເສັ້ນໃຈກາງ", "ໃຈກາງ"),
    # Absolute was "complete"
    # only via exact Absolute source; also fix common wrong phrase:
    ("ພິກັດສົມບູນ", "ພິກັດແທ້ຈິງ"),
    # Hatch parenthetical noise
    ("ລວດລາຍ (Hatch)", "ລວດລາຍ"),
    ("ລວດລາຍ (HATCH)", "ລວດລາຍ"),
    # Grid was "line grid"
    ("ເສັ້ນຕາຕະລາງ", "ຕາຕະລາງ"),
    # Diameter consistency
    ("ເສັ້ນຜ່ານໃຈກາງ", "ເສັ້ນຜ່ານສູນກາງ"),
    # Soft cleanup of double spaces
    ("  ", " "),
]

# When source is exactly these, force Absolute/Relative even if prior phrase pass missed.
SOURCE_FORCE: dict[str, str] = {
    "Absolute": "ແທ້ຈິງ",
    "Relative": "ສຳພັນ",
    "Center": "ໃຈກາງ",
    "Dimension": "ມິຕິ",
    "Dimensions": "ມິຕິ",
    "Undo": "ຍົກເລີກ",
    "Redo": "ເຮັດຄືນ",
    "Layer": "ເລເຢີ",
    "Layers": "ເລເຢີ",
    "Entity": "ອົງປະກອບ",
    "Entities": "ອົງປະກອບ",
    "Spline": "ເສັ້ນໂຄ້ງສະປຼາຍ",
    "Polyline": "ເສັ້ນຕໍ່ເນື່ອງ",
    "Hatch": "ລວດລາຍ",
    "Grid": "ຕາຕະລາງ",
    "Snap": "ເກາະຈັບ",
    "Block": "ບລັອກ",
    "Blocks": "ບລັອກ",
}


def unescape_src(s: str) -> str:
    return (
        s.replace("&amp;", "&")
        .replace("&lt;", "<")
        .replace("&gt;", ">")
        .replace("&quot;", '"')
        .replace("&apos;", "'")
    )


def apply_phrases(tr: str) -> str:
    for old, new in PHRASE_REPLACEMENTS:
        if old in tr:
            tr = tr.replace(old, new)
    return tr


def transform_translation(source: str, translation: str) -> str:
    src = unescape_src(source).strip()
    # Empty / vanished placeholders: leave
    if not translation.strip():
        return translation

    # Exact glossary win
    if src in SOURCE_FORCE:
        return SOURCE_FORCE[src]
    if src in EXACT_SOURCE:
        return EXACT_SOURCE[src]
    if src.lower() in EXACT_SOURCE_CI and len(src) < 40:
        # Only for short labels to avoid clobbering sentences
        return EXACT_SOURCE_CI[src.lower()]

    # Menu accelerators like &Layer
    if src.startswith("&") and src[1:] in SOURCE_FORCE:
        return "&" + SOURCE_FORCE[src[1:]]

    tr = apply_phrases(translation)

    # Source-aware sentence fixes
    if re.search(r"\b[Dd]imension", src):
        # Prefer ມິຕິ over bare size for dimensioning UI
        if tr.strip() == "ຂະໜາດ":
            tr = "ມິຕິ"
        tr = tr.replace("ເສັ້ນບອກຂະໜາດ", "ເສັ້ນມິຕິ")
        tr = tr.replace("ຂໍ້ຄວາມຂະໜາດ", "ຂໍ້ຄວາມມິຕິ")
    if re.search(r"\b[Uu]ndo\b", src):
        tr = tr.replace("ເລີກເຮັດ", "ຍົກເລີກ")
        if tr.strip() in ("ເລີກເຮັດ", "ກັບຄືນ"):
            tr = "ຍົກເລີກ"
    if re.search(r"\b[Aa]bsolute\b", src):
        if tr.strip() in ("ສົມບູນ", "ສຳເລັດ"):
            tr = "ແທ້ຈິງ"
    if re.search(r"\b[Rr]elative\b", src):
        if tr.strip() in ("ສຳພັດ", "ຄວາມສຳພັນ"):
            tr = "ສຳພັນ"
    if re.fullmatch(r"[Cc]enter", src):
        tr = "ໃຈກາງ"
    if re.search(r"\b[Ee]ntit", src):
        tr = tr.replace("ອອບເຈັກ", "ອົງປະກອບ")
        tr = tr.replace("ວັດຖຸ", "ອົງປະກອບ")

    return tr


MESSAGE_RE = re.compile(
    r"(<message(?:\s[^>]*)?>)(.*?)(</message>)",
    re.S,
)
SOURCE_RE = re.compile(r"<source>(.*?)</source>", re.S)
TRANS_RE = re.compile(
    r"(<translation)(\s+type=\"[^\"]*\")?(\s*>)(.*?)(</translation>)"
    r"|(<translation)(\s+type=\"[^\"]*\")?(\s*/>)",
    re.S,
)


def process_message(body: str) -> tuple[str, bool]:
    sm = SOURCE_RE.search(body)
    if not sm:
        return body, False
    source = sm.group(1)

    changed = False

    def repl_trans(m: re.Match) -> str:
        nonlocal changed
        if m.group(6):  # self-closing
            return m.group(0)
        open_tag, typ, mid, content, close = (
            m.group(1),
            m.group(2) or "",
            m.group(3),
            m.group(4),
            m.group(5),
        )
        # Do not rewrite vanished/obsolete content heavily — still phrase-fix finished
        if 'type="vanished"' in typ or 'type="obsolete"' in typ:
            return m.group(0)
        new_content = transform_translation(source, content)
        if new_content != content:
            changed = True
        return f"{open_tag}{typ}{mid}{new_content}{close}"

    new_body, n = TRANS_RE.subn(repl_trans, body)
    return new_body, changed


def process_file(path: Path) -> int:
    text = path.read_text(encoding="utf-8")
    # Normalize language tag for main app
    if path.name == "librecad_lo.ts":
        text2 = re.sub(
            r'<TS version="2\.1" language="lo(?:_LA)?">',
            '<TS version="2.1" language="lo_LA">',
            text,
            count=1,
        )
    else:
        text2 = re.sub(
            r'<TS version="2\.1" language="lo(?:_LA)?">',
            '<TS version="2.1" language="lo_LA">',
            text,
            count=1,
        )
    changes = 0 if text2 == text else 1
    text = text2

    out_parts: list[str] = []
    pos = 0
    for m in MESSAGE_RE.finditer(text):
        out_parts.append(text[pos : m.start()])
        open_m, body, close_m = m.group(1), m.group(2), m.group(3)
        new_body, ch = process_message(body)
        if ch:
            changes += 1
        out_parts.append(open_m + new_body + close_m)
        pos = m.end()
    out_parts.append(text[pos:])
    new_text = "".join(out_parts)
    path.write_text(new_text, encoding="utf-8")
    return changes


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print(__doc__)
        return 2
    total = 0
    for p in argv[1:]:
        path = Path(p)
        n = process_file(path)
        total += n
        print(f"{path}: {n} message(s)/header updated")
    print(f"total change events: {total}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
