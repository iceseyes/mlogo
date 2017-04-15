/**
 * @file: builtin.cpp
 *
 *  created on:  4 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include <iostream>
#include <sstream>
#include <string>

#include "types.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "turtle.hpp"

using namespace std;

namespace mlogo { namespace builtin {

using Value = types::Value;
using ListValue = types::ListValue;
using ValueBox = types::ValueBox;

using BuiltinProcedure = types::BasicProcedure;
using Stack = memory::Stack;
using Turtle = turtle::Turtle;

using types::toString;

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

/*
 *
 */

struct Make : BuiltinProcedure {
    Make() : BuiltinProcedure(2) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        auto value = fetchArg(1);
        Stack::instance().globalFrame().setVariable(varName, value);
    }
};

struct Thing : BuiltinProcedure {
    Thing() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        auto varName = fetchArg(0).word();
        setReturnValue(Stack::instance().getVariable(varName));
    }
};

struct Print : BuiltinProcedure {
	Print() : BuiltinProcedure(1) {}
	void operator()() const override {
		auto arg = fetchArg(0);
		auto str = arg.toString();

		if(arg.isList())					   // if arg is a ListValue
			str = str.substr(1, str.size()-2); // remove first and last square bracket

		cout << str << endl;
	}
};

struct Sum : BuiltinProcedure {
	Sum() : BuiltinProcedure(2, true) {}
	void operator()() const override {
		stringstream ss;
		double arg0 = fetchArg(0).asDouble();
		double arg1 = fetchArg(1).asDouble();

		double result = arg0 + arg1;
		long rlong = static_cast<long>(result);

		if(result - rlong < 1e-5)
			ss << rlong;
		else
			ss << result;

		setReturnValue(ss.str());
	}
};

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

/**
 * Turtle Graphics
 */
struct Forward : BuiltinProcedure {
	Forward() : BuiltinProcedure(1) {}
	void operator()() const override {
		int arg = fetchArg(0).asUnsigned();
		Turtle::instance().forward(arg);
	}
};

struct Right : BuiltinProcedure {
	Right() : BuiltinProcedure(1) {}
	void operator()() const override {
		double arg = fetchArg(0).asDouble();
		Turtle::instance().right(arg);
	}
};

struct Backward : BuiltinProcedure {
	Backward() : BuiltinProcedure(1) {}
	void operator()() const override {
		int arg = fetchArg(0).asUnsigned();
		Turtle::instance().forward(-1 * arg);
	}
};

struct Left : BuiltinProcedure {
	Left() : BuiltinProcedure(1) {}
	void operator()() const override {
		double arg = fetchArg(0).asDouble();
		Turtle::instance().right(-1 * arg);
	}
};

struct Home : BuiltinProcedure {
	Home() : BuiltinProcedure(0) {}
	void operator()() const override {
		Turtle::instance().home();
	}
};

struct Clean : BuiltinProcedure {
    Clean() : BuiltinProcedure(0) {}
    void operator()() const override {
        auto p = Turtle::instance().currentPosition();
        Turtle::instance().clear();
        Turtle::instance().currentPosition(p);
    }
};

struct ClearScreen : BuiltinProcedure {
	ClearScreen() : BuiltinProcedure(0) {}
	void operator()() const override {
		Turtle::instance().clear();
		Turtle::instance().home();
	}
};

struct SetPos : BuiltinProcedure {
    SetPos() : BuiltinProcedure(1) {}
    void operator()() const override {
        auto pos = fetchArg(0).list();
        if(pos.size()!=2) throw std::logic_error("Expected X,Y Coordinates");
        int x = ValueBox(pos[0]).asInteger();
        int y = ValueBox(pos[1]).asInteger();
        Turtle::instance().currentPosition(
            std::make_pair(x, y));
    }
};

struct SetXY : BuiltinProcedure {
    SetXY() : BuiltinProcedure(2) {}
    void operator()() const override {
        int x = fetchArg(0).asInteger();
        int y = fetchArg(1).asInteger();
        Turtle::instance().currentPosition(std::make_pair(x, y));
    }
};

struct SetX : BuiltinProcedure {
    SetX() : BuiltinProcedure(1) {}
    void operator()() const override {
        int x = fetchArg(0).asInteger();
        Turtle::instance().currentXPosition(x);
    }
};

struct SetY : BuiltinProcedure {
    SetY() : BuiltinProcedure(1) {}
    void operator()() const override {
        int y = fetchArg(0).asInteger();
        Turtle::instance().currentYPosition(y);
    }
};

struct SetHeading : BuiltinProcedure {
    SetHeading() : BuiltinProcedure(1) {}
    void operator()() const override {
        int alpha = fetchArg(0).asDouble();
        Turtle::instance().heading(alpha);
    }
};

/**
 * Register procedures in memory
 */

void initBuiltInProcedures() {
    /* Constructors */
    Stack::instance().setProcedure<Word>("word");
    Stack::instance().setProcedure<Sentence>("sentence");
    Stack::instance().setProcedure<List>("list");
    Stack::instance().setProcedure<Fput>("Fput");
    Stack::instance().setProcedure<Lput>("Lput");

    /* Data Selector*/
    Stack::instance().setProcedure<First>("first");
    Stack::instance().setProcedure<Last>("last");
    Stack::instance().setProcedure<ButFirst>("butfirst");
    Stack::instance().setProcedure<ButLast>("butlast");
    Stack::instance().setProcedure<Item>("item");

    /* */
    Stack::instance().setProcedure<Make>("make");
    Stack::instance().setProcedure<Thing>("thing");
    Stack::instance().setProcedure<Print>("print");
    Stack::instance().setProcedure<Sum>("sum");
	Stack::instance().setProcedure<Repeat>("repeat");

	// Turtle Graphics
	Stack::instance().setProcedure<Forward>("forward");
	Stack::instance().setProcedure<Forward>("fd");
	Stack::instance().setProcedure<Backward>("backward");
	Stack::instance().setProcedure<Backward>("bk");
	Stack::instance().setProcedure<Right>("right");
	Stack::instance().setProcedure<Right>("rt");
	Stack::instance().setProcedure<Left>("left");
	Stack::instance().setProcedure<Left>("lt");
	Stack::instance().setProcedure<Home>("home");
	Stack::instance().setProcedure<Clean>("clean");
	Stack::instance().setProcedure<ClearScreen>("clearscreen");
	Stack::instance().setProcedure<ClearScreen>("cs");
	Stack::instance().setProcedure<SetPos>("setpos");
	Stack::instance().setProcedure<SetXY>("setxy");
	Stack::instance().setProcedure<SetX>("setx");
	Stack::instance().setProcedure<SetY>("sety");
	Stack::instance().setProcedure<SetHeading>("setheading");
	Stack::instance().setProcedure<SetHeading>("seth");
}

}}
