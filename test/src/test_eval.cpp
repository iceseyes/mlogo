/**
 * @file: test_eval.cpp
 *
 *  created on:  4 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <sstream>

#include "eval.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "types.hpp"

using namespace std;

namespace parser = mlogo::parser;
namespace eval = mlogo::eval;
namespace memory = mlogo::memory;
namespace types = mlogo::types;

using Stack = memory::Stack;

namespace {

struct Nop : types::BasicProcedure {
    Nop() : BasicProcedure(1) {}
    void operator()() const {
        Stack::instance().globalFrame().setVariable("__test__", fetchArg(0));
    }
};

} /* ns */

struct SimpleOp : types::BasicProcedure {
    SimpleOp() : BasicProcedure(2, true) {}
    void operator()() const {
        stringstream ss;
        double arg0 = fetchArg(0).asDouble();
        double arg1 = fetchArg(1).asDouble();

        double result = op(arg0, arg1);
        long rlong = static_cast<long>(result);

        if (result - rlong < 1e-5) ss << rlong;

        setReturnValue(ss.str());
    }

    virtual double op(double a, double b) const = 0;
};

struct SimpleSum : SimpleOp {
    double op(double a, double b) const override { return a + b; }
};

struct SimpleProduct : SimpleOp {
    double op(double a, double b) const override { return a * b; }
};

struct SimpleDifference : SimpleOp {
    double op(double a, double b) const override { return a - b; }
};

struct SimpleQuotient : SimpleOp {
    double op(double a, double b) const override { return a / b; }
};

struct SimpleMax : SimpleOp {
    double op(double a, double b) const override { return std::max(a, b); }
};

void initProcedures() {
    Stack::instance().globalFrame().setProcedure<Nop>("eNop");
    Stack::instance().globalFrame().setProcedure<SimpleSum>("eSum");
    Stack::instance().globalFrame().setProcedure<SimpleSum>("sum");
    Stack::instance().globalFrame().setProcedure<SimpleProduct>("product");
    Stack::instance().globalFrame().setProcedure<SimpleDifference>(
        "difference");
    Stack::instance().globalFrame().setProcedure<SimpleQuotient>("quotient");
    Stack::instance().globalFrame().setProcedure<SimpleMax>("max");

    Stack::instance().globalFrame().setVariable("a_num", "123");
}

void clearValue() {
    Stack::instance().globalFrame().setVariable("__test__", "");
}

int readValue() {
    return Stack::instance().globalFrame().getVariable("__test__").asInteger();
}

