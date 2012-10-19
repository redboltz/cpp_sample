#include <boost/range/any_range.hpp>

typedef boost::any_range<int, boost::forward_traversal_tag, int, std::ptrdiff_t> test_range;

int main() {
    test_range r;
}
