#include <set>
#include <cassert>

int main() {
    typedef std::set<int> intset_t;
    intset_t s;
 
    s.insert(1);
    s.insert(2);
    s.insert(3);
 
    intset_t::iterator i = s.begin();
    intset_t::iterator end = s.end();
    while (i != end) {
        if (*i == 2) {
            s.erase(i++);
        }
        else {
            ++i;
        }
    }
    {
        intset_t::iterator i = s.begin();
        assert(*i++ == 1);
        assert(*i++ == 3);
    }
}
