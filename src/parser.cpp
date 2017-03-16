/**
 * @file: parser.cpp
 *
 *  Created on: 15 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include "parser.hpp"

#include <iostream>
#include <string>
#include <stdexcept>

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

		word = lexeme['"' >> +(alnum|punct)];
		variable = lexeme[':' >> +(alnum|punct)];
		proc_name = lexeme[+alpha];
		list = '[' >> *(+(alnum|punct)) >> ']';
		number = lexeme[+digit];

		argument = word|proc_name|variable|list|number;

		start =
			proc_name 	[at_c<0>(_val) = _1] >>
			*argument 	[push_back(at_c<1>(_val), _1)];
	}

	qi::rule<Iterator, std::string(), ascii::space_type> word;
	qi::rule<Iterator, std::string(), ascii::space_type> variable;
	qi::rule<Iterator, std::string(), ascii::space_type> proc_name;
	qi::rule<Iterator, std::string(), ascii::space_type> list;
	qi::rule<Iterator, std::string(), ascii::space_type> number;
	qi::rule<Iterator, Argument(), ascii::space_type> argument;
	qi::rule<Iterator, Statement(), ascii::space_type> start;
};

Statement parse(const std::string &line) {
	using iterator_type = std::string::const_iterator;

	Statement stmt;
	StatementParser<iterator_type> parser;
	iterator_type iter = line.begin();
	iterator_type end = line.end();

	bool r = phrase_parse(iter, end, parser, ascii::space, stmt);

	if (!r || iter != end) {
		std::string rest(iter, end);
		throw std::logic_error("Syntax Error at input line from: " + rest);
	}

	return stmt;
}

}
}

