// 元となるコード
// https://gist.github.com/1528856
// Accessing Private Data c.f. http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html

// privateメンバへのアクセス サンプル
// 型をintに限定し、かつ、アクセス可能な要素をひとつに限定した
// 動作の仕組み理解用コード

#include <iostream>

// ------------------------------------------------------
// Accessor 
// int型の、T::* (メンバポインタ)をstaticメンバとして定義
// Tに対象クラス型を渡せば、Initializerで初期化された
// メンバのアドレスにアクセス可能(privateであったとしても)
template <class T>
struct Accessor {
    static int T::* value;
};

// staticメンバの実体
template <class T>
int T::* Accessor<T>::value;

// ------------------------------------------------------
// Initializer
// Accessor型のstaticメンバを p (メンバのアドレス) 
// で初期化
template <class T, int T::* p>
struct Initializer {
    Initializer() { Accessor<T>::value = p; }
    static Initializer instance;
};

// 初期化を駆動するための static オブジェクト(自身)の定義
template <class T, int T::* p>
Initializer<T, p> Initializer<T, p>::instance;

// ------------------------------------------------------
// Target Class
// privateメンバにアクセスされるクラス
struct A {
    A():mem(42) {}
private:
    int mem;
};

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

template struct Initializer<A, &A::mem>;

int main() {
    A a;
    // Accessorを経由して (設定済みの )mem にアクセス
    std::cout << a.*Accessor<A>::value << std::endl;
	// ちなみに、Explicit instantiation 以外の箇所では
	// private メンバにアクセスできない
	// Initializer<A, &A::mem>(); // error A::mem is private
}
