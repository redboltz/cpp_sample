#include <iostream>

template <typename Derived>
class Bnt {
public:
	friend void foo(Derived const&)
	{
		std::cout << "Bnt::foo\n";
	}
};

class User:private Bnt<User> {
};

int main() {
	User u;
	foo(u);
}
