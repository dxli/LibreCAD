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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/math/special_functions/ellint_2.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>

#include <cmath>
#include <sstream>
#include <string>
#include <muParser.h>
#include <QString>
#include <QDebug>
#include "rs_line.h"
#include "lc_quadratic.h"

#include "rs_math.h"
#include "rs_vector.h"
#include "rs_debug.h"

#ifdef EMU_C99
#include "emu_c99.h"
#endif


namespace {
constexpr double m_piX2 = M_PI*2; //2*PI

using namespace boost::numeric::ublas;
using Vector = RS_Math::Vector;
using Matrix = RS_Math::Matrix;
void TransposeMultiply (Vector const& vector,
						Matrix& result)
{
	auto const size = vector.size();
	result.resize (size,size);
	result.clear ();
	for(size_t row=0; row< vector.size(); ++row)
		for(size_t col=0; col < vector.size(); ++col)
			result(row, col) = vector(col) * vector(row);
}

void HouseholderCornerSubstraction (Matrix& LeftLarge,
									Matrix const& RightSmall)
{
	if(
		!(
			(LeftLarge.size1() >= RightSmall.size1())
			&& (LeftLarge.size2() >= RightSmall.size2())
		)
	)
	{
		std::cerr << "invalid matrix dimensions" << std::endl;
		return;
	}
	size_t const offset = LeftLarge.size1() - RightSmall.size1();
	for(size_t row = 0; row < RightSmall.size2(); ++row )
		for(size_t col = 0; col < RightSmall.size1(); ++col )
			LeftLarge(offset+col, offset+row) -= RightSmall(col,row);
}

template<typename T>
std::string toString(matrix<T> const& mat)
{
	std::ostringstream oss;
	oss<<mat.size1()<<"x"<<mat.size2()<<'\n';
	for (size_t i=0; i < mat.size1(); i++) {
		for (size_t j=0; j < mat.size2(); j++)
			oss<< std::fixed << std::setw(6) << std::setprecision(4)<<mat(i, j)<<'\t';
		oss<<'\n';
	}
	return oss.str();
}

}

/**
 * Rounds the given double to the closest int.
 */
int RS_Math::round(double v) {
	return (int) lrint(v);
}

/**
 * Save pow function
 */
double RS_Math::pow(double x, double y) {
	errno = 0;
	double ret = ::pow(x, y);
	if (errno==EDOM) {
		RS_DEBUG->print(RS_Debug::D_ERROR,
						"RS_Math::pow: EDOM in pow");
		ret = 0.0;
	}
	else if (errno==ERANGE) {
		RS_DEBUG->print(RS_Debug::D_WARNING,
						"RS_Math::pow: ERANGE in pow");
		ret = 0.0;
	}
	return ret;
}

/* pow of vector components */
RS_Vector RS_Math::pow(RS_Vector vp, double y) {
	return RS_Vector(pow(vp.x,y),pow(vp.y,y));
}

/**
 * Converts radians to degrees.
 */
double RS_Math::rad2deg(double a) {
	return 180./M_PI*a;
}

/**
 * Converts degrees to radians.
 */
double RS_Math::deg2rad(double a) {
	return M_PI/180.0*a;
}

/**
 * Converts radians to gradians.
 */
double RS_Math::rad2gra(double a) {
	return 200./M_PI*a;
}

double RS_Math::gra2rad(double a) {
	return M_PI/200.*a;
}


/**
 * Finds greatest common divider using Euclid's algorithm.
 */
unsigned RS_Math::findGCD(unsigned a, unsigned b) {

	while (b) {
		unsigned rem = a % b;
		a = b;
		b = rem;
	}

	return a;
}



/**
 * Tests if angle a is between a1 and a2. a, a1 and a2 must be in the
 * range between 0 and 2*PI.
 * All angles in rad.
 *
 * @param reversed true for clockwise testing. false for ccw testing.
 * @return true if the angle a is between a1 and a2.
 */
bool RS_Math::isAngleBetween(double a,
							 double a1, double a2,
							 bool reversed) {

	if (reversed) std::swap(a1,a2);
	if(getAngleDifferenceU(a2, a1 ) < RS_TOLERANCE_ANGLE) return true;
	const double tol=0.5*RS_TOLERANCE_ANGLE;
	const double diff0=correctAngle(a2 -a1) + tol;

	return diff0 >= correctAngle(a - a1) || diff0 >= correctAngle(a2 - a);
}

/**
 * Corrects the given angle to the range of 0-2*Pi.
 */
double RS_Math::correctAngle(double a) {
	return M_PI + remainder(a - M_PI, m_piX2);
}

double RS_Math::correctAngleU(double a) {
	return std::abs(remainder(a, m_piX2));
}


/**
 * @return The angle that needs to be added to a1 to reach a2.
 *         Always positive and less than 2*pi.
 */
double RS_Math::getAngleDifference(double a1, double a2, bool reversed) {
	if(reversed) std::swap(a1, a2);
	return correctAngle(a2 - a1);
}

double RS_Math::getAngleDifferenceU(double a1, double a2)
{
	return correctAngleU(a1 - a2);
}


/**
* Makes a text constructed with the given angle readable. Used
* for dimension texts and for mirroring texts.
*
* @param readable true: make angle readable, false: unreadable
* @param corrected Will point to true if the given angle was
*   corrected, false otherwise.
*
 * @return The given angle or the given angle+PI, depending which on
 * is readable from the bottom or right.
 */
double RS_Math::makeAngleReadable(double angle, bool readable,
								  bool* corrected) {

	double ret=correctAngle(angle);

	bool cor = isAngleReadable(ret) ^ readable;

	// quadrant 1 & 4
	if (cor) {
		//        ret = angle;
		//    }
		// quadrant 2 & 3
		//    else {
		ret = correctAngle(angle+M_PI);
	}

	if (corrected) {
		*corrected = cor;
	}

	return ret;
}


/**
 * @return true: if the given angle is in a range that is readable
 * for texts created with that angle.
 */
bool RS_Math::isAngleReadable(double angle) {
	const double tolerance=0.001;
	if (angle>M_PI_2)
		return std::abs(remainder(angle, m_piX2)) < (M_PI_2 - tolerance);
	else
		return std::abs(remainder(angle, m_piX2)) < (M_PI_2 + tolerance);
}

/**
 * @param tol Tolerance in rad.
 * @retval true The two angles point in the same direction.
 */
bool RS_Math::isSameDirection(double dir1, double dir2, double tol) {
	return getAngleDifferenceU(dir1, dir2) < tol;
}

/**
 * Evaluates a mathematical expression and returns the result.
 * If an error occured, the given default value 'def' will be returned.
 */
double RS_Math::eval(const QString& expr, double def) {

	bool ok;
	double res = RS_Math::eval(expr, &ok);

	if (!ok) {
		//std::cerr << "RS_Math::evaluate: Parse error at col "
		//<< ret << ": " << fp.ErrorMsg() << "\n";
		return def;
	}

	return res;
}


/**
 * Evaluates a mathematical expression and returns the result.
 * If an error occured, ok will be set to false (if ok isn't NULL).
 */
