

struct Block {
    Block(int begin, int count):begin(begin), count(count) {}
    int get(int n) const { 
        assert(n <= count);
        return begin + n;
    }
    int length() const {
        return count + 1;
    }
    Block removeHead(int n) {
        assert(n < count);
        return Block(begin + n, count - n);
    }
    Block removeTail(int n) {
        assert(n < count);
        return Block(begin, count - n);
    }
    std::pair<Block, Block> removeCenter(int n, int c) {
        assert(n + c < count);
        int backStart = get(n + c + 1);
        return std::make_pair(Block(begin, n - 1), Block(backStart, count + 1 - backStart));
    }  
    int begin;
    int count;
};

struct Deck {
    Deck(int num) {
        assert(num > 0);
        blocks.push_back(Blocks(1, num - 1));
    }
    void suffle(int n, int c) {
    }
    int get(int n) const {
        assert(n > 0);
        auto b = blocks.begin();
        auto e = blocks.end();
        while (b != e) {
            if (n <=  b->length()) {
                return b->get(n - 1);
            }
            n -= b->length();
            ++b;
        }
    }
    std::list<Block> blocks;
};
