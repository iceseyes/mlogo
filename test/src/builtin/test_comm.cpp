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

    FAIL() << "Incomplete Test";
}

}  // namespace mlogo::test::comm
