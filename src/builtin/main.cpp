/**
 * @file: main.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */
#include <iostream>

#include "common.hpp"

static std::istream *inStream{nullptr};
static std::ostream *outStream{nullptr};
static std::ostream *errStream{nullptr};

extern "C" void initBuiltInProcedures() {
    mlogo::builtin::initArithmeticBuiltInProcedures();
    mlogo::builtin::initDataBuiltInProcedures();
    mlogo::builtin::initCommBuiltInProcedures();
    mlogo::builtin::initMemoryBuiltInProcedures();
    mlogo::builtin::initControlBuiltInProcedures();
    mlogo::builtin::initGraphicsBuiltInProcedures();
}

extern "C" void connectStreams(std::istream *is, std::ostream *os,
                               std::ostream *es) {
    inStream = is;
    outStream = os;
    errStream = es;
}

namespace mlogo {

namespace builtin {

std::istream &inputStream() {
    if (inStream) return *inStream;

    return std::cin;
}

std::ostream &outputStream() {
    if (outStream) return *outStream;

    return std::cout;
}

std::ostream &errorStream() {
    if (errStream) return *errStream;

    return std::cerr;
}

} /* ns: builtin */

} /* ns: mlogo */
