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

template <typename T, std::size_t numOfMaxObj>
class sss_allocator {
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

	template <typename U>
	struct rebind {
		typedef sss_allocator<U, numOfMaxObj> other;
	};
	sss_allocator() {
		sss_.add_block(buf, sizeof(T), sizeof(T) * numOfMaxObj);
	}
private:
	unsigned char buf_[sizeof(T) * numOfMaxObj];
};

template <std::size_t size>
struct fixed_array_allocator {
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    static char * malloc BOOST_PREVENT_MACRO_SUBSTITUTION(const size_type bytes) {
        std::size_t alignedSize = ((bytes - 1) / sizeof(int) + 1) * sizeof(int);
        char *p = &mem[position];
        std::cout <<"fixed new mem = " << reinterpret_cast<void*>(p) << " req size = " << bytes << std::endl;
        position += alignedSize;
        return p;
    }
    static void free BOOST_PREVENT_MACRO_SUBSTITUTION(char * const block) {
        std::cout << "fixed delete mem = " << reinterpret_cast<void*>(block) << std::endl;
    }
    static char mem[size];
    static std::size_t position;
};

template <std::size_t size> std::size_t 
fixed_array_allocator<size>::position = 0;
template <std::size_t size> char
fixed_array_allocator<size>::mem[size];

struct sss_with_difftype :  boost::simple_segregated_storage<std::size_t> 
{
	typedef std::ptrdiff_t difference_type;
};

struct MyTag {};
	
typedef boost::singleton_pool<MyTag, 16, sss_with_difftype> MySss;


void test() {
    std::cout << "Test" << std::endl;
    std::cout << "sizeof(My) = " << sizeof(My) << std::endl;

	//MySss s;
	//s.malloc();
	
    typedef boost::pool_allocator<My, MySss> alloc_type;

	alloc_type a;
#if 0
    std::cout << "Start" << std::endl;
    {
        boost::shared_ptr<My> p1 = boost::allocate_shared<My>(a, 1, 3, 42);
		std::list<My, alloc_type> l;

        l.push_back(My(1, 2, 3));
#if 0
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));

        l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));
		l.push_back(My(1, 2, 3));

		l.push_back(My(1, 2, 3));
#endif
    }
    {
#if 0
        boost::shared_ptr<My> p1 = boost::allocate_shared<My>(my_alloc(), 1, 3, 42);
        boost::shared_ptr<My> p2 = boost::allocate_shared<My>(my_alloc(), 1, 3, 42);
#endif
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
#endif
}


int main() {
    test();
}
