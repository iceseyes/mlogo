/**
 * @file: comm.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

/** TODO
READLIST
READWORD
READRAWLINE
READCHAR
READCHARS num
SHELL command
SETPREFIX string
PREFIX
OPENREAD filename
OPENWRITE filename
OPENAPPEND filename
OPENUPDATE filename
CLOSE filename
ALLOPEN
CLOSEALL                        (library procedure)
ERASEFILE filename
DRIBBLE filename
NODRIBBLE
SETREAD filename
SETWRITE filename
READER
WRITER
SETREADPOS charpos
SETWRITEPOS charpos
READPOS
WRITEPOS
EOFP
FILEP filename
KEYP
CLEARTEXT
*/
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
