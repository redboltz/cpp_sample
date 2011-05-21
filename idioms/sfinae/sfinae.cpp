#include <boost/type_traits.hpp>
#include <boost/utility.hpp>

class Foo {};

template <class T>
void test1(T t, typename boost::enable_if<boost::is_class<T> >::type* = 0) {
}

template <class T>
typename boost::enable_if<boost::is_class<T> >::type
test2(T t) {
}

template <class T, class Enable = void>
struct test3;

template <class T>
struct test3<T, typename boost::enable_if<boost::is_class<T> >::type> {
};

struct test4 {
	template <class T>
	test4(T t, typename boost::enable_if<boost::is_class<T> >::type* = 0) {}
	//template <class T>
	//operator T(typename boost::enable_if<boost::is_class<T> >::type* = 0) { return T(); }
};

struct converted {
	template <class T>
	converted(test4 const&, 
};

int main() {
	Foo f;
	test1(f);
	//test1(1);
	test2(f);
	//test2(1);
	test3<Foo> t3_1;
	//test3<int> t3_2;
	test4 t4_1(f);
	//test4 t4_2(1);
	converted c = t4_1;
	int i = t4_1;
}
