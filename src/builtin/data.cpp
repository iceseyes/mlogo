/**
 * @file: constructors.cpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "common.hpp"

#include <boost/algorithm/string.hpp>

#include "../exceptions.hpp"

namespace mlogo {

namespace builtin {

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
        out.push_back(arg0).push_back(arg1);

        setReturnValue(out);
    }
};

struct Sentence : BuiltinProcedure {
    Sentence() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        ValueBox out = ListValue();
        out.push_all_back(arg0).push_all_back(arg1);

        setReturnValue(out);
    }
};

struct Fput : BuiltinProcedure {
    Fput() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        ValueBox out;
        if (arg1.isList()) {
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
        if (arg1.isList()) {
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

struct SetItem : BuiltinProcedure {
    SetItem() : BuiltinProcedure(3, false) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).asUnsigned();
        auto arg1 = fetchArg(1);
        auto arg2 = fetchArg(2);

        arg1.set(arg0, arg2);
    }
};

struct SetFirst : BuiltinProcedure {
    SetFirst() : BuiltinProcedure(2, false) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        arg0.set(0, arg1);
    }
};

struct WordP : BuiltinProcedure {
    WordP() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.isWord());
    }
};

struct ListP : BuiltinProcedure {
    ListP() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.isList());
    }
};

struct EmptyP : BuiltinProcedure {
    EmptyP() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        setReturnValue(arg0.empty());
    }
};

struct EqualP : BuiltinProcedure {
    EqualP() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        setReturnValue(arg0 == arg1);
    }
};

struct NotEqualP : BuiltinProcedure {
    NotEqualP() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        setReturnValue(arg0 != arg1);
    }
};

struct BeforeP : BuiltinProcedure {
    BeforeP() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        setReturnValue(arg0 < arg1);
    }
};

struct MemberP : BuiltinProcedure {
    MemberP() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        setReturnValue(arg1.in(arg0));
    }
};

struct SubstringP : BuiltinProcedure {
    SubstringP() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        bool ris = !arg0.isList() && !arg1.isList();
        ris =
            ris && (arg1.toString().find(arg0.toString()) != std::string::npos);

        setReturnValue(ris);
    }
};

struct NumberP : BuiltinProcedure {
    NumberP() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);

        bool ris{true};
        try {
            arg0.asDouble();
        } catch (...) {
            ris = false;
        }

        setReturnValue(ris);
    }
};

struct Count : BuiltinProcedure {
    Count() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        setReturnValue(arg0.size());
    }
};

struct Ascii : BuiltinProcedure {
    Ascii() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        if (!arg0.isWord() || (arg0.size() != 1)) {
            throw exceptions::LogoErrorException("you have to provide a char.");
        }

        int ch = arg0.word()[0];
        setReturnValue(ch);
    }
};

struct Char : BuiltinProcedure {
    Char() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).asInteger();

        char ch = arg0;
        setReturnValue(std::string() + ch);
    }
};

struct Member : BuiltinProcedure {
    Member() : BuiltinProcedure(2, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0);
        auto arg1 = fetchArg(1);

        setReturnValue(arg1.member(arg0));
    }
};

struct Lowercase : BuiltinProcedure {
    Lowercase() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).word();

        setReturnValue(boost::to_lower_copy(arg0));
    }
};

struct Uppercase : BuiltinProcedure {
    Uppercase() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto arg0 = fetchArg(0).word();

        setReturnValue(boost::to_upper_copy(arg0));
    }
};

} /* ns */

void initDataBuiltInProcedures() {
    /* Constructors */
    Stack::instance()
        .setProcedure<Word>("word")
        .setProcedure<Sentence>("sentence")
        .setProcedure<List>("list")
        .setProcedure<Fput>("Fput")
        .setProcedure<Lput>("Lput")

        /* Data Selector*/
        .setProcedure<First>("first")
        .setProcedure<Last>("last")
        .setProcedure<ButFirst>("butfirst")
        .setProcedure<ButLast>("butlast")
        .setProcedure<Item>("item")

        /* Data Mutators */
        .setProcedure<SetItem>("setitem")
        .setProcedure<SetItem>(".setitem")
        .setProcedure<SetFirst>("setfirst")
        .setProcedure<SetFirst>(".setfirst")

        /* Queries */
        .setProcedure<Count>("count")
        .setProcedure<Ascii>("ascii")
        .setProcedure<Ascii>("rawascii")
        .setProcedure<Char>("char")
        .setProcedure<Member>("member")
        .setProcedure<Lowercase>("lowercase")
        .setProcedure<Uppercase>("uppercase")

        /* Predicates */
        .setProcedure<WordP>("wordp")
        .setProcedure<WordP>("word?")
        .setProcedure<ListP>("listp")
        .setProcedure<ListP>("list?")
        .setProcedure<EmptyP>("emptyp")
        .setProcedure<EmptyP>("empty?")
        .setProcedure<EqualP>("equalp")
        .setProcedure<EqualP>("equal?")
        .setProcedure<EqualP>(".eq")
        .setProcedure<NotEqualP>("notequalp")
        .setProcedure<NotEqualP>("notequal?")
        .setProcedure<BeforeP>("beforep")
        .setProcedure<BeforeP>("before?")
        .setProcedure<MemberP>("memberp")
        .setProcedure<MemberP>("member?")
        .setProcedure<SubstringP>("substringp")
        .setProcedure<NumberP>("numberp")
        .setProcedure<NumberP>("number?");

    // TODO: VBARREDP char
}

} /* ns: builtin */

} /* ns: mlogo */