TEST(Eval, makeStatementFromParser) {
    using eval::make_statement;
    using parser::parse;

    initProcedures();

    auto s = make_statement(parse("eNop eSum 1 2"));
    clearValue();
    s();
    ASSERT_EQ(3, readValue());

    s = make_statement(parse("eNop eSum 1 eSum 2 3"));
    clearValue();
    s();
    ASSERT_EQ(6, readValue());

    s = make_statement(parse("eNop eSum eSum 2 3 1"));
    clearValue();
    s();
    ASSERT_EQ(6, readValue());

    s = make_statement(parse("eNop eSum eSum 2 3 eSum 4 5"));
    clearValue();
    s();
    ASSERT_EQ(14, readValue());

    s = make_statement(parse("eNop eSum eSum eSum 2 1 3 eSum 4 5"));
    clearValue();
    s();
    ASSERT_EQ(15, readValue());

    s = make_statement(parse("eNop eSum eSum eSum 2 1 3 eSum 4 eSum 5 6"));
    clearValue();
    s();
    ASSERT_EQ(21, readValue());

    s = make_statement(
        parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6"));
    clearValue();
    s();
    ASSERT_EQ(28, readValue());

    ASSERT_THROW(make_statement(parse(
                     "eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6 7")),
                 std::logic_error);
    ASSERT_THROW(make_statement(parse("eNop eSum 4 5 eSum 6 7")),
                 std::logic_error);
    ASSERT_THROW(make_statement(parse("eNop \"Hello \"World")),
                 std::logic_error);
}

TEST(Eval, makeASTFromParser) {
    using eval::make_ast;
    using parser::parse;

    initProcedures();

    auto ast = make_ast(parse("eNop eSum 1 2"));
    clearValue();
    ast();
    ASSERT_EQ(3, readValue());

    ast = make_ast(parse("eNop eSum 1 eSum 2 3"));
    clearValue();
    ast();
    ASSERT_EQ(6, readValue());

    ast = make_ast(parse("eNop eSum eSum 2 3 1"));
    clearValue();
    ast();
    ASSERT_EQ(6, readValue());

    ast = make_ast(parse("eNop eSum eSum 2 3 eSum 4 5"));
    clearValue();
    ast();
    ASSERT_EQ(14, readValue());

    ast = make_ast(parse("eNop eSum eSum eSum 2 1 3 eSum 4 5"));
    clearValue();
    ast();
    ASSERT_EQ(15, readValue());

    ast = make_ast(parse("eNop eSum eSum eSum 2 1 3 eSum 4 eSum 5 6"));
    clearValue();
    ast();
    ASSERT_EQ(21, readValue());

    ast = make_ast(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6"));
    clearValue();
    ast();
    ASSERT_EQ(28, readValue());

    ast = make_ast(parse("eNop 2 eNop eSum 5 6"));
    clearValue();
    ast();
    ASSERT_EQ(11, readValue());

    ast =
        make_ast(parse("eNop sum 2 sum 2 sum 2 2 eNop eSum eSum eSum 2 1 eSum "
                       "3 7 eSum 4 eSum 5 6"));
    clearValue();
    ast();
    ASSERT_EQ(28, readValue());

    ASSERT_THROW(
        make_ast(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6 7")),
        std::logic_error);
    auto testAST{make_ast(parse("eNop eSum 4 5 eSum 6 7"))};

    ASSERT_EQ(2u, testAST.size());
    ASSERT_THROW(testAST(), std::logic_error);

    ASSERT_THROW(make_ast(parse("eNop \"Hello \"World")), std::logic_error);
}

TEST(Eval, makeASTFromParserExpression) {
    using eval::make_ast;
    using parser::parse;

    initProcedures();

    auto ast = make_ast(parse("eNop 1+2"));
    clearValue();
    ast();
    ASSERT_EQ(3, readValue());

    ast = make_ast(parse("eNop 1 +2"));
    clearValue();
    ast();
    ASSERT_EQ(3, readValue());

    ast = make_ast(parse("eNop 1 + 2"));
    clearValue();
    ast();
    ASSERT_EQ(3, readValue());

    ast = make_ast(parse("eNop 1+2*3"));
    clearValue();
    ast();
    ASSERT_EQ(7, readValue());

    ast = make_ast(parse("eNop 2*3+1"));
    clearValue();
    ast();
    ASSERT_EQ(7, readValue());

    ast = make_ast(parse("eNop 2*(3+1)/2+1"));
    clearValue();
    ast();
    ASSERT_EQ(5, readValue());

    ast = make_ast(parse("eNop 2*5+(3+1)/2+1"));
    clearValue();
    ast();
    ASSERT_EQ(13, readValue());

    ast = make_ast(parse("eNop 2*5+(3-1)/2-1"));
    clearValue();
    ast();
    ASSERT_EQ(10, readValue());

    ast = make_ast(parse("eNop max 5 9"));
    clearValue();
    ast();
    ASSERT_EQ(9, readValue());

    ast = make_ast(parse("eNop max 5 + 2 9"));
    clearValue();
    ast();
    ASSERT_EQ(9, readValue());

    ast = make_ast(parse("eNop max 5 2 *9"));
    clearValue();
    ast();
    ASSERT_EQ(18, readValue());

    ast = make_ast(parse("eNop 2 * max 5 2 *9"));
    clearValue();
    ast();
    ASSERT_EQ(36, readValue());

    ast = make_ast(parse("eNop 2 + (max 5 2 *9) * 2"));
    clearValue();
    ast();
    ASSERT_EQ(38, readValue());

    ast = make_ast(parse("eNop :a_num + (max 5 2 *9) * 2"));
    clearValue();
    ast();
    ASSERT_EQ(159, readValue());

    ast = make_ast(parse("eNop 1 + max :a_num 2 *9"));
    clearValue();
    ast();
    ASSERT_EQ(124, readValue());
}

TEST(Eval, astVariableTest) {
    mlogo::eval::ASTNode::Variable v("test_eval123");

    ASSERT_EQ(0u, v.nArgs());

    Stack::instance().setVariable("test_eval123", "abc");
    ASSERT_EQ("abc", v.value(nullptr).toString());
}

TEST(Eval, astListTest) {
    mlogo::types::ListValue list;
    list.push_back("123");
    list.push_back("654");
    mlogo::eval::ASTNode::List l(list);

    ASSERT_EQ(0u, l.nArgs());

    Stack::instance().setVariable("test_eval123", "abc");
    ASSERT_EQ(list, l.value(nullptr).list());
}
