#include <set>
#include <cassert>
#include <boost/config.hpp>

int main() {
    typedef std::set<int> intset_t;
    intset_t s;
 
    s.insert(1);
    s.insert(2);
    s.insert(3);
 
    intset_t::reverse_iterator ri = s.rbegin();
    intset_t::reverse_iterator end = s.rend();
    while (ri != end) {
        if (*ri == 2) {
#if defined(BOOST_MSVC)
            intset_t::iterator tmpIt = s.erase(--ri.base()); // VC
			ri = intset_t::reverse_iterator(tmpIt);
			assert(*ri == 1);
#else  // BOOST_MSVC
            s.erase((++ri).base()); // VC runtime error, g++ OK
#endif // BOOST_MSVC
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
