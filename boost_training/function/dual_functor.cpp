#include <boost/function.hpp>
#include <boost/current_function.hpp>

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

// #1
template <class T>
void foo(T const& f, typename T::result_type* = 0) {
	std::cout << "#1 " << BOOST_CURRENT_FUNCTION << std::endl;
    f(Derived());
    f(10);
}

// #2
template <class T>
void foo(boost::function<void(T const&)> f) {
	std::cout << "#2 " << BOOST_CURRENT_FUNCTION << std::endl;
    // f(Derived()); // error 
    f(10);
}

// #3
void foo(Base const&) {
	std::cout << "#3 " << BOOST_CURRENT_FUNCTION << std::endl;
}

int main() {
    foo(Functor()); // match #1
    foo(boost::function<void(int const&)>(Functor())); // match #2
    foo(Base());    // match #3
    foo(Derived()); // match #3
}

