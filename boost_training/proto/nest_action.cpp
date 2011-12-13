/*******************************************************************************
 *           Getting Jiggy with Boost.Proto - Boost'Con 2011 Tutorial
 *                                 by Joel Falcou
 *
 *                      Step 1 : Putting Expressions to Work
 *
 *             Distributed under the Boost Software License, Version 1.0.
 *                    See accompanying file LICENSE.txt or copy at
 *                       http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#include <iostream>
#include <boost/array.hpp>
#include <boost/proto/proto.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/proto/proto_fwd.hpp>


struct variable_tag {};
struct  analytical_function
      : boost::proto::or_
        <
        boost::proto::terminal< variable_tag >
        , boost::proto::or_
          < boost::proto::terminal< int >
          , boost::proto::terminal< float >
          , boost::proto::terminal< double >
          >
        , boost::proto::plus<analytical_function,analytical_function>
        , boost::proto::negate<analytical_function>
        , boost::proto::minus<analytical_function,analytical_function>
        , boost::proto::multiplies<analytical_function,analytical_function>
        , boost::proto::divides<analytical_function,analytical_function>
        >
{};

template<class Expression> inline void
check_for_match( Expression const& xpr )
{
  boost::proto::display_expr(xpr);

  // The meta-check
  std::cout
      << ( boost::proto::matches<Expression, analytical_function>::value
         ? "matches " : "doesn't match "
         )
      << "the analytical_function grammar\n\n";
}

struct  evaluate_
      : boost::proto::or_
        <
          boost::proto::when
          < boost::proto::terminal< variable_tag >
          , boost::proto::_state
          >
        , boost::proto::when
          < boost::proto::terminal< boost::proto::_ >
          , boost::proto::_value
          >
        , boost::proto::when
          < boost::proto::plus < boost::proto::_,
                                 boost::proto::_
                               >
//          , boost::proto::minus<evaluate_, evaluate_>
//          , boost::proto::_default<evaluate_>
          , boost::proto::functional::make_plus
            ( boost::proto::_left
            , boost::proto::_right
            )
          >
//        , boost::proto::otherwise< boost::proto::_default<evaluate_> >
        >
{};

template<typename AST>
struct analytical_expression;

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
    evaluate_ callee;
    return callee(*this,v0);
  }
};

analytical_expression< boost::proto::terminal< variable_tag >::type > const _x;

int main()
{
  std::cout << (_x * 3)(2) << "\n";
}
