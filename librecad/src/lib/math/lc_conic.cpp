/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) Vasilis Vlachoudis <Vasilis.Vlachoudis@cern.ch>
** Copyright (C) 2016 LibreCAD (http://librecad.org)
**

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

#include "lc_conic.h"
#include "rs_math.h"

namespace LC_Conic {
std::vector<LC_Quadratic> splitLines(Matrix const& m)
{
	using namespace boost::numeric::ublas;
	//3x3 symmetric matrix
	double B, G, F, C;	// normalized coef
	double x1{0}, x2{0}, y1{0}, y2{0};
	double const& a = m(0, 0);
	double const& b = m(1, 1);
	double const& c = m(2, 2);
	double const& h = m(0, 1);
	double const& f = m(1, 2);
	double const& g = m(0, 2);

	constexpr double TOLERENCE = 1e-20;
	constexpr double TOLERENCE_H = 1e-11;
	constexpr double CONICSMALL = 1e-9;

	auto ISZERO = [&TOLERENCE](double const& x) {return x <= TOLERENCE;};
	auto ISZEROH = [&TOLERENCE](double const& x) {return x <= TOLERENCE_H;};

	// Check coefficients of conic
	if (ISZERO(a)) {
		//assert(!ISZERO(b));
		if (ISZEROH(h)){
			/* no rotation
			 *	(y+c1)(y+c2) = 0
			 * 0 + 0 + y^2 + 0 + (c1+c2)*y + c1*c2 = 0
			 *
			 * 2*F = c1+c2 => c2 = 2*F-c1
			 *   C = c1*c2 = c1*(2*F-c1)
			 * <=> c1^2 - 2*F*c1 + C = 0
			 */
			//assert( ISZERO(g));
			F = -2 * f / b;
			C = c / b;
			auto const sol = RS_Math::quadraticSolver(std::vector<double>{F, C});
			return {{{0, 1, sol[0]}}, {{0, 1, sol[1]}}};
		} else {
			/* with rotation
			 *	(y+c1)(x+f2*y+c2) = 0
			 * 0 + xy + f2*y^2 + c1*x + (c2+c1*f2)*y + c1*c2 = 0
			 * A=0, H=0.5
			 * B=f2
			 * 2*G = c1
			 * 2*F = c2+c1*f2
			 * C   = c1*c2
			 */
			B = 0.5 * b / h;
			G = g / h;
			//F = f / h;
			C = 0.5 * c / h;
			return {{{0, 1, G}}, {{1, B, C/G}}};
//			conic1->set(0.0, 0.0, 0.0, 0.0, 0.5, G);
//			conic2->set(0.0, 0.0, 0.0, 0.5, 0.5*B, C/G);
		}
	} else
	if (ISZERO(b)) {
		if (ISZEROH(h)) {
			/* no rotation, like above but with y
			 *	(x+c1)(x+c2) = 0
			 */
			//assert( ISZERO(f));
			G = g / a;
			C = c / a;
			auto const sol = RS_Math::quadraticSolver(std::vector<double>{-2 * G, C});
			return {{{1, 0, sol[0]}}, {{1, 0, sol[1]}}};
//			quadratic(-2.0*G, C, &x1, &x2, CONICPREC);
//			conic1->set(0.0, 0.0, 0.0, 0.5, 0.0, x1);
//			conic2->set(0.0, 0.0, 0.0, 0.5, 0.0, x2);
		} else {
			/* with rotation, like above with y
			 * (x+c1)(g1*x+y+c2) = 0
			 * g1*x^2 + xy + 0 + (c1*g1+c2)*x + c1*y + c1*c2 = 0
			 */
			double A = 0.5 * a / h;
			//G = g / h;
			F = f / h;
			C = 0.5 * c / h;
			return {{{1, 0, F}}, {{A, 1, C/F}}};
//			conic1->set(0.0, 0.0, 0.0, 0.5, 0.0, F);
//			conic2->set(0.0, 0.0, 0.0, 0.5*A, 0.5, C/F);
		}
	} else
	if (ISZEROH(h)) {
		/*
		 * (x+f*y+c1)*(x-f*y+c2) = 0
		 * x^2 + 0 - f^2*y^2 + (c1+c2)*x + f*(c2-c1)*y + c1*c2 = 0
		 */
		//assert(a*b < 0.0);
		B = b / a;
		G = g / a;
		F = f / a;
		C = c / a;

		double xf;
		if (B>0.0)
			xf = 0.0;
		else
			xf = std::sqrt(-B);

		// assume that c=0 due to the transformation applied
		if (ISZERO(F)) {
			// (x+f*y+c1)*(x-f*y+c1) = 0   (c1=c2=G)
			x1 = x2 = y1 = y2 = G;
		} else
		if (ISZERO(G)) {
			// (x+f*y-c1)*(x-f*y+c1) = 0   (-c1=c2=G)
			// c1<0 and c2>0
			y2 = x1 = - F/xf;
			x2 = y1 = -x1;

		} else {
			/*
			 * B    = -f*f
			 * 2*G  = c1+c2   =>  c2 = 2*G - c1
			 * C    = c1*c2   = c1*(2*G-c1)
			 *               => c1^2 - 2*G*c1 + C = 0
			 */
			auto const sol = RS_Math::quadraticSolver(std::vector<double>{-2 * G, C});
			y1 = 2.0*G - sol[0];
			y2 = 2.0*G - sol[1];
		}
		// check which combination is closer to 2*F
		if (std::abs(xf*(y1-x1) - 2.0*F) <= std::abs(xf*(y2-x2) - 2.0*F)) {
			// normal
			return {{{1, xf, x1}}, {{1, - xf, y1}}};
//			conic1->set(0.0, 0.0, 0.0, 0.5, 0.5*xf, x1);
//			conic2->set(0.0, 0.0, 0.0, 0.5,-0.5*xf, y1);
		} else {
			// swap
			return {{{1, xf, x2}}, {{1, - xf, y2}}};
//			conic1->set(0.0, 0.0, 0.0, 0.5, 0.5*xf, x2);
//			conic2->set(0.0, 0.0, 0.0, 0.5,-0.5*xf, y2);
		}
	} else {
		/* generic case
		 * (x+f1*y+c1)*(x+f2*y+c2) = 0
		 * x^2 + (f1+f2)*xy + f1*f2*y^2 + (c1+c2)*x + (f1*c2+f2*c1)*y + c1*c2 = 0
		 * A   = 1
		 * 2*H = f1+f2   => f2 = 2*H-f1
		 * B   = f1*f2   = f1*(2*H-f1) => f1^2 -2*H*f1 + B = 0
		 * 2*G = c1+c2   => c2 = 2*G-c1
		 * C   = c1*c2   => c1^2 - 2*G*c1 + C = 0
		 *
		 * used for checking
		 * 2*F = f1*c2 + f2*c1
		 */
		double H = h / a;	// make A=1
		B = b / a;
		G = g / a;
		F = f / a;
		C = c / a;
		auto const sol = RS_Math::quadraticSolver(std::vector<double>{-2 * H, B});
//		quadratic(-2.0*H, B, &x1, &x2, CONICPREC);	// f1,f2

		// Substitute to 2*F to find c1,c2
		//    2*G = c1 + c2  =>  c2 = 2*G - c1
		//    2*F = f1*c2 + f2*c1 =>  c1 = (2*F - 2*G*f1) / (f2-f1)
		//                        =>  c1 = 2*(F-G*f1) / (2*H -2*f1)
		//                        =>  c1 = (F-G*f1) / (H-f1)
		if (ISZERO(G) && ISZERO(F)) {
			if (C<=0.0)
				y1 = sqrt(-C);
			else
				y1 = 0.0;
			y2 = -y1;
		} else {
			if (std::abs(a*b) < CONICSMALL*(1 + std::abs(a*b) + h*h)) {
				// Parallel lines
				x1 = x2 = H;
//				quadratic(-2.0*G, C, &y1, &y2, CONICPREC);	// c1,2
				auto const sol = RS_Math::quadraticSolver(std::vector<double>{-2*G, C});
				y1 = sol[0];
				y2 = sol[1];
			} else {
				// Coinciding lines
				y1 = (F - G*x1) / (H-x1);	// c1
				y2 = (F - G*x2) / (H-x2);	// c2
			}
		}
		return {{{1, x1, y1}}, {{1, x2, y2}}};
//		conic1->set(0.0, 0.0, 0.0, 0.5, 0.5*x1, y1);
//		conic2->set(0.0, 0.0, 0.0, 0.5, 0.5*x2, y2);

#if 0
		// check which combination f1,2, c1,2 is closer to 2*F
		if (Abs(x1*y2+x2*y1 -2.0*F) <= Abs(x1*y1+x2*y2 - 2.0*F)) {
#endif
	}

}
}

