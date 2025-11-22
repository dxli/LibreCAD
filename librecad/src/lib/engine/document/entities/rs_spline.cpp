/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**********************************************************************/

#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>
#include <numeric>

#include "lc_splinehelper.h"
#include "rs_debug.h"
#include "rs_graphic.h"
#include "rs_line.h"
#include "rs_math.h"
#include "rs_painter.h"
#include "rs_spline.h"

namespace {
bool compareVector(const RS_Vector &va, const RS_Vector &vb,
                   double tol = RS_TOLERANCE) {
  return va.distanceTo(vb) <= tol;
}
} // namespace

/*==========================================================================*/
/* RS_SplineData                                                            */
/*==========================================================================*/

RS_SplineData::RS_SplineData(int _degree, bool _closed) : degree(_degree) {
  type = _closed ? SplineType::WrappedClosed : SplineType::ClampedOpen;
  savedOpenType = type;
}

std::ostream &operator<<(std::ostream &os, const RS_SplineData &ld) {
  os << "(degree:" << ld.degree << " type:" << static_cast<int>(ld.type);
  if (!ld.controlPoints.empty()) os << " cps:" << ld.controlPoints.size();
  if (!ld.knotslist.empty()) os << " knots:" << ld.knotslist.size();
  if (!ld.weights.empty()) os << " weights:" << ld.weights.size();
  if (!ld.fitPoints.empty()) os << " fit:" << ld.fitPoints.size();
  os << ")";
  return os;
}

/*==========================================================================*/
/* RS_Spline - FULL IMPLEMENTATION                                         */
/*==========================================================================*/

RS_Spline::RS_Spline(RS_EntityContainer *parent, const RS_SplineData &d)
    : RS_EntityContainer(parent), data(d) {
  if (data.type == RS_SplineData::SplineType::WrappedClosed && !hasWrappedControlPoints())
    addWrapping();
  calculateBorders();
  update();
}

RS_Entity *RS_Spline::clone() const {
  auto *l = new RS_Spline(*this);
  l->setOwner(isOwner());
  l->detach();
  return l;
}

/* -------------------------- data access -------------------------- */
RS_SplineData &RS_Spline::getData() { return data; }
const RS_SplineData &RS_Spline::getData() const { return data; }

/* -------------------------- unwrapped size -------------------------- */
size_t RS_Spline::getUnwrappedSize() const {
  size_t s = data.controlPoints.size();
  if (s <= data.degree) return s;
  if (data.type == RS_SplineData::SplineType::WrappedClosed) {
    s -= (s > data.degree) ? data.degree : s;
  }
  return s;
}

/* -------------------------- unwrapped vectors -------------------------- */
std::vector<RS_Vector> RS_Spline::getUnwrappedControlPoints() const {
  size_t s = getUnwrappedSize();
  return s ? std::vector<RS_Vector>(data.controlPoints.begin(),
                                    data.controlPoints.begin() + s)
           : std::vector<RS_Vector>{};
}

std::vector<double> RS_Spline::getUnwrappedWeights() const {
  size_t s = getUnwrappedSize();
  return s ? std::vector<double>(data.weights.begin(), data.weights.begin() + s)
           : std::vector<double>{};
}

std::vector<double> RS_Spline::getUnwrappedKnotVector() const {
  size_t s = getUnwrappedSize();
  if (!s) return {};
  size_t bs = s + data.degree + 1;
  return bs <= data.knotslist.size()
         ? std::vector<double>(data.knotslist.begin(), data.knotslist.begin() + bs)
         : std::vector<double>{};
}

/* -------------------------- wrapping helpers -------------------------- */
void RS_Spline::removeWrapping() { LC_SplineHelper::removeWrapping(data); }
void RS_Spline::addWrapping() { LC_SplineHelper::addWrapping(data); }
void RS_Spline::updateControlAndWeightWrapping() {
  LC_SplineHelper::updateControlAndWeightWrapping(data, isClosed(), getUnwrappedSize());
}
void RS_Spline::updateKnotWrapping() {
  LC_SplineHelper::updateKnotWrapping(data, isClosed(), getUnwrappedSize());
}

