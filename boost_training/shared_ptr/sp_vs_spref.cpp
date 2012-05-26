#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

struct A {};

struct B:A {};

void fr(boost::shared_ptr<A>&) {}
void fcr(boost::shared_ptr<A> const&) {}
void fv(boost::shared_ptr<A>) {}

void fr(A*&) {}
void fcr(A* const&) {}
void fv(A*) {}

int main() {
    boost::shared_ptr<B> spb = boost::make_shared<B>();
    // fr(spb); // error as expected
    fcr(spb); // OK ? 
    fv(spb);

    B b;
    // fr(&b);
    fcr(&b);
    fv(&b);

	int i = 1;
	double * const & d = &i;
	double * pd = &i;
	double const& rd = i;
}
