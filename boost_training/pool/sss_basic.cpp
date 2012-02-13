#include <cassert>
#include <iostream>
#include <boost/pool/simple_segregated_storage.hpp>


struct My {
    My(int x_, int y_, int color_):x(x_), y(y_), color(color_) {
        std::cout << "My construct:" << this << std::endl; 
    }
    ~My() { 
        std::cout << "My destruct:" << this << std::endl; 
    }
    int x;
    int y;
    int color;
};


void test1() {
	std::size_t const UnitSize = sizeof(My);
	std::size_t const BufSize = UnitSize*3;
	char buf[BufSize];

	boost::simple_segregated_storage<std::size_t> sss;
	sss.add_block(buf, BufSize, UnitSize);
	void* mp1 = sss.malloc();
	assert(mp1 == buf);
	void* mp2 = sss.malloc();
	assert(mp2 == buf + UnitSize * 1);
	void* mp3 = sss.malloc();
	assert(mp3 == buf + UnitSize * 2);
	sss.free(mp3);
	void* mp4 = sss.malloc();
	assert(mp4 == buf + UnitSize * 2);
	sss.free(mp2);
	void* mp5 = sss.malloc();
	assert(mp5 == buf + UnitSize * 1);
}

int main() {
    test1();
}
