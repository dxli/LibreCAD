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

#include "lc_import_repair_flags.h"

#include <cstdlib>
#include <cstring>

namespace {

/**
 * Parse env as 0/1. Returns defaultValue when unset or empty.
 * Accepts "0", "false", "off", "no" (case-insensitive) as false;
 * "1", "true", "on", "yes" as true; otherwise defaultValue.
 */
bool envFlag(const char *name, bool defaultValue) {
    const char *v = std::getenv(name);
    if (v == nullptr || v[0] == '\0')
        return defaultValue;

    if (std::strcmp(v, "0") == 0
        || std::strcmp(v, "false") == 0
        || std::strcmp(v, "FALSE") == 0
        || std::strcmp(v, "off") == 0
        || std::strcmp(v, "OFF") == 0
        || std::strcmp(v, "no") == 0
        || std::strcmp(v, "NO") == 0) {
        return false;
    }
    if (std::strcmp(v, "1") == 0
        || std::strcmp(v, "true") == 0
        || std::strcmp(v, "TRUE") == 0
        || std::strcmp(v, "on") == 0
        || std::strcmp(v, "ON") == 0
        || std::strcmp(v, "yes") == 0
        || std::strcmp(v, "YES") == 0) {
        return true;
    }
    return defaultValue;
}

} // namespace

namespace LC_ImportRepairFlags {

bool denseViewFraming() {
    // Recommended default ON: fixes MDI framing without rewriting coordinates.
    return envFlag("LC_DENSE_VIEW_FRAMING", true);
}

bool repairBlockDefs() {
    // Recommended default OFF: fidelity-preserving; chicun UX uses dense framing.
    return envFlag("LC_REPAIR_BLOCK_DEFS", false);
}

bool repairModelPlacement() {
    // Recommended default OFF: same as repairBlockDefs.
    return envFlag("LC_REPAIR_MODEL_PLACEMENT", false);
}

} // namespace LC_ImportRepairFlags
