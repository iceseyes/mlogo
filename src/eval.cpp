/**
 * @file: eval.cpp
 *
 *  created on:  3 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "eval.hpp"

#include <boost/variant.hpp>

#include "memory.hpp"

#include "eval_impl.hpp"

namespace mlogo {

namespace eval {

using Stack = memory::Stack;

Statement *make_statement(mlogo::parser::Statement &stmt) {
    auto s = new Statement ( new Statement::Procedure(stmt.name.name) );
    impl::EvalStmtBuilderVisitor v(s);

    for(auto a : stmt.arguments) {
        boost::apply_visitor(v, a);
    }

    return s;
}

Statement *make_statement(mlogo::parser::Statement &&stmt) {
    auto s = new Statement ( new Statement::Procedure(stmt.name.name) );
    impl::EvalStmtBuilderVisitor v(s);

    for(auto a : stmt.arguments) {
        boost::apply_visitor(v, a);
    }

    return s;
}

AST make_ast(mlogo::parser::Statement &stmt) {
    AST ast;
    mlogo::parser::Argument proc { stmt.name };
    impl::EvalStmtBuilderVisitor v(&ast);


    boost::apply_visitor(v, proc);
    for(auto a : stmt.arguments) {
        boost::apply_visitor(v, a);
    }

    return ast;
}

AST make_ast(mlogo::parser::Statement &&stmt) {
    AST ast;
    mlogo::parser::Argument proc { stmt.name };
    impl::EvalStmtBuilderVisitor v(&ast);

    boost::apply_visitor(v, proc);
    for(auto a : stmt.arguments) {
        boost::apply_visitor(v, a);
    }

    return ast;
}

Statement::Procedure::Procedure(const std::string &name) :
    procName(name), _nargs(Stack::instance().getProcedureNArgs(name)) {}

std::string Statement::Procedure::value(const Statement *current) const {
    memory::ActualArguments args;
    for(auto child : current->children) {
        args.push_back((*child)());
    }

    Stack::instance().currentFrame().setVariable("__internal__returned__value__captured__", "");
    Stack::instance().callProcedure(procName, args, "__internal__returned__value__captured__");
    return Stack::instance().currentFrame().getVariable("__internal__returned__value__captured__");
}


Statement::Variable::Variable(const std::string &name) :
    varName(name) {}

std::string Statement::Variable::value(const Statement *) const {
    return Stack::instance().getVariable(varName);
}

Statement::Statement(Type *t, Statement *parent) :
    type(t), _parent(parent) {
    if(_parent) {
        _parent->children.push_back(this);
    }
}

Statement::~Statement() {
    for(auto ptr : children)
        delete ptr;

    delete type;
}

std::string Statement::apply() const {
    return type->value(this);
}

AST::AST(AST &&ast) :
    statements(std::move(ast.statements)) {
    ast.statements.clear();
}

AST::~AST() {
    for(auto s : statements) delete s;
}

AST &AST::operator=(AST &&ast) {
    statements = std::move(ast.statements);
    ast.statements.clear();
    return *this;
}

void AST::apply() const {
    for(auto s : statements) {
        std::string v = s->apply();
        if(!v.empty()) throw std::logic_error("You don't say what to do with " + v);
    }
}

Statement *AST::createStatement(const std::string &name) {
    Statement *s = new Statement(new Statement::Procedure(name));
    statements.push_back(s);

    return s;
}

} /* ns: eval */

} /* ns: mlogo */
