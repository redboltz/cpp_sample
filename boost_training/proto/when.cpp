#include <iostream>

#include <boost/proto/proto.hpp>
#include <boost/array.hpp>

struct variable_tag {};

struct  evaluate_;
struct  evaluate_cases
{
  // The primary template goes for the _default:
  template<typename Tag>
  struct case_ : boost::proto::_default<evaluate_> {};
};
struct  evaluate_ : boost::proto::switch_< evaluate_cases > {};


struct  derivate_cases
{
  // The primary template matches nothing
  template<typename Tag>
  struct case_ : boost::proto::not_<boost::proto::_> {};
};


struct  derivate_ : boost::proto::switch_< derivate_cases > {};

template<>
struct    derivate_cases
        ::case_< boost::proto::tag::multiplies >
        : boost::proto::when
          < boost::proto::plus< derivate_cases, derivate_cases >
          , boost::proto::functional::make_plus
            ( 
			  boost::proto::_left,
			  boost::proto::_left
#if 0
			  boost::proto::functional::make_plus
			  (
			    boost::proto::_left,
			    boost::proto::_left
			  ),
			  boost::proto::functional::make_plus
			  (
			    boost::proto::_left,
			    boost::proto::_left
			  )
//#if 0
			  boost::proto::functional::make_multiplies
              ( boost::proto::_left
              , derivate_(boost::proto::_right)
              )
            , boost::proto::functional::make_multiplies
              ( derivate_(boost::proto::_left)
              , boost::proto::_right
              )
#endif
            )
          >
{};

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
};

analytical_expression< boost::proto::nullary_expr< variable_tag,boost::mpl::int_<0> >::type > const _x;

template<typename Function, typename Variable> inline
typename boost::result_of<derivate_(Function const&, Variable const&)>::type
derivate( Function const& f, Variable const& v )
{
  derivate_ callee;
  return callee(f,v);
}


int main() {
  std::cout << derivate( _x, _x )(1.5)         << "\n";
}
