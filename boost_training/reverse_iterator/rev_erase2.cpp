#include <set>
#include <cassert>

int main() {
    typedef std::set<int> intset_t;
    intset_t s;
 
    s.insert(1);
    s.insert(2);
    s.insert(3);
 
    intset_t::reverse_iterator ri = s.rbegin();
    intset_t::reverse_iterator end = s.rend();
    //       b            e
    //   re          rb    
    //     | 1 | 2 | 3 |   
    //
    while (ri != end) {
        if (*ri == 2) {
            //       b            e
            //   re          rb    
            //           ri base   
            //     | 1 | 2 | 3 |   
            //

            //intset_t::iterator tmpIt = --ri.base();
            //s.erase(tmpIt++);
            s.erase((--ri.base())++);
            assert(*ri == 1);
        }
        else {
            ++ri;
        }
    }
    {
        intset_t::iterator i = s.begin();
        assert(*i++ == 1);
        assert(*i++ == 3);
    }
}
