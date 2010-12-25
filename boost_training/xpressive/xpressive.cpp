// http://ideone.com/PJUBT
#include <boost/xpressive/xpressive.hpp>
#include <string>
#include <iostream>

#include <boost/algorithm/string.hpp>

namespace {
namespace xpr = boost::xpressive;
struct UpCase {
    UpCase(int i):i_(i) {}
    std::string operator()(xpr::smatch const &what) const {
        return boost::to_upper_copy(what[i_].str());
    }
private:
    int i_;
};
}

int main(){
    using namespace boost::xpressive;
    std::string str("foobarbazbarfoo");

    {
        // 単純なマッチ
        // ruby     /bar/
        // expect   bar
        smatch what;
        sregex rex(as_xpr("bar"));
        if (regex_search(str, what, rex)) {
            std::cout << what[0] << std::endl;
        }
    }
    {
        // 後方参照を伴うマッチ
        // ruby     /(foo|bar|baz)(.*)\1/
        // expect   barbazbar
        smatch what;
        sregex rex = (s1 = as_xpr("foo") | as_xpr("bar") | as_xpr("baz")) >> (s2 = *_) >> s1;
        if (regex_search(str, what, rex)) {
            std::cout << what[2] << std::endl;
        }
    }
    {
        // 先読みを伴うマッチ
        // ruby     /bar(?=foo)/
        // expect   bar
        smatch what;
        sregex rex = as_xpr("bar") >> before(as_xpr("foo"));
        if (regex_search(str, what, rex)) {
            std::cout << what[0] << std::endl;
        }
    }
    {
        // 最短一致のマッチ
        // ruby     /.*?bar/
        // expect   foobar
        smatch what;
        sregex rex = -*_ >> as_xpr("bar");
        if (regex_search(str, what, rex)) {
            std::cout << what[0] << std::endl;
        }
    }
    {
        // 単純な置換
        // ruby     gsub(/foo/, "XXX")
        // expect   XXXbarbazbarXXX
        sregex rex = as_xpr("foo");
        std::cout << regex_replace(str, rex, "XXX") << std::endl;
    }
    {
        // ブロックを伴う置換
        // ruby     gsub(/foo|bar|baz/) { |s| s.upcase }
        // expect   FOOBARBAZBARFOO
        smatch what;
        sregex rex = (s1 = as_xpr("foo") | as_xpr("bar") | as_xpr("baz"));
        // functorとして外出しするのがC++03では現実的
        std::cout << regex_replace(str, rex, UpCase(1)) << std::endl;
    }
    {
        // 分割
        // ruby     split(/baz/)
        // expect   ["foobar", "barfoo"]
        smatch what;
        sregex rex = as_xpr("bar");
        // 正規表現をデリミタとするイテレータを作る
        // -1を与えると、マッチしない部分が順次帰る
        sregex_token_iterator cur(str.begin(), str.end(), rex, -1);
        sregex_token_iterator end;
        std::cout << "----" << std::endl;
        for( ; cur != end; ++cur ) {
            std::cout << *cur << std::endl;
        }
    }
    {
        // 切り出し
        // ruby     split(/baz/)
        // expect   baz
        smatch what;
        sregex rex = as_xpr("baz");
        // 正規表現をデリミタとするイテレータを作る
        // -1を与えないと、マッチする部分が順次帰る
        sregex_token_iterator cur(str.begin(), str.end(), rex);
        sregex_token_iterator end;
        std::cout << "----" << std::endl;
        for( ; cur != end; ++cur ) {
            std::cout << *cur << std::endl;
        }
    }
    {
        // 切り出し(追加)
        // ruby     split(/ba.?/)
        // expect   bar baz bar
        smatch what;
        sregex rex = as_xpr("ba") >> !_;
        // 正規表現をデリミタとするイテレータを作る
        // -1を与えないと、マッチする部分が順次帰る
        sregex_token_iterator cur(str.begin(), str.end(), rex);
        sregex_token_iterator end;
        std::cout << "----" << std::endl;
        for( ; cur != end; ++cur ) {
            std::cout << *cur << std::endl;
        }
    }
}
