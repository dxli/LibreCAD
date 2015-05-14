/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#ifndef RS_MATH_H
#define RS_MATH_H

// no idea why, but doesn't link without that under win32 / bcc55:
//#ifndef _MT
//#define _MT
//#endif

#include <cmath>
#include <vector>

typedef long double LDOUBLE;

class RS_Vector;
class RS_VectorSolutions;
class QString;

/**
 * Math functions.
 */
class RS_Math {
public:
	static int round(LDOUBLE v);
	static LDOUBLE pow(LDOUBLE x, LDOUBLE y);
	static RS_Vector pow(RS_Vector x, LDOUBLE y);

	static LDOUBLE rad2deg(LDOUBLE a);
	static LDOUBLE deg2rad(LDOUBLE a);
	static LDOUBLE rad2gra(LDOUBLE a);
	static LDOUBLE gra2rad(LDOUBLE a);
	static unsigned findGCD(unsigned a, unsigned b);
	static bool isAngleBetween(LDOUBLE a,
							   LDOUBLE a1, LDOUBLE a2,
                               bool reversed = false);
	//! \brief correct angle to be within [0, 2 Pi)
	static LDOUBLE correctAngle(LDOUBLE a);
	//! \brief correct angle to be undirectional [0, Pi)
	static LDOUBLE correctAngleU(LDOUBLE a);

	//! \brief angular difference
	static LDOUBLE getAngleDifference(LDOUBLE a1, LDOUBLE a2, bool reversed = false);
	/**
	 * @brief getAngleDifferenceU abs of minimum angular differenct, unsigned version of angular difference
	 * @param a1,a2 angles
	 * @return the minimum of angular difference a1-a2 and a2-a1
	 */
	static LDOUBLE getAngleDifferenceU(LDOUBLE a1, LDOUBLE a2);
	static LDOUBLE makeAngleReadable(LDOUBLE angle, bool readable=true,
									bool* corrected=nullptr);
	static bool isAngleReadable(LDOUBLE angle);
	static bool isSameDirection(LDOUBLE dir1, LDOUBLE dir2, LDOUBLE tol);

	//! \{ \brief evaluate a math string
	static LDOUBLE eval(const QString& expr, LDOUBLE def=0.0);
	static LDOUBLE eval(const QString& expr, bool* ok);
	//! \}

	static std::vector<LDOUBLE> quadraticSolver(const std::vector<LDOUBLE>& ce);
	static std::vector<LDOUBLE> cubicSolver(const std::vector<LDOUBLE>& ce);
    /** quartic solver
    * x^4 + ce[0] x^3 + ce[1] x^2 + ce[2] x + ce[3] = 0
    @ce, a vector of size 4 contains the coefficient in order
    @return, a vector contains real roots
    **/
	static std::vector<LDOUBLE> quarticSolver(const std::vector<LDOUBLE>& ce);
    /** quartic solver
* ce[4] x^4 + ce[3] x^3 + ce[2] x^2 + ce[1] x + ce[0] = 0
    @ce, a vector of size 5 contains the coefficient in order
    @return, a vector contains real roots
    **/
	static std::vector<LDOUBLE> quarticSolverFull(const std::vector<LDOUBLE>& ce);
    //solver for linear equation set
    /**
      * Solve linear equation set
	  *@param mt holds the augmented matrix
	  *@param sn holds the solution
	  *@param return true, if the equation set has a unique solution, return false otherwise
      *
	  *@author: Dongxu Li
      */
	static bool linearSolver(const std::vector<std::vector<LDOUBLE> >& m, std::vector<LDOUBLE>& sn);

    /** solver quadratic simultaneous equations of a set of two **/
    /* solve the following quadratic simultaneous equations,
      *  ma000 x^2 + ma011 y^2 - 1 =0
      * ma100 x^2 + 2 ma101 xy + ma111 y^2 + mb10 x + mb11 y +mc1 =0
      *
      *@m, a vector of size 8 contains coefficients in the strict order of:
      ma000 ma011 ma100 ma101 ma111 mb10 mb11 mc1
      *@return a RS_VectorSolutions contains real roots (x,y)
      */
	static RS_VectorSolutions simultaneousQuadraticSolver(const std::vector<LDOUBLE>& m);

    /** solver quadratic simultaneous equations of a set of two **/
	/** solve the following quadratic simultaneous equations,
      * ma000 x^2 + ma001 xy + ma011 y^2 + mb00 x + mb01 y + mc0 =0
      * ma100 x^2 + ma101 xy + ma111 y^2 + mb10 x + mb11 y + mc1 =0
      *
  *@param m a vector of size 2 each contains a vector of size 6 coefficients in the strict order of:
  ma000 ma001 ma011 mb00 mb01 mc0
  ma100 ma101 ma111 mb10 mb11 mc1
      *@return a RS_VectorSolutions contains real roots (x,y)
      */
	static RS_VectorSolutions simultaneousQuadraticSolverFull(const std::vector<std::vector<LDOUBLE> >& m);
	static RS_VectorSolutions simultaneousQuadraticSolverMixed(const std::vector<std::vector<LDOUBLE> >& m);

	/** \brief verify simultaneousQuadraticVerify a solution for simultaneousQuadratic
	  *@param m the coefficient matrix
	  *@param v a candidate to verify
      *@return true, for a valid solution
      **/
	static bool simultaneousQuadraticVerify(const std::vector<std::vector<LDOUBLE> >& m, RS_Vector& v);
    /** wrapper for elliptic integral **/
    /**
     * wrapper of elliptic integral of the second type, Legendre form
     *@k the elliptic modulus or eccentricity
     *@phi elliptic angle, must be within range of [0, M_PI]
     *
	 *@\author: Dongxu Li
     */
	static LDOUBLE ellipticIntegral_2(const LDOUBLE& k, const LDOUBLE& phi);

	static QString doubleToString(LDOUBLE value, LDOUBLE prec);
	static QString doubleToString(LDOUBLE value, int prec);

    static void test();
    };

#endif
