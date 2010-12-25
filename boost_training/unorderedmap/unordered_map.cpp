#include <unordered_map>
#include <string>
#include <boost/optional.hpp>
#include <iostream>

//using std::tr1::unordered_map;
using std::unordered_map;

int main() {
    unordered_map<std::string, boost::optional<int> > a;
    a["hoge"] = 10;
    a["fuga"] = boost::none;
    if (a.find("pnyo") == a.end()) {
        std::cout << "Expected1" << std::endl;
    }
    else {
        std::cout << "NG" << std::endl;
    }

    if (a["fuga"]) {
        std::cout << "NG" << std::endl;
    }
    else {
        std::cout << "Expected2" << std::endl;
    }
     
    if (a.find("fuga") != a.end()) {
        std::cout << "Expected3" << std::endl;
    }
    else {
        std::cout << "NG" << std::endl;
    }
     
    if (a.find("pnyo") != a.end()) {
        std::cout << "NG" << std::endl;
    }
    else {
        std::cout << "Expected4" << std::endl;
    }

    if (a["pnyo"]) { // ここで、エントリが追加される
        std::cout << "NG" << std::endl;
    }
    else {
        std::cout << "Expected5" << std::endl;
    }

    // 追加されたため、Expected4と結果が異なる。
    if (a.find("pnyo") != a.end()) { 
        std::cout << "Expected6" << std::endl;
    }
    else {
        std::cout << "NG" << std::endl;
    }

    // 効率的なチェック、アクセス
    unordered_map<std::string, boost::optional<int> >::iterator i(a.find("hoge"));
    if (i != a.end()) {
        std::cout << *i->second << std::endl;
        *i->second = 42;
    }
}
