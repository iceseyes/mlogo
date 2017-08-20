/**
 * @file: parser_impl.hpp
 *
 */
#ifndef PARSER_IMPL_HPP
#define PARSER_IMPL_HPP

#include "parser.hpp"

#include <boost/optional/optional_io.hpp>

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

		start = +(alnum | (punct - '[' - ']' - '(' - ')'));
	}

	qi::rule<Iterator, std::string()> start;
};

template<typename Iterator>
struct WordParser: qi::grammar<Iterator, Word()> {
	WordParser() :
		WordParser::base_type(start, "Word") {
	    using ascii::alnum;
        using ascii::punct;

        word = +(alnum | punct);
        start = '"' >> word;
	}

	qi::rule<Iterator, std::string()> word;
	qi::rule<Iterator, Word()> start;
};

template<typename Iterator>
struct VariableParser: qi::grammar<Iterator, Variable()> {
	VariableParser() :
		VariableParser::base_type(start, "Variable") {
		start = ':' >> simpleWord;
	}

	SimpleWordParser<Iterator> simpleWord;
	qi::rule<Iterator, Variable()> start;
};

template<typename Iterator>
struct ListParser: qi::grammar<Iterator, List()> {
	ListParser() :
		ListParser::base_type(start, "List") {
		using qi::lexeme;
		using qi::char_;
		using ascii::alnum;
		using ascii::alpha;
		using ascii::space;
		using ascii::punct;
		using phoenix::val;
		using phoenix::construct;
		using phoenix::push_back;
		using namespace qi::labels;

		start = '[' >> *space >> +(word [push_back(ref(_val), _1)] >> *space) >> ']';
	}

    SimpleWordParser<Iterator> word;
	qi::rule<Iterator, List()> start;
};

template<typename Iterator>
struct ProcNameParser: qi::grammar<Iterator, ProcName()> {
	ProcNameParser() :
		ProcNameParser::base_type(start, "ProcName") {
		using ascii::alnum;
		using ascii::alpha;

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
		using ascii::char_;
		using phoenix::val;
		using namespace qi::labels;

		number = +digit [ref(_val) += _1] ||
					+(char_('.') [ref(_val) += _1] >>
					digit [ref(_val) += _1] >>
					*digit [ref(_val) += _1]);
		start = number;
	}

	qi::rule<Iterator, std::string()> number;
	qi::rule<Iterator, Number()> start;
};

template<typename Iterator>
struct ExpressionParser: qi::grammar<Iterator, Expression(), ascii::space_type> {
    struct make_expression_impl {
        template <typename Sig>
        struct result;

        template <typename This, typename Left, typename Right>
        struct result<This(const Left &, const Right &)> {
            using type = Left;
        };

        template <typename Left, typename Right>
        Left operator()(const Left &left, const Right &right) const {
            if(right) {
                Left l(*right);
                l.children.insert(l.children.begin(), left);
                return l;
            }

            return left;
        }
    };

    phoenix::function<make_expression_impl> make_expression;

    ExpressionParser() :
        ExpressionParser::base_type(start, "Expression") {
        using ascii::digit;
        using ascii::char_;
        using phoenix::val;
        using phoenix::at_c;
        using phoenix::push_back;
        using phoenix::construct;
        using namespace qi::labels;

        // In UCBLogo if you have a variable like :a45+4, :45+4+4 is a valid expression.
        // In mLogo this is not possible, or at least you should handle this case like a variable
        // and manage the expression when you look up in the memory.

        argument = word | proc_name | list | start;

        function = proc_name[at_c<0>(_val) = _1] >>
                        *argument[push_back(at_c<1>(_val), _1)];

        simply_expression = number [_val = construct<Expression>(_1)]
                | variable [_val = construct<Expression>(_1)]
                | function [_val = construct<Expression>(_1)]
                | ('(' >> start >> ')') [_val = construct<Expression>(_1)]
                | ('-' >> start) [_val = construct<Expression>(_1)];

        expression = (char_("+*/-") >> start) [ (_val = _1) << _2 ] >> -(expression [ref(_val) << _1]);

        start = (simply_expression >> -expression) [ _val = make_expression(_1, _2)];
    }

    WordParser<Iterator> word;
    NumberParser<Iterator> number;
    VariableParser<Iterator> variable;
    ProcNameParser<Iterator> proc_name;
    ListParser<Iterator> list;
    qi::rule<Iterator, Argument(), ascii::space_type> argument;
    qi::rule<Iterator, Statement(), ascii::space_type> function;
    qi::rule<Iterator, Expression(), ascii::space_type> simply_expression;
    qi::rule<Iterator, Expression(), ascii::space_type> expression;
    qi::rule<Iterator, Expression(), ascii::space_type> start;
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

		// We prefere that a procedure call is not consider an expression unless
		// call happens in the expression. For example, "ln 5" is not an expression, but
		// "5 + ln 5" is so. So, we select proc_name before expression in argument rule.
		argument = word | proc_name | list | expression;
		start = proc_name[at_c<0>(_val) = _1] >>
				*argument[push_back(at_c<1>(_val), _1)];
	}

	WordParser<Iterator> word;
	ProcNameParser<Iterator> proc_name;
	ListParser<Iterator> list;
	ExpressionParser<Iterator> expression;
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
