//
// Created by iceseyes on 08/02/20.
//
#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>
#include <vector>

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

TEST_F(ArithmeticBuiltInTestCase, random_with_string) {
    ASSERT_THROW(run("pr random bye"), logic_error);
    ASSERT_THROW(run("pr random one"), logic_error);
}

TEST_F(ArithmeticBuiltInTestCase, random_with_negative) {
    auto reply = run("pr random -100");
    double v = stod(reply);
    ASSERT_EQ(0, v);

    reply = run("pr random 0");
    v = stod(reply);
    ASSERT_EQ(0, v);

    reply = run("pr random -123456");
    v = stod(reply);
    ASSERT_EQ(0, v);
}

TEST_F(ArithmeticBuiltInTestCase, rerandom) {
    vector<string> res;
    run("rerandom 0");

    for (int i = 0; i < 10; ++i) res.push_back(run("pr random 100"));
    for (int i = 0; i < 10; ++i) {
        run("rerandom 0");
        for (unsigned j = 0; j < res.size(); ++j)
            ASSERT_EQ(res[j], run("pr random 100"))
                << "Mismatch on sample " << j;
    }

    res.clear();
    run("rerandom 1234567");

    for (int i = 0; i < 10; ++i) res.push_back(run("pr random 100"));
    for (int i = 0; i < 10; ++i) {
        run("rerandom 1234567");
        for (unsigned j = 0; j < res.size(); ++j)
            ASSERT_EQ(res[j], run("pr random 100"))
                << "Mismatch on sample " << j;
        ;
    }
}

}  // namespace mlogo::test::arithmetic