double RS_Math::eval(const QString& expr, bool* ok) {
	bool okTmp(false);
	if(!ok) ok=&okTmp;
	if (expr.isEmpty()) {
		*ok = false;
		return 0.0;
	}
	double ret(0.);
	try{
		mu::Parser p;
		p.DefineConst("pi",M_PI);
		p.SetExpr(expr.toStdString());
		ret=p.Eval();
		*ok=true;
	}
	catch (mu::Parser::exception_type &e)
	{
		std::cout << e.GetMsg() << std::endl;
		*ok=false;
	}
	return ret;
}


/**
 * Converts a double into a string which is as short as possible
 *
 * @param value The double value
 * @param prec Precision e.g. a precision of 1 would mean that a
 *     value of 2.12030 will be converted to "2.1". 2.000 is always just "2").
 */
QString RS_Math::doubleToString(double value, double prec) {
	if (prec< RS_TOLERANCE ) {
		RS_DEBUG->print(RS_Debug::D_ERROR,
						"RS_Math::doubleToString: invalid precision");
		return QString().setNum(value, prec);
	}

	double const num = RS_Math::round(value / prec)*prec;

	QString exaStr = RS_Math::doubleToString(1./prec, 10);
	int const dotPos = exaStr.indexOf('.');

	if (dotPos==-1) {
		//big numbers for the precision
		return QString().setNum(RS_Math::round(num));
	} else {
		//number of digits after the point
		int digits = dotPos - 1;
		return RS_Math::doubleToString(num, digits);
	}
}

/**
 * Converts a double into a string which is as short as possible.
 *
 * @param value The double value
 * @param prec Precision
 */
QString RS_Math::doubleToString(double value, int prec) {
	QString valStr;

	valStr.setNum(value, 'f', prec);

	if(valStr.contains('.')) {
		// Remove tailing point and zeros:
		//        valStr.replace(QRegExp("0*$"), "");
		//        valStr.replace(QRegExp(R"(\.$)"), "");
		//        while (valStr.at(valStr.length()-1)=='0') {
		//            valStr.truncate(valStr.length()-1);
		//        }

		if(valStr.at(valStr.length()-1)=='.') {
			valStr.truncate(valStr.length()-1);
		}

	}

	return valStr;
}



/**
 * Performs some testing for the math class.
 */
void RS_Math::test() {
	{
		std::cout<<"testing quadratic solver"<<std::endl;
		//equations x^2 + v[0] x + v[1] = 0
		std::vector<std::vector<double>> const eqns{
			{-1., -1.},
			{-101., -1.},
			{-1., -100.},
			{2., 1.},
			{-2., 1.}
		};
		//expected roots
		std::vector<std::vector<double>> roots{
			{-0.6180339887498948, 1.6180339887498948},
			{-0.0099000196991084878, 101.009900019699108},
			{-9.5124921972503929, 10.5124921972503929},
			{-1.},
			{1.}
		};

		for(size_t i=0; i < eqns.size(); i++) {
			std::cout<<"Test quadratic solver, test case: x^2 + ("
					<<eqns[i].front()<<") x + ("
				   <<eqns[i].back()<<") = 0"<<std::endl;
			auto sol = quadraticSolver(eqns[i]);
			assert(sol.size()==roots[i].size());
			if (sol.front() > sol.back())
				std::swap(sol[0], sol[1]);
			auto expected=roots[i];
			if (expected.front() > expected.back())
				std::swap(expected[0], expected[1]);
			for (size_t j=0; j < sol.size(); j++) {
				double x0 = sol[j];
				double x1 = expected[j];
				double const prec = (x0 - x1)/(std::abs(x0 + x1) + RS_TOLERANCE2);
				std::cout<<"root "<<j<<" : precision level = "<<prec<<std::endl;
				std::cout<<std::setprecision(17)<<"found: "<<x0<<"\texpected: "<<x1<<std::endl;
				assert(prec < RS_TOLERANCE);
			}
			std::cout<<std::endl;
		}
		return;
	}
	QString s;
    double v;

	std::cout << "RS_Math::test: doubleToString:\n";

	v = 0.1;
	s = RS_Math::doubleToString(v, 0.1);
	assert(s=="0.1");
	s = RS_Math::doubleToString(v, 0.01);
	assert(s=="0.10");

	v = 0.01;
	s = RS_Math::doubleToString(v, 0.1);
	assert(s=="0.0");
	s = RS_Math::doubleToString(v, 0.01);
	assert(s=="0.01");
	s = RS_Math::doubleToString(v, 0.001);
	assert(s=="0.010");

	v = 0.001;
	s = RS_Math::doubleToString(v, 0.1);
	assert(s=="0.0");
	s = RS_Math::doubleToString(v, 0.01);
	assert(s=="0.00");
	s = RS_Math::doubleToString(v, 0.001);
	assert(s=="0.001");

	std::cout << "RS_Math::test: complete"<<std::endl;
}



//Equation solvers

// quadratic, cubic, and quartic equation solver
// @ ce[] contains coefficent of the cubic equation:
// @ returns a vector contains real roots
//
// solvers assume arguments are valid, and there's no attempt to verify validity of the argument pointers
//
// @author Dongxu Li <dongxuli2011@gmail.com>

template<>
std::vector<double> RS_Math::quadraticSolver(const std::vector<double>& ce)
//quadratic solver for
// x^2 + ce[0] x + ce[1] =0
{
	std::vector<double> ans(0, 0.);
	if (ce.size() != 2) return ans;
	using LDouble = long double;
	LDouble const b = -0.5L * ce[0];
	LDouble const c = ce[1];
	// x^2 -2 b x + c=0
	// (x - b)^2 = b^2 - c
	// b^2 >= std::abs(c)
	// x = b \pm b sqrt(1. - c/(b^2))
	LDouble const b2= b * b;
	LDouble const discriminant= b2 - c;
	LDouble const fc = std::abs(c);

	//TODO, fine tune to tolerance level

	if (discriminant < -10 * RS_TOLERANCE) {
		//negative discriminant, no real root
		return ans;
	}

	//find the radical
	LDouble r;

	// given |p| >= |q|
	// sqrt(p^2 \pm q^2) = p sqrt(1 \pm q^2/p^2)
	if (b2 >= fc)
		r = std::abs(b) * std::sqrt(1.L - c/b2);
	else
		// c is negative, because b2 - c is non-negative
		r = std::sqrt(fc) * std::sqrt((c - b2)/fc);

	constexpr LDouble TOL = 1e-24L;
	if (r >= TOL*std::abs(b)) {
		//two roots
		if (b >= 0.L)
			//since both (b,r)>=0, avoid (b - r) loss of significance
			ans.push_back(b + r);
		else
			//since b<0, r>=0, avoid (b + r) loss of significance
			ans.push_back(b - r);

		//Vieta's formulas for the second root
		ans.push_back(c/ans.front());
	} else
		//multiple roots
		ans.push_back(b);
	return ans;
}

