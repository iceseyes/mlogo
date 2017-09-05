/**
 * @file: memory.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

namespace mlogo {
namespace builtin {

namespace {

struct Make : BuiltinProcedure {
    Make() : BuiltinProcedure(2) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        auto value = fetchArg(1);

        // wrong should try to find varName and only when  it does not exist
        // create it in global frame
        Stack::instance().globalFrame().setVariable(varName, value);
    }
};

struct Name : BuiltinProcedure {
    Name() : BuiltinProcedure(2) {}
    void operator()() const override {
        auto value = fetchArg(0);
        auto varName = fetchArg(1).word();

        // wrong should try to find varName and only when  it does not exist
        // create it in global frame
        Stack::instance().globalFrame().setVariable(varName, value);
    }
};

struct Local : BuiltinProcedure {
    Local() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        Stack::instance().currentFrame().setVariable(varName, "");
    }
};

struct LocalMake : BuiltinProcedure {
    LocalMake() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        auto value = fetchArg(1);
        Stack::instance().currentFrame().setVariable(varName, value);
    }
};

struct Global : BuiltinProcedure {
    Global() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        Stack::instance().globalFrame().setVariable(varName, "");
    }
};

struct Thing : BuiltinProcedure {
    Thing() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        setReturnValue(Stack::instance().getVariable(varName));
    }
};

struct Procedurep : BuiltinProcedure {
    Procedurep() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto procName = fetchArg(0).word();
        setReturnValue(Stack::instance().hasProcedure(procName));
    }
};

} /* ns */

/**
 * Register procedures in memory
 */

void initMemoryBuiltInProcedures() {
    /* Memory Management */
    Stack::instance()
        .setProcedure<Make>("make")
        .setProcedure<Name>("name")
        .setProcedure<Local>("local")
        .setProcedure<LocalMake>("localmake")
        .setProcedure<Thing>("thing")
        .setProcedure<Procedurep>("procedurep")
        .setProcedure<Procedurep>("procedure?");

    Stack::instance().setVariable("startup", ListValue());
    Stack::instance().setVariable("__REPCOUNT__", "-1");
}

} /* ns: builtin */

} /* ns: mlogo */
