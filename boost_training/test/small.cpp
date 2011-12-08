#define BOOST_TEST_MODULE tests
#include <boost/test/included/unit_test.hpp>

struct Hoge {
	void foo() {
		int small;
		(void)small;
	}
};
BOOST_AUTO_TEST_CASE( case1 )
{
	int small = 0;
	(void)small;
}
