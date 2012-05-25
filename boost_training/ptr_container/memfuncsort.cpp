#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_container.hpp>
#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>

#include <algorithm>
#include <iostream>


struct My {
	My():i_(0) {}
	My(int i):i_(i) {}
	int i_;
};

typedef boost::ptr_vector<My, boost::view_clone_allocator> PV;

void foo(PV const& pv) {
	PV tmp;
	//tmp.front() = pv.front();
	//tmp.push_back(&pv.front());
	My const &mc1 = pv[0];
	PV tmp2 = pv;
	My &mc2 = tmp2[0];

	boost::range::copy(pv, boost::ptr_container::ptr_back_inserter(tmp));
	//pv[0] = My();
	My const &mc3 = tmp[0];
	tmp[0] = My(1);
	My const &mc4 = tmp[0];
	My const &mc5 = pv[0];
}

int main() {
	PV pv;
	My m(2);
	pv.push_back(&m);
	foo(pv);
}
