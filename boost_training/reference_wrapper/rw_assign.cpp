#include <boost/ref.hpp>
#include <cassert>

int main() {
	int i = 1;
	int j = 2;
	boost::reference_wrapper<int> rw1(i);
	boost::reference_wrapper<int> rw2(j);
	boost::reference_wrapper<int const> crw1(rw1);
	rw1 = rw2;
	rw1.get() = 5;
	assert(i == 1);
	assert(j == 5);
}