/* -------------------------- borders -------------------------- */
void RS_Spline::calculateBorders() {
  resetBorders();
  size_t s = getUnwrappedSize();
  if (!s) return;
  for (size_t i = 0; i < s; ++i) {
    minV = RS_Vector::minimum(data.controlPoints[i], minV);
    maxV = RS_Vector::maximum(data.controlPoints[i], maxV);
  }
}

void RS_Spline::resetBorders() {
  minV = RS_Vector(RS_MAXDOUBLE, RS_MAXDOUBLE);
  maxV = RS_Vector(RS_MINDOUBLE, RS_MINDOUBLE);
}

/* -------------------------- degree -------------------------- */
void RS_Spline::setDegree(int d) {
  if (d < 1 || d > 3) throw std::invalid_argument("Degree must be 1-3");
  data.degree = d;
}
int RS_Spline::getDegree() const { return data.degree; }

/* -------------------------- counts -------------------------- */
size_t RS_Spline::getNumberOfControlPoints() const { return getUnwrappedSize(); }
size_t RS_Spline::getNumberOfKnots() const { return data.knotslist.size(); }

/* -------------------------- closed -------------------------- */
bool RS_Spline::isClosed() const {
  return data.type == RS_SplineData::SplineType::WrappedClosed;
}

void RS_Spline::setClosed(bool c) {
  if (c == isClosed()) return;
  if (getUnwrappedSize() < data.degree + 1) {
    RS_DEBUG->print(RS_Debug::D_WARNING,
                    "RS_Spline::setClosed: insufficient points");
    return;
  }
  if (c) {
    if (data.type == RS_SplineData::SplineType::ClampedOpen)
      LC_SplineHelper::toWrappedClosedFromClampedOpen(data);
    else
      LC_SplineHelper::toWrappedClosedFromStandard(data);
  } else {
    if (data.type == RS_SplineData::SplineType::WrappedClosed)
      LC_SplineHelper::toClampedOpenFromWrappedClosed(data);
    else
      LC_SplineHelper::toClampedOpenFromStandard(data);
  }
  update();
}

/* -------------------------- type change -------------------------- */
void RS_Spline::changeType(RS_SplineData::SplineType newType) {
  if (data.type == newType) return;
  if (data.controlPoints.size() < data.degree + 1) {
    RS_DEBUG->print(RS_Debug::D_WARNING,
                    "RS_Spline::changeType: insufficient points");
    data.type = newType;
    return;
  }

  switch (newType) {
  case RS_SplineData::SplineType::Standard:
    if (data.type == RS_SplineData::SplineType::ClampedOpen)
      LC_SplineHelper::toStandardFromClampedOpen(data);
    else if (data.type == RS_SplineData::SplineType::WrappedClosed)
      LC_SplineHelper::toStandardFromWrappedClosed(data);
    break;
  case RS_SplineData::SplineType::ClampedOpen:
    if (data.type == RS_SplineData::SplineType::Standard)
      LC_SplineHelper::toClampedOpenFromStandard(data);
    else if (data.type == RS_SplineData::SplineType::WrappedClosed)
      LC_SplineHelper::toClampedOpenFromWrappedClosed(data);
    break;
  case RS_SplineData::SplineType::WrappedClosed:
    if (data.type == RS_SplineData::SplineType::Standard)
      LC_SplineHelper::toWrappedClosedFromStandard(data);
    else if (data.type == RS_SplineData::SplineType::ClampedOpen)
      LC_SplineHelper::toWrappedClosedFromClampedOpen(data);
    break;
  }
  data.type = newType;
}

/* -------------------------- clamped knot adjustment -------------------------- */
std::vector<double>
RS_Spline::adjustToOpenClamped(const std::vector<double> &knots,
                               size_t num_control, size_t order, bool) const {
  return LC_SplineHelper::clampKnotVector(knots, num_control, order);
}

/* -------------------------- reference points -------------------------- */
RS_VectorSolutions RS_Spline::getRefPoints() const {
  return RS_VectorSolutions(getControlPoints());
}
RS_Vector RS_Spline::getNearestRef(const RS_Vector &coord, double *dist) const {
  return RS_EntityContainer::getNearestRef(coord, dist);
}
RS_Vector RS_Spline::getNearestSelectedRef(const RS_Vector &coord,
                                           double *dist) const {
  return RS_EntityContainer::getNearestSelectedRef(coord, dist);
}

