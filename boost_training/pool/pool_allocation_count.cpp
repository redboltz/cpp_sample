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
	~My() { std::cout << "destruct" << std::endl; }
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

template <std::size_t size>
struct fixed_array_allocator {
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	static char * malloc BOOST_PREVENT_MACRO_SUBSTITUTION(const size_type bytes) {
		std::size_t alignedSize = ((bytes - 1) / sizeof(int) + 1) * 4izeof(int);
		void *p = &mem[position];
		position += alignedSize;
		return p;
	}
	static void free BOOST_PREVENT_MACRO_SUBSTITUTION(char * const block) {
	}
	char mem[size];
	static std::size_t position;
};

template <std::size_t size> size_t fixed_array_allocator::position = 0;

void test2() {
    std::cout << "Test2" << std::endl;
    std::cout << "sizeof(My) = " << sizeof(My) << std::endl;

	boost::pool_allocator<My, boost::default_user_allocator_new_delete, boost::details::pool::default_mutex, 1> a;
	//a.deallocate(a.allocate(100), sizeof(My));
    std::cout << "boost::shared_ptr<My> p = boost::make_shared<My>(1, 3, 42)" << std::endl;
    {
        boost::shared_ptr<My> p1 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p2 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p3 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p4 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p5 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p6 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p7 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p8 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p9 = boost::allocate_shared<My>(a, 1, 3, 42);
        boost::shared_ptr<My> p10 = boost::allocate_shared<My>(a, 1, 3, 42);
    }
	//boost::singleton_pool<boost::pool_allocator_tag, sizeof(My)>::release_memory();
}


int main() {
    //test1();
    test2();
}
