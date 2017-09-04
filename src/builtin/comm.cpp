/**
 * @file: comm.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

namespace mlogo {

namespace builtin {

namespace {

struct Print : BuiltinProcedure {
    Print() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto arg = fetchArg(0);
        auto str = arg.toString();

        outputStream() << str << endl;
    }
};

struct Type : BuiltinProcedure {
    Type() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto arg = fetchArg(0);
        auto str = arg.toString();

        outputStream() << str;
        outputStream().flush();
    }
};

struct Show : BuiltinProcedure {
    Show() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto arg = fetchArg(0);
        auto str = arg.toString(true);

        outputStream() << str << endl;
    }
};

} /* ns */

/**
 * Register procedures in memory
 */
void initCommBuiltInProcedures() {
    // I/O
    Stack::instance().setProcedure<Print>("print");
    Stack::instance().setProcedure<Print>("pr");
    Stack::instance().setProcedure<Type>("type");
    Stack::instance().setProcedure<Show>("show");
}

} /* ns: builtin */

} /* ns: mlogo */
