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
    while (ri != end) {
        if (*ri == 2) {
            // riが適切に進んでいると思う。VC++ではランタイムエラーになるのはなぜ？
#if 1
            s.erase((++ri).base()++); // VC runtime error, g++ OK
#else
            // VC++でもg++でも動くが、riは無効でたまたま実装が次（前）要素を指しているだけでは？
            s.erase(--ri.base()); // VC, g++ OK 
#endif
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
