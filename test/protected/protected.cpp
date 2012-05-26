class Base {
protected:
    void foo() {}
};

class Derived:public Base {
public:
    void bar();
};

class MoreDerived:public Derived {
};

void Derived::bar() {
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
    
    MoreDerived md;
    md.foo();
}


template <typename T>
class Derived2:public Base {
public:
	operator Derived2<char>() {
		return Derived2<char>();
	}
    void bar() {
        Derived2<T> d;
        d.foo();

        Derived2<int> di;
        // di.foo(); // error
		Derived2<char> dc = di;
		dc.foo();
		Derived2<char> dd;
		dd.foo();
    }
};

int main() {
    Derived d;
    d.bar();
    Derived2<char> d2;
    d2.bar();
}
