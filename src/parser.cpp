/**
 * @file: parser.cpp
 *
 *  Created on: 15 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include "parser.hpp"
#include "parser_impl.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/algorithm/string.hpp>

namespace mlogo {

namespace parser {

using iterator_type = std::string::const_iterator;
using Parser = StatementParser<iterator_type>;

/// To disambiguate the minus symbol, for unary operator we use this
static const char MINUS_SYMBOL{'_'};

const Expression Expression::MINUS{MINUS_SYMBOL};

namespace {

/**
 * Return the procedure name to use for operator computation.
 *
 * @param op operator symbol (+, -, *, /, ...)
 * @return a string, which is the name of a built-in procedure
 * @throws std::logic_error if oprator is unknown;
 */
std::string operator2proc_name(char op) {
    switch (op) {
    case '+':
        return "SUM";
        break;
    case '-':
        return "DIFFERENCE";
        break;
    case '*':
        return "PRODUCT";
        break;
    case '/':
        return "QUOTIENT";
        break;
    case '=':
        return "EQUALP";
        break;
    case MINUS_SYMBOL:
        return "MINUS";
        break;
    }

    std::stringstream ss;
    ss << "Unknown infix operator: " << op;
    throw std::logic_error(ss.str());
}

} /* ns: - */

Number::Number(long l) {
    std::stringstream ss;
    ss << l;
    value = ss.str();
}

Expression::Expression() {}

Expression::Expression(const Number &name)
    : name(name.value), node(Node::NUMBER) {}

Expression::Expression(const Variable &name)
    : name(name.name), node(Node::VARIABLE) {}

Expression::Expression(char functor)
    : name(operator2proc_name(functor)), node(Node::FUNCTION) {}

Expression::Expression(const Statement &function)
    : name(function.name.name),
      node(Node::STATEMENT),
      stmt(new Statement(function)) {
}  // Statement is stored as-it-is and not unpacked.

Expression::Expression(const Expression &e)
    : name(e.name), node(e.node), children(e.children) {
    if (e.stmt) stmt = new Statement(*e.stmt);
}

Expression::Expression(Expression &&e)
    : name(std::move(e.name)), node(e.node), children(std::move(e.children)) {
    if (e.stmt) {
        stmt = e.stmt;
        e.stmt = nullptr;
    }
}

Expression::~Expression() { delete stmt; }

Expression &Expression::operator=(const Expression &e) {
    name = e.name;
    node = e.node;
    children = e.children;

    delete stmt;
    stmt = nullptr;

    if (e.stmt) stmt = new Statement(*e.stmt);

    return *this;
}

Expression &Expression::operator=(Expression &&e) {
    name = std::move(e.name);
    node = e.node;
    children = std::move(e.children);

    delete stmt;
    stmt = nullptr;

    if (e.stmt) {
        stmt = e.stmt;
        e.stmt = nullptr;
    }

    return *this;
}

bool Expression::operator==(const Expression &b) const {
    bool eq = std::equal(children.begin(), children.end(), b.children.begin(),
                         b.children.end());
    if (node == Node::STATEMENT)
        eq = eq && stmt && b.stmt && ((*stmt) == (*b.stmt));
    return eq && (name == b.name) && (node == b.node);
}

Expression &Expression::operator<<(const Expression &b) {
    if (node != Node::FUNCTION)
        throw std::logic_error("Only Function node can have children");

    children.push_back(b);

    return *this;
}

Statement Expression::statement() const {
    if (stmt) return *stmt;

    throw std::logic_error("This expression is not a statement.");
}

Number Expression::number() const {
    if (node == Node::NUMBER) return Number(name);

    throw std::logic_error("This expression is not a number.");
}

Variable Expression::variable() const {
    if (node == Node::VARIABLE) return Variable(name);

    throw std::logic_error("This expression is not a variable.");
}

ProcName Expression::functor() const {
    if (node == Node::FUNCTION) return ProcName(name);

    throw std::logic_error("This expression is not a function.");
}

std::string Expression::debug() const {
    std::stringstream ss;

    switch (node) {
    case Expression::Node::NUMBER:
        ss << "NUMBER:" << name;
        break;

    case Expression::Node::VARIABLE:
        ss << "VARIABLE:" << name;
        break;

    case Expression::Node::FUNCTION:
        ss << "function:" << name;
        break;

    case Expression::Node::STATEMENT:
        if (stmt)
            ss << *stmt;
        else
            throw std::logic_error(
                "Node Statement does not define any statement");
    }

    for (auto &child : children) ss << " (" << child.debug() << ") ";

    return ss.str();
}