template<>
std::vector<long double> RS_Math::quadraticSolver(const std::vector<long double>& ce)
//quadratic solver for
// x^2 + ce[0] x + ce[1] =0
{
	using LDouble = long double;
	std::vector<LDouble> ans(0, 0.L);
	if (ce.size() != 2) return ans;
	LDouble const b = -0.5L * ce[0];
	LDouble const c = ce[1];
	// x^2 -2 b x + c=0
	// (x - b)^2 = b^2 - c
	// b^2 >= std::abs(c)
	// x = b \pm b sqrt(1. - c/(b^2))
	LDouble const b2= b * b;
	LDouble const discriminant= b2 - c;
	LDouble const fc = std::abs(c);

	//TODO, fine tune to tolerance level

	if (discriminant < -10 * RS_TOLERANCE) {
		std::cout<<__LINE__<<" discriminant="<<discriminant<<std::endl;
		return ans;
	}

	//find the radical
	LDouble r;

	// given |p| >= |q|
	// sqrt(p^2 \pm q^2) = p sqrt(1 \pm q^2/p^2)
	if (b2 >= fc)
		r = std::abs(b) * std::sqrt(1.L - c/b2);
	else
		// c is negative, because b2 - c is non-negative
		r = std::sqrt(fc) * std::sqrt((c - b2)/fc);

	constexpr LDouble TOL = 1e-24L;
	if (r >= TOL*std::abs(b)) {
		//two roots
		if (b >= 0.L)
			//since both (b,r)>=0, avoid (b - r) loss of significance
			ans.push_back(b + r);
		else
			//since b<0, r>=0, avoid (b + r) loss of significance
			ans.push_back(b - r);

		//Vieta's formulas for the second root
		ans.push_back(c/ans.front());
	} else
		//multiple roots
		ans.push_back(b);
	return ans;
}

std::vector<double> RS_Math::cubicSolver(const std::vector<double>& ce)
//cubic equation solver
// x^3 + ce[0] x^2 + ce[1] x + ce[2] = 0
{
	//    std::cout<<"x^3 + ("<<ce[0]<<")*x^2+("<<ce[1]<<")*x+("<<ce[2]<<")==0"<<std::endl;
	using LDouble = long double;
	if (ce.size() != 3) return {};
	std::vector<LDouble> ans(0, 0.L);
	// depressed cubic, Tschirnhaus transformation, x= t - b/(3a)
	// t^3 + p t +q =0
	std::array<LDouble, 3> lce{{ce[0], ce[1], ce[2]}};
	LDouble shift=(1.L/3)*lce[0];
	LDouble p=lce[1] -shift*lce[0];
	LDouble q=lce[0]*((2.L/27)*lce[0]*lce[0]-(1.L/3)*lce[1])+lce[2];
	//Cardano's method,
	//	t=u+v
	//	u^3 + v^3 + (3 uv + p ) (u+v) + q =0
	//	select 3uv + p =0, then,
	//	u^3 + v^3 = -q
	//	u^3 v^3 = - p^3/27
	//	so, u^3 and v^3 are roots of equation,
	//	z^2 + q z - p^3/27 = 0
	//	and u^3,v^3 are,
	//		-q/2 \pm sqrt(q^2/4 + p^3/27)
	//	discriminant= q^2/4 + p^3/27
	//std::cout<<"p="<<p<<"\tq="<<q<<std::endl;
	LDouble discriminant= (1.L/27)*p*p*p+ 0.25L*q*q;
	if (std::abs(p) < 1.0e-75L) {
		ans.push_back((q>0)?-pow(q,(1.L/3)):pow(-q,(1.L/3)) - shift);
		//        DEBUG_HEADER
		//        std::cout<<"cubic: one root: "<<ans[0]<<std::endl;
		return {double(ans[0])};
	}
	//std::cout<<"discriminant="<<discriminant<<std::endl;
	if(discriminant>0) {
		std::vector<LDouble> ce2(2,0.L);
		ce2[0]=q;
		ce2[1]=-1.L/27*p*p*p;
		auto r=quadraticSolver(ce2);
		if (r.size()==0 ) { //should not happen
			std::cerr<<__FILE__<<" : "<<__func__<<" : line"<<__LINE__<<" :cubicSolver()::Error cubicSolver("<<ce[0]<<' '<<ce[1]<<' '<<ce[2]<<")\n";
        }
		LDouble u= (q<=0) ? pow(r[0], 1.L/3): -pow(-r[1],1.L/3);
        //u=(q<=0)?pow(-0.5*q+sqrt(discriminant),1./3):-pow(0.5*q+sqrt(discriminant),1./3);
		LDouble v=(-1.L/3)*p/u;
        //std::cout<<"u="<<u<<"\tv="<<v<<std::endl;
        //std::cout<<"u^3="<<u*u*u<<"\tv^3="<<v*v*v<<std::endl;
		ans.push_back(sum({u, v, -shift}));

//        DEBUG_HEADER
//        std::cout<<"cubic: one root: "<<ans[0]<<std::endl;
	}else{
		std::complex<LDouble> u{q, 0.L},rt[3];
		u=std::pow(-0.5L*u-sqrt(0.25L*u*u+p*p*p/27), 1.L/3);
		rt[0]=u-p/(3.L*u)-shift;
		std::complex<LDouble> w(-0.5L, sqrt(3.L)/2);
		rt[1]=u*w-p/(3.L*u*w)-shift;
		rt[2]=u/w-p*w/(3.L*u)-shift;
		//        DEBUG_HEADER
		//        std::cout<<"Roots:\n";
		//        std::cout<<rt[0]<<std::endl;
		//        std::cout<<rt[1]<<std::endl;
		//        std::cout<<rt[2]<<std::endl;
		ans.push_back(rt[0].real());
		ans.push_back(rt[1].real());
		ans.push_back(rt[2].real());
	}
	// newton-raphson
	for(LDouble& x: ans){
		LDouble const x2 = x*x;
		LDouble const x3 = x2*x;
		LDouble fm=0;
		for(size_t i=0; i<20; ++i){
			LDouble f= sum({x3, lce[0]*x2, lce[1]*x, lce[2]});
			LDouble df=sum({3.L*x2, 2.L*lce[0]*x, lce[1]});
			if (!std::isnormal(f)||(std::isnormal(fm) && std::abs(f) > fm))
				break;
			fm = std::abs(f);
			LDouble const dx=f/df;
			x -= dx;
		}
	}

	return std::vector<double>(ans.begin(), ans.end());
}

