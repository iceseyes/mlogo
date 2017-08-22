/**
 * @file: test_eval.cpp
 *
 *  created on:  4 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include <gtest/gtest.h>

#include <sstream>

#include "eval.hpp"
#include "parser.hpp"
#include "types.hpp"
#include "memory.hpp"

using namespace std;

namespace parser = mlogo::parser;
namespace eval = mlogo::eval;
namespace memory = mlogo::memory;
namespace types = mlogo::types;

using Stack = memory::Stack;

struct Nop : types::BasicProcedure {
    Nop() : BasicProcedure(1) {}
    void operator()() const {
        Stack::instance().globalFrame().setVariable("__test__", fetchArg(0));
    }
};

struct SimpleOp : types::BasicProcedure {
    SimpleOp() : BasicProcedure(2, true) {}
    void operator()() const {
        stringstream ss;
		double arg0 = fetchArg(0).asDouble();
		double arg1 = fetchArg(1).asDouble();

		double result = op(arg0, arg1);
		long rlong = static_cast<long>(result);

		if(result - rlong < 1e-5)
			ss << rlong;
		else
			ss << result;

		setReturnValue(ss.str());
    }

    virtual double op(double a, double b) const = 0;
};

struct SimpleSum : SimpleOp {
    double op(double a, double b) const override {
        return a + b;
    }
};

void initProcedures() {
    Stack::instance().globalFrame().setProcedure<Nop>("eNop");
	Stack::instance().globalFrame().setProcedure<SimpleSum>("eSum");
    Stack::instance().globalFrame().setProcedure<SimpleSum>("sum");
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

    s = make_statement(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6"));
    clearValue();
    s();
    ASSERT_EQ(28, readValue());

    ASSERT_THROW(make_statement(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6 7")), std::logic_error);
    ASSERT_THROW(make_statement(parse("eNop eSum 4 5 eSum 6 7")), std::logic_error);
    ASSERT_THROW(make_statement(parse("eNop \"Hello \"World")), std::logic_error);
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

    ast = make_ast(parse("eNop sum 2 sum 2 sum 2 2 eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6"));
    clearValue();
    ast();
    ASSERT_EQ(28, readValue());

    ASSERT_THROW(make_ast(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6 7")), std::logic_error);
    auto testAST { make_ast(parse("eNop eSum 4 5 eSum 6 7")) };

    ASSERT_EQ(2u, testAST.size());
    ASSERT_THROW(testAST(), std::logic_error);

    ASSERT_THROW(make_ast(parse("eNop \"Hello \"World")), std::logic_error);
}


TEST(Eval, makeASTFromParserExpression) {
    using eval::make_ast;
    using parser::parse;
}
