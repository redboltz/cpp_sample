#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>


template <class T>
struct ref_wrap:boost::reference_wrapper<T> {
	ref_wrap(T& t):boost::reference_wrapper<T>(t) {}
};

struct A {
    void foo() {}
};

struct B {
    void foo() {}
};

typedef ref_wrap<A> ARR;
typedef ref_wrap<B> BRR;

typedef boost::variant<ARR, BRR> V;

// general cases
template <typename T> void some_func(const T &) {
    std::cout << "Primary" << std::endl;
}

// function template overload
template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
void some_func(const boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> &v) {
    std::cout << "Variant" << std::endl;
}

int main() {
	A a;
	B b;

    V v1 = a;
    some_func(a);
    some_func(v1);

}
