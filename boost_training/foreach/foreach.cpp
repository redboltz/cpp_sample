// http://ideone.com/ddmLN
#include <boost/foreach.hpp>
#include <iostream>
 
int main() {
    int a[] = { 1, 2, 3, 4, 5 };
    BOOST_FOREACH(int i, a) {
        std::cout << i << std::endl;
    }
}
