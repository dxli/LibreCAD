#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/math/quaternion.hpp>
#include "lc_quadratictest.h"
#include "lc_quadratic.h"

namespace{
using namespace boost::numeric::ublas;
using namespace boost::math;
using Vector = vector<double>;
using Matrix = matrix<double>;
using Quaternion = quaternion<double>;
constexpr double TEST_TOLERANCE = 1e-8;
}

void LC_QuadraticTest::testLinearReduction()
{
	int const N{5};
	std::vector<std::array<double, 3>> lineData{
		{1, 2, 1},
		{2, 3, 2},
		{1, 4, 5},
		{3, 1, 3},
		{1, 1, 4}
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

	for (size_t i=0; i + 1< lineData.size(); i++) {
		Vector const va = fl2v(i);
		for (size_t j=i+1; j < lineData.size(); j++) {
			Vector const vb = fl2v(j);
			Matrix Q = outer_prod(va, vb);
			auto const sol = LC_Quadratic::linearReduction(Q);
			assert(sol.size()==2);
			Vector va1 = fq2v(sol[0]);
			Vector vb1 = fq2v(sol[1]);
			if (inner_prod(va, va1) < inner_prod(vb, va1))
				swap(va1, vb1);

			QVERIFY(norm_inf(va - va1) < TEST_TOLERANCE);
		}
	}

}

//QTEST_MAIN(QuadraticTest)
//#include "lc_quadratictest.moc"
