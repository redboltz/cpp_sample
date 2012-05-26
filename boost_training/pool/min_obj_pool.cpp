#include <iostream>
#include <list>

#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/pool/simple_segregated_storage.hpp>

void* operator new(std::size_t s) {
    void* p = malloc(s);
    std::cout <<"global new mem = " << p << " req size = " << s << std::endl;
    return p;
}

void operator delete(void* p) {
    std::cout << "global delete mem = " << p << std::endl;
    free(p);
}

void* operator new[](std::size_t s) {
    void* p = malloc(s);
    std::cout <<"global new[] mem = " << p << " req size = " << s << std::endl;
    return p;
}

void operator delete[](void* p) {
    std::cout << "global delete[] mem = " << p << std::endl;
    free(p);
}

struct My {
    My(int x_, int y_, int color_):x(x_), y(y_), color(color_) {
        std::cout << "My construct:" << this << std::endl; 
    }
    ~My() { 
        std::cout << "My destruct:" << this << std::endl; 
    }
    int x;
    int y;
    int color;
};

void test() {

}

int main() {
    test();
}
