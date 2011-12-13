/*******************************************************************************
 *           Getting Jiggy with Boost.Proto - Boost'Con 2011 Tutorial
 *                                 by Joel Falcou
 *
 *                             Step 2 : Transformers !
 *
 *             Distributed under the Boost Software License, Version 1.0.
 *                    See accompanying file LICENSE.txt or copy at
 *                       http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/


#include <iostream>

#include <boost/proto/proto.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/array.hpp>

//----------------------------------------------------------------------

struct  analytical_function_cases
{
  // The primary template matches nothing:
  template<typename Tag>
  struct case_ : boost::proto::not_<boost::proto::_> {};
};

struct  analytical_function
: boost::proto::switch_< analytical_function_cases >
{};

template<>
struct    analytical_function_cases
        ::case_<boost::proto::tag::terminal>
        : boost::proto::and_
          < boost::proto::terminal< boost::proto::_ >
          , boost::proto::if_< boost::is_convertible< double
                                                    , boost::proto::_value
                                                    >()
                             >
          >
{};

template<>
struct    analytical_function_cases
        ::case_< boost::proto::tag::plus >
        : boost::proto::plus<analytical_function,analytical_function>
{};

template<>
struct    analytical_function_cases
        ::case_<boost::proto::tag::multiplies>
        : boost::proto::multiplies<analytical_function,analytical_function>
{};

//----------------------------------------------------------------------

struct fetch_variable : boost::proto::callable
{
  template<typename Sig> struct result;

  template<typename This,typename Index,typename Vars>
  struct result<This(Index,Vars)>
  {
    typedef typename boost::remove_reference<Vars>::type  base;
    typedef typename base::const_reference                type;
  };

  template<class Index, class Vars> inline
  typename result<fetch_variable(Index,Vars)>::type
  operator()(Index const&, Vars const& v) const
  {
    return v[Index::value];
  }
};


struct  evaluate_;

struct  evaluate_cases
{
  // The primary template goes for the _default:
  template<typename Tag>
  struct case_ : boost::proto::_default<evaluate_> {};
};

struct  evaluate_ : boost::proto::switch_< evaluate_cases > {};

template<>
struct    evaluate_cases
        ::case_<boost::proto::tag::terminal>
        : boost::proto::when
          < boost::proto::and_
            < boost::proto::terminal< boost::proto::_ >
            , boost::proto::if_< boost::is_convertible< double
                                                      , boost::proto::_value
                                                      >()
                               >
            >
          , boost::proto::_value
          >
{};

//----------------------------------------------------------------------

template<typename AST> struct analytical_expression;

struct  analytical_domain
      : boost::proto::domain< boost::proto::generator<analytical_expression>
                            , analytical_function
                            >
{};

template<typename AST>
struct  analytical_expression
      : boost::proto::extends<AST,analytical_expression<AST>,analytical_domain>
{
  typedef boost::proto::
          extends<AST,analytical_expression<AST>,analytical_domain> extendee;

  analytical_expression(AST const& ast = AST()) : extendee(ast) {}

  BOOST_PROTO_EXTENDS_USING_ASSIGN(analytical_expression)

  typedef double result_type;

  result_type operator()(double v0) const
  {
    boost::array<double,1> that = {{v0}};
    evaluate_ callee;
    return callee(*this,that);
  }

  result_type operator()(double v0,double v1) const
  {
    boost::array<double,2> that = {{v0,v1}};
    evaluate_ callee;
    return callee(*this,that);
  }

  result_type operator()(double v0,double v1,double v2) const
  {
    boost::array<double,3> that = {{v0,v1,v2}};
    evaluate_ callee;
    return callee(*this,that);
  }
};

//----------------------------------------------------------------------
struct variable_tag {};

template<>
struct    analytical_function_cases
        ::case_< variable_tag >
        : boost::proto::nullary_expr< variable_tag, boost::proto::_ >
{};

analytical_expression< boost::proto::nullary_expr< variable_tag,boost::mpl::int_<0> >::type > const _x;

template<>
struct    evaluate_cases
        ::case_< variable_tag >
        : boost::proto::when
          < boost::proto::nullary_expr< variable_tag, boost::proto::_ >
          , fetch_variable(boost::proto::_value,boost::proto::_state)
          >
{};

//----------------------------------------------------------------------

struct constant_tag {};

template<int N>
struct  constant_
      : analytical_expression
        < typename boost::proto::nullary_expr < constant_tag
                                              , boost::mpl::int_<N>
                                              >::type
        >
{};

template<>
struct    analytical_function_cases
        ::case_< constant_tag >
        : boost::proto::unary_expr< constant_tag, boost::proto::_ >
{};

template<>
struct    evaluate_cases
        ::case_< constant_tag >
        : boost::proto::when
          < boost::proto::nullary_expr< constant_tag, boost::proto::_ >
          , boost::proto::_value
          >
{};

//----------------------------------------------------------------------

struct  derivate_cases
{
  // The primary template matches nothing
  template<typename Tag>
  struct case_ : boost::proto::not_<boost::proto::_> {};
};

struct  derivate_ : boost::proto::switch_< derivate_cases > {};

template<>
struct    derivate_cases
        ::case_<boost::proto::tag::terminal>
        : boost::proto::when
          < boost::proto::and_
            < boost::proto::terminal< boost::proto::_ >
            , boost::proto::if_< boost::is_convertible< double
                                                      , boost::proto::_value
                                                      >()
                               >
            >
          , constant_<0>()
          >
{};

template<>
struct    derivate_cases
        ::case_<constant_tag>
        : boost::proto::when
          < boost::proto::nullary_expr< constant_tag, boost::proto::_ >
          , constant_<0>()
          >
{};

template<>
struct    derivate_cases
        ::case_< variable_tag >
        : boost::proto::when
          < boost::proto::nullary_expr< variable_tag, boost::proto::_ >
          , boost::proto::if_
            < boost::is_same< boost::proto::_value
                            , boost::proto::_value(boost::proto::_state)
                            >()
            , constant_<1>()
            , constant_<0>()
            >
          >
{};

template<>
struct    derivate_cases
        ::case_< boost::proto::tag::plus >
        : boost::proto::when
          < boost::proto::plus< derivate_cases, derivate_cases >
          , boost::proto::functional::make_plus
            ( derivate_(boost::proto::_left)
            , derivate_(boost::proto::_right)
            )
          >
{};

template<>
struct    derivate_cases
        ::case_< boost::proto::tag::multiplies >
        : boost::proto::when
          < boost::proto::plus< derivate_cases, derivate_cases >
          , boost::proto::call<
              boost::proto::functional::make_plus
              ( boost::proto::call<
                  boost::proto::functional::make_multiplies
                  ( boost::proto::_left
                  , boost::proto::call<derivate_(boost::proto::_right)>
                  )
                >
              , boost::proto::call<
                  boost::proto::functional::make_multiplies
                  ( boost::proto::call<derivate_(boost::proto::_left)>
                  , boost::proto::_right
                  )
                >
              )
            >
	  >
{};

template<int N>
struct nth_derivative
{
  template<typename Sig> struct result;

  template<typename This,typename F, typename V>
  struct  result<This(F,V)>
  {
    typedef typename boost::result_of<derivate_(F, V)>::type              base;
    typedef typename boost::result_of<nth_derivative<N-1>(base const&, V)>::type type;
  };

  template<typename F, typename V> inline
  typename result<nth_derivative<N>(F const&, V const&)>::type
  operator()( F const& f, V const& v ) const
  {
    nth_derivative<N-1> rec;
    derivate_           callee;
    return rec(callee(f,v),v);
  }
};

template<>
struct nth_derivative<1>
{
  template<typename Sig> struct result;

  template<typename This,typename F, typename V>
  struct  result<This(F,V)> : boost::result_of<derivate_(F, V)>
  {};

  template<typename F, typename V> inline
  typename result<nth_derivative<1>(F const&, V const&)>::type
  operator()( F const& f, V const& v ) const
  {
    derivate_ callee;
    return callee(f,v);
  }
};


template<int Degree,typename Function, typename Variable> inline
typename boost::result_of<nth_derivative<Degree>(Function const&, Variable const&)>::type
derivate( Function const& f, Variable const& v )
{
  nth_derivative<Degree> callee;
  return callee(f,v);
}

template<typename Function, typename Variable> inline
typename boost::result_of<derivate_(Function const&, Variable const&)>::type
derivate( Function const& f, Variable const& v )
{
  derivate_ callee;
  return callee(f,v);
}

//----------------------------------------------------------------------

int main() {
  std::cout << derivate( _x + _x, _x )(1.5)         << "\n";
  std::cout << derivate( _x * _x, _x )(1.5)         << "\n";
}