Statement::Statement(const Statement &stmt)
    : name(stmt.name), arguments(stmt.arguments) {}

Statement::Statement(Statement &&stmt)
    : name(stmt.name), arguments(std::move(stmt.arguments)) {}

Statement &Statement::operator=(const Statement &stmt) {
    name = stmt.name;
    arguments = stmt.arguments;

    return *this;
}

Statement &Statement::operator=(Statement &&stmt) {
    name = std::move(stmt.name);
    arguments = std::move(stmt.arguments);

    return *this;
}

bool Statement::operator==(const Statement &b) const {
    std::stringstream ss, ss1;
    ss << *this;
    ss1 << b;

    return ss.str() == ss1.str();
}

bool Statement::isStartProcedure() const {
    return boost::to_upper_copy(name.name) == START_PROCEDURE_KEYWORD;
}

bool Statement::isEndProcedure() const {
    return boost::to_upper_copy(name.name) == END_PROCEDURE_KEYWORD;
}

Procedure::Procedure(const Statement &prototype) {
    if (prototype.isStartProcedure())
        this->prototype = prototype;
    else
        throw std::logic_error("Statement is not a procedure definition.");
}

bool Procedure::addLine(const std::string &line) {
    if (completed) throw std::logic_error("Procedure is already closed.");

    auto stmt = parse(line);
    if (stmt.isStartProcedure()) {
        throw std::logic_error("Nested procedures are not supported.");
    } else if (stmt.isEndProcedure()) {
        completed = true;
        return true;
    }

    lines.push_back(stmt);
    return false;
}

std::string Procedure::name() const {
    return boost::get<ProcName>(prototype.arguments[0]).name;
}

std::size_t Procedure::nParams() const {
    return prototype.arguments.size() - 1;
}

std::vector<Variable> Procedure::parameters() const {
    std::vector<Variable> parameters;
    std::for_each(
        prototype.arguments.begin() + 1, prototype.arguments.end(),
        [&parameters](const Argument &a) {
            parameters.push_back(boost::get<Expression>(a).variable());
        });

    return parameters;
}

Statement parse(const std::string &line) {
    auto stmt = parse<StatementParser, Statement>(line);

    if (stmt.isStartProcedure()) {
        if (stmt.arguments.size() > 0) {
            try {
                boost::get<ProcName>(stmt.arguments[0]);
            } catch (std::exception &e) {
                throw std::logic_error("Procedure name must be a valid word.");
            }

            // parser doesn't accept any other
            // token but variables after TO keyword
            uint16_t i{0};
            for (i = 1; i < stmt.arguments.size(); ++i)
                boost::get<Expression>(stmt.arguments[i]).variable();
        } else {
            throw std::logic_error("Procedure must have a name!");
        }
    }

    return stmt;
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
    s << ":" << n.name;
    return s;
}

::std::ostream &operator<<(::std::ostream &s, const Expression &n) {
    switch (n.node) {
    case Expression::Node::NUMBER:
    case Expression::Node::VARIABLE:
    case Expression::Node::FUNCTION:
        s << n.name;
        break;
    case Expression::Node::STATEMENT:
        if (n.stmt)
            s << *n.stmt;
        else
            throw std::logic_error(
                "Node Statement does not define any statement");
    }

    for (auto &child : n.children) s << " " << child;

    return s;
}

::std::ostream &operator<<(::std::ostream &s, const ProcName &n) {
    s << n.name;
    return s;
}

::std::ostream &operator<<(::std::ostream &s, const List &n) {
    char space = 0;

    s << "[";
    for (auto &v : n.items) {
        if (space) s << space;
        space = ' ';
        s << v;
    }
    s << "]";

    return s;
}

::std::ostream &operator<<(::std::ostream &s, const Statement &n) {
    s << n.name;
    for (auto &a : n.arguments) s << " " << a;

    return s;
}

::std::ostream &operator<<(::std::ostream &s, const Procedure &p) {
    s << START_PROCEDURE_KEYWORD << " " << p.name();
    for (auto &param : p.parameters()) s << " " << param;
    s << std::endl;

    for (auto &stmt : p.lines) s << stmt << std::endl;
    s << END_PROCEDURE_KEYWORD;

    return s;
}

bool operator!=(const ProcName &a, const ProcName &b) { return !(a == b); }

bool operator==(const ProcName &a, const ProcName &b) {
    return a.name == b.name;
}

bool operator==(const std::string &a, const ProcName &b) { return a == b.name; }

} /* ns: parser */

} /* ns: mlogo */