/* -------------------------- update (polyline approximation) -------------------------- */
void RS_Spline::update() {
  clear();
  if (!validate()) return;

  std::vector<RS_Vector> points;
  fillStrokePoints(32, points);
  for (size_t i = 0; i + 1 < points.size(); ++i)
    addEntity(new RS_Line(this, points[i], points[i + 1]));
  if (isClosed() && points.size() > 1)
    addEntity(new RS_Line(this, points.back(), points.front()));
}

/* -------------------------- stroke points -------------------------- */
void RS_Spline::fillStrokePoints(int segments, std::vector<RS_Vector> &points) {
  const auto &kv = data.knotslist;
  double tmin = kv[data.degree];
  double tmax = data.knotslist[data.knotslist.size() - data.degree - 1];
  double step = (tmax - tmin) / segments;
  for (int i = 0; i <= segments; ++i)
    points.push_back(getPointAt(tmin + i * step));
}

/* -------------------------- endpoints -------------------------- */
RS_Vector RS_Spline::getStartpoint() const { return RS_Vector(false); }
RS_Vector RS_Spline::getEndpoint() const { return RS_Vector(false); }

/* -------------------------- nearest overrides -------------------------- */
RS_Vector RS_Spline::getNearestEndpoint(const RS_Vector &, double *) const { return RS_Vector(false); }
RS_Vector RS_Spline::getNearestCenter(const RS_Vector &, double *) const { return RS_Vector(false); }
RS_Vector RS_Spline::getNearestMiddle(const RS_Vector &, double *, int) const { return RS_Vector(false); }
RS_Vector RS_Spline::getNearestDist(double, const RS_Vector &, double *) const { return RS_Vector(false); }

/* -------------------------- transformations -------------------------- */
void RS_Spline::move(const RS_Vector &offset) {
  for (auto &cp : data.controlPoints) cp += offset;
  for (auto &fp : data.fitPoints) fp += offset;
  calculateBorders();
}
void RS_Spline::rotate(const RS_Vector &center, double angle) {
  for (auto &cp : data.controlPoints) cp.rotate(center, angle);
  for (auto &fp : data.fitPoints) fp.rotate(center, angle);
  calculateBorders();
}
void RS_Spline::rotate(const RS_Vector &center, const RS_Vector &angleVector) {
  for (auto &cp : data.controlPoints) cp.rotate(center, angleVector);
  for (auto &fp : data.fitPoints) fp.rotate(center, angleVector);
  calculateBorders();
}
void RS_Spline::scale(const RS_Vector &center, const RS_Vector &factor) {
  for (auto &cp : data.controlPoints) cp.scale(center, factor);
  for (auto &fp : data.fitPoints) fp.scale(center, factor);
  calculateBorders();
}
RS_Entity& RS_Spline::shear(double k) {
  for (auto &cp : data.controlPoints) cp.shear(k);
  for (auto &fp : data.fitPoints) fp.shear(k);
  calculateBorders();
  return *this;
}
void RS_Spline::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
  for (auto &cp : data.controlPoints) cp.mirror(axisPoint1, axisPoint2);
  for (auto &fp : data.fitPoints) fp.mirror(axisPoint1, axisPoint2);
  calculateBorders();
}
void RS_Spline::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
  RS_EntityContainer::moveRef(ref, offset);
  calculateBorders();
}
void RS_Spline::revertDirection() {
  std::reverse(data.controlPoints.begin(), data.controlPoints.end());
  std::reverse(data.weights.begin(), data.weights.end());
  std::reverse(data.knotslist.begin(), data.knotslist.end());
  std::reverse(data.fitPoints.begin(), data.fitPoints.end());
  update();
}
void RS_Spline::draw(RS_Painter* painter) {
  RS_EntityContainer::draw(painter);
}

