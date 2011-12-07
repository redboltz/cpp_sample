#include <boost/function.hpp>
#include <boost/current_function.hpp>
//#include <boost/enable_if.hpp>

struct Base {};
struct Derived:Base {};

struct Functor {
	typedef void result_type;
	void operator()(Base const&) const {
		std::cout << BOOST_CURRENT_FUNCTION << std::endl;
	}
	void operator()(int) const {
		std::cout << BOOST_CURRENT_FUNCTION << std::endl;
	}
};

template <class T>
void foo(T const& f, typename T::result_type* = 0) {
	std::cout << BOOST_CURRENT_FUNCTION << std::endl;
	f(Derived());
	f(10);
}

template <class T>
void foo(boost::function<void(T const&)> f) {
	std::cout << BOOST_CURRENT_FUNCTION << std::endl;
	// f(Derived()); // error 
	f(10);
}

void foo(Base const&) {
	std::cout << BOOST_CURRENT_FUNCTION << std::endl;
}

int main() {
	foo(Functor());
	foo(boost::function<void(int const&)>(Functor()));
	foo(Base());
	foo(Derived());
}

