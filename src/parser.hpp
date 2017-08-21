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

struct Statement;

struct Word {
	std::string name;

	Word() {}

	Word(const std::string &name) :
			name(name) {}

	Word(const char ch) {
	    name += ch;
	}

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

	bool operator!=(const Variable &b) const { return !(*this == b); }
	bool operator==(const Variable &b) const { return name==b.name; }
};

struct ProcName {
	std::string name;

	ProcName() {}

	ProcName(const std::string &name) :
			name(name) {}

	bool operator!=(const ProcName &b) const { return !(*this == b); }
	bool operator==(const ProcName &b) const { return name==b.name; }
};

struct List {
	std::vector<Word> items;

	List() {}

	bool operator!=(const List &b) const { return !(*this == b); }
	bool operator==(const List &b) const { return items==b.items; }

	void push_back(const Word &w) { items.push_back(w); }
	void push_back(Word &&w) { items.push_back(w); }
};

struct Expression {
    enum class Node { NUMBER, VARIABLE, FUNCTION, STATEMENT };

    std::string name { 0 };
    Node node { Node::NUMBER };
    std::vector<Expression> children;

    Expression();
    Expression(const Number &name);
    Expression(const Variable &name);
    Expression(char functor);
    Expression(const Statement &function);

    Expression(const Expression &e);
    Expression(Expression &&e);

    ~Expression();

    Expression& operator=(const Expression &e);
    Expression& operator=(Expression &&e);

    bool operator!=(const Expression &b) const { return !(*this == b); };
    bool operator==(const Expression &b) const;
    Expression &operator+=(const Expression &b);
    Expression &operator<<(const Expression &b) { return (*this += b); }

private:
    Statement *stmt { nullptr };

    friend ::std::ostream &operator<<(::std::ostream &s, const Expression &n);
};

using Argument = boost::variant<ProcName, Word, Number, Variable, List, Expression>;

struct Statement {
	ProcName name;
	std::vector<Argument> arguments;

	Statement() {}

	Statement(const std::string &name) {
		this->name = name;
	}

    bool operator!=(const Statement &b) const { return !(*this == b); };
    bool operator==(const Statement &b) const;
};

Statement parse(const std::string &line);

::std::ostream &operator<<(::std::ostream &s, const Word &n);
::std::ostream &operator<<(::std::ostream &s, const Number &n);
::std::ostream &operator<<(::std::ostream &s, const Variable &n);
::std::ostream &operator<<(::std::ostream &s, const Expression &n);
::std::ostream &operator<<(::std::ostream &s, const ProcName &n);
::std::ostream &operator<<(::std::ostream &s, const List &n);
::std::ostream &operator<<(::std::ostream &s, const Statement &n);

struct DisplayArgumentVisitor : boost::static_visitor<std::string> {
	template<typename Value>
	std::string operator()(Value &&v) const {
		return v.name;
	}

	std::string operator()(Number &v) const {
		return v.value;
	}

    std::string operator()(List &v) const {
        std::stringstream ss;
        ss << v;
		return ss.str();
    }
};

}

}

BOOST_FUSION_ADAPT_STRUCT(
    mlogo::parser::Statement,
    (mlogo::parser::ProcName, name)
    (std::vector<mlogo::parser::Argument>, arguments)
)

#endif
