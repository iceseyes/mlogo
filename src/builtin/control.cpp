/**
 * @file: control.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "common.hpp"

namespace mlogo { namespace builtin {

namespace {

struct Repeat : BuiltinProcedure {
    Repeat() : BuiltinProcedure(2) {}
    void operator()() const override {
        stringstream ss;
        int arg0 = fetchArg(0).asUnsigned();
        string arg1 = fetchArg(1).toString();

        for(int i=0; i<arg0; ++i) {
            auto stmt = parser::parse(arg1);
            auto ast = eval::make_ast(stmt);

            ast();
        }
    }
};

}

void initControlBuiltInProcedures() {
    // Control
    Stack::instance().setProcedure<Repeat>("repeat");
}

}}
