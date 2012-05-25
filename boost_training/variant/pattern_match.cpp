#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/ref.hpp>

template <class T>
struct ref_wrap:boost::reference_wrapper<T> {
    ref_wrap(T& t):boost::reference_wrapper<T>(t) {}
};

struct A1{};
struct A2{};

typedef boost::variant<ref_wrap<A1>, ref_wrap<A2> > A;

struct B1{};
struct B2{};

typedef boost::variant<ref_wrap<B1>, ref_wrap<B2> > B;

struct DD_AB : boost::static_visitor<> {
    void operator()(A1 const&, B2 const&) const {
        std::cout << "A1-B2" << std::endl;
    }
    void operator()(...) const {
        std::cout << "no match" << std::endl;
    }
};

int main() {
    A1 a1;
    A2 a2;
    B2 b2;

    A av1 = a1;
    A av2 = a2;
    B bv2 = b2;

    boost::apply_visitor(DD_AB(), av1, bv2);
    boost::apply_visitor(DD_AB(), av2, bv2);
}