/** quartic solver
* x^4 + ce[0] x^3 + ce[1] x^2 + ce[2] x + ce[3] = 0
@ce, a vector of size 4 contains the coefficient in order
@return, a vector contains real roots
**/
std::vector<double> RS_Math::quarticSolver(const std::vector<double>& ce)
{
    std::vector<double> ans(0,0.);
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
		DEBUG_HEADER
        std::cout<<"expected array size=4, got "<<ce.size()<<std::endl;
    }
    if(ce.size() != 4) return ans;
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        std::cout<<"x^4+("<<ce[0]<<")*x^3+("<<ce[1]<<")*x^2+("<<ce[2]<<")*x+("<<ce[3]<<")==0"<<std::endl;
    }

    // x^4 + a x^3 + b x^2 +c x + d = 0
    // depressed quartic, x= t - a/4
	// t^4 + (b - 3/8 a^2 ) t^2 + (c - a b/2 + a^3/8) t + d - a c /4 + a^2 b/16 - 3 a^4/256 =0
    // t^4 + p t^2 + q t + r =0
    // p= b - (3./8)*a*a;
    // q= c - 0.5*a*b+(1./8)*a*a*a;
    // r= d - 0.25*a*c+(1./16)*a*a*b-(3./256)*a^4
    double shift=0.25*ce[0];
    double shift2=shift*shift;
    double a2=ce[0]*ce[0];
    double p= ce[1] - (3./8)*a2;
    double q= ce[2] + ce[0]*((1./8)*a2 - 0.5*ce[1]);
    double r= ce[3] - shift*ce[2] + (ce[1] - 3.*shift2)*shift2;
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
		DEBUG_HEADER
        std::cout<<"x^4+("<<p<<")*x^2+("<<q<<")*x+("<<r<<")==0"<<std::endl;
    }
	if (q*q <= 1.e-4*RS_TOLERANCE*std::abs(p*r)) {// Biquadratic equations
        double discriminant= 0.25*p*p -r;
        if (discriminant < -1.e3*RS_TOLERANCE) {

//            DEBUG_HEADER
//            std::cout<<"discriminant="<<discriminant<<"\tno root"<<std::endl;
            return ans;
        }
        double t2[2];
		t2[0]=-0.5*p-sqrt(std::abs(discriminant));
        t2[1]= -p - t2[0];
        //        std::cout<<"t2[0]="<<t2[0]<<std::endl;
        //        std::cout<<"t2[1]="<<t2[1]<<std::endl;
		if (t2[1] >= 0.) { // two real roots
            ans.push_back(sqrt(t2[1])-shift);
            ans.push_back(-sqrt(t2[1])-shift);
        }
		if (t2[0] >= 0. ) {// four real roots
            ans.push_back(sqrt(t2[0])-shift);
            ans.push_back(-sqrt(t2[0])-shift);
        }
//        DEBUG_HEADER
//        for(int i=0;i<ans.size();i++){
//            std::cout<<"root x: "<<ans[i]<<std::endl;
//        }
        return ans;
    }
	if (std::abs(r)< 1.0e-75 ) {
        std::vector<double> cubic(3,0.);
        cubic[1]=p;
        cubic[2]=q;
        ans.push_back(0.);
		auto r=cubicSolver(cubic);
		std::copy(r.begin(),r.end(), std::back_inserter(ans));
        for(size_t i=0; i<ans.size(); i++) ans[i] -= shift;
        return ans;
    }
    // depressed quartic to two quadratic equations
	// t^4 + p t^2 + q t + r = (t^2 + u t + v) (t^2 - u t + w)
    // so,
    // 	p + u^2= w+v
    // 	q/u= w-v
    // 	r= wv
    // so,
    //  (p+u^2)^2 - (q/u)^2 = 4 r
    //  y=u^2,
	//  y^3 + 2 p y^2 + (p^2 - 4 r) y - q^2 =0
    //
    std::vector<double> cubic(3,0.);
    cubic[0]=2.*p;
    cubic[1]=p*p-4.*r;
    cubic[2]=-q*q;
	auto r3= cubicSolver(cubic);
    //std::cout<<"quartic_solver:: real roots from cubic: "<<ret<<std::endl;
    //for(unsigned int i=0; i<ret; i++)
    //   std::cout<<"cubic["<<i<<"]="<<cubic[i]<<" x= "<<croots[i]<<std::endl;
	//newton-raphson
    if (r3.size()==1) { //one real root from cubic
        if (r3[0]< 0.) {//this should not happen
			DEBUG_HEADER
			qDebug()<<"Quartic Error:: Found one real root for cubic, but negative\n";
            return ans;
        }
        double sqrtz0=sqrt(r3[0]);
        std::vector<double> ce2(2,0.);
        ce2[0]=	-sqrtz0;
        ce2[1]=0.5*(p+r3[0])+0.5*q/sqrtz0;
        auto r1=quadraticSolver(ce2);
        if (r1.size()==0 ) {
            ce2[0]=	sqrtz0;
            ce2[1]=0.5*(p+r3[0])-0.5*q/sqrtz0;
            r1=quadraticSolver(ce2);
        }
		for(auto& x: r1){
			x -= shift;
		}
		return r1;
	}
	if (r3[0]> 0. && r3[1] > 0. ) {
		double sqrtz0=sqrt(r3[0]);
		std::vector<double> ce2(2,0.);
		ce2[0]=	-sqrtz0;
		ce2[1]=0.5*(p+r3[0])+0.5*q/sqrtz0;
		ans=quadraticSolver(ce2);
		ce2[0]=	sqrtz0;
		ce2[1]=0.5*(p+r3[0])-0.5*q/sqrtz0;
		auto r1=quadraticSolver(ce2);
		std::copy(r1.begin(),r1.end(),std::back_inserter(ans));
		for(auto& x: ans){
			x -= shift;
		}
	}
	// newton-raphson
	for(double& x0: ans){
		double dx=0.;
		for(size_t i=0; i<20; ++i){
			double f=(((x0 + ce[0])*x0 + ce[1])*x0 +ce[2])*x0 + ce[3] ;
			double df=((4.*x0+3.*ce[0])*x0 +2.*ce[1])*x0+ce[2];
			//			DEBUG_HEADER
			//			qDebug()<<"i="<<i<<"\tx0="<<x0<<"\tf="<<f<<"\tdf="<<df;
			if(std::abs(df)>RS_TOLERANCE2){
				dx=f/df;
				x0 -= dx;
			}else
				break;
		}
	}

	return ans;
}

/** quartic solver
* ce[4] x^4 + ce[3] x^3 + ce[2] x^2 + ce[1] x + ce[0] = 0
@ce, a vector of size 5 contains the coefficient in order
@return, a vector contains real roots
*ToDo, need a robust algorithm to locate zero terms, better handling of tolerances
**/
std::vector<double> RS_Math::quarticSolverFull(const std::vector<double>& ce)
{
	if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
		DEBUG_HEADER
				std::cout<<ce[4]<<"*y^4+("<<ce[3]<<")*y^3+("<<ce[2]<<"*y^2+("<<ce[1]<<")*y+("<<ce[0]<<")==0"<<std::endl;
	}

	std::vector<double> roots(0,0.);
	if(ce.size()!=5) return roots;
	std::vector<double> ce2(4,0.);

	if (std::abs(ce[4]) < 1.0e-14) { // this should not happen
		if (std::abs(ce[3]) < 1.0e-14) { // this should not happen
			if (std::abs(ce[2]) < 1.0e-14) { // this should not happen
				if(std::abs(ce[1]) > 1.0e-14) {
					roots.push_back(-ce[0]/ce[1]);
				} else { // can not determine y. this means overlapped, but overlap should have been detected before, therefore return empty set
					return roots;
				}
			} else {
				ce2.resize(2);
				ce2[0]=ce[1]/ce[2];
				ce2[1]=ce[0]/ce[2];
				//std::cout<<"ce2[2]={ "<<ce2[0]<<' '<<ce2[1]<<" }\n";
				roots=RS_Math::quadraticSolver(ce2);
			}
		} else {
			ce2.resize(3);
			ce2[0]=ce[2]/ce[3];
			ce2[1]=ce[1]/ce[3];
			ce2[2]=ce[0]/ce[3];
			//std::cout<<"ce2[3]={ "<<ce2[0]<<' '<<ce2[1]<<' '<<ce2[2]<<" }\n";
			roots=RS_Math::cubicSolver(ce2);
		}
	} else {
		ce2[0]=ce[3]/ce[4];
		ce2[1]=ce[2]/ce[4];
		ce2[2]=ce[1]/ce[4];
		ce2[3]=ce[0]/ce[4];
		if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
			DEBUG_HEADER
					std::cout<<"ce2[4]={ "<<ce2[0]<<' '<<ce2[1]<<' '<<ce2[2]<<' '<<ce2[3]<<" }\n";
		}
		if(std::abs(ce2[3])<= RS_TOLERANCE15) {
			//constant term is zero, factor 0 out, solve a cubic equation
			ce2.resize(3);
			roots=RS_Math::cubicSolver(ce2);
			roots.push_back(0.);
		}else
			roots=RS_Math::quarticSolver(ce2);
	}
	return roots;
}

