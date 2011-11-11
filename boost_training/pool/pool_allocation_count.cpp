#include <iostream>

#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>

void* operator new(std::size_t s) {
    void* p = malloc(s);
    std::cout <<"new mem = " << p << " req size = " << s << std::endl;
    return p;
}

void operator delete(void* p) {
    std::cout << "delete mem = " << p << std::endl;
    free(p);
}

void* operator new[](std::size_t s) {
    void* p = malloc(s);
    std::cout <<"new[] mem = " << p << " req size = " << s << std::endl;
    return p;
}

void operator delete[](void* p) {
    std::cout << "delete[] mem = " << p << std::endl;
    free(p);
}

struct My {
    My(int x_, int y_, int color_):x(x_), y(y_), color(color_) {}
    int x;
    int y;
    int color;
};

void test1() {
    std::cout << "Test1" << std::endl;
    std::cout << "boost::shared_ptr<My> p(new My(1, 3, 42))" << std::endl;
    {
        boost::shared_ptr<My> p(new My(1, 3, 42));
    }
    std::cout << "boost::shared_ptr<My> p = boost::make_shared<My>(1, 3, 42)" << std::endl;
    {
        boost::shared_ptr<My> p = boost::make_shared<My>(1, 3, 42);
    }
}

void test2() {
    std::cout << "Test2" << std::endl;
    boost::pool_allocator<My> a;
    a.allocate(100);

    std::cout << "boost::shared_ptr<My> p = boost::make_shared<My>(1, 3, 42)" << std::endl;
    {
        boost::shared_ptr<My> p1 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p2 = boost::allocate_shared<My>(a, 1, 3, 42);
    }
}


int main() {
    test1();
    test2();
}
