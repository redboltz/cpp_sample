#include <string>
#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/vector.hpp>

namespace qi = boost::spirit::qi;

void test01() {
	std::string str = "(1,2)";
	//typedef boost::fusion::vector<std::string> value_type;
	typedef boost::fusion::vector<int, int> value_type;
	//typedef std::vector<int> value_type;
	value_type result;
	if (qi::phrase_parse(str.begin(), str.end(),
		qi::lit("(") >> qi::int_ >> qi::lit(",") >> qi::int_ >> qi::lit(")"),
		qi::ascii::space, // “r’†‚É‹ó”’‚ð“ü‚ê‚Ä‚à–³Ž‹‚·‚é
		result)) {
		std::cout << boost::fusion::at_c<0>(result);
		std::cout << boost::fusion::at_c<1>(result);
	}
}

int main() {
	test01();
}
