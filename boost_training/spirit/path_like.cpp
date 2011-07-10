#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix.hpp>

struct Result {
	std::vector<std::string> path;
	std::string elem;
};

BOOST_FUSION_ADAPT_STRUCT(
    Result,
    (std::vector<std::string>, path)
    (std::string, elem)
)


int main() {
	std::string s = "../../abc/def:ghi";
	//std::string s = "../../abc";
	
	namespace qi = boost::spirit::qi;
	namespace phx = boost::phoenix;

	int i = 0;
	Result result;
	qi::rule<std::string::const_iterator, std::vector<std::string>()> path = 
		*((qi::lexeme[*(qi::char_ - '/')] >> '/')[phx::push_back(qi::_val, phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1)))])
       >> (qi::lexeme[*(qi::char_ - ':')] >> ':')[phx::push_back(qi::_val, phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1)))];
	qi::rule<std::string::const_iterator, std::string()> elem =
		*(qi::char_ - qi::eol) >> qi::eol;

	bool rval = qi::parse(
		s.begin(), 
		s.end(), 
		(*qi::lit("../")[ phx::ref(i) += 1 ] >> path >> elem
		),
		result);
	std::cout << i << std::endl;
}