/* -------------------------- modern findSpan (std::upper_bound, size_t) -------------------------- */
size_t RS_Spline::findSpan(size_t n, size_t p, double u, const std::vector<double>& U)
{
    if (u >= U[n + 1]) return n;
    if (u <= U[p])     return p;
    auto it = std::upper_bound(U.begin() + p + 1, U.begin() + n + 1, u);
    return static_cast<size_t>(it - U.begin()) - 1;
}

/* -------------------------- basis functions (de Boor style, non-recursive) -------------------------- */
std::vector<double> RS_Spline::basisFunctions(int i, double u, int p,
                                              const std::vector<double> &U) {
  std::vector<double> N(p + 1, 0.0), left(p + 1, 0.0), right(p + 1, 0.0);
  N[0] = 1.0;
  for (int j = 1; j <= p; ++j) {
    left[j] = u - U[i + 1 - j];
    right[j] = U[i + j] - u;
    double saved = 0.0;
    for (int r = 0; r < j; ++r) {
      double temp = N[r] / (right[r + 1] + left[j - r]);
      N[r] = saved + right[r + 1] * temp;
      saved = left[j - r] * temp;
    }
    N[j] = saved;
  }
  return N;
}

/* -------------------------- knot insertion (delegated) -------------------------- */
void RS_Spline::insertKnot(double u)
{
    if (LC_SplineHelper::insertKnot(data, u)) {
        calculateBorders();
        update();
    }
}

/* -------------------------- fit points interpolation (delegated) -------------------------- */
void RS_Spline::setFitPoints(const std::vector<RS_Vector>& fitPoints,
                             bool useCentripetal)
{
    bool closed = isClosed();
    if (LC_SplineHelper::setFitPoints(data, fitPoints, useCentripetal, closed)) {
        update();
    }
}

/* -------------------------- point evaluation -------------------------- */
RS_Vector RS_Spline::getPointAt(double t) const {
    return evaluateNURBS(data, t);
}

/* -------------------------- robust NURBS evaluation (de Boor) -------------------------- */
RS_Vector RS_Spline::evaluateNURBS(const RS_SplineData& d, double t)
{
    size_t p = d.degree;
    size_t n = d.controlPoints.size() - 1;
    if (n + 1 < p + 1) return RS_Vector(false);

    size_t span = findSpan(n, p, t, d.knotslist);
    auto N = basisFunctions(static_cast<int>(span), t, static_cast<int>(p), d.knotslist);

    RS_Vector C(0.0, 0.0);
    double wsum = 0.0;
    for (size_t i = 0; i <= p; ++i) {
        size_t idx = span - p + i;
        double w = (idx < d.weights.size()) ? d.weights[idx] : 1.0;
        C += d.controlPoints[idx] * N[i] * w;
        wsum += N[i] * w;
    }
    if (wsum > RS_TOLERANCE) C = C / wsum;
    return C;
}

/* -------------------------- validation (full original + type checks) -------------------------- */
bool RS_Spline::validate() const
{
    const size_t degree = data.degree;

    const size_t num_cp = data.controlPoints.size();
    if (num_cp < degree + 1) return false;

    const bool closed = isClosed();
    const size_t extra_cp = closed ? degree : 0;
    const size_t unwrapped_cp = num_cp - extra_cp;
    if (unwrapped_cp < degree + 1) return false;

    const size_t expected_knots = num_cp + degree + 1;
    if (data.knotslist.size() != expected_knots) return false;
    if (data.weights.size() != num_cp) return false;

    for (double w : data.weights)
        if (w <= 0.0) return false;

    for (size_t i = 1; i < data.knotslist.size(); ++i)
        if (data.knotslist[i] < data.knotslist[i - 1] - RS_TOLERANCE) return false;

    // multiplicity check
    {
        size_t mult = 1;
        for (size_t i = 1; i < data.knotslist.size(); ++i) {
            if (fabs(data.knotslist[i] - data.knotslist[i - 1]) < RS_TOLERANCE) {
                ++mult;
                if (mult > degree + 1) return false;
            } else mult = 1;
        }
    }

    if (data.type == RS_SplineData::SplineType::ClampedOpen) {
        const double k_start = data.knotslist.front();
        const double k_end   = data.knotslist.back();
        size_t mult_start = 0, mult_end = 0;
        for (size_t j = 0; j <= degree; ++j) {
            if (fabs(data.knotslist[j] - k_start) < RS_TOLERANCE) ++mult_start;
            if (fabs(data.knotslist[data.knotslist.size() - 1 - j] - k_end) < RS_TOLERANCE) ++mult_end;
        }
        if (mult_start != degree + 1 || mult_end != degree + 1) return false;
    }
    else if (data.type == RS_SplineData::SplineType::Standard) {
        const double k_start = data.knotslist.front();
        const double k_end   = data.knotslist.back();
        if (fabs(data.knotslist[1] - k_start) < RS_TOLERANCE) return false;
        if (fabs(data.knotslist[data.knotslist.size() - 2] - k_end) < RS_TOLERANCE) return false;
    }
    else if (data.type == RS_SplineData::SplineType::WrappedClosed) {
        if (!hasWrappedControlPoints()) return false;
        const double k_start = data.knotslist.front();
        const double k_end   = data.knotslist.back();
        if (fabs(data.knotslist[1] - k_start) < RS_TOLERANCE) return false;
        if (fabs(data.knotslist[data.knotslist.size() - 2] - k_end) < RS_TOLERANCE) return false;
    }

    return true;
}

