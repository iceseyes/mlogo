/**
 * @file: eval.hpp
 *
 *  created on:  3 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#ifndef __EVAL_HPP__
#define __EVAL_HPP__

#include <string>
#include <vector>

namespace mlogo {

namespace parser {

class Statement;

} /* ns: parser */

namespace eval {

class Statement {
public:
    struct Type {
        virtual ~Type() {};

        virtual std::string value(const Statement *) const = 0;
    };

    struct Procedure : Type {
        Procedure(const std::string &name);
        std::string value(const Statement *) const override;

        const std::string procName;
    };

    struct Variable : Type {
        Variable(const std::string &name);
        std::string value(const Statement *) const override;

        const std::string varName;
    };

    struct Const : Type {
        Const(const std::string &value) :
            _value (value) {}

        std::string value(const Statement *) const override { return _value; }

        const std::string _value;
    };

    Statement(Type *t, Statement *parent = nullptr);
    ~Statement();

    std::string operator()() const;

    Statement *parent() { return _parent; }

private:
    Type *type;
    Statement *_parent { nullptr };
    std::vector<Statement *> children;

    friend struct Statement::Procedure;
};

Statement *make_statement(mlogo::parser::Statement &stmt);

} /* ns: eval */

} /* ns: mlogo */

#endif /* __EVAL_HPP__ */
