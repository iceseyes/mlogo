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

struct SimpleSum : types::BasicProcedure {
    SimpleSum() : BasicProcedure(2, true) {}
    void operator()() const {
        stringstream ss;
		double arg0 = stod(fetchArg(0));
		double arg1 = stod(fetchArg(1));

		double result = arg0 + arg1;
		long rlong = static_cast<long>(result);

		if(result - rlong < 1e-5)
			ss << rlong;
		else
			ss << result;

		setReturnValue(ss.str());
    }
};

void initProcedures() {
    Stack::instance().globalFrame().setProcedure<Nop>("eNop");
	Stack::instance().globalFrame().setProcedure<SimpleSum>("eSum");
}

void clearValue() {
    Stack::instance().globalFrame().setVariable("__test__", "");
}

int readValue() {
    return std::stoi(Stack::instance().globalFrame().getVariable("__test__"));
}

TEST(Eval, makeStatementFromParser) {
    using eval::make_statement;
    using parser::parse;

    initProcedures();

    auto *s = make_statement(parse("eNop eSum 1 2"));
    clearValue();
    s->apply();
    ASSERT_EQ(3, readValue());
    delete s;

    s = make_statement(parse("eNop eSum 1 eSum 2 3"));
    clearValue();
    s->apply();
    ASSERT_EQ(6, readValue());
    delete s;

    s = make_statement(parse("eNop eSum eSum 2 3 1"));
    clearValue();
    s->apply();
    ASSERT_EQ(6, readValue());
    delete s;

    s = make_statement(parse("eNop eSum eSum 2 3 eSum 4 5"));
    clearValue();
    s->apply();
    ASSERT_EQ(14, readValue());
    delete s;

    s = make_statement(parse("eNop eSum eSum eSum 2 1 3 eSum 4 5"));
    clearValue();
    s->apply();
    ASSERT_EQ(15, readValue());
    delete s;

    s = make_statement(parse("eNop eSum eSum eSum 2 1 3 eSum 4 eSum 5 6"));
    clearValue();
    s->apply();
    ASSERT_EQ(21, readValue());
    delete s;

    s = make_statement(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6"));
    clearValue();
    s->apply();
    ASSERT_EQ(28, readValue());
    delete s;

    ASSERT_THROW(make_statement(parse("eNop eSum eSum eSum 2 1 eSum 3 7 eSum 4 eSum 5 6 7")), std::logic_error);
    ASSERT_THROW(make_statement(parse("eNop eSum 4 5 eSum 6 7")), std::logic_error);
    ASSERT_THROW(make_statement(parse("eNop \"Hello \"World")), std::logic_error);
}
