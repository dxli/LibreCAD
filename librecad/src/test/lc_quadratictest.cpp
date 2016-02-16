#include <algorithm>
#include <array>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/math/quaternion.hpp>
#include <QDebug>
#include "lc_quadratictest.h"
#include "lc_quadratic.h"


namespace{
using namespace boost::numeric::ublas;
using namespace boost::math;
using Vector = vector<double>;
using Matrix = matrix<double>;
using Quaternion = quaternion<double>;
constexpr double TEST_TOLERANCE = 1e-6;
}

LC_QuadraticTest::LC_QuadraticTest(QObject *parent) : QObject(parent)
{}

void LC_QuadraticTest::initTestCase()
{
	qDebug()<<"Begin testing: Linear Reduction of quadratic forms";
}

void LC_QuadraticTest::testLinearReduction()
{
	//test cases of lines
	std::vector<std::array<double, 3>> lineData{
		{1, 2, 1}, // 1*x + 2*y + 1 = 0, the array holds coefficients in order
		{2, 3, 2},
		{1, 4, 5},
		{4, 1, 7},
		{3, 1, 3},
		{1, 1, 4},
		{1, 1e4, 1},
		{1, 1e4 + 1, 1},
		{1, 1e4 + 1, 1e4},
		{1e4, 1e4, 1e4+1},
		{1e4+1, 1e4, 1e4},
		{10, 1, 11},
		//		{2, -3, 2},
		//		{1, -4, 5},
		//		{4, -1, 7},
		{4, 0, 7},
		//		{10, -1, 11}
	};
	//form Vector from line data
	// normalized
	auto fl2v = [&lineData](size_t i) -> Vector {
		Vector ret(3);
		std::copy(lineData[i].begin(), lineData[i].end(), ret.begin());
		ret = ret * (1./norm_2(ret));
		return ret;
	};
	//form Vector from lc_quadratic
	auto fq2v = [](LC_Quadratic const& q) -> Vector{
		Vector ret(3);
		ret(0) = q.getLinear()(0);
		ret(1) = q.getLinear()(1);
		ret(2) = q.constTerm();
		ret = ret * (1./norm_2(ret));
		return ret;
	};
	//format linear form
	auto toStr = [&lineData](size_t i){
		return QString("%1*x %2 %3*y %4 %5 = 0")
				.arg(lineData[i][0])
				.arg(lineData[i][1]>=0?'+':'-')
				.arg(fabs(lineData[i][1]))
				.arg(lineData[i][2]>=0?'+':'-')
				.arg(fabs(lineData[i][2])).toStdString();
	};

	double maxErr=0.;
	size_t maxI=0, maxJ=0;

	for (size_t i=0; i + 1 < lineData.size(); i++) {
		Vector const va = fl2v(i);
		for (size_t j=i+1; j < lineData.size(); j++) {
			Vector const vb = fl2v(j);
			Matrix Q = outer_prod(va, vb);
			Q = (trans(Q) + Q) * 0.5;
			auto const sol = LC_Quadratic::linearReduction(Q);
			assert(sol.size()==2);
			Vector va1 = fq2v(sol[0]);
			Vector vb1 = fq2v(sol[1]);
			if (inner_prod(va, va1) < inner_prod(vb, va1))
				swap(va1, vb1);

			auto const diff = norm_inf(va - va1);
			if (diff>maxErr) {
				maxErr=diff;
				maxI=i;
				maxJ=j;
			}



			std::cout<<toStr(i)<<' '<<toStr(j)<<" err = "<<diff<<std::endl;
			//			qDebug()<<"expected: ";
			//				qDebug()<<va(0)<<va(1)<<va(2);
			//				qDebug()<<vb(0)<<vb(1)<<vb(2);
			//			qDebug()<<"found: ";
			//				qDebug()<<va1(0)<<va1(1)<<va1(2);
			//				qDebug()<<vb1(0)<<vb1(1)<<vb1(2);
			//qDebug()<<"diff = "<<diff;
			QVERIFY(norm_inf(va - va1) <= TEST_TOLERANCE);
			//qDebug()<<"testcase "<<i<<j<<'\n';
		}
	}
	std::cout<<"maximum error: "<<maxErr<<'\n'
			<<toStr(maxI)<<' '<<toStr(maxJ)<<std::endl;

}

void LC_QuadraticTest::test()
{
	LC_QuadraticTest testObj;
	QTest::qExec(&testObj);
}

//QTEST_MAIN(LC_QuadraticTest)
//#include "lc_quadratictest.moc"
