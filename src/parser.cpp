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

Expression::Expression(char functor) :
        node(Node::FUNCTION) {
    this->name += functor;
}

Expression::Expression(const Statement &function) :
        node(Node::FUNCTION) {
    std::stringstream ss;
    ss << function.name;
    for(auto &a : function.arguments) {
        ss << " " << a;
    }

    this->name = ss.str();
}

bool Expression::operator==(const Expression &b) const {
    bool eq = std::equal(children.begin(), children.end(), b.children.begin());
    return eq && (name==b.name) && (node==b.node);
}

Expression &Expression::operator+=(const Expression &b) { name += b.name; return *this; }

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
    s << n.name;
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

