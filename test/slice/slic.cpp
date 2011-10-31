
struct c1 {
	c1():a(1), b(2), c(3) {}
	int a;
	int b;
	int c;
};

struct c2 {
	c2():d(4), e(5), f(6) {}
	int d;
	int e;
	int f;
};

struct c3 {
	c3():g(7), h(8), i(9) {}
	int g;
	int h;
	int i;
};

struct mixin:c1, c2, c3 {};

int main()
{
	mixin m;
	c1 ic1 = m;
	c2 ic2 = m;
	c3 ic3 = m;
}
