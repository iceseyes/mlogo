/**
 * @file: parser.hpp
 *
 *  Created on: 16 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>

namespace mlogo {
namespace parser {

struct Word {
	std::string name;

	Word() {}

	Word(const std::string &name) :
			name(name) {}
};

struct Number {
	std::string value;

	Number() {}

	Number(const std::string &value) :
			value(value) {}
};

struct Variable {
	std::string name;

	Variable() {}

	Variable(const std::string &name) :
			name(name) {}
};

struct ProcName {
	std::string name;

	ProcName() {}

	ProcName(const std::string &name) :
			name(name) {}
};

using Argument_t = boost::variant<ProcName, Word, Number, Variable>;

struct Argument {
 Argument_t arg;
};

struct Statement {
	ProcName name;
	std::vector<Argument> arguments;

	Statement() {}

	Statement(const std::string &name) {
		this->name = name;
	}

};

Statement parse(const std::string &line);

}

}

BOOST_FUSION_ADAPT_STRUCT(
	mlogo::parser::Argument,
	(mlogo::parser::Argument_t, arg)
)

BOOST_FUSION_ADAPT_STRUCT(
	mlogo::parser::Statement,
	(mlogo::parser::ProcName, name)
	(std::vector<mlogo::parser::Argument>, arguments)
)

#endif
