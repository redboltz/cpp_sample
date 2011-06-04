#include <iostream>

template <typename Derived>
class Bnt {
public:
	friend void foo(Derived const&)
	{
		std::cout << "Bnt::foo\n";
	}
	friend void bar(Derived& d)
	{
		d.bar();
	}
	friend void haz() {}
};

class User:Bnt<User> { // private inheritance
public:
	void bar() { std::cout << "User::bar() called" << std::endl; }
};

int main() {
	User u;
	foo(u);
	bar(u);
	baz();
}
