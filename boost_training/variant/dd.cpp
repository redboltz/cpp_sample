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

struct A {};
struct A1:A {};
struct A2:A {};

typedef ref_wrap<A> ARR;
typedef ref_wrap<A1> A1RR;
typedef ref_wrap<A2> A2RR;

typedef boost::variant<A1RR, A2RR> AV;

struct B {};
struct B1:B {};
struct B2:B {};

typedef ref_wrap<B> BRR;
typedef ref_wrap<B1> B1RR;
typedef ref_wrap<B2> B2RR;

typedef boost::variant<B1RR, B2RR> BV;

struct DD_AB : boost::static_visitor<> {
	void operator()(A1RR const&, B2RR const&) const {}
	template <class T, class U>
	void operator()(T const&, U const&, 
		typename boost::enable_if<
			boost::mpl::and_<
				typename boost::is_base_of<
					A, typename T::type
				>::type, 
				typename boost::is_base_of<
					B, typename U::type
				>::type 
			> 
		>::type* = 0) const {}
	void operator()(...) const {}
};

int main() {
	A1 a1;
	A2 a2;
	B1 b1;
	B2 b2;

	AV av1 = a1;
	AV av2 = a2;
	BV bv2 = b2;

	boost::apply_visitor(DD_AB(), av1, bv2);
	boost::apply_visitor(DD_AB(), av2, bv2);
}
