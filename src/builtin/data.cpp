/**
 * @file: constructors.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "common.hpp"

namespace mlogo { namespace builtin {

namespace {

/*
 * Constructors
 */

struct Word : BuiltinProcedure {
    Word() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).word();
        auto arg1 = fetchArg(1).word();

        setReturnValue(arg0 + arg1);
    }
};

struct List : BuiltinProcedure {
    List() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        ValueBox out = ListValue();
        out
            .push_back(arg0)
            .push_back(arg1);

        setReturnValue(out);
    }
};

struct Sentence : BuiltinProcedure {
    Sentence() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        ValueBox out = ListValue();
        out
            .push_all_back(arg0)
            .push_all_back(arg1);

        setReturnValue(out);
    }
};

struct Fput : BuiltinProcedure {
    Fput() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        ValueBox out;
        if(arg1.isList()) {
            out = Value(arg1.value());
            out.push_front(arg0);
        } else {
            out = arg0.word() + arg1.word();
        }

        setReturnValue(out);
    }
};

struct Lput : BuiltinProcedure {
    Lput() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        ValueBox out;
        if(arg1.isList()) {
            out = Value(arg1.value());
            out.push_back(arg0);
        } else {
            out = arg1.word() + arg0.word();
        }

        setReturnValue(out);
    }
};

/*
 * Data Selector
 */

struct First : BuiltinProcedure {
    First() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.front());
    }
};

struct Last : BuiltinProcedure {
    Last() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.back());
    }
};

struct ButFirst : BuiltinProcedure {
    ButFirst() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.butFirst());
    }
};

struct ButLast : BuiltinProcedure {
    ButLast() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.butLast());
    }
};

struct Item : BuiltinProcedure {
    Item() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).asUnsigned();
        auto arg1 = fetchArg(1);

        setReturnValue(arg1.at(arg0));
    }
};

}

void initDataBuiltInProcedures() {
    /* Constructors */
    Stack::instance().setProcedure<Word>("word")
        .setProcedure<Sentence>("sentence")
        .setProcedure<List>("list")
        .setProcedure<Fput>("Fput")
        .setProcedure<Lput>("Lput")

    /* Data Selector*/
        .setProcedure<First>("first")
        .setProcedure<Last>("last")
        .setProcedure<ButFirst>("butfirst")
        .setProcedure<ButLast>("butlast")
        .setProcedure<Item>("item");
}

}}

