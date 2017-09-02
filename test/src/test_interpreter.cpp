/**
 * @file: test_interpreter.cpp
 *
 *  created on:  2 settembre 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "exceptions.hpp"
#include "interpreter.hpp"
#include "memory.hpp"

using namespace mlogo;
using namespace mlogo::memory;
using namespace mlogo::exceptions;

extern "C" void initBuiltInProcedures();
extern "C" void connectStreams(std::istream *is, std::ostream *os,
                               std::ostream *es);

TEST(Interpreter, oneMethod) {
    Stack::instance().clear(); /* clear memory state */
    auto interpreter = getInterpreter(std::cin, std::cout, std::cerr, false);
    ASSERT_NO_THROW(interpreter.one(""));
    ASSERT_THROW(interpreter.one("TO MOVE"), InvalidStatmentException);
    ASSERT_THROW(interpreter.one("TO MOVE\nFD 100\nEND"), SyntaxError);
    ASSERT_THROW(interpreter.one("TO MOVE FD 100 END"), SyntaxError);
    ASSERT_ANY_THROW(interpreter.one("TEST MOVE.TO FD 100 END"));
    ASSERT_THROW(interpreter.one("TEST MOVE.TO + FD 100"), SyntaxError);
    ASSERT_ANY_THROW(
        interpreter.one("PR [CHECK INFO FILE FOR MORE INFORMATIONS]"));

    initBuiltInProcedures();

    ASSERT_NO_THROW(
        interpreter.one("PR [CHECK INFO FILE FOR MORE INFORMATIONS]"));
}

TEST(Interpreter, onePrintMethod) {
    std::stringstream ss;
    Stack::instance().clear(); /* clear memory state */
    auto interpreter = getInterpreter(std::cin, std::cout, std::cerr, false);
    initBuiltInProcedures();
    connectStreams(nullptr, &ss, nullptr);

    interpreter.one("PR [CHECK INFO FILE FOR MORE INFORMATIONS]");
    ASSERT_EQ("CHECK INFO FILE FOR MORE INFORMATIONS\n", ss.str());
}
