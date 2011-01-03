// http://ideone.com/gqMwS
#include <boost/lambda/lambda.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include <iostream>
 
struct My {
    My(int val_):val(val_){}
    int val;
};
 
int main() {
    using boost::assign::list_of;
    using boost::lambda::_1;
    using boost::lambda::_2;
 
    std::vector<My> m = list_of(5)(3)(1)(2)(4);
    std::for_each(m.begin(), m.end(), std::cout << (&_1->* &My::val) << " ");
    std::cout << "\n";
    std::sort(m.begin(), m.end(), (&_1->* &My::val) < (&_2->* &My::val));
    std::for_each(m.begin(), m.end(), std::cout << (&_1->* &My::val) << " ");
    std::cout << "\n";
    std::sort(m.begin(), m.end(), (&_1->* &My::val) > (&_2->* &My::val));
    std::for_each(m.begin(), m.end(), std::cout << (&_1->* &My::val) << " ");
    std::cout << "\n";
}
