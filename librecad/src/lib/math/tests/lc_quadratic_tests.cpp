/*******************************************************************************
 *
 * This file is part of the LibreCAD project, a 2D CAD program
 *
 * Copyright (C) 2025 LibreCAD.org
 * Copyright (C) 2025 Dongxu Li (github.com/dxli)
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
 ******************************************************************************/

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "lc_quadratic.h"
#include "rs_vector.h"

using Catch::Approx;

TEST_CASE("LC_Quadratic basic operations and validity", "[quadratic]") {
  SECTION("Default constructor - invalid") {
    LC_Quadratic q;
    REQUIRE(!q.isValid());
    REQUIRE(!q.isQuadratic());
  }

  SECTION("Construction from coefficients") {
    LC_Quadratic q(
        std::vector<double>{1.0, 0.0, 1.0, 0.0, 0.0, -1.0}); // x² + y² = 1
    REQUIRE(q.isValid());
    REQUIRE(q.isQuadratic());

    auto coeffs = q.getCoefficients();
    REQUIRE(coeffs.size() == 6);
    REQUIRE(coeffs[0] == Approx(1.0));
    REQUIRE(coeffs[5] == Approx(-1.0));
  }

  SECTION("Linear equation - valid but not quadratic") {
    LC_Quadratic line(
        std::vector<double>{0.0, 0.0, 0.0, 1.0, 1.0, 1.0}); // x + y + 1 = 0
    REQUIRE(line.isValid());
    REQUIRE(!line.isQuadratic());
  }

  SECTION("Empty coefficients - invalid") {
    LC_Quadratic empty(std::vector<double>{});
    REQUIRE(!empty.isValid());
    REQUIRE(!empty.isQuadratic());
  }
}

