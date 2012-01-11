// privateメンバへのアクセス サンプル

// 元となるコード
// https://gist.github.com/1528856
// Accessing Private Data 
// c.f. http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html

#include <iostream>

// ----------------------------------------------------------
// Accessor 
// Tag には、後述のA_mem1やA_mem2が渡される。
// Tag::type は、アクセスするメンバのメンバポインタ型となる。
template <class Tag>
struct Accessor {
    static typename Tag::type value;
};

// staticメンバの実体
// アクセスしたい型のメンバポインタを保持する。
template <class Tag>
typename Tag::type Accessor<Tag>::value;

// ----------------------------------------------------------
// Initializer
template <class Tag, typename Tag::type p>
struct Initializer {
    // コンストラクタにおいて
    // Accessor型のstaticメンバ value に p (メンバポインタ) 
    // を設定する。
    Initializer() { Accessor<Tag>::value = p; }
    static Initializer instance;
};

// 初期化を駆動するための static オブジェクト(自身)の定義
// explicit instantiation によって、int main() 前に
// 生成されることが確定し、生成の際、コンストラクタが
// 呼び出されることになる。
template <class Tag, typename Tag::type p>
Initializer<Tag, p> Initializer<Tag, p>::instance;

// ------------------------------------------------------
// Target Class
// privateメンバにアクセスされるクラス
struct A {
    A(int mem1, int mem2):mem1(mem1), mem2(mem2) {}
    void print() const { 
        std::cout << "mem1 = " << mem1 << " mem2 = " << mem2 << std::endl;
    }
private:
    int mem1;
    int mem2;
};

// Tagクラス。アクセスするメンバの、メンバポインタ型を
// をタイプメンバ type として持つ
struct A_mem1 { typedef int A::* type; };
struct A_mem2 { typedef int A::* type; };


// ------------------------------------------------------
// Template の Explicit instantiation
// ここでは private メンバにアクセスできる
// A::memのアドレスで、Initialize経由でAccessor::valueを
// 設定

// 14.7.2/12 The usual access checking rules do not apply
// to names used to specify explicit instantiations. 
// [ Note: In particular, the template arguments and 
// names used in the function declarator (including 
// parameter types,return types and exception 
// specifications) may be private types or objects 
// which would normally not be accessible and the 
// template may be a member template or member function
// which would not normally be accessible. —end note ]

template struct Initializer<A_mem1, &A::mem1>;
template struct Initializer<A_mem2, &A::mem2>;

int main() {
    A a(1, 2);
    // Accessorを経由して (設定済みの )mem にアクセス
    std::cout << a.*Accessor<A_mem1>::value << std::endl;
    std::cout << a.*Accessor<A_mem2>::value << std::endl;
    a.*Accessor<A_mem1>::value = 3;
    a.*Accessor<A_mem2>::value = 4;
    a.print();
    // ちなみに、Explicit instantiation 以外の箇所では
    // private メンバにアクセスできない
    // Initializer<A, &A::mem>(); // error A::mem is private
}
// Output:
/*
1
2
mem1 = 3 mem2 = 4
*/
