#include <boost/ref.hpp>

class My {
public:
  My() {}
  My(int) {}
  My(My const&) {}
};

void foo(My const&) {}
//void foo(boost::reference_wrapper<My const>) {}
void foo(...) {}

int main() {
  My m;
  foo(m);
  foo(boost::cref(m));
}
