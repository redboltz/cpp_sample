class Base {
protected:
	void foo() {}
};

class Derived:public Base {
public:
	void bar() {
		// Implicit this
		foo();
		// Explicit this
		this->foo();
		// this via Derived reference
		Derived& d = *this;
		d.foo();

		// this via Base reference
		Base& b = *this;
		// b.foo(); // error

		// Derived not this
		Derived d2;
		d2.foo();

		// Base not this
		Base b2;
		// b2.foo(); //error
	}
};

template <typename T>
class Derived2:public Base {
public:
	void bar() {
		Derived2<T> d;
		d.foo();

		Derived2<int> di;
		// di.foo(); // error
	}
};

int main() {
	Derived d;
	d.bar();
	Derived2<char> d2;
	d2.bar();
}
