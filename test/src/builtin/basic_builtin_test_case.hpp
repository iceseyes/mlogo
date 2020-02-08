//
// Created by iceseyes on 08/02/20.
//

#ifndef MLOGO_BASIC_INTERPRETER_HPP
#define MLOGO_BASIC_INTERPRETER_HPP

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

#include "interpreter.hpp"
#include "memory.hpp"

extern "C" void initBuiltInProcedures();
extern "C" void connectStreams(std::istream *is, std::ostream *os,
                               std::ostream *es);

namespace mlogo::test {

class BasicBuiltInTestCase : public ::testing::Test {
protected:
    void SetUp() override {
        ss.str("");
        ss.clear();

        memory::Stack::instance().clear(); /* clear memory state */
        interpreter = getInterpreterPtr(std::cin, std::cout, std::cerr, false);

        initBuiltInProcedures();
        connectStreams(nullptr, &ss, nullptr);
    }

    void TearDown() override { delete interpreter; }

    std::string run(const std::string &line) {
        interpreter->one(line);
        return ss.str();
    }

private:
    StdDynamicInterpreter *interpreter{nullptr};
    std::stringstream ss;
};

}  // namespace mlogo::test

#endif  // MLOGO_BASIC_INTERPRETER_HPP
