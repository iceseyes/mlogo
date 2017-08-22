/**
 * @file: parser.cpp
 *
 *  Created on: 15 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include "parser.hpp"
#include "parser_impl.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace mlogo {
namespace parser {

using iterator_type = std::string::const_iterator;
using Parser = StatementParser<iterator_type>;

/// To disambiguate the minus symbol, for unary operator we use this
static const char MINUS_SYMBOL { '_' };

const Expression Expression::MINUS { MINUS_SYMBOL };

namespace {

/**
 * Return the procedure name to use for operator computation.
 *
 * @param op operator symbol (+, -, *, /, ...)
 * @return a string, which is the name of a built-in procedure
 * @throws std::logic_error if oprator is unknown;
 */
std::string operator2proc_name(char op) {
    switch(op) {
    case '+': return "SUM"; break;
    case '-': return "DIFFERENCE"; break;
    case '*': return "PRODUCT"; break;
    case '/': return "QUOTIENT"; break;
    case MINUS_SYMBOL: return "MINUS"; break;
    }

    std::stringstream ss;
    ss << "Unknown infix operator: " << op;
    throw std::logic_error(ss.str());
}

}

Expression::Expression() {}

Expression::Expression(const Number &name) :
        name(name.value), node(Node::NUMBER) {}

Expression::Expression(const Variable &name) :
        name(name.name), node(Node::VARIABLE) {}

Expression::Expression(char functor) :
        name(operator2proc_name(functor)), node(Node::FUNCTION) {}

Expression::Expression(const Statement &function) :
       name(function.name.name), node(Node::STATEMENT),
       stmt(new Statement(function)) {} // Statement is stored as-it-is and not unpacked.

Expression::Expression(const Expression &e) :
    name(e.name), node(e.node), children(e.children) {
    if(e.stmt) stmt = new Statement(*e.stmt);
}

Expression::Expression(Expression &&e) :
    name(std::move(e.name)), node(e.node), children(std::move(e.children)) {
    if(e.stmt) {
        stmt = e.stmt;
        e.stmt = nullptr;
    }
}

Expression::~Expression() {
    delete stmt;
}

Expression& Expression::operator=(const Expression &e) {
    name = e.name;
    node = e.node;
    children = e.children;

    if(e.stmt) stmt = new Statement(*e.stmt);

    return *this;
}

Expression& Expression::operator=(Expression &&e) {
    name = std::move(e.name);
    node = e.node;
    children = std::move(e.children);

    if(e.stmt) {
        stmt = e.stmt;
        e.stmt = nullptr;
    }

    return *this;
}

bool Expression::operator==(const Expression &b) const {
    bool eq = std::equal(children.begin(), children.end(), b.children.begin(), b.children.end());
    if(node==Node::STATEMENT) eq = eq && stmt && b.stmt && ((*stmt)==(*b.stmt));
    return eq && (name==b.name) && (node==b.node);
}

Expression &Expression::operator<<(const Expression &b) {
    if(node!=Node::FUNCTION)
        throw std::logic_error("Only Function node can have children");

    children.push_back(b);

    return *this;
}

Statement Expression::statement() const {
    if(stmt) return *stmt;

    throw std::logic_error("This expression is not a statement.");
}

bool Statement::operator==(const Statement &b) const {
    std::stringstream ss, ss1;
    ss << *this;
    ss1 << b;

    return ss.str() == ss1.str();
}

Statement parse(const std::string &line) {
	return parse<StatementParser, Statement>(line);
}

::std::ostream &operator<<(::std::ostream &s, const Number &n) {
	s << n.value;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const Word &n) {
	s << n.name;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const Variable &n) {
	s << n.name;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const Expression &n) {
    s << "(";

    switch(n.node) {
    case Expression::Node::NUMBER:
    case Expression::Node::VARIABLE:
    case Expression::Node::FUNCTION:
        s << n.name;
        break;
    case Expression::Node::STATEMENT:
        if(n.stmt) s << *n.stmt;
        else throw std::logic_error("Node Statement does not define any statement");
        break;
    }

    for(auto &child : n.children) s << " " << child;
    s << ")";

    return s;
}

::std::ostream &operator<<(::std::ostream &s, const ProcName &n) {
	s << n.name;
	return s;
}

::std::ostream &operator<<(::std::ostream &s, const List &n) {
    char space = 0;
    for(auto &v : n.items) {
        if(space) s << space;
        space = ' ';
        s << v;
    }

    return s;
}

::std::ostream &operator<<(::std::ostream &s, const Statement &n) {
    s << n.name << " ( ";
    for(auto &a : n.arguments) s << "[" << a << "] ";
    s << ")";

    return s;
}
}
}