//linear Equation solver by Gauss-Jordan
/**
  * Solve linear equation set
  *@ mt holds the augmented matrix
  *@ sn holds the solution
  *@ return true, if the equation set has a unique solution, return false otherwise
  *
  *@Author: Dongxu Li
  */

bool RS_Math::linearSolver(const std::vector<std::vector<double> >& mt, std::vector<double>& sn){
	//verify the matrix size
	size_t mSize(mt.size()); //rows
	size_t aSize(mSize+1); //columns of augmented matrix
	if(std::any_of(mt.begin(), mt.end(), [&aSize](const std::vector<double>& v)->bool{
				   return v.size() != aSize;
}))
		return false;
	sn.resize(mSize);//to hold the solution
#if 0
	using namespace boost::numeric::ublas;
	Matrix bm(mSize, mSize);
	for (size_t i=0; i < mSize; i++)
		for (size_t j=0; j < mSize; j++)
			bm(i, j) = mt[i][j];

	Vector bs(mSize);
	for (size_t i=0; i < mSize; i++)
		bs(i) = mt[i].back();

	//householder decomposition, M = QR
	Matrix Q, R;
	HouseholderQR(bm, Q, R);
	std::cout<<"Q=\n"<<toString(Q)<<std::endl;
	std::cout<<"R=\n"<<toString(R)<<std::endl;
	bs = prod(Q, bs); // Q is Hermitan and unitary, Q = Q^-1
	//solve the linear equation set by LU decomposition in boost ublas
	auto const um = triangular_adaptor<Matrix, upper>(R);
	std::cout<<"um=\n"<<um<<std::endl;


	boost::numeric::ublas::inplace_solve(um, bs, boost::numeric::ublas::upper_tag());
	for(size_t i=0; i< mSize; i++){
		sn[i]=bs(i);
	}
	//    std::cout<<"dn="<<dn<<std::endl;
	//    data.center.set(-0.5*dn(1)/dn(0),-0.5*dn(3)/dn(2)); // center
	//    double d(1.+0.25*(dn(1)*dn(1)/dn(0)+dn(3)*dn(3)/dn(2)));
	//    if(std::abs(dn(0))<RS_TOLERANCE2
	//            ||std::abs(dn(2))<RS_TOLERANCE2
	//            ||d/dn(0)<RS_TOLERANCE2
	//            ||d/dn(2)<RS_TOLERANCE2
	//            ) {
	//        //ellipse not defined
	//        return false;
	//    }
	//    d=sqrt(d/dn(0));
	//    data.majorP.set(d,0.);
	//    data.ratio=sqrt(dn(0)/dn(2));
#else
	// solve the linear equation by Gauss-Jordan elimination
	std::vector<std::vector<double> > mt0(mt); //copy the matrix;
	for(size_t i=0;i<mSize;++i){
		size_t imax(i);
		double cmax(std::abs(mt0[i][i]));
		for(size_t j=i+1;j<mSize;++j) {
			if(std::abs(mt0[j][i]) > cmax ) {
				imax=j;
				cmax=std::abs(mt0[j][i]);
			}
		}
		if(cmax<RS_TOLERANCE2) return false; //singular matrix
		if(imax != i) {//move the line with largest absolute value at column i to row i, to avoid division by zero
			std::swap(mt0[i],mt0[imax]);
		}
		for(size_t k=i+1;k<=mSize;++k) { //normalize the i-th row
			mt0[i][k] /= mt0[i][i];
		}
		mt0[i][i]=1;
		for(size_t j=0;j<mSize;++j) {//Gauss-Jordan
			if(j != i ) {
				double& a = mt0[j][i];
				for(size_t k=i+1;k<=mSize;++k) {
					mt0[j][k] -= mt0[i][k]*a;
				}
				a=0.;
			}
		}
		//output gauss-jordan results for debugging
		//		std::cout<<"========"<<i<<"==========\n";
		//		for(auto v0: mt0){
		//			for(auto v1:v0)
		//				std::cout<<v1<<'\t';
		//			std::cout<<std::endl;
		//		}
	}
	for(size_t i=0;i<mSize;++i) {
		sn[i]=mt0[i][mSize];
	}
#endif

	return true;
}

bool RS_Math::HouseholderQR(Matrix const& M, Matrix& Q, Matrix& R)
{
	using namespace boost::numeric::ublas;
	using T = double;
	assert(M.size1() == M.size2());
	size_t size = M.size1();

	// init Matrices
	matrix<T> H, HTemp;
	HTemp = identity_matrix<T>(size);
	Q = identity_matrix<T>(size);
	R = M;

	// find Householder reflection matrices
	for(unsigned int col = 0; col < size-1; ++col)
	{
		// create X vector
		vector<T> RowView = column(R,col);
		vector_range<vector<T> > X2 (RowView, range (col, size));
		vector<T> X = X2;
			DEBUG_HEADER
		std::cout<<"HH: R: "<<toString(R)<<std::endl;

		if (inner_prod(X,X) < RS_TOLERANCE2*1e4 * std::abs(R(0, 0))) {
			//all zero in the rest of column
			continue;
		}

		// X -> U~
		X(0) += copysign(norm_2(X), X(0));

		HTemp.resize(X.size(),X.size(),true);

		TransposeMultiply(X, HTemp);

		// HTemp = the 2UUt part of H
		std::cout<<std::endl;
		DEBUG_HEADER
		qDebug()<<"Length: "<<inner_prod(X,X);

		std::cout<<R<<std::endl;

		HTemp *= ( 2 / inner_prod(X,X) );

		// H = I - 2UUt
		H = identity_matrix<T>(size);
		HouseholderCornerSubstraction(H, HTemp);

		// add H to Q and R
		Q = prod(Q,H);
		R = prod(H,R);
		DEBUG_HEADER
		std::cout<<"HH: R: "<<toString(R)<<std::endl;
	}
	// the bottom-right element is non-zero, HR failed due to an all-zero sub-column
	std::cout<<"R(2, 2)/R(0, 0) = "<<R(2, 2)/(R(0, 0) + DBL_EPSILON)<<std::endl;
	return std::abs(R(2, 2)) < RS_TOLERANCE * 10 * std::abs(R(0, 0));
}

/**
 * @brief HouseholderTridiagonal tridiagonalization of sysmmetric matrix
 * @param M input symmetric 3x3
 * @param T transform matrix
 * @param D tridiagonal matrix after transform
 */