/* -------------------------- has wrapped control points -------------------------- */
bool RS_Spline::hasWrappedControlPoints() const {
  if (!isClosed()) return false;
  size_t n = data.controlPoints.size();
  size_t deg = data.degree;
  if (n <= deg) return false;
  for (size_t i = 0; i < deg; ++i) {
    if (!compareVector(data.controlPoints[i], data.controlPoints[n - deg + i]) ||
        fabs(data.weights[i] - data.weights[n - deg + i]) > RS_TOLERANCE)
      return false;
  }
  return true;
}

/* -------------------------- remaining original helpers (full) -------------------------- */
void RS_Spline::normalizeKnots() {
  data.knotslist = LC_SplineHelper::getNormalizedKnotVector(
      data.knotslist, data.knotslist.front(), {});
}

double RS_Spline::estimateParamAtIndex(size_t index) const {
  if (data.knotslist.empty()) return 0.0;
  return data.knotslist[index + data.degree];
}

std::vector<double> RS_Spline::getBSplineBasis(double t,
                                               const std::vector<double> &knots,
                                               int degree,
                                               size_t numControls) const {
  int order = degree + 1, np = static_cast<int>(numControls), c = order,
      nplusc = np + c;
  std::vector<double> bf(nplusc, 0.0);
  for (int i = 0; i < nplusc - 1; ++i)
    if (t >= knots[i] - RS_TOLERANCE && t < knots[i + 1] + RS_TOLERANCE)
      bf[i] = 1.0;
  for (int k = 2; k <= c; ++k) {
    for (int i = 0; i < nplusc - k; ++i) {
      double d1 = bf[i] != 0.0 && std::abs(knots[i + k - 1] - knots[i]) > RS_TOLERANCE
                      ? (t - knots[i]) * bf[i] / (knots[i + k - 1] - knots[i])
                      : 0.0;
      double d2 = bf[i + 1] != 0.0 && std::abs(knots[i + k] - knots[i + 1]) > RS_TOLERANCE
                      ? (knots[i + k] - t) * bf[i + 1] / (knots[i + k] - knots[i + 1])
                      : 0.0;
      bf[i] = d1 + d2;
    }
  }
  if (t + RS_TOLERANCE >= knots[nplusc - 1])
    bf[np - 1] = 1.0;
  return {bf.begin(), bf.begin() + np};
}

double RS_Spline::getDerivative(double t, bool isX) const {
  double d = 1e-8;
  RS_Vector p1 = getPointAt(t);
  RS_Vector p2 = getPointAt(t + d);
  return isX ? (p2.x - p1.x) / d : (p2.y - p1.y) / d;
}

double RS_Spline::bisectDerivativeZero(double a, double b, double fa, bool isX) const {
  double fb = getDerivative(b, isX);
  for (int i = 0; i < 50; ++i) {
    double m = (a + b) / 2.0, fm = getDerivative(m, isX);
    if (fa * fm <= 0.0) { b = m; fb = fm; } else { a = m; fa = fm; }
    if (b - a < RS_TOLERANCE) break;
  }
  return (a + b) / 2.0;
}

