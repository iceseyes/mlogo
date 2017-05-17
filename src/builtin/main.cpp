/**
 * @file: main.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

extern "C" void initBuiltInProcedures() {
    mlogo::builtin::initArithmeticBuiltInProcedures();
    mlogo::builtin::initDataBuiltInProcedures();
    mlogo::builtin::initCommBuiltInProcedures();
    mlogo::builtin::initMemoryBuiltInProcedures();
    mlogo::builtin::initControlBuiltInProcedures();
    mlogo::builtin::initGraphicsBuiltInProcedures();
}


