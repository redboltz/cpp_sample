#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/numeric/interval/compare/lexicographic.hpp>

#include <vector>
#include <cassert>

using IR = boost::numeric::interval<int>;
using IRCol = std::vector<IR>;

IRCol add(IRCol c) {
    using namespace boost::numeric::interval_lib::compare::lexicographic;
    IRCol result;
    boost::sort(c, [](IR const& lhs, IR const& rhs) { return lhs < rhs; } );
    auto it = c.cbegin();
    auto end = c.cend();
    if (it == end) return result;
    
    IR current = *it;
    ++it;
    for (; it != end; ++it) {
        if (current.upper() >= it->lower()) {
            current = IR(current.lower(), it->upper());
        }
        else {
            result.push_back(current);
            current = *it;
        }
    }
    result.push_back(current);
    return result;
}

IRCol sub(IRCol c1, IRCol c2) {
    IRCol result;
    c1 = add(c1);
    c2 = add(c2);

    auto it1 = c1.begin();
    auto it2 = c2.begin();
    while (it1 != c1.cend() && it2 != c2.cend()) {
        // 1    <--
        // 2 -->
        if (it1->lower() > it2->upper()) {
            ++it2; // Increment iterator2 only here.
        }
        // 1   <--
        // 2 -->
        // OR
        // 1   <--
        // 2  -->
        else {
            assert(it1->lower() <= it2->upper());

            // Add result.
            // 1  <----
            // 2     <-
            // r  <->
            if (it1->lower() < it2->lower()) {
                auto resultUpper = std::min(it1->upper(), it2->lower() - 1);
                result.push_back(IR(it1->lower(), resultUpper));
            }

            // 1 ---->
            // 2 ->
            // Shrink it1, and it1 becomes the next candidate
            // 1   <->
            // 2 ->
            if (it1->upper() >  it2->upper()) {
                // shrink it1's lower to it2's upper positon + 1
                // This means modify it1's lower.
                auto it1upper = it1->upper();
                ++it1; // Adjust insertion point.
                // it1 points to inserted element.
                it1 = c1.insert(it1, IR(it2->upper() + 1, it1upper));
            }
            // 1 -->
            // 2 -->
            // OR
            // 1 -->
            // 2 --->
            else {
                assert(it1->upper() <= it2->upper());
                ++it1; // Increment iterator1 only here.
            }
        }
    }
    // Add rest
    for (;it1 != c1.end(); ++it1) result.push_back(*it1);
    return result;
}

int main() {
    using namespace boost::numeric::interval_lib::compare::lexicographic;

    // tests

    // add
    {
        IRCol c { {1, 1}, {1, 2}, {1, 2}, {2, 4}, {3, 3}, {4, 5} };
        IRCol r = add(c);
        assert(r.size() == 2);
        assert(r[0] == IR(1, 3));
        assert(r[1] == IR(4, 5));
    }

    // sub
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {0, 0} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(2, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {0, 1} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(2, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {0, 2} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(3, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {0, 3} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(4, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {0, 5} };
        IRCol r = sub(c1, c2);
        assert(r.empty());
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {0, 6} };
        IRCol r = sub(c1, c2);
        assert(r.empty());
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {2, 3} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(4, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {3, 3} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 2);
        assert(r[0] == IR(2, 2));
        assert(r[1] == IR(4, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {3, 4} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 2);
        assert(r[0] == IR(2, 2));
        assert(r[1] == IR(5, 5));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {4, 5} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(2, 3));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {4, 6} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(2, 3));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {5, 6} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(2, 4));
    }
    {
        IRCol c1 { {2, 5} };
        IRCol c2 { {6, 7} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 1);
        assert(r[0] == IR(2, 5));
    }

    {
        IRCol c1 { {1, 9} };
        IRCol c2 { {2, 2}, {4, 4}, {7, 8} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 4);
        assert(r[0] == IR(1, 1));
        assert(r[1] == IR(3, 3));
        assert(r[2] == IR(5, 6));
        assert(r[3] == IR(9, 9));
    }
    {
        IRCol c1 { {0, 1}, {1, 9}, {5, 9} };
        IRCol c2 { {2, 2}, {4, 4}, {7, 8}, {7, 7} };
        IRCol r = sub(c1, c2);
        assert(r.size() == 4);
        assert(r[0] == IR(0, 1));
        assert(r[1] == IR(3, 3));
        assert(r[2] == IR(5, 6));
        assert(r[3] == IR(9, 9));
    }

}

