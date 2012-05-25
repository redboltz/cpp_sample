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

struct My2 {
    My2() {
        std::cout << "My2 construct:" << this << std::endl; 
    }
    ~My2() { 
        std::cout << "My2 destruct:" << this << std::endl; 
    }
    int x[10];
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
	assert(mp1 == buf + UnitSize * 0);
	void* mp2 = sss.malloc();
	assert(mp2 == buf + UnitSize * 1);
	void* mp3 = sss.malloc();
	assert(mp3 == buf + UnitSize * 2);
	assert(sss.empty());
	sss.free(mp3);
	void* mp4 = sss.malloc();
	assert(mp4 == buf + UnitSize * 2);
	sss.free(mp2);
	void* mp5 = sss.malloc();
	assert(mp5 == buf + UnitSize * 1);
	assert(sss.empty());
	char buf2[BufSize];
	sss.add_block(buf2, BufSize, UnitSize);
	void* mp6 = sss.malloc();
	assert(mp6 == buf2 + UnitSize * 0);
	void* mp7 = sss.malloc();
	assert(mp7 == buf2 + UnitSize * 1);
	void* mp8 = sss.malloc();
	assert(mp8 == buf2 + UnitSize * 2);
	assert(sss.empty());
	sss.free(mp1);
	sss.free(mp2);
	sss.free(mp7);
	sss.free(mp6);
	void* mp9 = sss.malloc();
	assert(mp9 == mp6);
	void* mpa = sss.malloc();
	assert(mpa == mp7);
	void* mpb = sss.malloc();
	assert(mpb == mp2);
	void* mpc = sss.malloc();
	assert(mpc == mp1);
	assert(sss.empty());
	
	std::size_t const UnitSize2 = sizeof(My2);
	std::size_t const BufSize2 = UnitSize2*2;
	char buf3[BufSize2];
	sss.add_block(buf3, BufSize2, UnitSize2);
	void* mpd = sss.malloc();
	assert(mpd == buf3 + UnitSize2 * 0);
	void* mpe = sss.malloc();
	assert(mpe == buf3 + UnitSize2 * 1);
	assert(sss.empty());
	sss.free(mpe);
	sss.free(mpa);
	sss.free(mpb);
	sss.free(mpc);
	sss.free(mpa);
	void* mpg = sss.malloc();
	assert(mpg == mpe);
}

int main() {
    test1();
}
