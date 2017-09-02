/**
 * @file: defines.hpp
 */

#ifndef DEFINES_HPP_
#define DEFINES_HPP_

#include <string>

#define FRIEND_TEST(test_case_name, test_name) \
    friend class test_case_name##_##test_name##_Test

namespace mlogo {

/* KEYWORDS */

/// Start new user defined procedure keyword
const std::string START_PROCEDURE_KEYWORD{"TO"};

/// Close an user defined procedure keyword
const std::string END_PROCEDURE_KEYWORD{"END"};

/* Interpreter commands */

/// Leave interpreter
const std::string BYE_BYE_COMMAND{"BYE"};

}  // ns: mlogo

#endif /* DEFINES_HPP_ */
