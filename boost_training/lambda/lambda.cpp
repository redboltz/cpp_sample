// http://ideone.com/L7bEb
#include <boost/lambda/lambda.hpp>
#include <boost/assign/list_of.hpp>
#include <vector>
#include <iostream>
 
int main() {
    using boost::assign::list_of;
    using boost::lambda::_1;
    using boost::lambda::_2;
 
    std::vector<int> m = list_of(5)(3)(1)(2)(4);
    std::for_each(m.begin(), m.end(), std::cout << _1 << " ");
    std::cout << "\n";
    std::sort(m.begin(), m.end(), _1 < _2);
    std::for_each(m.begin(), m.end(), std::cout << _1 << " ");
    std::cout << "\n";
    std::sort(m.begin(), m.end(), _1 > _2);
    std::for_each(m.begin(), m.end(), std::cout << _1 << " ");
    std::cout << "\n";
}

なかなかやるでしょ？

では、int型ではなく、自前のクラスでやってみよう。

rubyではこんな感じ。
http://ideone.com/xrQKz
class My 
    def initialize(val)
        @val = val
    end
    attr_reader :val
end
 
a = [ My.new(5), My.new(3), My.new(1), My.new(2), My.new(4)]
puts a.inject("") { |result, v| result += v.val.to_s + " " }
a.sort! { |lhs, rhs| lhs.val <=> rhs.val }
puts a.inject("") { |result, v| result += v.val.to_s + " " }
a.sort! { |lhs, rhs| rhs.val <=> lhs.val }
puts a.inject("") { |result, v| result += v.val.to_s + " " }
