/**
 * @file: eval.cpp
 *
 *  created on:  3 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "eval.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/variant.hpp>

#include "memory.hpp"

#include "eval_impl.hpp"

using namespace std;

namespace mlogo {

namespace eval {

using Stack = memory::Stack;

ASTNode make_statement(const mlogo::parser::Statement &stmt) {
    ASTNode s{new ASTNode::Procedure(stmt.name.name)};
    impl::EvalStmtBuilderVisitor v(&s);

    for (auto a : stmt.arguments) {
        boost::apply_visitor(v, a);
    }

    return s;
}

AST make_ast(const mlogo::parser::Statement &stmt) {
    AST ast;
    mlogo::parser::Argument proc{stmt.name};
    impl::EvalStmtBuilderVisitor v(&ast);

    boost::apply_visitor(v, proc);
    for (auto a : stmt.arguments) {
        boost::apply_visitor(v, a);
    }

    return ast;
}

ASTNode::Procedure::Procedure(const string &name)
    : procName(name), _nargs(Stack::instance().getProcedureNArgs(name)) {}

ValueBox ASTNode::Procedure::value(const ASTNode *current) const {
    memory::ActualArguments args;
    for (auto child : current->children) {
        args.push_back((*child)());
    }

    Stack::instance().currentFrame().setVariable(
        "__internal__returned__value__captured__", "");
    Stack::instance().callProcedure(procName, args,
                                    "__internal__returned__value__captured__");
    return Stack::instance().currentFrame().getVariable(
        "__internal__returned__value__captured__");
}

ASTNode::Variable::Variable(const string &name) : varName(name) {}

ValueBox ASTNode::Variable::value(const ASTNode *) const {
    return Stack::instance().getVariable(varName);
}

ASTNode::ASTNode(Type *t, ASTNode *parent) : type(t), _parent(parent) {
    if (_parent) {
        _parent->children.push_back(this);
    }
}

ASTNode::ASTNode(ASTNode &&stmt)
    : type(stmt.type), _parent(stmt._parent), children(move(stmt.children)) {
    stmt.type = nullptr;
    stmt._parent = nullptr;
    stmt.children.clear();
}

ASTNode::~ASTNode() {
    for (auto ptr : children) delete ptr;

    delete type;
}

ASTNode &ASTNode::operator=(ASTNode &&stmt) {
    type = stmt.type;
    _parent = stmt._parent;
    children = move(stmt.children);
    stmt.type = nullptr;
    stmt._parent = nullptr;
    stmt.children.clear();

    return *this;
}

ValueBox ASTNode::apply() const { return type->value(this); }

AST::AST(AST &&ast) : statements(move(ast.statements)) {
    ast.statements.clear();
}

AST::~AST() {
    for (auto s : statements) delete s;
}

AST &AST::operator=(AST &&ast) {
    statements = move(ast.statements);
    ast.statements.clear();
    return *this;
}

void AST::apply() const {
    for (auto s : statements) {
        auto v = s->apply();
        if (!v.empty()) {
            throw logic_error("You don't say what to do with " + v.toString());
        }
    }
}

ASTNode *AST::createNode(const string &name) {
    ASTNode *s = new ASTNode(new ASTNode::Procedure(name));
    statements.push_back(s);

    return s;
}

AST &AST::include(AST &&ast) {
    statements.insert(statements.end(), ast.statements.begin(),
                      ast.statements.end());
    ast.statements.clear();

    return *this;
}

} /* ns: eval */

} /* ns: mlogo */
