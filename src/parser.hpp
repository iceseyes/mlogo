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
#include <sstream>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>

namespace mlogo {
namespace parser {

struct Word {
	std::string name;

	Word() {}

	Word(const std::string &name) :
			name(name) {}

	bool operator!=(const Word &b) const { return !(*this == b); }
	bool operator==(const Word &b) const { return name==b.name; }
};

struct Number {
	std::string value;

	Number() {}

	Number(const std::string &value) :
			value(value) {}

	bool operator!=(const Number &b) const { return !(*this == b); }
	bool operator==(const Number &b) const { return value==b.value; }
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

using Argument = boost::variant<ProcName, Word, Number, Variable>;

struct DisplayArgumentVisitor : boost::static_visitor<std::string> {
	template<typename Value>
	std::string operator()(Value &&v) const {
		return v.name;
	}

	std::string operator()(Number &v) const {
		return v.value;
	}
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

::std::ostream &operator<<(::std::ostream &s, const Word &n);
::std::ostream &operator<<(::std::ostream &s, const Number &n);


}

}

BOOST_FUSION_ADAPT_STRUCT(
	mlogo::parser::Statement,
	(mlogo::parser::ProcName, name)
	(std::vector<mlogo::parser::Argument>, arguments)
)

#endif
