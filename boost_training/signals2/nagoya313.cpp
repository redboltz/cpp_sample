#include <iostream>
#include <memory>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/trackable.hpp>

struct hoge : boost::signals2::trackable {
  hoge() : p_(new int()) {}

  hoge(hoge &&rhs) : p_(std::move(rhs.p_)) {}

  void foo() {
    std::cout << *p_ << std::endl;
  }

  std::unique_ptr<int> p_;
};

boost::signals2::signal<void ()> signal;

int main() {
  hoge a;
  signal.connect(boost::bind(&hoge::foo, &a));
  hoge b = std::move(a);
  signal();
  return 0;
}
