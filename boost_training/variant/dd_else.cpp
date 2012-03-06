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
    A() {}
    A(A const&) {}
};

struct B {
    B() {}
    B(B const&) {}
};

struct C {
    C() {}
    C(C const&) {}
};


struct X {
    X() {}
    X(X const&) {}
};

struct Y {
    Y() {}
    Y(Y const&) {}
};

struct Z {
    Z() {}
    Z(Z const&) {}
};

typedef ref_wrap<A> ARR;
typedef ref_wrap<B> BRR;
typedef ref_wrap<C> CRR;

typedef ref_wrap<X> XRR;
typedef ref_wrap<Y> YRR;
typedef ref_wrap<Z> ZRR;

typedef boost::variant<ARR, BRR, CRR> VABC;
typedef boost::variant<XRR, YRR, ZRR> VXYZ;

struct DD1 : boost::static_visitor<> {
    void operator()(A const&, X const&) const {}
    void operator()(...) const {}
};

int main() {
    A a;
    VABC v1 = a;
    X x;
    VXYZ v2 = x;
    boost::apply_visitor(DD1(), v1, v2);
}
