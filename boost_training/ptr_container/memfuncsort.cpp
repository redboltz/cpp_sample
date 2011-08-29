#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_container.hpp>

#include <algorithm>
#include <iostream>

int g;
struct Base {
	Base():memb(g++){}
    int memb;
};

struct Derived:Base {
	Derived():memd(g++){}
	int memd;
};

Base *p1;
Base *p2;

struct Comp:std::binary_function<Base const&, Base const&, bool> {
    bool operator()(Base const& lhs, Base const& rhs) const {
        std::cout << "&lhs = " << &lhs << std::endl;
        std::cout << "&rhs = " << &rhs << std::endl;
//        assert(
//            (&lhs == p1 && &rhs == p2) ||
//            (&lhs == p2 && &rhs == p1));
        return &lhs < &rhs;
    }
};

int main() {
    boost::ptr_vector<Base> v;
    boost::ptr_vector<Base> r;
    v.push_back(p1 = new Base);
    v.push_back(p2 = new Derived);
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;

    std::cout << "member sort" << std::endl;
    v.sort(Comp());
	std::set_intersection(v.begin(), v.end(), v.begin(), v.end(), boost::ptr_container::ptr_back_inserter(r), Comp());
    std::cout << "std::sort" << std::endl;
    std::sort(v.begin(), v.end(), Comp());
}
