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
#ifdef HAS_BOOST
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/math/special_functions/ellint_2.hpp>
#endif

#include <muParser.h>

#include "rs_math.h"
#include "rs_vector.h"
#include "rs_debug.h"

#ifdef EMU_C99
#include "emu_c99.h"
#endif

/**
 * Rounds the given LDOUBLE to the closest int.
 */
int RS_Math::round(LDOUBLE v) {
    return (int) lrint(v);
}

/**
 * Save pow function
 */
LDOUBLE RS_Math::pow(LDOUBLE x, LDOUBLE y) {
    errno = 0;
	LDOUBLE ret = ::pow(x, y);
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
RS_Vector RS_Math::pow(RS_Vector vp, LDOUBLE y) {
    return RS_Vector(pow(vp.x,y),pow(vp.y,y));
}

/**
 * Converts radians to degrees.
 */
LDOUBLE RS_Math::rad2deg(LDOUBLE a) {
	return 180./M_PI*a;
}

/**
 * Converts degrees to radians.
 */
LDOUBLE RS_Math::deg2rad(LDOUBLE a) {
	return M_PI/180.0*a;
}

/**
 * Converts radians to gradians.
 */
LDOUBLE RS_Math::rad2gra(LDOUBLE a) {
	return 200./M_PI*a;
}

LDOUBLE RS_Math::gra2rad(LDOUBLE a) {
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
bool RS_Math::isAngleBetween(LDOUBLE a,
							 LDOUBLE a1, LDOUBLE a2,
                             bool reversed) {

	if (reversed) std::swap(a1,a2);
	if(getAngleDifferenceU(a2, a1 ) < RS_TOLERANCE_ANGLE) return true;
	const LDOUBLE tol=0.5*RS_TOLERANCE_ANGLE;
	const LDOUBLE diff0=correctAngle(a2 -a1) + tol;

	return diff0 >= correctAngle(a - a1) || diff0 >= correctAngle(a2 - a);
}

/**
 * Corrects the given angle to the range of 0-2*Pi.
 */
LDOUBLE RS_Math::correctAngle(LDOUBLE a) {
    return M_PI + remainder(a - M_PI, 2*M_PI);
}

LDOUBLE RS_Math::correctAngleU(LDOUBLE a) {
	return fabsl(remainder(a, 2*M_PI));
}


/**
 * @return The angle that needs to be added to a1 to reach a2.
 *         Always positive and less than 2*pi.
 */
LDOUBLE RS_Math::getAngleDifference(LDOUBLE a1, LDOUBLE a2, bool reversed) {
	if(reversed) std::swap(a1, a2);
	return correctAngle(a2 - a1);
}

LDOUBLE RS_Math::getAngleDifferenceU(LDOUBLE a1, LDOUBLE a2)
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
LDOUBLE RS_Math::makeAngleReadable(LDOUBLE angle, bool readable,
                                  bool* corrected) {

	LDOUBLE ret=correctAngle(angle);

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
bool RS_Math::isAngleReadable(LDOUBLE angle) {
	const LDOUBLE tolerance=0.001;
	//return true for angle in 1st and 4th quadrants
	return fabsl(remainder(angle, 2.*M_PI)) < M_PI_2 - tolerance;
}

/**
 * @param tol Tolerance in rad.
 * @retval true The two angles point in the same direction.
 */
bool RS_Math::isSameDirection(LDOUBLE dir1, LDOUBLE dir2, LDOUBLE tol) {
	return getAngleDifferenceU(dir1, dir2) < tol;
}

/**
 * Evaluates a mathematical expression and returns the result.
 * If an error occured, the given default value 'def' will be returned.
 */
LDOUBLE RS_Math::eval(const QString& expr, LDOUBLE def) {

    bool ok;
	LDOUBLE res = RS_Math::eval(expr, &ok);

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
LDOUBLE RS_Math::eval(const QString& expr, bool* ok) {
    bool okTmp(false);
    if(ok==NULL) ok=&okTmp;
    if (expr.isEmpty()) {
        *ok = false;
        return 0.0;
    }
	LDOUBLE ret(0.);
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
 * Converts a LDOUBLE into a string which is as short as possible
 *
 * @param value The LDOUBLE value
 * @param prec Precision e.g. a precision of 1 would mean that a
 *     value of 2.12030 will be converted to "2.1". 2.000 is always just "2").
 */
QString RS_Math::doubleToString(LDOUBLE value, LDOUBLE prec) {
    if (prec< RS_TOLERANCE ) {
		std::cerr << __func__<<": invalid precision\n";
        return "";
    }

    QString ret;
    QString exaStr;
    int dotPos;
    int num = RS_Math::round(value / prec);

    exaStr = RS_Math::doubleToString(prec, 10);
    dotPos = exaStr.indexOf('.');

    if (dotPos==-1) {
        ret.sprintf("%d", RS_Math::round(num*prec));
    } else {
        int digits = exaStr.length() - dotPos - 1;
        ret = RS_Math::doubleToString(num*prec, digits);
    }

    return ret;
}




/**
 * Converts a double into a string which is as short as possible.
 *
 * @param value The double value
 * @param prec Precision
 */
QString RS_Math::doubleToString(LDOUBLE value, int prec) {
    QString valStr;

	valStr.setNum((double) value, 'f', prec);

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
    QString s;
	LDOUBLE v;

    std::cout << "RS_Math::test: doubleToString:\n";

	v = 0.1L;
	s = RS_Math::doubleToString(v, 0.1L);
    assert(s=="0.1");
	s = RS_Math::doubleToString(v, 0.01L);
    assert(s=="0.1");
	s = RS_Math::doubleToString(v, 0.0L);
    assert(s=="0");

	v = 0.01L;
	s = RS_Math::doubleToString(v, 0.1L);
    assert(s=="0");
	s = RS_Math::doubleToString(v, 0.01L);
    assert(s=="0.01");
	s = RS_Math::doubleToString(v, 0.0L);
    assert(s=="0");

	v = 0.001L;
	s = RS_Math::doubleToString(v, 0.1L);
    assert(s=="0");
	s = RS_Math::doubleToString(v, 0.01L);
    assert(s=="0");
	s = RS_Math::doubleToString(v, 0.001L);
    assert(s=="0.001");
	s = RS_Math::doubleToString(v, 0.0L);
    assert(s=="0");

    std::cout << "RS_Math::test: complete\n";
}



//Equation solvers

// quadratic, cubic, and quartic equation solver
// @ ce[] contains coefficent of the cubic equation:
// @ returns a vector contains real roots
//
// solvers assume arguments are valid, and there's no attempt to verify validity of the argument pointers
//
// @author Dongxu Li <dongxuli2011@gmail.com>

std::vector<LDOUBLE> RS_Math::quadraticSolver(const std::vector<LDOUBLE>& ce)
//quadratic solver for
// x^2 + ce[0] x + ce[1] =0
{
	std::vector<LDOUBLE> ans(0,0.);
    if(ce.size() != 2) return ans;
	LDOUBLE b = 0.25*ce[0]*ce[0];
	LDOUBLE discriminant=b-ce[1];
	if (discriminant >= - RS_TOLERANCE*qMax(fabsl(b), fabsl(ce[1])) ){
		b =  sqrtl(fabsl(discriminant));
		LDOUBLE a=-0.5*ce[0];
		if( b >= RS_TOLERANCE*fabsl(a) ) {
            ans.push_back(a + b);
            ans.push_back(a - b);
        }else
            ans.push_back(a);
    }
    return ans;
}

std::vector<LDOUBLE> RS_Math::cubicSolver(const std::vector<LDOUBLE>& ce)
//cubic equation solver
// x^3 + ce[0] x^2 + ce[1] x + ce[2] = 0
{
//    std::cout<<"x^3 + ("<<ce[0]<<")*x^2+("<<ce[1]<<")*x+("<<ce[2]<<")==0"<<std::endl;
	std::vector<LDOUBLE> ans(0,0.);
    if(ce.size() != 3) return ans;
    // depressed cubic, Tschirnhaus transformation, x= t - b/(3a)
    // t^3 + p t +q =0
	LDOUBLE shift=(1./3)*ce[0];
	LDOUBLE p=ce[1] -shift*ce[0];
	LDOUBLE q=ce[0]*( (2./27)*ce[0]*ce[0]-(1./3)*ce[1])+ce[2];
    //Cardano's method,
    //	t=u+v
    //	u^3 + v^3 + ( 3 uv + p ) (u+v) + q =0
    //	select 3uv + p =0, then,
    //	u^3 + v^3 = -q
    //	u^3 v^3 = - p^3/27
    //	so, u^3 and v^3 are roots of equation,
    //	z^2 + q z - p^3/27 = 0
    //	and u^3,v^3 are,
	//		-q/2 \pm sqrtl(q^2/4 + p^3/27)
    //	discriminant= q^2/4 + p^3/27
    //std::cout<<"p="<<p<<"\tq="<<q<<std::endl;
	LDOUBLE discriminant= (1./27)*p*p*p+(1./4)*q*q;
	if ( fabsl(p)< 1.0e-75) {
        ans.push_back((q>0)?-pow(q,(1./3)):pow(-q,(1./3)));
        ans[0] -= shift;
//        DEBUG_HEADER();
//        std::cout<<"cubic: one root: "<<ans[0]<<std::endl;
        return ans;
    }
    //std::cout<<"discriminant="<<discriminant<<std::endl;
    if(discriminant>0) {
		std::vector<LDOUBLE> ce2(2,0.);
        ce2[0]=q;
        ce2[1]=-1./27*p*p*p;
        auto&& r=quadraticSolver(ce2);
        if ( r.size()==0 ) { //should not happen
			std::cerr<<__FILE__<<" : "<<__func__<<" : line"<<__LINE__<<" :cubicSolver()::Error cubicSolver("<<ce[0]<<' '<<ce[1]<<' '<<ce[2]<<")\n";
        }
		LDOUBLE u,v;
        u= (q<=0) ? pow(r[0], 1./3): -pow(-r[1],1./3);
		//u=(q<=0)?pow(-0.5*q+sqrtl(discriminant),1./3):-pow(0.5*q+sqrtl(discriminant),1./3);
        v=(-1./3)*p/u;
        //std::cout<<"u="<<u<<"\tv="<<v<<std::endl;
        //std::cout<<"u^3="<<u*u*u<<"\tv^3="<<v*v*v<<std::endl;
        ans.push_back(u+v - shift);

//        DEBUG_HEADER();
//        std::cout<<"cubic: one root: "<<ans[0]<<std::endl;
	}else{
		std::complex<LDOUBLE> u(q,0),rt[3];
		u=std::pow(-0.5L*u-sqrt(0.25L*u*u+p*p*p/27),1.L/3);
		rt[0]=u-p/(3.L*u)-shift;
		std::complex<LDOUBLE> w(-0.5L,sqrtl(3.L)/2);
		rt[1]=u*w-p/(3.L*u*w)-shift;
		rt[2]=u/w-p*w/(3.L*u)-shift;
		//        DEBUG_HEADER();
		//        std::cout<<"Roots:\n";
		//        std::cout<<rt[0]<<std::endl;
		//        std::cout<<rt[1]<<std::endl;
		//        std::cout<<rt[2]<<std::endl;
		ans.push_back(rt[0].real());
		ans.push_back(rt[1].real());
		ans.push_back(rt[2].real());
	}
	// newton-raphson
	for(LDOUBLE& x0: ans){
		LDOUBLE dx=0.;
		for(size_t i=0; i<20; ++i){
			LDOUBLE f=( (x0 + ce[0])*x0 + ce[1])*x0 +ce[2];
			LDOUBLE df=(3.*x0+2.*ce[0])*x0 +ce[1];
			if(fabsl(df)>fabsl(f)+RS_TOLERANCE){
				dx=f/df;
				x0 -= dx;
			}else
				break;
		}
	}

    return ans;
}

/** quartic solver
* x^4 + ce[0] x^3 + ce[1] x^2 + ce[2] x + ce[3] = 0
@ce, a vector of size 4 contains the coefficient in order
@return, a vector contains real roots
**/
std::vector<LDOUBLE> RS_Math::quarticSolver(const std::vector<LDOUBLE>& ce)
{
	std::vector<LDOUBLE> ans(0,0.);
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        DEBUG_HEADER();
        std::cout<<"expected array size=4, got "<<ce.size()<<std::endl;
    }
    if(ce.size() != 4) return ans;
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        std::cout<<"x^4+("<<ce[0]<<")*x^3+("<<ce[1]<<")*x^2+("<<ce[2]<<")*x+("<<ce[3]<<")==0"<<std::endl;
    }

    // x^4 + a x^3 + b x^2 +c x + d = 0
    // depressed quartic, x= t - a/4
    // t^4 + ( b - 3/8 a^2 ) t^2 + (c - a b/2 + a^3/8) t + d - a c /4 + a^2 b/16 - 3 a^4/256 =0
    // t^4 + p t^2 + q t + r =0
    // p= b - (3./8)*a*a;
    // q= c - 0.5*a*b+(1./8)*a*a*a;
    // r= d - 0.25*a*c+(1./16)*a*a*b-(3./256)*a^4
	LDOUBLE shift=0.25*ce[0];
	LDOUBLE shift2=shift*shift;
	LDOUBLE a2=ce[0]*ce[0];
	LDOUBLE p= ce[1] - (3./8)*a2;
	LDOUBLE q= ce[2] + ce[0]*((1./8)*a2 - 0.5*ce[1]);
	LDOUBLE r= ce[3] - shift*ce[2] + (ce[1] - 3.*shift2)*shift2;
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        DEBUG_HEADER();
        std::cout<<"x^4+("<<p<<")*x^2+("<<q<<")*x+("<<r<<")==0"<<std::endl;
    }
	if (q*q <= 1.e-4*RS_TOLERANCE*fabsl(p*r)) {// Biquadratic equations
		LDOUBLE discriminant= 0.25*p*p -r;
        if (discriminant < -1.e3*RS_TOLERANCE) {

//            DEBUG_HEADER();
//            std::cout<<"discriminant="<<discriminant<<"\tno root"<<std::endl;
            return ans;
        }
		LDOUBLE t2[2];
		t2[0]=-0.5*p-sqrtl(fabsl(discriminant));
        t2[1]= -p - t2[0];
        //        std::cout<<"t2[0]="<<t2[0]<<std::endl;
        //        std::cout<<"t2[1]="<<t2[1]<<std::endl;
        if ( t2[1] >= 0.) { // two real roots
			ans.push_back(sqrtl(t2[1])-shift);
			ans.push_back(-sqrtl(t2[1])-shift);
        }
        if ( t2[0] >= 0. ) {// four real roots
			ans.push_back(sqrtl(t2[0])-shift);
			ans.push_back(-sqrtl(t2[0])-shift);
        }
//        DEBUG_HEADER();
//        for(int i=0;i<ans.size();i++){
//            std::cout<<"root x: "<<ans[i]<<std::endl;
//        }
        return ans;
    }
	if ( fabsl(r)< 1.0e-75 ) {
		std::vector<LDOUBLE> cubic(3,0.);
        cubic[1]=p;
        cubic[2]=q;
        ans.push_back(0.);
        auto&& r=cubicSolver(cubic);
		std::copy(r.begin(),r.end(), std::back_inserter(ans));
        for(size_t i=0; i<ans.size(); i++) ans[i] -= shift;
        return ans;
    }
    // depressed quartic to two quadratic equations
    // t^4 + p t^2 + q t + r = ( t^2 + u t + v) ( t^2 - u t + w)
    // so,
    // 	p + u^2= w+v
    // 	q/u= w-v
    // 	r= wv
    // so,
    //  (p+u^2)^2 - (q/u)^2 = 4 r
    //  y=u^2,
    //  y^3 + 2 p y^2 + ( p^2 - 4 r) y - q^2 =0
    //
	std::vector<LDOUBLE> cubic(3,0.);
    cubic[0]=2.*p;
    cubic[1]=p*p-4.*r;
    cubic[2]=-q*q;
    auto&& r3= cubicSolver(cubic);
    //std::cout<<"quartic_solver:: real roots from cubic: "<<ret<<std::endl;
    //for(unsigned int i=0; i<ret; i++)
    //   std::cout<<"cubic["<<i<<"]="<<cubic[i]<<" x= "<<croots[i]<<std::endl;
	//newton-raphson
    if (r3.size()==1) { //one real root from cubic
        if (r3[0]< 0.) {//this should not happen
			DEBUG_HEADER();
			qDebug()<<"Quartic Error:: Found one real root for cubic, but negative\n";
            return ans;
        }
		LDOUBLE sqrtz0=sqrtl(r3[0]);
		std::vector<LDOUBLE> ce2(2,0.);
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
    if ( r3[0]> 0. && r3[1] > 0. ) {
		LDOUBLE sqrtz0=sqrtl(r3[0]);
		std::vector<LDOUBLE> ce2(2,0.);
        ce2[0]=	-sqrtz0;
        ce2[1]=0.5*(p+r3[0])+0.5*q/sqrtz0;
        ans=quadraticSolver(ce2);
        ce2[0]=	sqrtz0;
        ce2[1]=0.5*(p+r3[0])-0.5*q/sqrtz0;
        auto&& r1=quadraticSolver(ce2);
		std::copy(r1.begin(),r1.end(),std::back_inserter(ans));
		for(auto& x: ans){
			x -= shift;
		}
    }
	// newton-raphson
	for(LDOUBLE& x0: ans){
		LDOUBLE dx=0.;
		for(size_t i=0; i<20; ++i){
			LDOUBLE f=(( (x0 + ce[0])*x0 + ce[1])*x0 +ce[2])*x0 + ce[3] ;
			LDOUBLE df=((4.*x0+3.*ce[0])*x0 +2.*ce[1])*x0+ce[2];
//			DEBUG_HEADER();
//			qDebug()<<"i="<<i<<"\tx0="<<x0<<"\tf="<<f<<"\tdf="<<df;
			if(fabsl(df)>RS_TOLERANCE2){
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
std::vector<LDOUBLE> RS_Math::quarticSolverFull(const std::vector<LDOUBLE>& ce)
{
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        DEBUG_HEADER();
        std::cout<<ce[4]<<"*y^4+("<<ce[3]<<")*y^3+("<<ce[2]<<"*y^2+("<<ce[1]<<")*y+("<<ce[0]<<")==0"<<std::endl;
    }

	std::vector<LDOUBLE> roots(0,0.);
    if(ce.size()!=5) return roots;
	std::vector<LDOUBLE> ce2(4,0.);

	if ( fabsl(ce[4]) < 1.0e-14) { // this should not happen
		if ( fabsl(ce[3]) < 1.0e-14) { // this should not happen
			if ( fabsl(ce[2]) < 1.0e-14) { // this should not happen
				if( fabsl(ce[1]) > 1.0e-14) {
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
            DEBUG_HEADER();
            std::cout<<"ce2[4]={ "<<ce2[0]<<' '<<ce2[1]<<' '<<ce2[2]<<' '<<ce2[3]<<" }\n";
        }
		if(fabsl(ce2[3])<= RS_TOLERANCE15) {
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

bool RS_Math::linearSolver(const std::vector<std::vector<LDOUBLE> >& mt, std::vector<LDOUBLE>& sn){
    //verify the matrix size
	size_t mSize(mt.size()); //rows
	size_t aSize(mSize+1); //columns of augmented matrix
	if(std::any_of(mt.begin(), mt.end(), [&aSize](const std::vector<LDOUBLE>& v)->bool{
				   return v.size() != aSize;
}))
		return false;
    sn.resize(mSize);//to hold the solution
    //#ifdef	HAS_BOOST
#if false
	boost::numeric::ublas::matrix<LDOUBLE> bm (mSize, mSize);
	boost::numeric::ublas::vector<LDOUBLE> bs(mSize);

    for(int i=0;i<mSize;i++) {
        for(int j=0;j<mSize;j++) {
            bm(i,j)=mt[i][j];
        }
        bs(i)=mt[i][mSize];
    }
    //solve the linear equation set by LU decomposition in boost ublas

	if ( boost::numeric::ublas::lu_factorize<boost::numeric::ublas::matrix<LDOUBLE> >(bm) ) {
		std::cout<<__FILE__<<" : "<<__func__<<" : line "<<__LINE__<<std::endl;
        std::cout<<" linear solver failed"<<std::endl;
        //        RS_DEBUG->print(RS_Debug::D_WARNING, "linear solver failed");
        return false;
    }

	boost::numeric::ublas:: triangular_matrix<LDOUBLE, boost::numeric::ublas::unit_lower>
			lm = boost::numeric::ublas::triangular_adaptor< boost::numeric::ublas::matrix<LDOUBLE>,  boost::numeric::ublas::unit_lower>(bm);
	boost::numeric::ublas:: triangular_matrix<LDOUBLE,  boost::numeric::ublas::upper>
			um =  boost::numeric::ublas::triangular_adaptor< boost::numeric::ublas::matrix<LDOUBLE>,  boost::numeric::ublas::upper>(bm);
    ;
    boost::numeric::ublas::inplace_solve(lm,bs, boost::numeric::ublas::lower_tag());
    boost::numeric::ublas::inplace_solve(um,bs, boost::numeric::ublas::upper_tag());
    for(int i=0;i<mSize;i++){
        sn[i]=bs(i);
    }
    //    std::cout<<"dn="<<dn<<std::endl;
    //    data.center.set(-0.5*dn(1)/dn(0),-0.5*dn(3)/dn(2)); // center
	//    LDOUBLE d(1.+0.25*(dn(1)*dn(1)/dn(0)+dn(3)*dn(3)/dn(2)));
	//    if(fabsl(dn(0))<RS_TOLERANCE2
	//            ||fabsl(dn(2))<RS_TOLERANCE2
    //            ||d/dn(0)<RS_TOLERANCE2
    //            ||d/dn(2)<RS_TOLERANCE2
    //            ) {
    //        //ellipse not defined
    //        return false;
    //    }
	//    d=sqrtl(d/dn(0));
    //    data.majorP.set(d,0.);
	//    data.ratio=sqrtl(dn(0)/dn(2));
#else
    // solve the linear equation by Gauss-Jordan elimination
	std::vector<std::vector<LDOUBLE> > mt0(mt); //copy the matrix;
	for(size_t i=0;i<mSize;++i){
		size_t imax(i);
		LDOUBLE cmax(fabsl(mt0[i][i]));
		for(size_t j=i+1;j<mSize;++j) {
			if(fabsl(mt0[j][i]) > cmax ) {
                imax=j;
				cmax=fabsl(mt0[j][i]);
            }
        }
        if(cmax<RS_TOLERANCE2) return false; //singular matrix
        if(imax != i) {//move the line with largest absolute value at column i to row i, to avoid division by zero
            std::swap(mt0[i],mt0[imax]);
		}
		for(size_t k=i+1;k<=mSize;++k) { //normalize the i-th row
            mt0[i][k] /= mt0[i][i];
        }
		mt0[i][i]=1.;
		for(size_t j=0;j<mSize;++j) {//Gauss-Jordan
            if(j != i ) {
				LDOUBLE& a = mt0[j][i];
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

/**
 * wrapper of elliptic integral of the second type, Legendre form
 * @param k the elliptic modulus or eccentricity
 * @param phi elliptic angle, must be within range of [0, M_PI]
 *
 * @author: Dongxu Li
 */
LDOUBLE RS_Math::ellipticIntegral_2(const LDOUBLE& k, const LDOUBLE& phi)
{
	LDOUBLE a= remainder(phi-M_PI_2,M_PI);
    if(a>0.) {
		return boost::math::ellint_2<LDOUBLE,LDOUBLE>(k,a);
    } else {
		return - boost::math::ellint_2<LDOUBLE,LDOUBLE>(k,fabsl(a));
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
RS_VectorSolutions RS_Math::simultaneousQuadraticSolver(const std::vector<LDOUBLE>& m)
{
    RS_VectorSolutions ret(0);
    if(m.size() != 8 ) return ret; // valid m should contain exact 8 elements
	std::vector< LDOUBLE> c1(0,0.);
	std::vector< std::vector<LDOUBLE> > m1(0,c1);
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
RS_VectorSolutions RS_Math::simultaneousQuadraticSolverFull(const std::vector<std::vector<LDOUBLE> >& m)
{
    RS_VectorSolutions ret;
    if(m.size()!=2)  return ret;
    if( m[0].size() ==3 || m[1].size()==3 ){
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
	LDOUBLE a2=a*a;
	LDOUBLE b2=b*b;
	LDOUBLE c2=c*c;
	LDOUBLE d2=d*d;
	LDOUBLE e2=e*e;
	LDOUBLE f2=f*f;

	LDOUBLE g2=g*g;
	LDOUBLE  h2=h*h;
	LDOUBLE  i2=i*i;
	LDOUBLE  j2=j*j;
	LDOUBLE  k2=k*k;
	LDOUBLE  l2=l*l;
	std::vector<LDOUBLE> qy(5,0.);
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
            - ( f2*g2 - d*f*g*j + a*f*j2 - 2.*a*f*g*l);
	if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        DEBUG_HEADER();
        std::cout<<qy[4]<<"*y^4 +("<<qy[3]<<")*y^3+("<<qy[2]<<")*y^2+("<<qy[1]<<")*y+("<<qy[0]<<")==0"<<std::endl;
	}
    //quarticSolver
    auto&& roots=quarticSolverFull(qy);
    if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
        std::cout<<"roots.size()= "<<roots.size()<<std::endl;
    }

    if (roots.size()==0 ) { // no intersection found
        return ret;
    }
	std::vector<LDOUBLE> ce(0,0.);

    for(size_t i0=0;i0<roots.size();i0++){
        if(RS_DEBUG->getLevel()>=RS_Debug::D_INFORMATIONAL){
            DEBUG_HEADER();
            std::cout<<"y="<<roots[i0]<<std::endl;
        }
        /*
          Collect[Eliminate[{ a*x^2 + b*x*y+c*y^2+d*x+e*y+f==0,g*x^2+h*x*y+i*y^2+j*x+k*y+l==0},x],y]
          */
        ce.resize(3);
        ce[0]=a;
        ce[1]=b*roots[i0]+d;
        ce[2]=c*roots[i0]*roots[i0]+e*roots[i0]+f;
//    DEBUG_HEADER();
//                std::cout<<"("<<ce[0]<<")*x^2 + ("<<ce[1]<<")*x + ("<<ce[2]<<") == 0"<<std::endl;
		if(fabsl(ce[0])<1e-75 && fabsl(ce[1])<1e-75) {
            ce[0]=g;
            ce[1]=h*roots[i0]+j;
            ce[2]=i*roots[i0]*roots[i0]+k*roots[i0]+f;
//            DEBUG_HEADER();
//            std::cout<<"("<<ce[0]<<")*x^2 + ("<<ce[1]<<")*x + ("<<ce[2]<<") == 0"<<std::endl;

        }
		if(fabsl(ce[0])<1e-75 && fabsl(ce[1])<1e-75) continue;

		if(fabsl(a)>1e-75){
			std::vector<LDOUBLE> ce2(2,0.);
            ce2[0]=ce[1]/ce[0];
            ce2[1]=ce[2]/ce[0];
//                DEBUG_HEADER();
//                        std::cout<<"x^2 +("<<ce2[0]<<")*x+("<<ce2[1]<<")==0"<<std::endl;
            auto&& xRoots=quadraticSolver(ce2);
            for(size_t j0=0;j0<xRoots.size();j0++){
//                DEBUG_HEADER();
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
        DEBUG_HEADER();
        std::cout<<"ret="<<ret<<std::endl;
	}
    return ret;
}

RS_VectorSolutions RS_Math::simultaneousQuadraticSolverMixed(const std::vector<std::vector<LDOUBLE> >& m)
{
    RS_VectorSolutions ret;
    auto p0=& (m[0]);
    auto p1=& (m[1]);
    if(p1->size()==3){
        std::swap(p0,p1);
    }
    if(p1->size()==3) {
            //linear
			std::vector<LDOUBLE> sn(2,0.);
			std::vector<std::vector<LDOUBLE> > ce;
			ce.push_back(m[0]);
			ce.push_back(m[1]);
            ce[0][2]=-ce[0][2];
            ce[1][2]=-ce[1][2];
            if( RS_Math::linearSolver(ce,sn)) ret.push_back(RS_Vector(sn[0],sn[1]));
            return ret;
    }
//    DEBUG_HEADER();
//    std::cout<<"p0: size="<<p0->size()<<"\n Solve[{("<< p0->at(0)<<")*x + ("<<p0->at(1)<<")*y + ("<<p0->at(2)<<")==0,";
//    std::cout<<"("<< p1->at(0)<<")*x^2 + ("<<p1->at(1)<<")*x*y + ("<<p1->at(2)<<")*y^2 + ("<<p1->at(3)<<")*x +("<<p1->at(4)<<")*y+("
//            <<p1->at(5)<<")==0},{x,y}]"<<std::endl;
	const LDOUBLE& a=p0->at(0);
	const LDOUBLE& b=p0->at(1);
	const LDOUBLE& c=p0->at(2);
	const LDOUBLE& d=p1->at(0);
	const LDOUBLE& e=p1->at(1);
	const LDOUBLE& f=p1->at(2);
	const LDOUBLE& g=p1->at(3);
	const LDOUBLE& h=p1->at(4);
	const LDOUBLE& i=p1->at(5);
    /**
      y (2 b c d-a c e)-a c g+c^2 d = y^2 (a^2 (-f)+a b e-b^2 d)+y (a b g-a^2 h)+a^2 (-i)
      */
	std::vector<LDOUBLE> ce(3,0.);
	const LDOUBLE&& a2=a*a;
	const LDOUBLE&& b2=b*b;
	const LDOUBLE&& c2=c*c;
    ce[0]= -f*a2+a*b*e-b2*d;
    ce[1]=a*b*g-a2*h- (2*b*c*d-a*c*e);
    ce[2]=a*c*g-c2*d-a2*i;
//    DEBUG_HEADER();
//    std::cout<<"("<<ce[0]<<") y^2 + ("<<ce[1]<<") y + ("<<ce[2]<<")==0"<<std::endl;
	std::vector<LDOUBLE> roots(0,0.);
	if( fabsl(ce[1])>RS_TOLERANCE15 && fabsl(ce[0]/ce[1])<RS_TOLERANCE15){
        roots.push_back( - ce[2]/ce[1]);
    }else{
		std::vector<LDOUBLE> ce2(2,0.);
        ce2[0]=ce[1]/ce[0];
        ce2[1]=ce[2]/ce[0];
        roots=quadraticSolver(ce2);
    }
//    for(size_t i=0;i<roots.size();i++){
//    std::cout<<"x="<<roots.at(i)<<std::endl;
//    }


    if(roots.size()==0)  {
        return RS_VectorSolutions();
    }
    for(size_t i=0;i<roots.size();i++){
        ret.push_back(RS_Vector(-(b*roots.at(i)+c)/a,roots.at(i)));
//        std::cout<<ret.at(ret.size()-1).x<<", "<<ret.at(ret.size()-1).y<<std::endl;
    }

    return ret;

}

/** verify a solution for simultaneousQuadratic
  *@m the coefficient matrix
  *@v, a candidate to verify
  *@return true, for a valid solution
  **/
bool RS_Math::simultaneousQuadraticVerify(const std::vector<std::vector<LDOUBLE> >& m, RS_Vector& v)
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
	LDOUBLE sum0=0., sum1=0.;
	LDOUBLE f00,f01;
	LDOUBLE amax0, amax1;
	for(size_t i0=0; i0<20; ++i0){
		LDOUBLE& x=v.x;
		LDOUBLE& y=v.y;
		LDOUBLE x2=x*x;
		LDOUBLE y2=y*y;
		LDOUBLE const terms0[12]={ a*x2, b*x*y, c*y2, d*x, e*y, f, g*x2, h*x*y, i*y2, j*x, k*y, l};
		amax0=fabsl(terms0[0]), amax1=fabsl(terms0[6]);
		LDOUBLE px=2.*a*x+b*y+d;
		LDOUBLE py=b*x+2.*c*y+e;
		sum0=0.;
		for(int i=0; i<6; i++) {
			if(amax0<fabsl(terms0[i])) amax0=fabsl(terms0[i]);
			sum0 += terms0[i];
		}
		std::vector<std::vector<LDOUBLE>> nrCe;
		nrCe.push_back(std::vector<LDOUBLE>{px, py, sum0});
		px=2.*g*x+h*y+j;
		py=h*x+2.*i*y+k;
		sum1=0.;
		for(int i=6; i<12; i++) {
			if(amax1<fabsl(terms0[i])) amax1=fabsl(terms0[i]);
			sum1 += terms0[i];
		}
		nrCe.push_back(std::vector<LDOUBLE>{px, py, sum1});
		std::vector<LDOUBLE> dn;
		bool ret=linearSolver(nrCe, dn);
//		DEBUG_HEADER();
//		qDebug()<<"i0="<<i0<<"\tf=("<<sum0<<','<<sum1<<")\tdn=("<<dn[0]<<","<<dn[1]<<")";
		if(!i0){
			f00=sum0;
			f01=sum1;
		}
		if(!ret) break;
		v -= RS_Vector(dn[0], dn[1]);
	}
	if( fabsl(sum0)> fabsl(f00) && fabsl(sum1)>fabsl(f01)){
		v=v0;
		sum0=f00;
		sum1=f01;
	}

//    DEBUG_HEADER();
//    std::cout<<"verifying: x="<<x<<"\ty="<<y<<std::endl;
//    std::cout<<"0: maxterm: "<<amax0<<std::endl;
//    std::cout<<"verifying: fabsl(a*x2 + b*x*y+c*y2+d*x+e*y+f)/maxterm="<<fabsl(sum0)/amax0<<" required to be smaller than "<<sqrtl(6.)*sqrtl(DBL_EPSILON)<<std::endl;
//    std::cout<<"1: maxterm: "<<amax1<<std::endl;
//    std::cout<<"verifying: fabsl(g*x2+h*x*y+i*y2+j*x+k*y+l)/maxterm="<< fabsl(sum1)/amax1<<std::endl;
	const LDOUBLE tols=2.*sqrtl(6.)*sqrtl(DBL_EPSILON); //experimental tolerances to verify simultaneous quadratic

	return (amax0<=tols || fabsl(sum0)/amax0<tols) &&  (amax1<=tols || fabsl(sum1)/amax1<tols);
}
//EOF
