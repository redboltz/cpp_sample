#include <iostream>
#include <memory>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/trackable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

struct hoge {
  hoge() : p_(new int()) {}
  hoge(int i) : p_(new int(i)) {}

  hoge(hoge &&rhs) : p_(std::move(rhs.p_)) {
    std::cout << "Move Ctor called" << std::endl;
  }

  void foo() {
    std::cout << *p_ << std::endl;
  }

  std::unique_ptr<int> p_;
};

typedef boost::signals2::signal<void ()> signal_type;
signal_type signal;

int main() {
  boost::shared_ptr<hoge> a = boost::make_shared<hoge>(1);
  signal.connect(signal_type::slot_type(&hoge::foo, &*a).track(a));
  //boost::shared_ptr<hoge> b = std::move(a);
  a.reset();
  signal();
  return 0;
}
