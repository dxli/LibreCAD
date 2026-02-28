/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2015-2024 LibreCAD.org
** Copyright (C) 2015-2024 Dongxu Li (dongxuli2011@gmail.com)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**********************************************************************/

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <numeric>

#include "lc_quadratic.h"
#include "lc_hyperbola.h"
#include "lc_parabola.h"

#include "rs_atomicentity.h"
#include "rs_circle.h"
#include "rs_debug.h"
#include "rs_ellipse.h"
#include "rs_information.h"
#include "rs_line.h"
#include "rs_math.h"
#include "rs_pen.h"
#include "rs_point.h"
#include "rs_polyline.h"

#ifdef EMU_C99
#include "emu_c99.h" /* C99 math */
#endif

#include "lc_quadraticutils.h"
#include "rs_math.h"

namespace LC_QuadraticUtils {

bool isValid(const LC_Quadratic& q) {
  return !(RS_Math::equal(q.getA(), 0) &&
           RS_Math::equal(q.getB(), 0) &&
           RS_Math::equal(q.getC(), 0) &&
           RS_Math::equal(q.getD(), 0) &&
           RS_Math::equal(q.getE(), 0) &&
           RS_Math::equal(q.getF(), 0));
}

bool isQuadratic(const LC_Quadratic& q) {
  return std::hypot(q.getA(), q.getB(), q.getC()) > RS_TOLERANCE * 100;
}

bool isDegenerate(const LC_Quadratic& q) {
  double det3 = computeHomogeneousDeterminant(q);
  double s = computeScale(q);
  return std::abs(det3) < 1e-8 * s * s;
}

bool isParabolaCondition(double discriminant, const LC_Quadratic& q)
{
  double mag = std::max({
      std::abs(q.getA()),
      std::abs(q.getB()),
      std::abs(q.getC()),
      1e-10
  });

         // Require discriminant to be very close to zero relative to quadratic terms
  double relTol = 1e-10 * mag * mag;

         // Also require that at least one quadratic coefficient is meaningfully non-zero
  bool hasQuadraticTerm = mag > RS_TOLERANCE * 10;

  return hasQuadraticTerm && std::abs(discriminant) <= relTol;
}

RS_Vector computeCenter(const LC_Quadratic& q) {
  double detQ = 4 * q.getA() * q.getC() - q.getB() * q.getB();
  double qn = computeQuadNorm(q);
  if (std::abs(detQ) <= RS_TOLERANCE * qn * qn) {
    return RS_Vector(false);
  }
  return RS_Vector(
      (q.getB()*q.getE() - 2*q.getC()*q.getD()) / detQ,
      (q.getB()*q.getD() - 2*q.getA()*q.getE()) / detQ
      );
}

double evaluateAt(const LC_Quadratic& q, const RS_Vector& p) {
  double x = p.x, y = p.y;
  return q.getA()*x*x + q.getB()*x*y + q.getC()*y*y + q.getD()*x + q.getE()*y + q.getF();
}

double computeDiscriminant(const LC_Quadratic& q) {
  return q.getB() * q.getB() - 4 * q.getA() * q.getC();
}

double computeQuadNorm(const LC_Quadratic& q) {
  return std::hypot(q.getA(), q.getB(), q.getC());
}

double computeLinearNorm(const LC_Quadratic& q) {
  return std::hypot(q.getD(), q.getE());
}

double computeScale(const LC_Quadratic& q) {
  return std::max({computeQuadNorm(q), computeLinearNorm(q), std::abs(q.getF()), 1e-6});
}

double computeHomogeneousDeterminant(const LC_Quadratic& q) {
  double A = q.getA(), h = q.getB()/2., C = q.getC();
  double D = q.getD(), E = q.getE(), F = q.getF();

  return A*(C*F - E*E/4) - h*(h*F - D*E/4) + (D/2)*(h*E/2 - C*D/2);
}

double computeRotationAngle(const LC_Quadratic& q) {
  double B = q.getB();
  double diff = q.getA() - q.getC();
  if (std::abs(B) > RS_TOLERANCE) {
    return 0.5 * std::atan2(B, diff);
  }
  return (q.getA() < q.getC() - RS_TOLERANCE) ? M_PI / 2.0 : 0.0;
}

std::pair<double, double> computePrincipalCoefficients(const LC_Quadratic& q, double theta) {
  double ct = std::cos(theta), st = std::sin(theta);
  double Ap = q.getA()*ct*ct + q.getB()*ct*st + q.getC()*st*st;
  double Cp = q.getA()*st*st - q.getB()*ct*st + q.getC()*ct*ct;
  return {Ap, Cp};
}

std::array<RS_Vector, 3> computeParabolaControlPoints(
    const RS_Vector& center, double p, const RS_Vector& axis, const RS_Vector& perp)
{
  return {
      center + p*4*axis - 4*p*perp,
      center,
      center + p*4*axis + 4*p*perp
  };
}

std::pair<RS_Vector, double> computeHyperbolaMajorPAndRatio(
    double Ap, double Cp, double v, double theta)
{
  double a2 = -v / Ap;
  double b2 = -v / Cp;
  if (a2 <= 0 || b2 <= 0 || std::min(a2, b2) < 1e-8) {
    return {{}, 0};
  }

  bool xTrans = Ap * v < 0;
  double semiTrans = std::sqrt(xTrans ? a2 : b2);
  double semiConj  = std::sqrt(xTrans ? b2 : a2);

  RS_Vector majorP = xTrans
                         ? RS_Vector(std::cos(theta)*semiTrans, std::sin(theta)*semiTrans)
                         : RS_Vector(-std::sin(theta)*semiTrans, std::cos(theta)*semiTrans);

  return {majorP, semiConj / semiTrans};
}

std::tuple<double, double, RS_Vector> computeEllipseSemiAxesAndMajorP(
    double Ap, double Cp, double v, double theta)
{
  double α = -v / Ap;
  double β = -v / Cp;

  if (α <= 0 || β <= 0 || α < 1e-8 || β < 1e-8) {
    return {0, 0, RS_Vector()};
  }

  double sm = std::sqrt(std::max(α, β));
  double sn = std::sqrt(std::min(α, β));
  double r = sn / sm;

  RS_Vector mp = α >= β
                     ? RS_Vector(std::cos(theta)*sm, std::sin(theta)*sm)
                     : RS_Vector(-std::sin(theta)*sm, std::cos(theta)*sm);

  return {sm, r, mp};
}

// ─── Entity creation helpers ──────────────────────────────────────────────

RS_Entity* createLineFromLinearCoefficients(const LC_Quadratic& q) {
  double D = q.getD(), E = q.getE(), F = q.getF();
  double ln = std::hypot(D, E);
  double s = std::max({ln, std::abs(F), 1e-6});
  const double e = RS_TOLERANCE;

  if (ln < e * s) return nullptr;

  RS_Vector base = std::abs(E) > e ? RS_Vector(0, -F/E) : RS_Vector(-F/D, 0);
  RS_Vector dir  = std::abs(E) > e ? RS_Vector(-E, D) : RS_Vector(0, 1);
  dir.normalize();

  return new RS_Line(nullptr, {base - dir*400, base + dir*400});
}

/**
 * Creates a degenerate entity (point or pair of intersecting lines)
 * from a degenerate quadratic when a finite center is present.
 *
 * @param q              The original degenerate quadratic
 * @param center         The computed center point (must be valid)
 * @param valueAtCenter  The quadratic evaluated at the center (should be ≈0)
 * @return
 *   - RS_Point* if degenerate to a single point
 *   - RS_Polyline* if degenerate to two intersecting lines
 *   - nullptr if invalid or imaginary
 */
RS_Entity* createDegeneratePointOrIntersecting(
    const LC_Quadratic& q,
    const RS_Vector& center,
    double valueAtCenter)
{
  if (!center.valid) {
    return nullptr;
  }

  double scale = computeScale(q);

         // If value at center is significantly non-zero → imaginary / invalid
  if (std::abs(valueAtCenter) > 1e-6 * scale) {
    return nullptr;
  }

         // Try to factor the quadratic at the center point (y = center.y fixed)
  double yy = center.y;
  double aa = q.getA();
  double bb = q.getB() * yy + q.getD();
  double cc = q.getC() * yy * yy + q.getE() * yy + q.getF();

  double dd = bb * bb - 4 * aa * cc;

         // Not a factorable quadratic at this y → single point degenerate
  if (std::abs(dd) >= 1e-6 * scale * scale || std::abs(aa) < RS_TOLERANCE) {
    return new RS_Point(nullptr, RS_PointData(center));
  }

         // Compute the two x roots (intersection points with horizontal line y = center.y)
  double sd = std::sqrt(std::max(0.0, dd));
  double x1 = (-bb + sd) / (2 * aa);
  double x2 = (-bb - sd) / (2 * aa);

  RS_Vector dir1(x1 - center.x, yy - center.y);
  RS_Vector dir2(x2 - center.x, yy - center.y);

         // Check if directions are effectively the same (double line / parallel)
  double cosAngle = std::abs(dir1.dotP(dir2)) /
                    (dir1.magnitude() * dir2.magnitude() + 1e-10);

  if (cosAngle > 0.999 || dir1.squared() < 1e-8 || dir2.squared() < 1e-8) {
    // Degenerate to a single line (double line or parallel case)
    RS_Vector commonDir = dir1.magnitude() > dir2.magnitude() ? dir1 : dir2;
    return new RS_Line(nullptr, {center - commonDir*500, center + commonDir*500});
  }

         // Two distinct intersecting lines → create polyline with two segments
  auto poly = new RS_Polyline(nullptr, RS_PolylineData());
  RS_Vector u1 = dir1.normalized();
  RS_Vector u2 = dir2.normalized();

  poly->addVertex(center - u1 * 500);
  poly->addVertex(center + u1 * 500);
  poly->addVertex(center - u2 * 500);
  poly->addVertex(center + u2 * 500);

  return poly;
}

RS_Entity* createEllipseOrCircle(
    const LC_Quadratic& q,
    const RS_Vector& center,
    double valueAtCenter,
    double Ap,
    double Cp,
    double theta)
{
  double alpha = -valueAtCenter / Ap;
  double beta  = -valueAtCenter / Cp;

  if (alpha <= 0 || beta <= 0 || alpha < 1e-8 || beta < 1e-8) {
    return new RS_Point(nullptr, RS_PointData(center));
  }

  double semiMajor = std::sqrt(std::max(alpha, beta));
  double semiMinor = std::sqrt(std::min(alpha, beta));
  double ratio     = semiMinor / semiMajor;

  RS_Vector majorP;
  if (alpha >= beta) {
    majorP = RS_Vector(std::cos(theta) * semiMajor, std::sin(theta) * semiMajor);
  } else {
    majorP = RS_Vector(-std::sin(theta) * semiMajor, std::cos(theta) * semiMajor);
  }

         // Fast-path circle detection
  if (ratio > 0.999 && std::abs(theta) < 1e-5) {
    return new RS_Circle(nullptr, RS_CircleData(center, semiMajor));
  }

  RS_EllipseData ed;
  ed.center     = center;
  ed.majorP     = majorP;
  ed.ratio      = ratio;
  ed.angle1     = 0.0;
  ed.angle2     = 2.0 * M_PI;
  ed.reversed = false;

  return new RS_Ellipse(nullptr, ed);
}

RS_Entity* createParabola(
    const LC_Quadratic& q,
    const RS_Vector& center,
    double valueAtCenter,
    double Ap,
    double Cp)
{
  // ─── Determine parabola axis direction ───────────────────────────────
  RS_Vector axis;
  double B = q.getB();
  double A = q.getA();
  double C = q.getC();

  if (std::abs(B) < RS_TOLERANCE) {
    axis = (std::abs(A) <= std::abs(C)) ? RS_Vector(1,0) : RS_Vector(0,1);
  } else {
    double angle = 0.5 * std::atan2(B, A - C);
    axis = RS_Vector(std::cos(angle), std::sin(angle));
  }
  axis.normalize();

  RS_Vector perp(-axis.y, axis.x);

         // ─── Estimate focal parameter p (4p = latus rectum) ───────────────────
  double denom = std::abs(Ap) + std::abs(Cp) + 0.5 * std::abs(B);
  double p = std::abs(valueAtCenter) / (denom + RS_TOLERANCE);

  if (p < 1e-6) {
    return new RS_Point(nullptr, RS_PointData(center));
  }

  if (valueAtCenter < 0) {
    perp = -perp;
  }

         // ─── Three control points (t = -2, 0, +2) ─────────────────────────────
  std::array<RS_Vector, 3> cps = {
      center + p*4*axis - 4*p*perp,
      center,
      center + p*4*axis + 4*p*perp
  };

  LC_ParabolaData data;
  data.controlPoints = cps;

  return new LC_Parabola(nullptr, data);
}

RS_Entity* createHyperbola(
    const LC_Quadratic& q,
    const RS_Vector& center,
    double valueAtCenter,
    double Ap,
    double Cp,
    double theta)
{
  double a2 = -valueAtCenter / Ap;
  double b2 = -valueAtCenter / Cp;

  if (a2 <= 0 || b2 <= 0 || std::min(a2, b2) < 1e-8) {
    return nullptr;
  }

  bool xTransverse = Ap * valueAtCenter < 0;
  double semiTrans = std::sqrt(xTransverse ? a2 : b2);
  double semiConj  = std::sqrt(xTransverse ? b2 : a2);

  RS_Vector majorP = xTransverse
                         ? RS_Vector(std::cos(theta) * semiTrans, std::sin(theta) * semiTrans)
                         : RS_Vector(-std::sin(theta) * semiTrans, std::cos(theta) * semiTrans);

  LC_HyperbolaData data;
  data.center   = center;
  data.majorP   = majorP;
  data.ratio    = semiConj / semiTrans;
  data.angle1   = 0.0;
  data.angle2   = 2.0 * M_PI;
  data.reversed = false;

  return new LC_Hyperbola(nullptr, data);
}

RS_Entity* createDualAroundCenter(
    RS_Entity* entity,
    const RS_Vector& center)
{
  if (!entity || !center.valid) {
    return nullptr;
  }

         // 1. Get quadratic representation of the original entity
  LC_Quadratic q = entity->getQuadratic();
  if (!q.isValid()) {
    return nullptr;
  }

         // 2. Compute the point-symmetric dual around the given center
  LC_Quadratic dualQ = q.move(- center).getDualCurve().move(center);

         // 3. Convert the dual quadratic back to an entity
  std::unique_ptr<RS_Entity> dualEntity(dualQ.toEntity());
  if (!dualEntity) {
    return nullptr;
  }

         // 4. Preserve original visual attributes (pen, layer)
  dualEntity->setPen(entity->getPen());
  dualEntity->setLayer(entity->getLayer());

         // Return ownership to caller
  return dualEntity.release();
}

}
  // namespace LC_QuadraticUtils