bool RS_Math::HouseholderTridiagonal(Matrix const& M, Matrix& T, Matrix& D)
{
	T = identity_matrix<double>(3);
	DEBUG_HEADER
	if (std::abs(M(0, 0)) < RS_TOLERANCE2) {
		DEBUG_HEADER
		//reorder to avoid zero first column
		int j = std::abs(M(1, 1)) > std::abs(M(2, 2)) ? 1: 2;
		T(0, 0) = 0;
		T(j, j) = 0;
		T(0, j) = 1;
		T(j, 0) = 1;
		Matrix M1 = prod(T, M);
		M1 = prod(M1, T);
		Matrix T0;
		bool ret = HouseholderTridiagonal(M1, T0, D);
		// M = T M1 T = T T0 D T0 T = (T0 T)' D (T0 T)
		T = prod(T0, T);
		return ret;
	}

	D = M;
	// whether matrix is already tridiagonal
	if (std::abs(M(2, 0)) < RS_TOLERANCE15)
		return false;
	// form the sub-vector for HH reflection
	Vector c(2);
	c(0) = M(1, 0);
	c(1) = M(2, 0);
	double const s = std::hypot(c(0), c(1));
	if (s <= RS_TOLERANCE15)
		return false;
	double sg = std::copysign(1., M(1, 0));
	double z = (1 + sg * M(1, 0)/s)/2;
	double vk1 = std::sqrt(z);
	Vector v(3);
	v(0) = 0;
	v(1) = vk1;
	v(2) = sg * M(2,0)/(2 * vk1 *s);
	// HH reflection matrix
	T = T - outer_prod(v, v) * 2;
	D = prod(T, M);
	D = prod(D, T);
	return true;
}

#ifdef LC_DEBUGGING
#include <eigen3/Eigen/Dense>
std::pair<RS_Math::Vector, RS_Math::Matrix> RS_Math::eigenSystemSym3x3(Matrix const& m1)
{
	using Eigen::MatrixXd;
	MatrixXd em(3, 3);
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			em(i, j) = m1(i, j);

	Eigen::SelfAdjointEigenSolver<MatrixXd> es(em);
	if (false) {
		using namespace  std;

		cout << "matrix is : \n"<<em << std::endl;
		for (int i=0; i<3; i++)
		cout <<es.eigenvalues()(i) << endl;

		for (int i=0; i<3; i++) {
			cout<<es.eigenvectors().col(i)<<endl;
			for (int j=0; j<3; j++)
				cout<<es.eigenvectors().col(i)(j)<<' ';
			cout<<endl;
		}
	}
	//indices to be sorted
	auto const& li = es.eigenvalues();
	std::array<int, 3> il{{0, 1, 2}};
	if (fabs(li(0)) < fabs(li(1)))
		std::swap(il[0], il[1]);
	if (fabs(li(il[1])) < fabs(li(il[2])))
		std::swap(il[1], il[2]);
	if (li(il[0]) < li(il[1]))
		std::swap(il[0], il[1]);

	Vector ev(3);
	Matrix oL{3, 3};
	for (int i=0; i < 3; i++) {
		ev(i)=es.eigenvalues()(il[i]);
		for(int j=0; j<3; j++)
			oL(j, i) = es.eigenvectors().col(il[i])(j);
	}
	/* //eigen values in descending order
		qDebug()<<"eigen values:";
		for (int i=0; i<3; i++)
			qDebug()<<ev(i);
		*/
	return {ev, oL};
}

std::pair<RS_Math::Vector, RS_Math::Matrix> RS_Math::eigenSystemSym2x2_0(Matrix const& m)
{
	using Eigen::MatrixXd;
	MatrixXd em(2, 2);

	for (int i=0; i<2; i++)
		for (int j=0; j<2; j++)
			em(i, j) = m(i, j);

	Eigen::SelfAdjointEigenSolver<MatrixXd> es(em);
	Vector E(2);
	Matrix V(2, 2);
	auto const& e = es.eigenvalues();
	auto const& v = es.eigenvectors();
	int offset = (e(0) < e(1)) ? 1 : 0;
	for (int i = 0; i < 2; i++) {
		int const i2 = (i + offset) % 2;
		E(i) = e(i2);
		V(0, i) = v.col(i2)(0);
		V(1, i) = v.col(i2)(1);
	}
	return {E, V};
}
#endif

std::pair<RS_Math::Vector, RS_Math::Matrix> RS_Math::eigenSystemSym2x2(Matrix const& m)
{
	double a=m(0, 0);
	const double c=2. * m(0, 1);
	double b=m(1, 1);

	//Eigen system
	const double d = a - b;
	const double s=hypot(d, c);
	// { a>b, d>0
	// eigenvalue: ( a+b - s)/2, eigenvector: ( -c, d + s)
	// eigenvalue: ( a+b + s)/2, eigenvector: ( d + s, c)
	// }
	// { a<b, d<0
	// eigenvalue: ( a+b - s)/2, eigenvector: ( s-d,-c)
	// eigenvalue: ( a+b + s)/2, eigenvector: ( c, s-d)
	// }
	Vector E(2);
	Matrix V(2, 2);
	E(0) = sum({a, b, s}) * 0.5;
	E(1) = E(0) - s;
	if (d >= 0.) {
		V(0, 0) = d + s;
		V(1, 0) = c;
		V(0, 1) = -c;
		V(1, 1) = d + s;
	} else {
		V(0, 0) = c;
		V(1, 0) = s - d;
		V(0, 1) = s - d,
		V(1, 1) = -c;
	}
	return {E, V};
}

/**
 * wrapper of elliptic integral of the second type, Legendre form
 * @param k the elliptic modulus or eccentricity
 * @param phi elliptic angle, must be within range of [0, M_PI]
 *
 * @author: Dongxu Li
 */
double RS_Math::ellipticIntegral_2(const double& k, const double& phi)
{
	double a= remainder(phi-M_PI_2,M_PI);
	if(a>0.) {
		return boost::math::ellint_2<double,double>(k,a);
	} else {
		return - boost::math::ellint_2<double,double>(k,std::abs(a));
	}
}

/** solver quadratic simultaneous equations of set two **/
/* solve the following quadratic simultaneous equations,
  *  ma000 x^2 + ma011 y^2 - 1 =0
  * ma100 x^2 + 2 ma101 xy + ma111 y^2 + mb10 x + mb11 y +mc1 =0
  *
  *@m, a vector of size 8 contains coefficients in the strict order of:
  ma000 ma011 ma100 ma101 ma111 mb10 mb11 mc1
  * m[0] m[1] must be positive
  *@return a vector contains real roots
  */
RS_VectorSolutions RS_Math::simultaneousQuadraticSolver(const std::vector<double>& m)
{
	RS_VectorSolutions ret(0);
	if(m.size() != 8 ) return ret; // valid m should contain exact 8 elements
	std::vector< double> c1(0,0.);
	std::vector< std::vector<double> > m1(0,c1);
	c1.resize(6);
	c1[0]=m[0];
	c1[1]=0.;
	c1[2]=m[1];
	c1[3]=0.;
	c1[4]=0.;
	c1[5]=-1.;
	m1.push_back(c1);
	c1[0]=m[2];
	c1[1]=2.*m[3];
	c1[2]=m[4];
	c1[3]=m[5];
	c1[4]=m[6];
	c1[5]=m[7];
	m1.push_back(c1);

	return simultaneousQuadraticSolverFull(m1);
}

