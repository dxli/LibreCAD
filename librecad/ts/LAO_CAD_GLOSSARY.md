# Lao CAD glossary (authoritative for `librecad_lo` / `plugins_lo`)

Locale: **`lo_LA`**. Apply via `scripts/lao_cad_glossary_apply.py` when refreshing `.ts` files.

## Core nouns

| English | Lao | Notes |
|---------|-----|--------|
| Layer / Layers | ເລເຢີ | CAD loanword; do not mix ຊັ້ນວຽກ / ເລຢີ / ເລເຍີ |
| Entity / Entities | ອົງປະກອບ | Not ວັດຖຸ / ອອບເຈັກ / ເອນຕິຕີ |
| Block / Blocks | ບລັອກ | |
| Dimension / Dimensions | ມິຕິ | Dimensioning tool/menu |
| dimension line / styles / regenerate | ເສັ້ນມິຕິ / ຮູບແບບມິຕິ / ສ້າງມິຕິໃໝ່ | Not ເສັ້ນຂະໜາດ |
| size / tick size / arrow size | ຂະໜາດ… | Keep ຂະໜາດ only for true *size* |
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
| Absolute: / Absolute Pos / absolute coordinates | ແທ້ຈິງ: / ຕຳແໜ່ງແທ້ຈິງ / ພິກັດແທ້ຈິງ |
| Relative | ສຳພັນ |
| Relative Zero | ສູນສຳພັນ |
| Center (snap label) | ໃຈກາງ |
| Center (large/small) linetype | ເສັ້ນໃຈກາງ (ໃຫຍ່/ນ້ອຍ) | centerline pattern, not snap |
| Geometric center (circles, etc.) | ຈຸດສູນກາງ / ສູນກາງ (allowed in compounds) |
| Tangential / tangental | ສຳຜັດ | Never ສຳພັນ (that is Relative) |
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

**Do not** use ສຳບູນ / ສຳມະບູນ for Absolute coordinates.  
ສົມບູນ remains valid only for “fully / complete” (*not fully supported*).

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
5. Deep-review compound rules (source-keyword): Absolute*, Dimension*, Layer*, Undo*, Tangent* are applied in the script — re-check counts:
   - `absolute` + ສຳບູນ → 0  
   - `dimension` + ເສັ້ນຂະໜາດ / ຮູບແບບຂະໜາດ → 0  
   - `layer` without ເລເຢີ still using bare ຊັ້ນ → 0  
   - `undo` + ຍ້ອນກັບ → 0  
   - `tangent|tangental` + ສຳພັນ (wrong) → 0; should use ສຳຜັດ  
   - `Center (large|small)` → ເສັ້ນໃຈກາງ (…)
