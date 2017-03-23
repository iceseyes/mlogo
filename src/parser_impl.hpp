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
struct SimpleWordParser: qi::grammar<Iterator, std::string()> {
	SimpleWordParser() :
		SimpleWordParser::base_type(start, "SimpleWord") {
		using ascii::alnum;
		using ascii::punct;

		start = alnum >> *(alnum | punct);
	}

	qi::rule<Iterator, std::string()> start;
};

template<typename Iterator>
struct WordParser: qi::grammar<Iterator, Word()> {
	WordParser() :
		WordParser::base_type(start, "Word") {
		using phoenix::val;
		using phoenix::at_c;
		using namespace qi::labels;

		start = '"' >> simpleWord;
	}

	SimpleWordParser<Iterator> simpleWord;
	qi::rule<Iterator, Word()> start;
};

template<typename Iterator>
struct VariableParser: qi::grammar<Iterator, Variable()> {
	VariableParser() :
		VariableParser::base_type(start, "Variable") {
		using phoenix::val;
		using phoenix::at_c;
		using namespace qi::labels;

		start = ':' >> simpleWord;
	}

	SimpleWordParser<Iterator> simpleWord;
	qi::rule<Iterator, Variable()> start;
};

template<typename Iterator>
struct ProcNameParser: qi::grammar<Iterator, ProcName()> {
	ProcNameParser() :
		ProcNameParser::base_type(start, "ProcName") {
		using ascii::alnum;
		using ascii::alpha;
		using phoenix::val;
		using phoenix::at_c;
		using namespace qi::labels;

		procname = alpha >> *alnum;
		start = procname;
	}

	qi::rule<Iterator, std::string()> procname;
	qi::rule<Iterator, ProcName()> start;
};

template<typename Iterator>
struct NumberParser: qi::grammar<Iterator, Number()> {
	NumberParser() :
			NumberParser::base_type(start, "Number") {
		using ascii::digit;


		number = +digit;
		start = number;
	}

	qi::rule<Iterator, std::string()> number;
	qi::rule<Iterator, Number()> start;
};

template<typename Iterator>
struct StatementParser: qi::grammar<Iterator, Statement(), ascii::space_type> {
	StatementParser() :
			StatementParser::base_type(start, "statement") {
		using ascii::alnum;
		using ascii::punct;

		using phoenix::val;
		using phoenix::construct;
		using phoenix::at_c;
		using phoenix::push_back;

		using namespace qi::labels;

		list = '[' >> *(+(alnum | punct)) >> ']';

		argument = word | proc_name | variable | number;

		start = proc_name[at_c<0>(_val) = _1] >>
				*argument[push_back(at_c<1>(_val), _1)];
	}

	WordParser<Iterator> word;
	NumberParser<Iterator> number;
	VariableParser<Iterator> variable;
	ProcNameParser<Iterator> proc_name;
	qi::rule<Iterator, std::string(), ascii::space_type> list;
	qi::rule<Iterator, Argument(), ascii::space_type> argument;
	qi::rule<Iterator, Statement(), ascii::space_type> start;
};

template<template<class > class Parser, typename Result = std::string>
Result parse(const std::string &line) {
	using iterator_type = std::string::const_iterator;
	Result stmt;
	Parser<iterator_type> parser;
	iterator_type iter = line.begin();
	iterator_type end = line.end();

	bool r = phrase_parse(iter, end, parser, ascii::space, stmt);

	if(!r || iter != end) {
		std::string rest(iter, end);
		throw std::logic_error("Syntax Error at input line from: " + rest);
	}

	return stmt;
}

} /* ns parser */

} /* ns mlogo */

#endif
