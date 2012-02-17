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
	My(My const& other):x(other.x), y(other.y), color(other.color) {
        std::cout << "My copy construct:" << this << std::endl; 
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
    typedef T value_type;
    typedef value_type * pointer;
    typedef const value_type * const_pointer;
    typedef value_type & reference;
    typedef const value_type & const_reference;

	template <typename U>
	struct rebind {
		typedef sss_allocator<U, numOfMaxObj> other;
	};
	sss_allocator() {
		sss_.add_block(buf_, sizeof(T) * numOfMaxObj, sizeof(T));
	}
    template <typename U>
    sss_allocator(const sss_allocator<U, numOfMaxObj> &) {
		sss_.add_block(buf_, sizeof(U) * numOfMaxObj, sizeof(U));
    }
    static pointer address(reference r) {
      return &r;
    }
    static const_pointer address(const_reference r) { 
		return &r;
	}
    static size_type max_size() { 
		return sizeof(T) * numOfMaxObj;
	}
    void construct(const pointer ptr, const value_type & t) { 
		new (ptr) T(t);
	}
    void destroy(const pointer ptr) { 
		//! Destroy ptr using destructor.
		ptr->~T();
		(void) ptr; // Avoid unused variable warning.
    }

    bool operator==(const sss_allocator &) const {
		return true; 
	}
    bool operator!=(const sss_allocator &) const {
		return false; 
	}

    static pointer allocate(const size_type n) {
		pointer p = static_cast<pointer>(sss_.malloc());
		std::cout << "allocate size = " << n << ", addr = " << p << std::endl;
		if (p == 0)
			boost::throw_exception(std::bad_alloc());
		return p;
	}
	static pointer allocate(const size_type n, const void * const) { 
		//! Allocate memory .
		return allocate(n);
    }
	static void deallocate(const pointer p, const size_type n) {
		std::cout << "deallocate size = " << n << ", addr = " << p << std::endl;
		sss_.free(p);
	}
private:
	static boost::simple_segregated_storage<std::size_t> sss_;	
	static unsigned char buf_[sizeof(T) * numOfMaxObj];
};

template <typename T, std::size_t numOfMaxObj>
boost::simple_segregated_storage<std::size_t> 
sss_allocator<T, numOfMaxObj>::sss_;
template <typename T, std::size_t numOfMaxObj>
unsigned char 
sss_allocator<T, numOfMaxObj>::buf_[sizeof(T) * numOfMaxObj];

struct MyTag {};

void test() {
    std::cout << "Test" << std::endl;
    std::cout << "sizeof(My) = " << sizeof(My) << std::endl;

	//MySss s;
	//s.malloc();
	
    typedef sss_allocator<My, 100> alloc_type;

	alloc_type a;
    std::cout << "Start" << std::endl;
    {
		boost::shared_ptr<My> p1 = boost::allocate_shared<My>(a, 1, 3, 42);
#if 0
		std::list<My, alloc_type> l;

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
#endif
    }

#if 0
    {
        boost::shared_ptr<My> p1 = boost::allocate_shared<My>(my_alloc(), 1, 3, 42);
        boost::shared_ptr<My> p2 = boost::allocate_shared<My>(my_alloc(), 1, 3, 42);
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
#if 0
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