/** solver quadratic simultaneous equations of a set of two **/
/* solve the following quadratic simultaneous equations,
  * ma000 x^2 + ma001 xy + ma011 y^2 + mb00 x + mb01 y + mc0 =0
  * ma100 x^2 + ma101 xy + ma111 y^2 + mb10 x + mb11 y + mc1 =0
  *
  *@m, a vector of size 2 each contains a vector of size 6 coefficients in the strict order of:
  ma000 ma001 ma011 mb00 mb01 mc0
  ma100 ma101 ma111 mb10 mb11 mc1
  *@return a RS_VectorSolutions contains real roots (x,y)
  */
RS_VectorSolutions RS_Math::simultaneousQuadraticSolverFull(const std::vector<std::vector<double> >& m)
{
	RS_VectorSolutions ret;
	if(m.size()!=2)  return ret;
	if(m[0].size() ==3 || m[1].size()==3 ){
		return simultaneousQuadraticSolverMixed(m);
	}
	if(m[0].size()!=6 || m[1].size()!=6) return ret;
	/** eliminate x, quartic equation of y **/
	auto& a=m[0][0];
	auto& b=m[0][1];
	auto& c=m[0][2];
	auto& d=m[0][3];
	auto& e=m[0][4];
	auto& f=m[0][5];

	auto& g=m[1][0];
	auto& h=m[1][1];
	auto& i=m[1][2];
	auto& j=m[1][3];
	auto& k=m[1][4];
	auto& l=m[1][5];
	/**
	  Collect[Eliminate[{ a*x^2 + b*x*y+c*y^2+d*x+e*y+f==0,g*x^2+h*x*y+i*y^2+j*x+k*y+l==0},x],y]
	  **/
	/*
	 f^2 g^2 - d f g j + a f j^2 - 2 a f g l + (2 e f g^2 - d f g h - b f g j + 2 a f h j - 2 a f g k) y + (2 c f g^2 - b f g h + a f h^2 - 2 a f g i) y^2
 ==
 -(d^2 g l) + a d j l - a^2 l^2
+
 (d e g j - a e j^2 - d^2 g k + a d j k - 2 b d g l + 2 a e g l + a d h l + a b j l - 2 a^2 k l) y
+
 (-(e^2 g^2) + d e g h - d^2 g i + c d g j + b e g j - 2 a e h j + a d i j - a c j^2 - 2 b d g k + 2 a e g k + a d h k + a b j k - a^2 k^2 - b^2 g l + 2 a c g l + a b h l - 2 a^2 i l) y^2
 +
(-2 c e g^2 + c d g h + b e g h - a e h^2 - 2 b d g i + 2 a e g i + a d h i + b c g j - 2 a c h j + a b i j - b^2 g k + 2 a c g k + a b h k - 2 a^2 i k) y^3
+
 (-(c^2 g^2) + b c g h - a c h^2 - b^2 g i + 2 a c g i + a b h i - a^2 i^2) y^4


	  */
	double a2=a*a;
	double b2=b*b;
	double c2=c*c;
	double d2=d*d;
	double e2=e*e;
	double f2=f*f;

	double g2=g*g;
	double  h2=h*h;
	double  i2=i*i;
	double  j2=j*j;
	double  k2=k*k;
	double  l2=l*l;
	std::vector<double> qy(5,0.);
	//y^4
	qy[4]=-c2*g2 + b*c*g*h - a*c*h2 - b2*g*i + 2.*a*c*g*i + a*b*h*i - a2*i2;
	//y^3
	qy[3]=-2.*c*e*g2 + c*d*g*h + b*e*g*h - a*e*h2 - 2.*b*d*g*i + 2.*a*e*g*i + a*d*h*i +
			b*c*g*j - 2.*a*c*h*j + a*b*i*j - b2*g*k + 2.*a*c*g*k + a*b*h*k - 2.*a2*i*k;
	//y^2
	qy[2]=(-e2*g2 + d*e*g*h - d2*g*i + c*d*g*j + b*e*g*j - 2.*a*e*h*j + a*d*i*j - a*c*j2 -
		   2.*b*d*g*k + 2.*a*e*g*k + a*d*h*k + a*b*j*k - a2*k2 - b2*g*l + 2.*a*c*g*l + a*b*h*l - 2.*a2*i*l)
			- (2.*c*f*g2 - b*f*g*h + a*f*h2 - 2.*a*f*g*i);
	//y
	qy[1]=(d*e*g*j - a*e*j2 - d2*g*k + a*d*j*k - 2.*b*d*g*l + 2.*a*e*g*l + a*d*h*l + a*b*j*l - 2.*a2*k*l)
			-(2.*e*f*g2 - d*f*g*h - b*f*g*j + 2.*a*f*h*j - 2.*a*f*g*k);
	//y^0
	qy[0]=-d2*g*l + a*d*j*l - a2*l2
			- (f2*g2 - d*f*g*j + a*f*j2 - 2.*a*f*g*l);
	if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
		DEBUG_HEADER
				std::cout<<qy[4]<<"*y^4 +("<<qy[3]<<")*y^3+("<<qy[2]<<")*y^2+("<<qy[1]<<")*y+("<<qy[0]<<")==0"<<std::endl;
	}
	//quarticSolver
	auto roots=quarticSolverFull(qy);
	if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
		std::cout<<"roots.size()= "<<roots.size()<<std::endl;
	}

	if (roots.size()==0 ) { // no intersection found
		return ret;
	}
	std::vector<double> ce(0,0.);

	for(size_t i0=0;i0<roots.size();i0++){
		if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
			DEBUG_HEADER
					std::cout<<"y="<<roots[i0]<<std::endl;
		}
		/*
		  Collect[Eliminate[{ a*x^2 + b*x*y+c*y^2+d*x+e*y+f==0,g*x^2+h*x*y+i*y^2+j*x+k*y+l==0},x],y]
		  */
		ce.resize(3);
		ce[0]=a;
		ce[1]=b*roots[i0]+d;
		ce[2]=c*roots[i0]*roots[i0]+e*roots[i0]+f;
		//    DEBUG_HEADER
		//                std::cout<<"("<<ce[0]<<")*x^2 + ("<<ce[1]<<")*x + ("<<ce[2]<<") == 0"<<std::endl;
		if(std::abs(ce[0])<1e-75 && std::abs(ce[1])<1e-75) {
			ce[0]=g;
			ce[1]=h*roots[i0]+j;
			ce[2]=i*roots[i0]*roots[i0]+k*roots[i0]+f;
			//            DEBUG_HEADER
			//            std::cout<<"("<<ce[0]<<")*x^2 + ("<<ce[1]<<")*x + ("<<ce[2]<<") == 0"<<std::endl;

		}
		if(std::abs(ce[0])<1e-75 && std::abs(ce[1])<1e-75) continue;

		if(std::abs(a)>1e-75){
			std::vector<double> ce2(2,0.);
			ce2[0]=ce[1]/ce[0];
			ce2[1]=ce[2]/ce[0];
			//                DEBUG_HEADER
			//                        std::cout<<"x^2 +("<<ce2[0]<<")*x+("<<ce2[1]<<")==0"<<std::endl;
			auto xRoots=quadraticSolver(ce2);
			for(size_t j0=0;j0<xRoots.size();j0++){
				//                DEBUG_HEADER
				//                std::cout<<"x="<<xRoots[j0]<<std::endl;
				RS_Vector vp(xRoots[j0],roots[i0]);
				if(simultaneousQuadraticVerify(m,vp)) ret.push_back(vp);
			}
			continue;
		}
		RS_Vector vp(-ce[2]/ce[1],roots[i0]);
		if(simultaneousQuadraticVerify(m,vp)) ret.push_back(vp);
	}
	if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
		DEBUG_HEADER
				std::cout<<"ret="<<ret<<std::endl;
	}
	return ret;
}

