#include <iostream>
#include <list>

#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/pool/pool_alloc.hpp>
#include <boost/pool/object_pool.hpp>

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
    std::cout << "Test" << std::endl;
    std::cout << "sizeof(My) = " << sizeof(My) << std::endl;

	typedef boost::fast_pool_allocator<My, boost::default_user_allocator_new_delete, boost::details::pool::default_mutex, 1, 0> my_alloc;
	my_alloc a;
    std::cout << "Start" << std::endl;
    {
        boost::shared_ptr<My> p1 = boost::allocate_shared<My>(a, 1, 3, 42);
		std::list<My, my_alloc> l;

        l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));

        l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));

		l.push_back(My(1, 2, 3));
    }
}


int main() {
    test();
}
