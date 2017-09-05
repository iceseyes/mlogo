/**
 * @file: control.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

#include "../interpreter.hpp"
#include "../memory.hpp"

using namespace mlogo::memory;

namespace mlogo {

namespace builtin {

namespace {

struct NewFrameRAII {
    NewFrameRAII() { Stack::instance().openFrame(); }
    ~NewFrameRAII() { Stack::instance().closeFrame(); }
};

struct Run : BuiltinProcedure {
    Run() : BuiltinProcedure(1) {}
    void operator()() const override {
        string arg0 = fetchArg(0).toString();

        try {
            NewFrameRAII frameGuard;
            Stack::instance().setVariable("__REPCOUNT__", ss.str());
            auto interpreter =
                getInterpreter(inputStream(), outputStream(), errorStream());
            interpreter.one(arg0);
        } catch (exceptions::StopException &e) {
        }
    }
};

struct Repeat : BuiltinProcedure {
    Repeat() : BuiltinProcedure(2) {}
    void operator()() const override {
        int arg0 = fetchArg(0).asUnsigned();
        string arg1 = fetchArg(1).toString();

        try {
            NewFrameRAII frameGuard;
            for (int i = 0; i < arg0; ++i) {
                stringstream ss;
                ss << i;
                Stack::instance().setVariable("__REPCOUNT__", ss.str());
                auto interpreter = getInterpreter(inputStream(), outputStream(),
                                                  errorStream());
                interpreter.one(arg1);
            }
        } catch (exceptions::StopException &e) {
        }
    }
};

struct Forever : BuiltinProcedure {
    Forever() : BuiltinProcedure(1) {}
    void operator()() const override {
        string arg0 = fetchArg(0).toString();

        try {
            NewFrameRAII frameGuard;
            while (true) {
                auto interpreter = getInterpreter(inputStream(), outputStream(),
                                                  errorStream());
                interpreter.one(arg0);
            }
        } catch (exceptions::StopException &e) {
        }
    }
};

struct Repcount : BuiltinProcedure {
    Repcount() : BuiltinProcedure(0, true) {}
    void operator()() const override {
        setReturnValue(Stack::instance().getVariable("__REPCOUNT__"));
    }
};

struct If : BuiltinProcedure {
    If() : BuiltinProcedure(2) {}
    void operator()() const override {
        bool arg0 = fetchArg(0).toBool();
        auto arg1 = fetchArg(1).toString();

        if (arg0) {
            NewFrameRAII frameGuard;
            auto i =
                getInterpreter(inputStream(), outputStream(), errorStream());
            i.one(arg1);
        }
    }
};

struct IfElse : BuiltinProcedure {
    IfElse() : BuiltinProcedure(3) {}
    void operator()() const override {
        bool arg0 = fetchArg(0).toBool();
        auto arg1 = fetchArg(1).toString();
        auto arg2 = fetchArg(2).toString();
        auto i = getInterpreter(inputStream(), outputStream(), errorStream());

        NewFrameRAII frameGuard;
        if (arg0)
            i.one(arg1);
        else
            i.one(arg2);
    }
};

struct Test : BuiltinProcedure {
    Test() : BuiltinProcedure(1) {}
    void operator()() const override {
        Stack::instance().setVariable("__LASTTEST__", fetchArg(0));
    }
};

struct IfTrue : BuiltinProcedure {
    IfTrue() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).toString();
        bool lastTest = Stack::instance().getVariable("__LASTTEST__").toBool();

        if (lastTest) {
            NewFrameRAII frameGuard;
            auto i =
                getInterpreter(inputStream(), outputStream(), errorStream());
            i.one(arg0);
        }
    }
};

struct IfFalse : BuiltinProcedure {
    IfFalse() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).toString();
        bool lastTest = Stack::instance().getVariable("__LASTTEST__").toBool();

        if (!lastTest) {
            NewFrameRAII frameGuard;
            auto i =
                getInterpreter(inputStream(), outputStream(), errorStream());
            i.one(arg0);
        }
    }
};

struct Bye : BuiltinProcedure {
    Bye() : BuiltinProcedure(0) {}
    void operator()() const override { InterpreterState::instance().bye(); }
};

struct Stop : BuiltinProcedure {
    Stop() : BuiltinProcedure(0) {}
    void operator()() const override { throw exceptions::StopException(); }
};

} /* ns */

void initControlBuiltInProcedures() {
    // Control
    Stack::instance().setProcedure<Repeat>("repeat");
    Stack::instance().setProcedure<Forever>("forever");
    Stack::instance().setProcedure<Repcount>("repcount");
    Stack::instance().setProcedure<If>("if");
    Stack::instance().setProcedure<IfElse>("ifelse");
    Stack::instance().setProcedure<Test>("test");
    Stack::instance().setProcedure<IfTrue>("iftrue");
    Stack::instance().setProcedure<IfTrue>("ift");
    Stack::instance().setProcedure<IfFalse>("iffalse");
    Stack::instance().setProcedure<IfFalse>("iff");
    Stack::instance().setProcedure<Bye>("bye");
    Stack::instance().setProcedure<Stop>("stop");
}

} /* ns: builtin */

} /* ns: mlogo */
