// File: lc_wipeout.h

/*
 * ********************************************************************************
 * This file is part of the LibreCAD project, a 2D CAD program
 *
 * Copyright (C) 2026 LibreCAD (librecad.org)
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

#ifndef LC_WIPEOUT_H
#define LC_WIPEOUT_H

#include <cstdint>
#include <vector>

#include "rs_atomicentity.h"

struct LC_WipeoutData {
  LC_WipeoutData() = default;
  explicit LC_WipeoutData(std::vector<RS_Vector> verts)
      : vertices(std::move(verts)) {}

  // Imported WIPEOUTs retain their raster-image frame verbatim.  `vertices`
  // is always derived from this frame for drawing/hit testing; it is never an
  // alternate source of truth while hasNativeFrame is true.
  bool hasNativeFrame = false;
  RS_Vector insertionPoint;
  RS_Vector uPixel;
  RS_Vector vPixel;
  double sizeU = 0.0;
  double sizeV = 0.0;
  int displayProps = 0;
  std::uint32_t imageDefHandle = 0;
  std::uint32_t imageDefReactorHandle = 0;
  int clip = 0;
  int brightness = 50;
  int contrast = 50;
  int fade = 0;
  int clipBoundaryType = 2;
  bool clipMode = false;
  std::vector<RS_Vector> clipPath;
  std::vector<RS_Vector> vertices;

  void rebuildWorldVertices();
};

class LC_Wipeout : public RS_AtomicEntity {
public:
  LC_Wipeout(RS_EntityContainer *parent, LC_WipeoutData d);

  RS_Entity *clone() const override;

  RS2::EntityType rtti() const override { return RS2::EntityWipeout; }

  const LC_WipeoutData &getData() const { return m_data; }
  const std::vector<RS_Vector> &getVertices() const { return m_data.vertices; }

  void calculateBorders() override;
  void draw(RS_Painter *painter) override;


  void move(const RS_Vector &offset) override;
  void rotate(const RS_Vector &center, double angle) override;
  void rotate(const RS_Vector &center, const RS_Vector &angleVector) override;
  void scale(const RS_Vector &center, const RS_Vector &factor) override;
  void mirror(const RS_Vector &axisPoint1,
              const RS_Vector &axisPoint2) override;
  RS_Entity &shear(double k) override;

protected:
  LC_WipeoutData m_data;

  RS_Vector doGetNearestEndpoint(const RS_Vector& coord, double* dist, RS_Entity** entity) const override;
  RS_Vector doGetNearestRef(const RS_Vector& coord, double* dist) const override;
  RS_Vector doGetNearestSelectedRef(const RS_Vector& coord, double* dist) const override;
  RS_Vector doGetNearestCenter(const RS_Vector& coord, double* dist, RS_Entity** centerEntity) const override;
  RS_Vector doGetNearestPointOnEntity(const RS_Vector& coord, bool onEntity,
                                      double* dist,
                                      RS_Entity** entity) const override;
  RS_Vector doGetNearestMiddle(const RS_Vector& coord, double* dist,
                               int middlePoints) const override;
  RS_Vector doGetNearestDist(double distance, const RS_Vector& coord,
                             double* dist = nullptr) const override;
  double doGetDistanceToPoint(const RS_Vector& coord,
                              RS_Entity** entity = nullptr,
                              RS2::ResolveLevel level = RS2::ResolveNone,
                              double solidDist = RS_MAXDOUBLE) const override;

};

#endif
