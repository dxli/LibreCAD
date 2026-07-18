/*
 * ********************************************************************************
 * This file is part of the LibreCAD project, a 2D CAD program
 *
 * Copyright (C) 2026 LibreCAD.org
 * Copyright (C) 2026 Dongxu Li (github.com/dxli)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 * ********************************************************************************
 */

#ifndef LC_INSERT_TRANSFORM_H
#define LC_INSERT_TRANSFORM_H

#include "rs_vector.h"

struct RS_InsertData;

enum class LC_InsertTransformStatus {
    Ok,
    InvalidFields,
    UnsupportedNormal,
    NonFiniteResult
};

enum class LC_InsertTransformDecompositionStatus {
    Ok,
    NonFinite,
    Degenerate,
    Shear
};

enum class LC_InsertSourceEditStatus {
    Ok,
    InvalidEdit,
    InvalidSource,
    UnsupportedSourceNormal,
    NonFinite,
    Unrepresentable
};

struct LC_InsertTransformParts {
    double scaleX = 1.0;
    double scaleY = 1.0;
    double angle = 0.0;
    bool reversesOrientation = false;
};

/**
 * Planar affine map used for INSERT expansion and exact source-field edits.
 * It is deliberately independent of BLOCK traversal and derived children.
 */
struct LC_InsertTransform {
    static constexpr double Zero = 0.0;
    static constexpr double IdentityScale = 1.0;

    double a = IdentityScale;
    double b = Zero;
    double c = Zero;
    double d = IdentityScale;
    double tx = Zero;
    double ty = Zero;

    [[nodiscard]] bool isFinite() const;
    [[nodiscard]] RS_Vector mapVector(const RS_Vector& vector) const;
    [[nodiscard]] RS_Vector mapLeafPoint(const RS_Vector& point) const;

    static bool compose(const LC_InsertTransform& parent,
                        const LC_InsertTransform& child,
                        LC_InsertTransform& result);
    static bool translation(const RS_Vector& offset, LC_InsertTransform& result);
    static bool rotation(const RS_Vector& center, double angle,
                         LC_InsertTransform& result);
    static bool scale(const RS_Vector& center, const RS_Vector& factor,
                      LC_InsertTransform& result);
    static bool reflection(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2,
                           LC_InsertTransform& result);
    static LC_InsertTransformStatus fromInsert(const RS_InsertData& data,
                                                const RS_Vector& base, int column,
                                                int row, LC_InsertTransform& result);

    [[nodiscard]] LC_InsertTransformDecompositionStatus
    decompose(LC_InsertTransformParts& result) const;

private:
    static RS_Vector mapArrayOffset(const RS_Vector& spacing, int column, int row,
                                    double cosine, double sine, double ocsXAxis);
};

[[nodiscard]] const char* lcInsertTransformStatusName(LC_InsertTransformStatus status);
[[nodiscard]] const char* lcInsertTransformDecompositionStatusName(
    LC_InsertTransformDecompositionStatus status);
[[nodiscard]] const char* lcInsertSourceEditStatusName(
    LC_InsertSourceEditStatus status);
[[nodiscard]] const RS_Vector& lcInsertTransformOrigin();
[[nodiscard]] const RS_Vector& lcInsertTransformReflectionAxisPoint();
[[nodiscard]] double lcInsertTransformFullTurnRadians();

/**
 * Applies a world-space edit to INSERT source fields only when the result is
 * representable by axial OCS, rotation, diagonal scales, and MINSERT spacing.
 */
[[nodiscard]] LC_InsertSourceEditStatus
lcApplyInsertSourceEdit(const RS_InsertData& source, const LC_InsertTransform& edit,
                        RS_InsertData& result);

#endif
