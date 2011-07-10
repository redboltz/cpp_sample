#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix.hpp>

struct Result {
	Result():parentDepth(0) {}
	int parentDepth;
	std::vector<std::string> path;
	std::string elem;
};

BOOST_FUSION_ADAPT_STRUCT(
    Result,
	(int, parentDepth)
    (std::vector<std::string>, path)
    (std::string, elem)
)

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

template <typename Iterator>
struct path_like_grammar 
	: qi::grammar<Iterator, Result()> {

	path_like_grammar():path_like_grammar::base_type(path_like, "path_like") {
		path_like = parentDepth >> path >> elem;
		parentDepth = *qi::lit("../")[ qi::_val += 1 ];
		path = 
			*(
				(
					qi::lexeme[*(qi::char_ - '/')] >> '/'
				)
				[phx::push_back(qi::_val, phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1)))]
			)
			>> (
					qi::lexeme[*(qi::char_ - ':')] >> ':'
				)
				[phx::push_back(qi::_val, phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1)))];
		elem = +(qi::char_ );
#if 1
		qi::on_error<qi::fail>
		(
			path_like
		  , std::cout
		        << phx::val("Error! Expecting ")
				<< qi::_4                               // what failed?
				<< phx::val(" here: \"")
				<< phx::construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
				<< phx::val("\"")
				<< std::endl
		);
#endif
	}
	qi::rule<std::string::const_iterator, Result()> path_like;
	qi::rule<std::string::const_iterator, int()> parentDepth;
	qi::rule<std::string::const_iterator, std::vector<std::string>()> path;
	qi::rule<std::string::const_iterator, std::string()> elem;

};


namespace {

void test1() {
	typedef path_like_grammar<std::string::const_iterator> path_like;
	std::string s = "../../abc/def:ghi";
	Result result;
	path_like pl;
	bool rval = qi::parse(
		s.begin(), 
		s.end(), 
		pl,
		result);
}

void test2() {
	std::string s = "../../abc/def:ghi";
	Result result;
	qi::rule<std::string::const_iterator, int()> parentDepth
		= *qi::lit("../")[ qi::_val += 1 ];
	qi::rule<std::string::const_iterator, std::vector<std::string>()> path
		=  *((qi::lexeme[*(qi::char_ - '/')] >> '/')
			[phx::push_back(qi::_val, phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1)))]
			)
			>>
			(qi::lexeme[*(qi::char_ - ':')] >> ':')
			[phx::push_back(qi::_val, phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1)))];
	qi::rule<std::string::const_iterator, std::string()> elem = +(qi::char_ );
	qi::rule<std::string::const_iterator, Result()> path_like = parentDepth >> path >> elem;
	bool rval = qi::parse(
		s.begin(), 
		s.end(), 
		path_like,
		result);
}

}

int main() {
	test1();
	test2();
}
