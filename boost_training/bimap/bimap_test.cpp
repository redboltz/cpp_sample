#include <string>
#include <iostream>

#include <boost/bimap/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

namespace bm = boost::bimaps;

struct T1 {}; // Area Handle
struct T2 {}; // Area

int main() {
    typedef bm::bimap<
        bm::tagged< std::string, T1>,
        bm::multiset_of< bm::tagged < int , T2 > >
    > MyMap;
    
    MyMap mm;
    
    mm.insert(MyMap::value_type("H1", 1));
    mm.insert(MyMap::value_type("H2", 2));
    mm.insert(MyMap::value_type("H3", 2));

    std::cout << "T1-Key" << std::endl;
    {
        MyMap::map_by<T1>::const_iterator it = mm.by<T1>().find("H1");
        if (it != mm.by<T1>().end()) {
            std::cout << "T1: " << it->get<T1>() << std::endl
                      << "T2: " << it->get<T2>() << std::endl;
        }
    }
    {
        MyMap::map_by<T1>::const_iterator it = mm.by<T1>().find("H2");
        if (it != mm.by<T1>().end()) {
            std::cout << "T1: " << it->get<T1>() << std::endl
                      << "T2: " << it->get<T2>() << std::endl;
        }
    }
    {
        MyMap::map_by<T1>::const_iterator it = mm.by<T1>().find("H3");
        if (it != mm.by<T1>().end()) {
            std::cout << "T1: " << it->get<T1>() << std::endl
                      << "T2: " << it->get<T2>() << std::endl;
        }
    }
    std::cout << "T2-Key" << std::endl;
    {
        std::pair<
            MyMap::map_by<T2>::const_iterator,
            MyMap::map_by<T2>::const_iterator> pair = mm.by<T2>().equal_range(1);
        for (; pair.first != pair.second; ++pair.first) {
            std::cout << "T1: " << pair.first->get<T1>() << std::endl
                      << "T2: " << pair.first->get<T2>() << std::endl;
        }
    }
    {
        std::pair<
            MyMap::map_by<T2>::const_iterator,
            MyMap::map_by<T2>::const_iterator> pair = mm.by<T2>().equal_range(2);
        for (; pair.first != pair.second; ++pair.first) {
            std::cout << "T1: " << pair.first->get<T1>() << std::endl
                      << "T2: " << pair.first->get<T2>() << std::endl;
        }
    }
}
