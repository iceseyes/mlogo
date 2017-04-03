/**
 * @file: eval.cpp
 *
 *  created on:  3 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "eval.hpp"

#include "memory.hpp"

namespace mlogo {

namespace eval {

using Stack = memory::Stack;

Statement::Procedure::Procedure(const std::string &name) :
    procName(name) {}

std::string Statement::Procedure::value(const Statement *current) const {
    memory::ActualArguments args;
    for(auto child : current->children)
        args.push_back((*child)());

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

std::string Statement::operator()() const {
    return type->value(this);
}

}

}