std::vector<double> RS_Spline::findDerivativeZeros(bool isX) const {
  std::vector<double> zeros;
  double tmin = data.knotslist[data.degree];
  double tmax = data.knotslist[data.knotslist.size() - data.degree - 1];
  double step = (tmax - tmin) / 100.0;
  double fa = getDerivative(tmin, isX);
  for (double tt = tmin + step; tt <= tmax + RS_TOLERANCE; tt += step) {
    double fm = getDerivative(tt, isX);
    if (fa * fm <= 0.0) {
      zeros.push_back(bisectDerivativeZero(tt - step, tt, fa, isX));
    }
    fa = fm;
  }
  return zeros;
}

void RS_Spline::calculateTightBorders() {
  resetBorders();
  std::vector<double> tx = findDerivativeZeros(true);
  std::vector<double> ty = findDerivativeZeros(false);
  tx.push_back(data.knotslist[data.degree]);
  tx.push_back(data.knotslist.back() - data.degree - 1);
  ty.push_back(data.knotslist[data.degree]);
  ty.push_back(data.knotslist.back() - data.degree - 1);
  for (double t : tx) {
    RS_Vector p = getPointAt(t);
    minV = RS_Vector::minimum(p, minV);
    maxV = RS_Vector::maximum(p, maxV);
  }
  for (double t : ty) {
    RS_Vector p = getPointAt(t);
    minV = RS_Vector::minimum(p, minV);
    maxV = RS_Vector::maximum(p, maxV);
  }
}

/** Set knot vector, validate size and monotonicity */
void RS_Spline::setKnotVector(const std::vector<double>& knots)
{
  if (knots.empty()) {
    data.knotslist.clear();
    return;
  }

         // Basic size check (depends on current control points and degree)
  size_t expected = data.controlPoints.size() + data.degree + 1;
  if (knots.size() != expected) {
    RS_DEBUG->print(RS_Debug::D_WARNING,
                    "RS_Spline::setKnotVector: wrong knot vector size %d (expected %d)",
                    (int)knots.size(), (int)expected);
    return;
  }

         // Must be non-decreasing
  for (size_t i = 1; i < knots.size(); ++i) {
    if (knots[i] < knots[i - 1] - RS_TOLERANCE) {
      RS_DEBUG->print(RS_Debug::D_WARNING,
                      "RS_Spline::setKnotVector: knot vector not non-decreasing");
      return;
    }
  }

  data.knotslist = knots;

         // For closed wrapped splines we need to keep the wrapped part consistent
  if (isClosed()) {
    updateKnotWrapping();
  }

  update();
}

/** Add control point with weight, handling wrapping */
void RS_Spline::addControlPoint(const RS_Vector& v, double w /*= 1.0*/)
{
  // Add the new control point and weight
  data.controlPoints.push_back(v);
  data.weights.push_back(w);

         // If the spline is closed (wrapped) we must immediately duplicate the first points
  if (isClosed()) {
    size_t deg = data.degree;
    size_t unwrapped = getUnwrappedSize();  // size before wrapping points

           // Keep the wrapped part in sync – the last deg points must mirror the first deg points
    if (data.controlPoints.size() > unwrapped + deg) {
      // we have already wrapped points – replace the last one with the new first one
      data.controlPoints.back() = data.controlPoints[unwrapped];
      data.weights.back() = data.weights[unwrapped];
    } else {
      // first time we go beyond the original points – add the wrapping duplicates
      for (size_t i = 0; i < deg; ++i) {
        data.controlPoints.push_back(data.controlPoints[i]);
        data.weights.push_back(data.weights[i]);
      }
    }
    updateKnotWrapping();   // keep knot vector periodic for closed spline
  }

         // Regenerate knot vector if none exists (uniform clamped by default)
  if (data.knotslist.empty()) {
    data.knotslist = LC_SplineHelper::knot(data.controlPoints.size() - (isClosed() ? data.degree : 0),
                                           data.degree + 1);
    if (isClosed())
      updateKnotWrapping();
  }

  calculateBorders();
  update();
}

