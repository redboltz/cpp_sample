#include <boost/current_function.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <cassert>


struct Foo {
    void method() {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
    }
    void method() const {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
    }
};

struct Bar {
    boost::optional<Foo&> getFoo() {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return foo_;
    }
    boost::optional<Foo const&> getFoo() const {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return foo_ ? *foo_ : boost::optional<Foo const&>();
    }
    boost::optional<Foo&> foo_;
};

boost::optional<Foo const&> hoge() {
    boost::optional<Foo&> opref;
    boost::optional<Foo const&> opcref(opref);
    return boost::optional<Foo const&>(opref);
}

int main() {
    boost::optional<Foo&> opref;
    boost::optional<Foo const&> opcref;
    Bar b;
    Foo f;
    Bar const& br = b;
    assert(!br.getFoo());
    assert(!b.getFoo());
    b.foo_ = f;
    br.getFoo()->method();
    b.getFoo()->method();

    opref = f;
    opcref = f;
    opcref = opref;
}

