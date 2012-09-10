#include <utility>
#include <string>
#include <iostream>

struct Base1 {
    explicit Base1(int*) {}
};

struct Derived1:Base1 {
    template <typename... T>
    Derived1(T&&... t):Base1(std::forward<T>(t)...) {}
};

void test1() {
    {
        Base1 b1(0);
        // Derived1 d1(0); // compile error
    }
    {
        Base1 b1(nullptr);
        Derived1 d1(nullptr); // OK
    }
}

struct Base2 {
    explicit Base2(std::initializer_list<int>) {}
};


struct Derived2:Base2 {
    template <typename... T>
    Derived2(T&&... t):Base2(std::forward<T>(t)...) {}
};

void test2() {
    Base2 b2 { 1, 2, 3 };
    // Derived2 d2 { 1, 2, 3 }; // compile error
    auto a {1, 2, 3};
    Derived2 dd2(a);
}

struct Base3 {
    explicit Base3(std::string) {}
};

struct Derived3:Base3 {
    template <typename... T>
    Derived3(T&&... t):Base3(std::forward<T>(t)...) {}
};

void test3() {
    {
        Base3 b3({ 'a', 'b', 'c' });
        // Derived3 d3({ 'a', 'b', 'c' }); // compile error
    }
    {
        Base3 b3("abc");
        Derived3 d3("abc"); // OK
    }
}

struct Base4 {
    explicit Base4(int) {}
    static const int mem = 42;
};

struct Derived4:Base4 {
    template <typename... T>
    Derived4(T&&... t):Base4(std::forward<T>(t)...) {}
};

void test4() {
    Base4 b4(Base4::mem);
    // Derived4 d4(Base4::mem); // link error
}

struct Base5 {
    explicit Base5(void(int)) {}
};

struct Derived5:Base5 {
    template <typename... T>
    Derived5(T&&... t):Base5(std::forward<T>(t)...) {}
};

void f5(int) {}
template <typename T> void ft5(T) {}

void test5() {
    {
        Base5 b5(ft5);
        // Derived5 d5(ft5); // compile error
    }
    {
        Base5 b5(f5);
        Derived5 d5(f5); // OK
    }
}

struct Base6 {
    explicit Base6(std::ostream& (*)(std::ostream&)) {}
};

struct Derived6:Base6 {
    template <typename... T>
    Derived6(T&&... t):Base6(std::forward<T>(t)...) {}
};

void test6() {
    Base6 b6(std::endl);
    // Derived6 d6(std::endl); // compile error
}

struct Base7 {
    explicit Base7(int) {}
};

struct Derived7:Base7 {
    template <typename... T>
    Derived7(T&&... t):Base7(std::forward<T>(t)...) {}
};

struct Bf7 {
    int f:1;
};

void test7() {
    Bf7 b;
    Base7 b7(b.f);
    // Derived7 d7(b.f); // compile error
}




#if 0
void f(int) { std::cout << "f(int)" << std::endl; }
void f(char*) { std::cout << "f(char*)" << std::endl; }
void f(void*) { std::cout << "f(void*)" << std::endl; }
#endif

// still potentially misleading (to humans)
int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
}