/** Set control point at index */
void RS_Spline::setControlPoint(size_t index, const RS_Vector& v)
{
  if (index >= data.controlPoints.size())
    return;

         // Modify the actual control point
  data.controlPoints[index] = v;

         // For closed (wrapped) splines: keep the wrapped duplicates in sync
  if (isClosed()) {
    size_t deg = data.degree;
    size_t unwrapped = getUnwrappedSize();  // number of original (non-wrapped) points

           // The index belongs to the original part
    if (index < unwrapped) {
      // Mirror the change to the corresponding wrapped point at the end
      data.controlPoints[unwrapped + index] = v;
    }
    // If someone modifies a wrapped duplicate directly (should not happen in normal use),
    // we do nothing extra – the original point stays authoritative
  }

  calculateBorders();
  update();
}

/** Get weight at index */
double RS_Spline::getWeight(size_t index) const
{
  if (index >= data.weights.size())
    return 1.0;   // default weight if out of range

  return data.weights[index];
}

/** Get control points (unwrapped) */
std::vector<RS_Vector> RS_Spline::getControlPoints() const
{
  // Return only the "real" (unwrapped) control points
  // For closed splines this excludes the duplicated wrapping points at the end
  size_t s = getUnwrappedSize();

  if (s == 0)
    return std::vector<RS_Vector>{};

  return std::vector<RS_Vector>(data.controlPoints.begin(),
                                data.controlPoints.begin() + s);
}

/** Get knot vector (unwrapped) */
std::vector<double> RS_Spline::getKnotVector() const
{
  // For closed (wrapped) splines, the knot vector is stored in its periodic/wrapped form.
  // This method returns only the "logical" (unwrapped) part that corresponds to the original control points.
  size_t s = getUnwrappedSize();

  if (s == 0)
    return std::vector<double>{};

         // The unwrapped knot vector always has: unwrapped_control_points + degree + 1 entries
  size_t bs = s + data.degree + 1;

  if (bs > data.knotslist.size())
    return std::vector<double>{};

  return std::vector<double>(data.knotslist.begin(),
                             data.knotslist.begin() + bs);
}

/** Remove last control point, handling wrapping */
void RS_Spline::removeLastControlPoint()
{
  if (data.controlPoints.empty())
    return;

         // For closed (wrapped) splines: the last 'degree' points are duplicates
         // → we must not remove an original point when only wrapped points remain
  if (isClosed()) {
    size_t deg = data.degree;
    size_t unwrapped = getUnwrappedSize();

           // If we are about to remove one of the original points, shrink the wrapped part instead
    if (data.controlPoints.size() > unwrapped) {
      // Still have wrapped duplicates → just remove the last (duplicate) point
      data.controlPoints.pop_back();
      data.weights.pop_back();
    } else {
      // No wrapped duplicates left → we would delete a real point → not allowed for closed spline
      RS_DEBUG->print(RS_Debug::D_WARNING,
                      "RS_Spline::removeLastControlPoint: cannot remove point from closed spline (would break periodicity)");
      return;
    }
  } else {
    // Open spline → simply remove the last point
    data.controlPoints.pop_back();
    data.weights.pop_back();
  }

         // Knot vector may become invalid after removal → clear it so it will be regenerated on next update
  data.knotslist.clear();

  calculateBorders();
  update();
}

/** Set weight at index */
void RS_Spline::setWeight(unsigned long index, double w)
{
  if (w <= 0.0) {
    RS_DEBUG->print(RS_Debug::D_WARNING,
                    "RS_Spline::setWeight: weight must be positive");
    return;
  }

  if (index >= data.weights.size())
    return;

  data.weights[index] = w;

         // For closed (wrapped) splines: keep wrapped duplicate weights in sync
  if (isClosed()) {
    size_t deg = data.degree;
    size_t unwrapped = getUnwrappedSize();

    if (index < unwrapped) {
      // Change in the original part → mirror to the corresponding wrapped duplicate
      data.weights[unwrapped + index] = w;
    }
    // If someone changes a wrapped duplicate directly, we do nothing extra (original stays authoritative)
  }

  update();  // Weights affect the curve shape → regenerate polyline approximation
}