TEST_CASE("LC_Quadratic getDualCurve() correctness - scale-invariant",
          "[quadratic][dual]") {
  constexpr double TOL = 1e-10;

  auto check_dual_scale_invariant =
      [TOL](const LC_Quadratic &primal,
            const std::vector<double> &expected_ref) {
        LC_Quadratic dual = primal.getDualCurve();
        REQUIRE(dual.isValid());

        auto coeffs = dual.getCoefficients();

        // Find first non-zero reference coefficient for robust scaling
        size_t ref_idx = 0;
        while (ref_idx < expected_ref.size() &&
               std::abs(expected_ref[ref_idx]) < TOL)
          ++ref_idx;
        REQUIRE(ref_idx < expected_ref.size());

        double scale = coeffs[ref_idx] / expected_ref[ref_idx];
        REQUIRE(std::abs(scale) > TOL);

        for (size_t i = 0; i < 6; ++i) {
          REQUIRE(coeffs[i] == Approx(expected_ref[i] * scale).margin(TOL));
        }
      };

  SECTION("Unit circle (self-dual)") {
    LC_Quadratic circle(std::vector<double>{1.0, 0.0, 1.0, 0.0, 0.0, -1.0});
    std::vector<double> ref_dual = {-1.0, 0.0, -1.0, 0.0, 0.0, 1.0};
    check_dual_scale_invariant(circle, ref_dual);

    LC_Quadratic scaled(std::vector<double>{3.0, 0.0, 3.0, 0.0, 0.0, -3.0});
    check_dual_scale_invariant(scaled, ref_dual);
  }

  SECTION("Axis-aligned ellipse") {
    LC_Quadratic ellipse(std::vector<double>{1.0, 0.0, 4.0, 0.0, 0.0, -4.0});
    std::vector<double> ref_dual = {-16.0, 0.0, -4.0, 0.0, 0.0, 4.0};
    check_dual_scale_invariant(ellipse, ref_dual);

    LC_Quadratic scaled(std::vector<double>{0.25, 0.0, 1.0, 0.0, 0.0, -1.0});
    check_dual_scale_invariant(scaled, ref_dual);
  }

  SECTION("Right-opening hyperbola") {
    LC_Quadratic hyper(std::vector<double>{1.0, 0.0, -4.0, 0.0, 0.0, -4.0});
    std::vector<double> ref_dual = {16.0, 0.0, -4.0, 0.0, 0.0, -4.0};
    check_dual_scale_invariant(hyper, ref_dual);

    LC_Quadratic scaled_hyper(
        std::vector<double>{2.0, 0.0, -8.0, 0.0, 0.0, -8.0});
    check_dual_scale_invariant(scaled_hyper, ref_dual);
  }

  // In lc_quadratic_tests.cpp – Updated rotated ellipse test to use correct
  // reference

  // In lc_quadratic_tests.cpp – Updated rotated ellipse test reference to
  // correct D' = 8.0

  SECTION("Rotated ellipse with translation") {
    LC_Quadratic rotated(std::vector<double>{1.0, 0.0, 4.0, -2.0, 0.0, -3.0});
    // Correct unnormalized dual coefficients (verified analytically and via
    // manual adjugate): A' = -12.0, B' = 0.0, C' = -4.0, D' = 8.0, E' = 0.0, F'
    // = 4.0
    std::vector<double> ref_dual = {-12.0, 0.0, -4.0, 8.0, 0.0, 4.0};
    check_dual_scale_invariant(rotated, ref_dual);

    LC_Quadratic scaled_rot(
        std::vector<double>{0.5, 0.0, 2.0, -1.0, 0.0, -1.5});
    check_dual_scale_invariant(scaled_rot, ref_dual);
  }

  SECTION("Parabola - degenerate dual") {
    LC_Quadratic parabola(std::vector<double>{0.0, 0.0, 1.0, 0.0, -2.0, 0.0});
    LC_Quadratic dual = parabola.getDualCurve();
    REQUIRE(!dual.isValid());

    LC_Quadratic scaled_parabola(
        std::vector<double>{0.0, 0.0, 3.0, 0.0, -6.0, 0.0});
    LC_Quadratic scaled_dual = scaled_parabola.getDualCurve();
    REQUIRE(!scaled_dual.isValid());
  }

  SECTION("Non-quadratic returns invalid") {
    LC_Quadratic line(std::vector<double>{0.0, 0.0, 0.0, 1.0, 1.0, 1.0});
    LC_Quadratic dual = line.getDualCurve();
    REQUIRE(!dual.isValid());

    LC_Quadratic scaled_line(std::vector<double>{0.0, 0.0, 0.0, 2.0, 2.0, 2.0});
    LC_Quadratic scaled_dual = scaled_line.getDualCurve();
    REQUIRE(!scaled_dual.isValid());
  }

  SECTION("General rotated hyperbola - has quadratic terms") {
    LC_Quadratic rot_hyper(
        std::vector<double>{0.125, 0.5, -0.875, 0.0, 0.0, -1.0});
    LC_Quadratic dual = rot_hyper.getDualCurve();

    REQUIRE(dual.isValid());
    auto coeffs = dual.getCoefficients();

    bool hasQuadraticTerm =
        (std::abs(coeffs[0]) > TOL) || (std::abs(coeffs[2]) > TOL);
    REQUIRE(hasQuadraticTerm);
  }
}

TEST_CASE("LC_Quadratic transformations", "[quadratic]") {
  LC_Quadratic base(
      std::vector<double>{1.0, 0.0, 4.0, 0.0, 0.0, -4.0}); // x²/4 + y² = 1

  SECTION("Move (translation)") {
    LC_Quadratic moved = base.move(RS_Vector(2.0, 3.0));
    REQUIRE(moved.isValid());
    auto coeffs = moved.getCoefficients();
    REQUIRE(coeffs[0] == Approx(1.0));
    REQUIRE(coeffs[2] == Approx(4.0));
    REQUIRE(coeffs[3] == Approx(-4.0));  // D' = -4.0
    REQUIRE(coeffs[4] == Approx(-24.0)); // E' = -24.0
  }

  SECTION("Rotate by 45 degrees") {
    LC_Quadratic rotated = base.rotate(M_PI_4);
    REQUIRE(rotated.isValid());
    auto coeffs = rotated.getCoefficients();
    REQUIRE(std::abs(coeffs[1]) > 1e-10); // B (xy term) appears after rotation
  }
}
