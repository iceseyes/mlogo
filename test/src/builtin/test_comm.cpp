//
// Created by iceseyes on 08/02/20.
//
#include <gtest/gtest.h>

#include <stdexcept>
#include <vector>

#include "basic_builtin_test_case.hpp"

using namespace std;
using namespace mlogo;
using namespace mlogo::memory;
using namespace mlogo::exceptions;

namespace mlogo::test::comm {

class CommBuiltInTestCase : public BasicBuiltInTestCase {};

TEST_F(CommBuiltInTestCase, form_double) {
    auto ris = run("form 3.141516 10 3");
    ASSERT_EQ("      3.14", ris.substr(0, 10));

    ris = run("form 3.141516 5 2");
    ASSERT_EQ("  3.1", ris.substr(0, 5));

    ris = run("form 3.141516 0 2");
    ASSERT_EQ("3.1", ris.substr(0, 3));

    ris = run("form 3.141516 5 4");
    ASSERT_EQ("3.142", ris.substr(0, 5));
}

TEST_F(CommBuiltInTestCase, form_printf) {
    auto ris = run("format 10 \"0x%02X");
    ASSERT_EQ("0x0A", ris.substr(0, 4));
}

}  // namespace mlogo::test::comm
