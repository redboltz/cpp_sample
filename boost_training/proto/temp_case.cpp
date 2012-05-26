#include <iostream>

#include <boost/proto/proto.hpp>
#include <boost/proto/proto_fwd.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/array.hpp>

namespace proto = boost::proto;

//----------------------------------------------------------------------
// Grammer チェック
struct analytical_function_cases {
	// 何者にもマッチしないプライマリテンプレート
	template<typename Tag>
	struct case_ : proto::not_<proto::_> {};
};

// switchによる分岐
struct analytical_function
	: proto::switch_< analytical_function_cases > {};

// terminal
template<>
struct analytical_function_cases::case_<proto::tag::terminal>
	: proto::and_<
		proto::terminal< proto::_ >, 
		proto::if_<
			boost::is_convertible<
				double, 
				proto::_value
			>()
		>
	>
{};

// +
template<>
struct analytical_function_cases::case_< proto::tag::plus >
	: proto::plus<analytical_function, analytical_function>
{};

// *
template<>
struct analytical_function_cases::case_<proto::tag::multiplies>
	: proto::multiplies<analytical_function, analytical_function>
{};

//----------------------------------------------------------------------
// ArrayからN番目の要素を取り出す仕組み
struct fetch_variable : proto::callable {
	template<typename Sig> struct result;

	template<typename This, typename Index, typename Vars>
	struct result<This(Index, Vars)> {
		typedef typename boost::remove_reference<Vars>::type  base;
		typedef typename base::const_reference                type;
	};

	template<class Index, class Vars> inline
	typename result<fetch_variable(Index,Vars)>::type
	operator()(Index const&, Vars const& v) const {
		return v[Index::value];
	}
};


//----------------------------------------------------------------------
struct evaluate_;

struct evaluate_cases {
	// The primary template goes for the _default:
	template<typename Tag>
	struct case_ : proto::_default<evaluate_> {};
};

struct evaluate_ : proto::switch_< evaluate_cases > {};

template<>
struct evaluate_cases::case_<proto::tag::terminal>
	: proto::when<
		// 条件
		proto::and_< 
			proto::terminal< proto::_ >, 
			proto::if_< 
				boost::is_convertible< 
					double, 
					proto::_value
				>()
			>
		>, 
		// アクション
		proto::_value
	>
{};

//----------------------------------------------------------------------
// expressionとfunctionの結びつけ
template<typename AST> struct analytical_expression;

struct analytical_domain
	: proto::domain< 
		proto::generator<
			analytical_expression
		>
		, analytical_function
	>
{};

template<typename AST>
struct analytical_expression
	: proto::extends<AST, analytical_expression<AST>, analytical_domain> {
	typedef proto::extends<AST,analytical_expression<AST>,analytical_domain> extendee;

	analytical_expression(AST const& ast = AST()) : extendee(ast) {}

	BOOST_PROTO_EXTENDS_USING_ASSIGN(analytical_expression)

	typedef double result_type;

	result_type operator()(double v0) const {
		boost::array<double,1> that = {{v0}};
		evaluate_ callee;
		return callee(*this,that);
	}

	result_type operator()(double v0,double v1) const {
		boost::array<double,2> that = {{v0,v1}};
		evaluate_ callee;
		return callee(*this,that);
	}

	result_type operator()(double v0,double v1,double v2) const {
		boost::array<double,3> that = {{v0,v1,v2}};
		evaluate_ callee;
		return callee(*this,that);
	}
};

//----------------------------------------------------------------------
struct variable_tag {};

template<>
struct analytical_function_cases::case_< variable_tag >
	: proto::nullary_expr< variable_tag, proto::_ > {};

// プレースホルダ定義
analytical_expression< proto::nullary_expr< variable_tag, boost::mpl::int_<0> >::type > const _x;
analytical_expression< proto::nullary_expr< variable_tag, boost::mpl::int_<0> >::type > const _y;
analytical_expression< proto::nullary_expr< variable_tag, boost::mpl::int_<0> >::type > const _z;

template<>
struct evaluate_cases::case_< variable_tag >
	: proto::when< 
		// 条件
		proto::nullary_expr< variable_tag, proto::_ >, 
		// アクション
		fetch_variable(proto::_value,proto::_state)
	>
{};

//----------------------------------------------------------------------

struct constant_tag {};

