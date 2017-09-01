/**
 * @file: control.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

#include "../memory.hpp"

using namespace mlogo::memory;

namespace mlogo {

namespace builtin {

namespace {

struct Repeat : BuiltinProcedure {
    Repeat() : BuiltinProcedure(2) {}
    void operator()() const override {
        int arg0 = fetchArg(0).asUnsigned();
        string arg1 = fetchArg(1).toString();

        // arg1 is something like [...] where
        // ... maybe empty string or a very complex
        // expression. We have to take only string into brackets (like
        // arg1[1:-1])
        // for parsing, so check if arg1.size() is at least 2 ([])
        if (arg1.size() > 2) {
            for (int i = 0; i < arg0; ++i) {
                stringstream ss;
                ss << i;
                Stack::instance().setVariable("__REPCOUNT__", ss.str());
                auto stmt = parser::parse(arg1.substr(1, arg1.size() - 2));
                auto ast = eval::make_ast(stmt);

                ast();
            }
        }
    }
};

struct Repcount : BuiltinProcedure {
    Repcount() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        setReturnValue(Stack::instance().getVariable("__REPCOUNT__"));
    }
};

} /* ns */

void initControlBuiltInProcedures() {
    // Control
    Stack::instance().setProcedure<Repeat>("repeat");
    Stack::instance().setProcedure<Repcount>("repcount");
}

} /* ns: builtin */

} /* ns: mlogo */
