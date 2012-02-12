#include <iostream>
#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>

// ref_wraps is a reference wrapper that has non explicit constructor
template <class T>
struct ref_wrap:boost::reference_wrapper<T> {
	ref_wrap(T& t):boost::reference_wrapper<T>(t) {}
};

struct TtA {};
struct TtB : TtA {};
struct TtC : TtA {};
struct TtD : TtB, TtC {};

struct A {
	typedef TtA trait;
};
struct B {
	typedef TtB trait;
};
struct C {
	typedef TtC trait;
};
struct D {
	typedef TtD trait;
};

typedef ref_wrap<A> RrA;
typedef ref_wrap<B> RrB;
typedef ref_wrap<C> RrC;
typedef ref_wrap<D> RrD;

typedef boost::variant<RrA, RrB, RrC, RrD> V;

struct visitor1 : boost::static_visitor<> {
	// visitor1は、クラスCを継承したクラスに対する操作
	template <class T>
	void operator()(ref_wrap<T>,
		typename boost::enable_if<
			typename boost::is_base_of<
				TtC, typename T::trait
			> 
		>::type* = 0) const {
		std::cout << "C family" << std::endl;
	}
	void operator()(...) const {}
};

int main() {
	A a;
	B b;
	C c;
	D d;
	{
		V v = a;
		boost::apply_visitor(visitor1(), v);
	}
	{
		V v = b;
		boost::apply_visitor(visitor1(), v);
	}
	{
		V v = c;
		boost::apply_visitor(visitor1(), v);
	}
	{
		V v = d;
		boost::apply_visitor(visitor1(), v);
	}
}