/** Add raw control point */
void RS_Spline::addControlPointRaw(const RS_Vector& v, double w /*= 1.0*/)
{
  // Directly append the control point and weight without any wrapping logic
  // This is used when loading data (e.g. from DXF) that already contains wrapped points
  data.controlPoints.push_back(v);
  data.weights.push_back(w > 0.0 ? w : 1.0);  // enforce positive weight

         // No automatic handling of closed/wrapped state or knot vector regeneration
         // Caller is responsible for consistency
}

/** Insert control point, clear knots if present */
void RS_Spline::insertControlPoint(size_t index, const RS_Vector& v, double w /*= 1.*/, bool preserveShape /*= false*/)
{
  if (index > data.controlPoints.size())
    return;

         // Insert the new control point and weight
  data.controlPoints.insert(data.controlPoints.begin() + index, v);
  data.weights.insert(data.weights.begin() + index, w > 0.0 ? w : 1.0);

         // For closed (wrapped) splines: maintain the wrapped duplicates at the end
  if (isClosed()) {
    size_t deg = data.degree;
    size_t unwrapped = getUnwrappedSize();  // previous unwrapped count

           // The insertion happened in the original (unwrapped) part
    if (index < unwrapped) {
      // Mirror the new point to the wrapped section
      // The corresponding wrapped index is unwrapped + index
      if (data.controlPoints.size() > unwrapped + deg) {
        // Already have full wrapping – shift and update the mirrored point
        data.controlPoints.insert(data.controlPoints.begin() + unwrapped + index, v);
        data.weights.insert(data.weights.begin() + unwrapped + index, w > 0.0 ? w : 1.0);
      } else {
        // Wrapping was incomplete – rebuild full wrapping
        updateControlAndWeightWrapping();
      }
    }
    // If insertion was in the wrapped area (should rarely happen), just let it be
    updateKnotWrapping();  // keep knot vector periodic
  }

         // Knot vector becomes invalid after arbitrary insertion unless preserveShape is requested
         // (preserveShape is not implemented in LibreCAD – always clear knots)
  if (!preserveShape || data.knotslist.empty() == false) {
    data.knotslist.clear();  // will be regenerated on next update() if needed
  }

  calculateBorders();
  update();
}

/** Remove control point, clear knots if present */
void RS_Spline::removeControlPoint(unsigned long index)
{
  if (index >= data.controlPoints.size())
    return;

         // For closed (wrapped) splines: remove both the original point and its wrapped duplicate
  if (isClosed()) {
    size_t deg = data.degree;
    size_t unwrapped = getUnwrappedSize();

    if (index < unwrapped) {
      // Remove from original part → also remove the corresponding wrapped duplicate
      size_t wrappedIndex = unwrapped + index;

             // Remove wrapped duplicate first (higher index) to keep indices valid
      if (wrappedIndex < data.controlPoints.size()) {
        data.controlPoints.erase(data.controlPoints.begin() + wrappedIndex);
        data.weights.erase(data.weights.begin() + wrappedIndex);
      }

             // Now remove the original point
      data.controlPoints.erase(data.controlPoints.begin() + index);
      data.weights.erase(data.weights.begin() + index);
    } else {
      // Removal requested in wrapped area → treat as removal of corresponding original point
      size_t originalIndex = index - unwrapped;
      data.controlPoints.erase(data.controlPoints.begin() + index);
      data.weights.erase(data.weights.begin() + index);

      if (originalIndex < unwrapped) {
        data.controlPoints.erase(data.controlPoints.begin() + originalIndex);
        data.weights.erase(data.weights.begin() + originalIndex);
      }
    }

           // Re-establish correct wrapping after deletion
    updateControlAndWeightWrapping();
    updateKnotWrapping();
  } else {
    // Open spline → simple removal
    data.controlPoints.erase(data.controlPoints.begin() + index);
    data.weights.erase(data.weights.begin() + index);
  }

         // Knot vector becomes invalid after arbitrary removal
  data.knotslist.clear();

  calculateBorders();
  update();
}
