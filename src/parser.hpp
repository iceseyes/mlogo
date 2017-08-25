/**
 * @file: parser.hpp
 *
 *  Created on: 16 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#ifndef PARSER_HPP
#define PARSER_HPP

#include <sstream>
#include <string>
#include <vector>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>

#include "defines.hpp"

namespace mlogo {

namespace parser {

struct Statement;

struct Word {
    std::string name;

    Word() {}

    Word(const std::string &name) : name(name) {}

    Word(const char ch) { name += ch; }

    bool operator!=(const Word &b) const { return !(*this == b); }
    bool operator==(const Word &b) const { return name == b.name; }
};

struct Number {
    std::string value;

    Number() {}

    Number(const std::string &value) : value(value) {}

    bool operator!=(const Number &b) const { return !(*this == b); }
    bool operator==(const Number &b) const { return value == b.value; }
};

struct Variable {
    std::string name;

    Variable() {}

    Variable(const std::string &name) : name(name) {}

    bool operator!=(const Variable &b) const { return !(*this == b); }
    bool operator==(const Variable &b) const { return name == b.name; }
};

struct ProcName {
    std::string name;

    ProcName() {}
    ProcName(const ProcName &proc) : name(proc.name) {}
    ProcName(ProcName &&proc) : name(std::move(proc.name)) {}
    ProcName(const std::string &name) : name(name) {}

    ProcName &operator=(const ProcName &proc) {
        name = proc.name;
        return *this;
    }

    ProcName &operator=(ProcName &&proc) {
        name = std::move(proc.name);
        return *this;
    }
    bool operator!=(const ProcName &b) const { return !(*this == b); }
    bool operator==(const ProcName &b) const { return name == b.name; }
    operator bool() const { return !name.empty(); }
};

struct List {
    std::vector<Word> items;

    List() {}

    bool operator!=(const List &b) const { return !(*this == b); }
    bool operator==(const List &b) const { return items == b.items; }

    void push_back(const Word &w) { items.push_back(w); }
    void push_back(Word &&w) { items.push_back(w); }
};

struct Expression {
    enum class Node { NUMBER, VARIABLE, FUNCTION, STATEMENT };

    std::string name{0};
    Node node{Node::NUMBER};
    std::vector<Expression> children;

    Expression();
    Expression(const Number &name);
    Expression(const Variable &name);
    Expression(char functor);
    Expression(const Statement &function);

    Expression(const Expression &e);
    Expression(Expression &&e);

    ~Expression();

    Expression &operator=(const Expression &e);
    Expression &operator=(Expression &&e);

    bool operator!=(const Expression &b) const { return !(*this == b); }
    bool operator==(const Expression &b) const;
    Expression &operator<<(const Expression &b);

    Statement statement() const;
    Number number() const;
    Variable variable() const;
    ProcName functor() const;

    static const Expression MINUS;  ///< The expression for unary minus
private:
    Statement *stmt{nullptr};

    friend ::std::ostream &operator<<(::std::ostream &s, const Expression &n);
};

using Argument = boost::variant<ProcName, Word, List, Expression, Statement>;

struct Statement {
    ProcName name;
    std::vector<Argument> arguments;

    Statement() {}
    Statement(const Statement &stmt);
    Statement(Statement &&stmt);
    Statement(const std::string &name) { this->name = name; }

    Statement &operator=(const Statement &stmt);
    Statement &operator=(Statement &&stmt);

    bool operator!=(const Statement &b) const { return !(*this == b); };
    bool operator==(const Statement &b) const;
    operator bool() const { return name; }

    bool isStartProcedure() const;
    bool isEndProcedure() const;
};

struct Procedure {
    Statement prototype;
    std::vector<Statement> lines;

    Procedure(const Statement &prototype);

    bool addLine(const std::string &line);

    std::string name() const;

    std::size_t nParams() const;
    std::vector<Variable> parameters() const;

private:
    bool completed{false};
};

Statement parse(const std::string &line);

::std::ostream &operator<<(::std::ostream &s, const Word &n);
::std::ostream &operator<<(::std::ostream &s, const Number &n);
::std::ostream &operator<<(::std::ostream &s, const Variable &n);
::std::ostream &operator<<(::std::ostream &s, const Expression &n);
::std::ostream &operator<<(::std::ostream &s, const ProcName &n);
::std::ostream &operator<<(::std::ostream &s, const List &n);
::std::ostream &operator<<(::std::ostream &s, const Statement &n);
::std::ostream &operator<<(::std::ostream &s, const Procedure &n);

struct DisplayArgumentVisitor : boost::static_visitor<std::string> {
    template <typename Value>
    std::string operator()(Value &&v) const {
        return v.name;
    }

    std::string operator()(Number &v) const { return v.value; }

    std::string operator()(List &v) const {
        std::stringstream ss;
        ss << v;
        return ss.str();
    }
};

} /* ns: parser */

} /* ns: mlogo */

// clang-format off
BOOST_FUSION_ADAPT_STRUCT(
        mlogo::parser::Statement,
        (mlogo::parser::ProcName, name)
        (std::vector<mlogo::parser::Argument>, arguments))
// clang-format on

#endif
