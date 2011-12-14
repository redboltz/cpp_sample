/*******************************************************************************
 * Modified by Takatoshi Kondo.
 * This code is based on the below.
 * -----------------------------------------------------------------------------
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
#include <boost/format.hpp>

#include <boost/array.hpp>
#include <boost/proto/proto.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/proto/proto_fwd.hpp>

namespace proto = boost::proto;

// 変数用タグの定義
struct variable_tag {};

// Grammerの定義
struct  analytical_function
    : proto::or_<
        proto::terminal< variable_tag >, 
        proto::or_< 
            proto::terminal< int >, 
            proto::terminal< float >, 
            proto::terminal< double >
        >, 
        proto::plus<analytical_function, analytical_function>, 
        proto::negate<analytical_function>, 
        proto::minus<analytical_function, analytical_function>, 
        proto::multiplies<analytical_function, analytical_function>, 
        proto::divides<analytical_function, analytical_function>
    >
    {};

// Grammerのチェック関数
template<class Expression> inline void
check_for_match( Expression const& xpr )
{
    // 式の表示
    proto::display_expr(xpr);
    // 式のチェック
    std::cout
        <<  ( proto::matches<Expression, analytical_function>::value
            ? "matches " 
            : "doesn't match "
            )
        << "the analytical_function grammar\n";
}

// Custom operator

// こんな風にstd::plusの再利用もできる
// struct dplus : std::plus<double>, proto::callable {};

// 自前で作ってみる
struct dplus : proto::callable {
    typedef double result_type;
    double operator()(double lhs, double rhs) const {
        std::cout << boost::format("custom plus called!! %1% + %2%\n") % lhs % rhs;
        return lhs + rhs;
    }
};

// Evaluator
struct  evaluate_
    : proto::or_<
        // 変数
        proto::when< 
            proto::terminal< variable_tag >, 
            proto::_state
        >, 
        // 変数以外のterminal (つまり定数）
        proto::when< 
            proto::terminal< proto::_ >, 
            proto::_value
        >, 
        // + 演算子
        proto::when< 
            proto::plus < 
                proto::_,
                proto::_
            >,
            // ここを自前の演算子で実現してみる
            dplus(
                evaluate_(proto::_left),
                evaluate_(proto::_right)
            )
        >, 
        // それ以外はC++のルールに従う
        proto::otherwise< proto::_default<evaluate_> >
    >
    {};


// analytical_domainのための先行宣言
template<typename AST>
struct analytical_expression;

// functionとexpressionの関連づけ
struct analytical_domain
    : proto::domain< 
        proto::generator<analytical_expression>, 
        analytical_function
    >
{};

// expressionの定義
template<typename AST>
struct analytical_expression
    : proto::extends<AST, analytical_expression<AST>, analytical_domain> {
    typedef proto::
          extends<AST, analytical_expression<AST>, analytical_domain> extendee;

    analytical_expression(AST const& ast = AST()) : extendee(ast) {}

    BOOST_PROTO_EXTENDS_USING_ASSIGN(analytical_expression)

    typedef double result_type;

    result_type operator()(double v0) const {
        evaluate_ callee;
        return callee(*this, v0);
    }
};

// placeholderの定義。この方式だと、まだ1つしか定義できない。
analytical_expression< proto::terminal< variable_tag >::type > const _x;

int main()
{
    std::cout << (_x + (2+_x))(5) << "\n";
    std::cout << (_x * 6)(8) << "\n";
}
