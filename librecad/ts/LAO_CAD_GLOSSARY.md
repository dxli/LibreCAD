# Lao CAD glossary (authoritative for `librecad_lo` / `plugins_lo`)

Locale: **`lo_LA`**. Apply via `scripts/lao_cad_glossary_apply.py` when refreshing `.ts` files.

## Core nouns

| English | Lao | Notes |
|---------|-----|--------|
| Layer / Layers | ເລເຢີ | CAD loanword; do not mix ຊັ້ນວຽກ / ເລຢີ / ເລເຍີ |
| Entity / Entities | ອົງປະກອບ | Not ວັດຖຸ / ອອບເຈັກ / ເອນຕິຕີ |
| Block / Blocks | ບລັອກ | |
| Dimension / Dimensions | ມິຕິ | Dimensioning tool/menu — not ຂະໜາດ (“size”) |
| Polyline | ເສັ້ນຕໍ່ເນື່ອງ | |
| Spline | ເສັ້ນໂຄ້ງສະປຼາຍ | One transliteration only (ສະປຼາຍ) |
| Hatch | ລວດລາຍ | |
| Insert | ແຊກ | |
| Snap | ເກາະຈັບ | |
| Grid | ຕາຕະລາງ | |
| Viewport | ວິວພອດ | |
| UCS | UCS | Keep English acronym |

## Edit history

| English | Lao |
|---------|-----|
| Undo | ຍົກເລີກ |
| Redo | ເຮັດຄືນ |

Do **not** use ເລີກເຮັດ for Undo.

## Coordinates & snaps

| English | Lao |
|---------|-----|
| Absolute | ແທ້ຈິງ |
| Relative | ສຳພັນ |
| Center | ໃຈກາງ |
| Endpoint | ຈຸດປາຍ |
| Middle | ຈຸດກາງ |
| Intersection | ຈຸດຕັດ |
| Nearest | ໃກ້ທີ່ສຸດ |
| Perpendicular | ຕັ້ງສາກ |
| Tangent | ສຳຜັດ |
| Coordinates | ພິກັດ |
| Radius | ລັດສະໝີ |
| Diameter | ເສັ້ນຜ່ານສູນກາງ |
| Angle | ມຸມ |

## Common UI (stable)

| English | Lao |
|---------|-----|
| File | ໄຟລ໌ |
| Edit | ແກ້ໄຂ |
| View | ມຸມມອງ |
| Draw | ແຕ້ມ |
| Modify | ປັບປ່ຽນ |
| Save | ບັນທຶກ |
| Delete | ລຶບ |
| Line | ເສັ້ນ |
| Circle | ວົງມົນ |
| Arc | ເສັ້ນໂຄ້ງ |
| Point | ຈຸດ |
| Text | ຂໍ້ຄວາມ |

## Policy

1. Prefer **one Lao term per CAD concept** across menus, tooltips, and errors.
2. Keep Qt placeholders (`%1`, `%n`) and accelerators (`&`) intact.
3. English in parentheses only when introducing a loanword the first time is unnecessary if the glossary term is locked.
4. After `lupdate`, re-run `scripts/lao_cad_glossary_apply.py` and spot-check Dimension / Layer / Undo / Absolute / Relative / Center.
