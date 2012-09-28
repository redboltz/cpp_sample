#include <iostream>
#include <vector>

#include <boost/range/algorithm.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/type_traits.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <boost/mpl/print.hpp>

template <class Iter>
class MyIter : public boost::iterator_facade<
                   MyIter<Iter>,
                   typename Iter::value_type,
                   typename Iter::iterator_category,
                   typename boost::add_pointer< 
                       typename boost::add_const<
                           typename boost::remove_pointer<
                               typename Iter::value_type
                           >::type
                       >::type 
                   >::type
               >
{
public:
    typedef typename ::MyIter<Iter> this_type;
    typedef typename Iter::iterator_category iterator_category;
    MyIter<Iter>(Iter const& base):base_(base) {}
    typename MyIter::reference dereference() const {
        return *base_;
    }
    bool equal(this_type const& other) const {
        return base_ == other.base_;
    }
    void increment() {
        ++base_;
    }
    void decrement() {
        --base_;
    }
    void advance(typename Iter::difference_type n) {
        std::advance(base_, n);
    }
    typename Iter::difference_type distance_to(this_type const& other) const {
        return std::distance(base_, other.base_);
    }
private:
    Iter base_;
};

template <class T>
class ConstRange {
public:
    typedef MyIter<typename T::const_iterator> const_iterator;
    typedef typename const_iterator::reference const_reference;
    ConstRange<T>(T const& t):t_(t) {}
    const_iterator begin() const { return t_.begin(); }
    const_iterator end() const { return t_.end(); }
    const_reference operator[](std::size_t idx) const { return t_[idx]; }
    
private:
    T const& t_;
};

template <class T>
ConstRange<T> make_ConstRange(T const& t) {
    return ConstRange<T>(t);
};

template <class T>
void foo(ConstRange<T> const& t) {
    std::cout << t[0] << std::endl;
    std::cout << **t.begin() << std::endl;
    // int *p = t[0];
    // int *q = *t.begin();
    //t[0] = 1;
}


int main() {
    namespace range = boost::range;
    namespace assign = boost::assign;
    {
        std::vector<int>   v  = assign::list_of(0)(1)(2)(3)(4)(5)(6);
        std::vector<int *> v1 = assign::list_of(&v[0])(&v[1])(&v[2])(&v[3])(&v[4]);
        std::vector<int *> v2 = assign::list_of(&v[0])(&v[1])(&v[2])(&v[5]);

        MyIter<std::vector<int*>::iterator> ib(v2.begin());
        MyIter<std::vector<int*>::iterator> ie(v2.end());
        std::vector<int *> v4;
        std::set_difference(v1.begin(), v1.end(), ib, ie, std::back_inserter(v4));
        BOOST_FOREACH(int* i, v4) {
            std::cout << *i << ",";
        }
        std::cout << std::endl;
    }
    {
        std::vector<int>   v  = assign::list_of(0)(1)(2)(3)(4)(5)(6);
        std::vector<int *> v1 = assign::list_of(&v[0])(&v[1])(&v[2])(&v[3])(&v[4]);
        std::vector<int *> v2 = assign::list_of(&v[0])(&v[1])(&v[2])(&v[5]);

        ConstRange<std::vector<int*> > v3(v2);
        foo(make_ConstRange(v2));
        std::vector<int *> v4;
        range::set_difference(v1, v2, std::back_inserter(v4));
        BOOST_FOREACH(int* i, v4) {
            std::cout << *i << ",";
        }
        std::cout << std::endl;
    }
}
