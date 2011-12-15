#include <iostream>
#include <type_traits>
#include <memory>

struct My {
    My() = default;
    My(My const&) {
        std::cout << "My copy ctor" << std::endl;
    }
    My(My&&) {
        std::cout << "My move ctor" << std::endl;
    }
};

template<typename T>
struct holder{
    T value;
};

template<typename T>
holder<typename std::remove_reference<T>::type>
make_holder(T&& t){
    return { std::forward<T>(t) };
}

int main() {
    My m;
    holder<My> v1 = make_holder(m);     // ok
    holder<My> v2 = make_holder(My());  // ok but copy
}
