#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

template <typename Iterator>
struct xml_replacer 
    : qi::grammar<Iterator, std::string()> {
    xml_replacer():xml_replacer::base_type(all) {
        all = *(+(qi::char_ - '"') 
            >> qi::char_('"')
            >> +expr 
            >> qi::char_('"'));
        expr
            = qi::string("&amp;")  [qi::_val += qi::_1]
            | qi::string("&quot;") [qi::_val += qi::_1]
            | qi::string("&lt;")   [qi::_val += qi::_1]
            | qi::string("&gt;")   [qi::_val += qi::_1]
            | qi::char_('&') [qi::_val += "&amp;"]
         // | qi::char_('"') [qi::_val += "&quot;"]
            | qi::char_('<') [qi::_val += "&lt;"]
            | qi::char_('>') [qi::_val += "&gt;"]
            | text [qi::_val += qi::_1];
        text %= qi::lexeme[+(qi::char_ - qi::char_("&\"<>"))];
#if 1
        qi::on_error<qi::fail>
        (
            all
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
    qi::rule<std::string::const_iterator, std::string()> all;
    qi::rule<std::string::const_iterator, std::string()> expr;
    qi::rule<std::string::const_iterator, std::string()> text;
};

template <typename Iterator>
struct xml_revert_replacer 
    : qi::grammar<Iterator, std::string()> {
    xml_revert_replacer():xml_revert_replacer::base_type(all) {
        all %= *(+(qi::char_ - '"') 
            >> qi::char_('"')
            >> +expr 
            >> qi::char_('"'));
        expr
            = qi::string("&amp;")  [qi::_val += '&']
         // | qi::string("&quot;") [qi::_val += '"']
            | qi::string("&lt;")   [qi::_val += '<']
            | qi::string("&gt;")   [qi::_val += '>']
            | text [qi::_val += qi::_1];
        text %= qi::lexeme[+(qi::char_ - qi::char_("&\"<>"))];
#if 1
        qi::on_error<qi::fail>
        (
            all
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
    qi::rule<std::string::const_iterator, std::string()> all;
    qi::rule<std::string::const_iterator, std::string()> expr;
    qi::rule<std::string::const_iterator, std::string()> text;
};


namespace {

void test1() {
    xml_replacer<std::string::const_iterator> xr;
    xml_revert_replacer<std::string::const_iterator> xrr;
    std::string s = "<Tag attr1=\"C1&C2>C3\" attr2=\"C1&C2<C3\"/> <Hoge attr3=\"C&D\">abc</Hoge>&";

    std::string result1;
    bool rval1 = qi::phrase_parse(
        s.begin(), 
        s.end(), 
        xr,
        qi::ascii::space,
        result1);

    std::string result2;
    bool rval2 = qi::phrase_parse(
        result1.begin(), 
        result1.end(), 
        xrr,
        qi::ascii::space,
        result2);

    std::string result3;
    bool rval3 = qi::phrase_parse(
        result1.begin(), 
        result1.end(), 
        xr,
        qi::ascii::space,
        result3);

}

}

int main() {
    test1();
}
