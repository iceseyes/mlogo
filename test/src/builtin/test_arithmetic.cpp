//
// Created by iceseyes on 08/02/20.
//
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.hpp"
#include "memory.hpp"

using namespace std;
using namespace mlogo;
using namespace mlogo::memory;
using namespace mlogo::exceptions;

extern "C" void initBuiltInProcedures();
extern "C" void connectStreams(istream *is, ostream *os, ostream *es);

namespace test::arithmetic {

class ArithmeticBuiltInTestCase : public ::testing::Test {
protected:
    void SetUp() override {
        ss.str("");
        ss.clear();

        Stack::instance().clear(); /* clear memory state */
        interpreter = getInterpreterPtr(cin, cout, cerr, false);

        initBuiltInProcedures();
        connectStreams(nullptr, &ss, nullptr);
    }

    void TearDown() override { delete interpreter; }

    StdDynamicInterpreter *interpreter{nullptr};
    stringstream ss;
};

TEST_F(ArithmeticBuiltInTestCase, random) {
    for (int i = 0; i < 1000; ++i) {
        interpreter->one("pr random 100");
        double v = stod(ss.str());
        ASSERT_GE(100, v);
        ASSERT_LE(0, v);
        ASSERT_EQ(v, round(v));
    }
}

}  // namespace test::arithmetic
