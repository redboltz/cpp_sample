#include <string>
#include <boost/config/warning_disable.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/vector.hpp>
#include <vector>
#include <istream>
#include <sstream>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace classic = boost::spirit::classic;

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
		std::cout << std::endl;
	}
}

void test02() {
	std::string str = ")(abc,def)";
	std::stringstream ss(str);
	std::string const filename = "result.txt";
	//typedef boost::fusion::vector<std::string> value_type;
	typedef boost::fusion::vector<std::string, std::string> value_type;
	//typedef std::vector<int> value_type;
	// iterate over stream input
	typedef std::istreambuf_iterator<char> base_iterator_type;
	base_iterator_type in_begin(ss);
	
	// convert input iterator to forward iterator, usable by spirit parser
	typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
	forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
	forward_iterator_type fwd_end;

	// wrap forward iterator with position iterator, to record the position
	typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
	pos_iterator_type position_begin(fwd_begin, fwd_end, filename);
	pos_iterator_type position_end;

	value_type result;
	try {
		qi::phrase_parse(position_begin, position_end,
		qi::lit("(") > *(qi::char_ - qi::lit(",")) > qi::lit(",") > *(qi::char_ - qi::lit(")")) > qi::lit(")"),
		qi::ascii::space, result);
		std::cout << "hoge" << std::endl;
		std::cout << boost::fusion::at_c<0>(result) << std::endl;
		std::cout << boost::fusion::at_c<1>(result) << std::endl;
	}
	catch (const qi::expectation_failure<pos_iterator_type>& e) {
		std::cout << "fuga" << std::endl;
		const classic::file_position_base<std::string>& pos =
			e.first.get_position();
		std::stringstream msg;
		msg <<
			"parse error at file " << pos.file <<
			" line " << pos.line << " column " << pos.column << std::endl <<
			"'" << e.first.get_currentline() << "'" << std::endl <<
			std::setw(pos.column) << " " << "^- here";
		throw std::runtime_error(msg.str());
	}

}




int main() {
	test01();
	test02();
}
