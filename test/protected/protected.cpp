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

int main() {
	Derived d;
	d.bar();
}
