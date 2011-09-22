#include <iostream>
#include <vector>

#include <boost/ref.hpp>
#include <boost/current_function.hpp>

template <class T>
class const_reference_wrapper:public boost::reference_wrapper<T const> {
public:
	explicit const_reference_wrapper(T& t):boost::reference_wrapper<T const>(t) {}
	const_reference_wrapper(reference_wrapper<T> r):boost::reference_wrapper<T const>(r.get()) {}
	const_reference_wrapper(reference_wrapper<T const> r):boost::reference_wrapper<T const>(r.get()) {}
};


struct My {
	void method() {
		std::cout << BOOST_CURRENT_FUNCTION << std::endl;
	}
	void const_method() const {
		std::cout << BOOST_CURRENT_FUNCTION << std::endl;
	}
};

typedef boost::reference_wrapper<My> MyRw;
typedef const_reference_wrapper<My> MyConstRw;

void func(MyRw const& my) {
	my.get().const_method();
	my.get().method();
}

void const_func(MyConstRw my) {
	my.get().const_method();
	// my.get().method(); // NG
}

typedef std::vector<MyRw> MyRwCol;
typedef std::vector<MyConstRw> MyConstRwCol;

void func(MyRwCol const& my) {
	my[0].get().const_method();
	my[0].get().method();
}

void const_func(MyConstRwCol my) {
	my[0].get().const_method();
	// my[0].get().method(); // NG
}

int main() {
	My m1;

	func(boost::ref(m1));
	// func(boost::cref(m1));             // NG
	const_func(boost::ref(m1));        // const_reference_wrapper OK
	const_func(boost::cref(m1));

	std::vector<MyRw> vmrw;	
	// vmrw.push_back(m1);                // NG

	MyRwCol mrc;
	mrc.push_back(boost::ref(m1));
	// mrc.push_back(boost::cref(m1));    // NG
	func(mrc);
	// const_func(mrc);                   // NG

	MyConstRwCol mrcc;
	mrcc.push_back(boost::ref(m1));    // const_reference_wrapper OK
	mrcc.push_back(boost::cref(m1));
	// func(mrcc);                        // NG
	const_func(mrcc);

}
