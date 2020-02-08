//
// Created by iceseyes on 08/02/20.
//
#include <gtest/gtest.h>

#include "basic_builtin_test_case.hpp"

using namespace std;
using namespace mlogo;
using namespace mlogo::memory;
using namespace mlogo::exceptions;

namespace mlogo::test::arithmetic {

class ArithmeticBuiltInTestCase : public BasicBuiltInTestCase {};

TEST_F(ArithmeticBuiltInTestCase, random) {
    for (int i = 0; i < 1000; ++i) {
        auto reply = run("pr random 100");
        double v = stod(reply);
        ASSERT_GE(100, v);
        ASSERT_LE(0, v);
        ASSERT_EQ(v, round(v));
    }
}

}  // namespace mlogo::test::arithmetic
