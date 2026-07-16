/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2026 LibreCAD.org
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
**********************************************************************/
#ifndef LC_IMPORT_REPAIR_FLAGS_H
#define LC_IMPORT_REPAIR_FLAGS_H

/**
 * Runtime switches for import geometry repair vs view-only framing.
 *
 * Env overrides (0/1; unset uses defaults):
 *   LC_DENSE_VIEW_FRAMING      default ON  — getViewBorders dense leaf envelope
 *   LC_REPAIR_BLOCK_DEFS       default OFF — prepareForInsertExpansion mutators
 *   LC_REPAIR_MODEL_PLACEMENT  default OFF — post-updateInserts far re-base / islands
 *
 * Crash/close safety is unconditional and is not gated here.
 */
namespace LC_ImportRepairFlags {

/** Dense leaf envelope for zoomAuto / MDI resize framing (not model coords). */
bool denseViewFraming();

/** Mutate block definitions in prepareForInsertExpansion. */
bool repairBlockDefs();

/** Far re-base compact inserts / model islands after updateInserts. */
bool repairModelPlacement();

/** True when either repair switch is enabled. */
inline bool anyRepairEnabled() {
    return repairBlockDefs() || repairModelPlacement();
}

} // namespace LC_ImportRepairFlags

#endif // LC_IMPORT_REPAIR_FLAGS_H
