#include <etude/types/indices.hpp>

#include <tuple>
#include <utility>
#include <functional>
#include <cassert>
#include <string>

template<class F, class Tuple, std::size_t... Indices>
inline typename std::function<F>::result_type 
apply_(F const & f, Tuple && t, etude::indices<Indices...> ) 
{
    (void)t;
    return f( std::get<Indices>( std::forward<Tuple>(t) )... );
}

template<class F, class Tuple>
inline typename std::function<F>::result_type 
apply(F const & f, Tuple && t ) 
{
    
    return apply_(f, 
          std::forward<Tuple>(t),
                  etude::make_indices<
                      std::tuple_size<
              typename std::remove_reference<Tuple>::type
              >::value
          >());
}

// test
double fun(int a, float b, std::string c)
{
    return a + b + c.size();
}


int main()
{
    assert(apply(fun, std::make_tuple(42, 1.5, "hello")) == 48.5);
}