RS_VectorSolutions RS_Math::simultaneousQuadraticSolverMixed(const std::vector<std::vector<double> >& m)
{
	RS_VectorSolutions ret;
	auto p0=& (m[0]);
	auto p1=& (m[1]);
	if(p1->size()==3){
		std::swap(p0,p1);
	}
	if(p1->size()==3) {
		//linear
		std::vector<double> sn(2,0.);
		std::vector<std::vector<double> > ce;
		ce.push_back(m[0]);
		ce.push_back(m[1]);
		ce[0][2]=-ce[0][2];
		ce[1][2]=-ce[1][2];
		if(RS_Math::linearSolver(ce,sn)) ret.push_back(RS_Vector(sn[0],sn[1]));
		return ret;
	}
	//    DEBUG_HEADER
	//    std::cout<<"p0: size="<<p0->size()<<"\n Solve[{("<< p0->at(0)<<")*x + ("<<p0->at(1)<<")*y + ("<<p0->at(2)<<")==0,";
	//    std::cout<<"("<< p1->at(0)<<")*x^2 + ("<<p1->at(1)<<")*x*y + ("<<p1->at(2)<<")*y^2 + ("<<p1->at(3)<<")*x +("<<p1->at(4)<<")*y+("
	//            <<p1->at(5)<<")==0},{x,y}]"<<std::endl;
	const double& a=p0->at(0);
	const double& b=p0->at(1);
	const double& c=p0->at(2);
	const double& d=p1->at(0);
	const double& e=p1->at(1);
	const double& f=p1->at(2);
	const double& g=p1->at(3);
	const double& h=p1->at(4);
	const double& i=p1->at(5);
	/**
	  y (2 b c d-a c e)-a c g+c^2 d = y^2 (a^2 (-f)+a b e-b^2 d)+y (a b g-a^2 h)+a^2 (-i)
	  */
	std::vector<double> ce(3,0.);
	const double& a2=a*a;
	const double& b2=b*b;
	const double& c2=c*c;
	ce[0]= RS_Math::sum({-f*a2, a*b*e, -b2*d});
	ce[1]= RS_Math::sum({a*b*g, -a2*h, -2*b*c*d, a*c*e});
	ce[2]= RS_Math::sum({a*c*g, -c2*d, -a2*i});
	//    DEBUG_HEADER
		std::cout<<"("<<ce[0]<<") y^2 + ("<<ce[1]<<") y + ("<<ce[2]<<")==0"<<std::endl;
	std::vector<double> roots(0, 0.);
	if (std::abs(ce[1])>RS_TOLERANCE15 && std::abs(ce[0]/ce[1])<RS_TOLERANCE15){
		roots.push_back(- ce[2]/ce[1]);
	}else{
		std::vector<double> ce2(2,0.);
		ce2[0]=ce[1]/ce[0];
		ce2[1]=ce[2]/ce[0];
		roots=quadraticSolver(ce2);
	}
	//    for(size_t i=0;i<roots.size();i++){
	//    std::cout<<"x="<<roots.at(i)<<std::endl;
	//    }


	if(roots.size()==0)
		return {};

	for(auto const r: roots)
		ret.push_back({-(b*r + c)/a, r});

	return ret;

}

/** verify a solution for simultaneousQuadratic
  *@m the coefficient matrix
  *@v, a candidate to verify
  *@return true, for a valid solution
  **/
bool RS_Math::simultaneousQuadraticVerify(const std::vector<std::vector<double> >& m, RS_Vector& v)
{
	RS_Vector v0=v;
	auto& a=m[0][0];
	auto& b=m[0][1];
	auto& c=m[0][2];
	auto& d=m[0][3];
	auto& e=m[0][4];
	auto& f=m[0][5];

	auto& g=m[1][0];
	auto& h=m[1][1];
	auto& i=m[1][2];
	auto& j=m[1][3];
	auto& k=m[1][4];
	auto& l=m[1][5];
	/**
	  * tolerance test for bug#3606099
	  * verifying the equations to floating point tolerance by terms
	  */
	double sum0=0., sum1=0.;
	double f00=0.,f01=0.;
	double amax0, amax1;
	for(size_t i0=0; i0<20; ++i0){
		double& x=v.x;
		double& y=v.y;
		double x2=x*x;
		double y2=y*y;
		double const terms0[12]={ a*x2, b*x*y, c*y2, d*x, e*y, f, g*x2, h*x*y, i*y2, j*x, k*y, l};
		amax0=std::abs(terms0[0]), amax1=std::abs(terms0[6]);
		double px=2.*a*x+b*y+d;
		double py=b*x+2.*c*y+e;
		sum0=0.;
		for(int i=0; i<6; i++) {
			if(amax0<std::abs(terms0[i])) amax0=std::abs(terms0[i]);
			sum0 += terms0[i];
		}
		std::vector<std::vector<double>> nrCe;
		nrCe.push_back(std::vector<double>{px, py, sum0});
		px=2.*g*x+h*y+j;
		py=h*x+2.*i*y+k;
		sum1=0.;
		for(int i=6; i<12; i++) {
			if(amax1<std::abs(terms0[i])) amax1=std::abs(terms0[i]);
			sum1 += terms0[i];
		}
		nrCe.push_back(std::vector<double>{px, py, sum1});
		std::vector<double> dn;
		bool ret=linearSolver(nrCe, dn);
		//		DEBUG_HEADER
		//		qDebug()<<"i0="<<i0<<"\tf=("<<sum0<<','<<sum1<<")\tdn=("<<dn[0]<<","<<dn[1]<<")";
		if(!i0){
			f00=sum0;
			f01=sum1;
		}
		if(!ret) break;
		v -= RS_Vector(dn[0], dn[1]);
	}
	if(std::abs(sum0)> std::abs(f00) && std::abs(sum1)>std::abs(f01)){
		v=v0;
		sum0=f00;
		sum1=f01;
	}

	//    DEBUG_HEADER
	//    std::cout<<"verifying: x="<<x<<"\ty="<<y<<std::endl;
	//    std::cout<<"0: maxterm: "<<amax0<<std::endl;
	//    std::cout<<"verifying: std::abs(a*x2 + b*x*y+c*y2+d*x+e*y+f)/maxterm="<<std::abs(sum0)/amax0<<" required to be smaller than "<<sqrt(6.)*sqrt(DBL_EPSILON)<<std::endl;
	//    std::cout<<"1: maxterm: "<<amax1<<std::endl;
	//    std::cout<<"verifying: std::abs(g*x2+h*x*y+i*y2+j*x+k*y+l)/maxterm="<< std::abs(sum1)/amax1<<std::endl;
	const double tols=2.*sqrt(6.)*sqrt(DBL_EPSILON); //experimental tolerances to verify simultaneous quadratic

	return (amax0<=tols || std::abs(sum0)/amax0<tols) &&  (amax1<=tols || std::abs(sum1)/amax1<tols);
}
//EOF