template<int N>
struct constant_
	: analytical_expression< 
		typename proto::nullary_expr <
			constant_tag, 
			boost::mpl::int_<N>
		>::type
	>
{};

template<>
struct analytical_function_cases::case_< constant_tag >
	: proto::unary_expr< constant_tag, proto::_ >
{};

template<>
struct evaluate_cases::case_< constant_tag >
	: proto::when< 
		// 条件
		proto::nullary_expr< constant_tag, proto::_ >, 
		// アクション
		proto::_value
	>
{};

//----------------------------------------------------------------------

struct differential_cases
{
	// The primary template matches nothing
	template<typename Tag>
	struct case_ : proto::not_<proto::_> {};
};

struct differential_ : proto::switch_< differential_cases > {};

// terminal
template<>
struct differential_cases::case_<proto::tag::terminal>
	: proto::when< 
		proto::and_< 
			proto::terminal< proto::_ >, 
			proto::if_< 
				boost::is_convertible< 
					double, 
					proto::_value
				>()
			>
		>, 
		constant_<0>()
	>
{};

// constant
template<>
struct differential_cases::case_<constant_tag>
	: proto::when< 
		proto::nullary_expr< 
			constant_tag, 
			proto::_ 
		>, 
		constant_<0>()
	>
{};


// variable
template<>
struct differential_cases::case_< variable_tag >
	: proto::when< 
		proto::nullary_expr< variable_tag, proto::_ >, 
		proto::if_< 
			boost::is_same< 
				proto::_value, 
				proto::_value(proto::_state)
			>(), 
			constant_<1>(), 
			constant_<0>()
		>
	>
{};

// +
template<>
struct differential_cases::case_< proto::tag::plus >
	: proto::when< 
		proto::plus< differential_cases, differential_cases >, 
		proto::functional::make_plus( 
			differential_(proto::_left), 
			differential_(proto::_right)
		)
	>
{};

// *
template<>
struct differential_cases::case_< proto::tag::multiplies >
	: proto::when< 
		proto::plus< differential_cases, differential_cases >, 
		proto::call<
			proto::functional::make_plus( 
				proto::call<
					proto::functional::make_multiplies( 
						proto::_left, 
						differential_(proto::_right)
					)
				>, 
				proto::call<
					proto::functional::make_multiplies( 
						differential_(proto::_left), 
						proto::_right
					)
				>
			)
		>
	>
{};

template<int N>
struct nth_differential {
	template<typename Sig> struct result;

	template<typename This,typename F, typename V>
	struct result<This(F,V)> {
		typedef typename boost::result_of<differential_(F, V)>::type base;
		typedef typename boost::result_of<nth_differential<N-1>(base const&, V)>::type type;
	};

	template<typename F, typename V> inline
	typename result<nth_differential<N>(F const&, V const&)>::type
	operator()( F const& f, V const& v ) const {
		nth_differential<N-1> rec;
		differential_ callee;
		return rec(callee(f,v),v);
	}
};

template<>
struct nth_differential<1> {
	template<typename Sig> struct result;

	template<typename This,typename F, typename V>
	struct result<This(F,V)> : boost::result_of<differential_(F, V)> {};

	template<typename F, typename V> inline
	typename result<nth_differential<1>(F const&, V const&)>::type
	operator()( F const& f, V const& v ) const {
		differential_ callee;
		return callee(f,v);
	}
};


template<int Degree,typename Function, typename Variable> inline
typename boost::result_of<nth_differential<Degree>(Function const&, Variable const&)>::type
differential( Function const& f, Variable const& v )
{
	nth_differential<Degree> callee;
	return callee(f,v);
}

template<typename Function, typename Variable> inline
typename boost::result_of<differential_(Function const&, Variable const&)>::type
differential( Function const& f, Variable const& v )
{
	differential_ callee;
	return callee(f,v);
}

//----------------------------------------------------------------------

int main() {
	std::cout << differential( _x + _x, _x )(5)         << "\n"; // x + x => 1 + 1 =  2
	std::cout << differential( _x * _x, _x )(5)         << "\n"; // x^2   => 2*x   = 10
	std::cout << differential( _x * _x * _x, _x )(5)    << "\n"; // x^3   => 3*x^2 = 75
	std::cout << differential<2>( _x * _x * _x, _x )(5) << "\n"; // x^3   => 3*x^2 => 6*x = 30
}
