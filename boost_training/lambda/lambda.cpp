// http://ideone.com/L7bEb
#include <boost/lambda/lambda.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include <iostream>
 
int main() {
    using boost::assign::list_of;
    using boost::lambda::_1;
    using boost::lambda::_2;
 
    std::vector<int> m = list_of(5)(3)(1)(2)(4);
    std::for_each(m.begin(), m.end(), std::cout << _1 << " ");
    std::cout << "\n";
    std::sort(m.begin(), m.end(), _1 < _2);
    std::for_each(m.begin(), m.end(), std::cout << _1 << " ");
    std::cout << "\n";
    std::sort(m.begin(), m.end(), _1 > _2);
    std::for_each(m.begin(), m.end(), std::cout << _1 << " ");
    std::cout << "\n";
}

