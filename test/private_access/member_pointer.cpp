#include <iostream>

template <class MemPtr, MemPtr p>
struct Accessor {
    static MemPtr value() {
        return p;
    }
};

// ------------------------------------------------------
// Target Class
// privateメンバにアクセスされるクラス
struct A {
    A(int mem1, int mem2):mem1(mem1), mem2(mem2) {}
    void print() const { 
        std::cout << "mem1 = " << mem1 << " mem2 = " << mem2 << std::endl;
    }
    int mem1;
    int mem2;
};

int main() {
    A a(1, 2);
    // メンバポインタを経由してのアクセス
    std::cout << a.*&A::mem2 << std::endl;
    // Accessorを経由して (設定済みの )mem にアクセス
    std::cout << a.*Accessor<int A::*, &A::mem1>::value() << std::endl;
    std::cout << a.*Accessor<int A::*, &A::mem2>::value() << std::endl;
    a.*Accessor<int A::*, &A::mem1>::value() = 3;
    a.*Accessor<int A::*, &A::mem2>::value() = 4;
    a.print();
}
// Output:
/*
2
1
2
mem1 = 3 mem2 = 4
*/
