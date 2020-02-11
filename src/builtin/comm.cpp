/**
 * @file: comm.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <iomanip>
#include <iostream>

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

struct Form : BuiltinProcedure {
    Form() : BuiltinProcedure(3) {}
    void operator()() const override {
        double num = fetchArg(0).asDouble();
        int width = fetchArg(1).asInteger();
        auto format = fetchArg(2);

        std::streamsize ss = outputStream().precision();
        outputStream() << setw(width) << setprecision(format.asInteger()) << num
                       << endl
                       << setprecision(ss);
    }
};

struct Format : BuiltinProcedure {
    Format() : BuiltinProcedure(2) {}
    void operator()() const override {
        int num = fetchArg(0).asInteger();
        auto format = fetchArg(1);
        char buffer[256 + 1];
        sprintf(buffer, format.toString().c_str(), num);
        outputStream() << buffer << endl;
    }
};

}  // namespace

/**
 * Register procedures in memory
 */
void initCommBuiltInProcedures() {
    // I/O
    Stack::instance()
        .setProcedure<Print>("print")
        .setProcedure<Print>("pr")
        .setProcedure<Form>("form")
        .setProcedure<Format>("format")
        .setProcedure<Type>("type")
        .setProcedure<Show>("show");
}

}  // namespace builtin

}  // namespace mlogo
