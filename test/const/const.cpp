
struct My {
	void m() {}
	void cm() const {}
};

typedef My* Myp;
typedef My const* Mycp;

typedef Myp MypCol[10];
typedef Mycp MycpCol[10];

typedef Myp * MypCol2;
typedef Mycp * MycpCol2;

void foo(MypCol p) {
}

void foo_const(MycpCol p) {
}

void boo(MypCol2 p) {
}

void boo_const(MycpCol2 p) {
}

void test1(My * p) {
}

void test2(My const * p) {
}

void test3(My *& p) {
}

void test4(My const *& p) {
}

void test5(My * const & p) {
}

void test6(My const * const & p) {
}

int main() {
	MypCol mpc;
	foo(mpc);
	//foo_const(mpc);
	boo(mpc);
	//boo_const(mpc);

	My m;
	test1(&m);
	test2(&m);
	test3(&m);
	test4(&m);

	My* pm = &m;
	My const* pcm = &m;

	test1(pm);
	test2(pm);
	test3(pm);
	test4(pm);
	test5(pm);

	test1(pcm);
	test2(pcm);
	test3(pcm);
	test4(pcm);
	test5(pcm);
	test6(pcm);

}

