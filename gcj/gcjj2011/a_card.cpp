#include <list>
#include <boost/assert.hpp>

struct Cont {
    Cont(int begin, int count):begin_(begin), count_(count) {
        BOOST_ASSERT(begin > 0);
        BOOST_ASSERT(count > 0);
    }
    int get(int n) const { 
        BOOST_ASSERT(n < count_);
        return begin_ + n;
    }
    int length() const {
        return count_;
    }
    std::list<Cont> remove(int n, int c) {
        BOOST_ASSERT(c > 0);
        std::list<Cont> ret;
        if (n > 0) ret.push_back(Cont(begin_, n));
        if (n + c < count_) ret.push_back(Cont(get(n + c), count_ - (n + c)));
        return ret;
    }
private:
    int begin_;
    int count_;
};

void block_test() {
    {
        auto b = Cont(1, 1);
        BOOST_ASSERT(b.length() == 1);
        BOOST_ASSERT(b.get(0) == 1);
    }
    {
        auto b = Cont(1, 2);
        BOOST_ASSERT(b.length() == 2);
        BOOST_ASSERT(b.get(0) == 1);
        BOOST_ASSERT(b.get(1) == 2);
        {
            auto r = b.remove(0, 1);
            BOOST_ASSERT(r.size() == 1);
            BOOST_ASSERT(r.front().get(0) == 2);
            BOOST_ASSERT(r.front().length() == 1);
        }
        {
            auto r = b.remove(1, 1);
            BOOST_ASSERT(r.size() == 1);
            BOOST_ASSERT(r.front().get(0) == 1);
            BOOST_ASSERT(r.front().length() == 1);
        }
        {
            auto r = b.remove(0, 2);
            BOOST_ASSERT(r.size() == 0);
        }
    }
    {
        auto b = Cont(1, 3);
        BOOST_ASSERT(b.length() == 3);
        BOOST_ASSERT(b.get(0) == 1);
        BOOST_ASSERT(b.get(1) == 2);
        BOOST_ASSERT(b.get(2) == 3);
        {
            auto r = b.remove(0, 1);
            BOOST_ASSERT(r.size() == 1);
            BOOST_ASSERT(r.front().get(0) == 2);
            BOOST_ASSERT(r.front().length() == 2);
        }
        {
            auto r = b.remove(1, 1);
            BOOST_ASSERT(r.size() == 2);
            BOOST_ASSERT(r.front().get(0) == 1);
            BOOST_ASSERT(r.front().length() == 1);
            BOOST_ASSERT(r.back().get(0) == 3);
            BOOST_ASSERT(r.back().length() == 1);
        }
        {
            auto r = b.remove(2, 1);
            BOOST_ASSERT(r.size() == 1);
            BOOST_ASSERT(r.front().get(0) == 1);
            BOOST_ASSERT(r.front().length() == 2);
        }
        {
            auto r = b.remove(0, 2);
            BOOST_ASSERT(r.size() == 1);
            BOOST_ASSERT(r.front().get(0) == 3);
            BOOST_ASSERT(r.front().length() == 1);
        }
    }
}


struct Deck {
    Deck(int num) {
        BOOST_ASSERT(num > 0);
        blocks.push_back(Conts(1, num - 1));
    }
    void suffle(int n, int c) {
        BOOST_ASSERT(n >= 0);
        BOOST_ASSERT(c >= 0);
        auto b = blocks.begin();
        auto e = blocks.cend();
        while (b != e) {
            auto len = b->length();
            if (n < len) {
                len -= n;
                do {
                    auto ret = remove(n, c);
                    b = blocks.erase(b);
                    blocks.insert(b, ret.begin(), ret.end());
                    
                } while (c < len);
                return;
            }
            n -= len;
            ++b;
        }
    }
    int get(int n) const {
        BOOST_ASSERT(n >= 0);
        auto b = blocks.cbegin();
        auto e = blocks.cend();
        while (b != e) {
            if (n < b->length()) {
                return b->get(n);
            }
            n -= b->length();
            ++b;
        }
    }
    std::list<Cont> blocks;
};

int main() {
    block_test();
}
