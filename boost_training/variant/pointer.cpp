#include <boost/variant.hpp>
#include <boost/variant/static_visitor.hpp>

struct A {
	int mema;
};

struct B:A {
	int memb;
};

class visitor : public boost::static_visitor<int>
{
public:
	int operator()(A* a) const {
		return a->mema;
	}
	int operator()(B* b) const {
		return b->memb;
	}
};

typedef boost::variant<A*, B*> Vari;

int main() {
	A a;
	B b;
	a.mema = 10;
	b.memb = 20;
	Vari v = &a;
	std::cout << boost::apply_visitor(visitor(), v) << std::endl;
	v = &b;
	std::cout << boost::apply_visitor(visitor(), v) << std::endl;
}

