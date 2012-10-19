#include <vector>
#include <boost/range/iterator_range.hpp>
#include <iostream>

template <typename T>
void foo(boost::iterator_range<T> const& r) {
    std::cout << r.size() << std::endl;
}

int main() {
    std::vector<int> v1;
    foo(boost::make_iterator_range(v1));
}
