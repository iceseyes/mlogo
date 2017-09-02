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

        if (arg.isList())  // if arg is a ListValue
            str = str.substr(
                1, str.size() - 2);  // remove first and last square bracket

        outputStream() << str << endl;
    }
};
}

/**
 * Register procedures in memory
 */

void initCommBuiltInProcedures() {
    // I/O
    Stack::instance().setProcedure<Print>("print");
    Stack::instance().setProcedure<Print>("pr");
}

} /* ns: builtin */

} /* ns: mlogo */
