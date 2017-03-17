/**
 * @file: parser_impl.hpp
 *
 */
#ifndef PARSER_IMPL_HPP
#define PARSER_IMPL_HPP

#include "parser.hpp"

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

namespace mlogo {
namespace parser {

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

template<typename Iterator>
struct WordParser: qi::grammar<Iterator, std::string()> {
  WordParser() : WordParser::base_type(start, "Word") {
    using ascii::alnum;
    using ascii::punct;

    start = '"' >> alnum >> *(alnum|punct);
  }

  qi::rule<Iterator, std::string()> start;
};

template<typename Iterator>
struct NumberParser: qi::grammar<Iterator, std::string()> {
  NumberParser() : NumberParser::base_type(start, "Number") {
    using ascii::digit;
    using ascii::char_;

    start = -(char_('-')|char_('+')) >> ((+digit) || (char_('.') >> *digit));
  }

  qi::rule<Iterator, std::string()> start;
};
  
template<typename Iterator>
struct StatementParser: qi::grammar<Iterator, Statement(), ascii::space_type> {
	StatementParser() : StatementParser::base_type(start, "statement") {
		using qi::lit;
		using qi::lexeme;
		using qi::on_error;
		using qi::fail;
		using ascii::char_;
		using ascii::alpha;
		using ascii::alnum;
		using ascii::digit;
		using ascii::space;
		using ascii::punct;

		using phoenix::val;
		using phoenix::construct;
		using phoenix::at_c;
		using phoenix::push_back;

		using namespace qi::labels;

		variable = lexeme[':' >> +(alnum|punct)];
		proc_name = lexeme[+alpha];
		list = '[' >> *(+(alnum|punct)) >> ']';

		argument = word|proc_name|variable|list|number;

		start =
			proc_name 	[at_c<0>(_val) = _1] >>
			*argument 	[push_back(at_c<1>(_val), _1)];
	}

  WordParser<Iterator> word;
  NumberParser<Iterator> number;
	qi::rule<Iterator, std::string(), ascii::space_type> variable;
	qi::rule<Iterator, std::string(), ascii::space_type> proc_name;
	qi::rule<Iterator, std::string(), ascii::space_type> list;
	qi::rule<Iterator, Argument(), ascii::space_type> argument;
	qi::rule<Iterator, Statement(), ascii::space_type> start;
};


}
  
}

#endif