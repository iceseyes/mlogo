/**
 * @file: memory.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "common.hpp"

namespace mlogo { namespace builtin {

namespace {

struct Make : BuiltinProcedure {
    Make() : BuiltinProcedure(2) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        auto value = fetchArg(1);
        Stack::instance().globalFrame().setVariable(varName, value);
    }
};

struct Thing : BuiltinProcedure {
    Thing() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        setReturnValue(Stack::instance().getVariable(varName));
    }
};

}

/**
 * Register procedures in memory
 */

void initMemoryBuiltInProcedures() {
    /* Memory Management */
    Stack::instance()
        .setProcedure<Make>("make")
        .setProcedure<Thing>("thing");
}

}}


