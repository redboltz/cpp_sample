// http://ideone.com/94AMD
#include <vector>
#include <iostream>
#include <algorithm>
 
struct My {
    My(int val_):val(val_){}
    int val;
};
 
int main() {
    std::vector<My> m = { 5, 3, 1, 2, 4};
    std::for_each(m.begin(), m.end(), [](My const& v) { std::cout << v.val << " "; });
    std::cout << "\n";
    std::sort(m.begin(), m.end(), [](My const& lhs, My const& rhs) { return lhs.val < rhs.val; });
    std::for_each(m.begin(), m.end(), [](My const& v) { std::cout << v.val << " "; });
    std::cout << "\n";
    std::sort(m.begin(), m.end(), [](My const& lhs, My const& rhs) { return lhs.val > rhs.val; });
    std::for_each(m.begin(), m.end(), [](My const& v) { std::cout << v.val << " "; });
    std::cout << "\n";
}
