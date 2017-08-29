/**
 * @file: eval.hpp
 *
 *  created on:  3 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef __EVAL_HPP__
#define __EVAL_HPP__

#include "types.hpp"

#include <string>
#include <vector>

#include "defines.hpp"

namespace mlogo {

namespace parser {

struct Statement;

} /* ns: parser */

namespace eval {

using ValueBox = types::ValueBox;

class ASTNode {
public:
    struct Type {
        virtual ~Type(){};

        virtual ValueBox value(const ASTNode*) const = 0;

        virtual std::size_t nArgs() const { return 0; }
    };

    struct Procedure : Type {
        Procedure(const std::string& name);
        ValueBox value(const ASTNode*) const override;

        const std::string procName;
        std::size_t nArgs() const override { return _nargs; }

    private:
        std::size_t _nargs;
    };

    struct Variable : Type {
        Variable(const std::string& name);
        ValueBox value(const ASTNode*) const override;

        const std::string varName;
    };

    struct Const : Type {
        Const(const std::string& value) : _value(value) {}

        ValueBox value(const ASTNode*) const override { return _value; }

        const std::string _value;
    };

    struct List : Type {
        List(const types::ListValue& value) : _value(value) {}

        ValueBox value(const ASTNode*) const override { return _value; }

        const types::ListValue _value;
    };

    ASTNode(Type* t, ASTNode* parent = nullptr);
    ASTNode(ASTNode&& stmt);
    ~ASTNode();

    ASTNode& operator=(ASTNode&& stmt);

    ValueBox apply() const;
    ValueBox operator()() const { return apply(); }

    ASTNode* parent() { return _parent; }
    std::size_t nArgs() const { return type->nArgs(); }
    std::size_t size() const { return children.size(); }
    bool completed() const { return nArgs() == size(); }

    ASTNode* setParent(ASTNode* node);

private:
    ASTNode(const ASTNode& stmt) = delete;
    ASTNode& operator=(const ASTNode& stmt) = delete;

    Type* type;
    ASTNode* _parent{nullptr};
    std::vector<ASTNode*> children;

    friend struct ASTNode::Procedure;
    FRIEND_TEST(Eval, moveASTNode);
    FRIEND_TEST(Eval, reParentASTNode);
};

class AST {
public:
    AST(){};
    AST(AST&& ast);
    ~AST();

    AST& operator=(AST&& ast);

    void apply() const;
    void operator()() const { apply(); }

    ASTNode* createNode(const std::string& name);

    std::size_t size() const { return statements.size(); }

    AST& include(AST&& ast);

private:
    AST(const AST&) = delete;
    AST& operator=(const AST&) = delete;

    std::vector<ASTNode*> statements;

    friend AST make_ast(const mlogo::parser::Statement& stmt);
};

ASTNode make_statement(const mlogo::parser::Statement& stmt);
AST make_ast(const mlogo::parser::Statement& stmt);

} /* ns: eval */

} /* ns: mlogo */

#endif /* __EVAL_HPP__ */
