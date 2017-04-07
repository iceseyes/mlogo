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

using BuiltinProcedure = types::BasicProcedure;
using Stack = memory::Stack;
using Turtle = turtle::Turtle;

struct Make : BuiltinProcedure {
    Make() : BuiltinProcedure(2) {}
    void operator()() const override {
        string varName = fetchArg(0);
        string value = fetchArg(1);
        Stack::instance().globalFrame().setVariable(varName, value);
    }
};

struct Thing : BuiltinProcedure {
    Thing() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        string varName = fetchArg(0);
        setReturnValue(Stack::instance().getVariable(varName));
    }
};

struct Print : BuiltinProcedure {
	Print() : BuiltinProcedure(1) {}
	void operator()() const override {
		string arg = fetchArg(0);
		cout << arg << endl;
	}
};

struct First : BuiltinProcedure {
    First() : BuiltinProcedure(1, true) {}
    void operator()() const override {
        std::string arg0 = fetchArg(0);

        setReturnValue(arg0.substr(0, 1));
    }
};

struct Sum : BuiltinProcedure {
	Sum() : BuiltinProcedure(2, true) {}
	void operator()() const override {
		stringstream ss;
		double arg0 = stod(fetchArg(0));
		double arg1 = stod(fetchArg(1));

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
        int arg0 = stoi(fetchArg(0));
        std::string arg1 = fetchArg(1);

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
		int arg = std::stoi(fetchArg(0));
		Turtle::instance().forward(arg);
	}
};

struct Right : BuiltinProcedure {
	Right() : BuiltinProcedure(1) {}
	void operator()() const override {
		double arg = std::stod(fetchArg(0));
		Turtle::instance().right(arg);
	}
};

struct Backward : BuiltinProcedure {
	Backward() : BuiltinProcedure(1) {}
	void operator()() const override {
		int arg = std::stoi(fetchArg(0));
		Turtle::instance().forward(-1 * arg);
	}
};

struct Left : BuiltinProcedure {
	Left() : BuiltinProcedure(1) {}
	void operator()() const override {
		double arg = std::stod(fetchArg(0));
		Turtle::instance().right(-1 * arg);
	}
};

struct Home : BuiltinProcedure {
	Home() : BuiltinProcedure(0) {}
	void operator()() const override {
		Turtle::instance().home();
	}
};

struct ClearScreen : BuiltinProcedure {
	ClearScreen() : BuiltinProcedure(0) {}
	void operator()() const override {
		Turtle::instance().clear();
		Turtle::instance().home();
	}
};

/**
 * Register procedures in memory
 */

void initBuiltInProcedures() {
    Stack::instance().globalFrame().setProcedure<Make>("make");
    Stack::instance().globalFrame().setProcedure<Thing>("thing");
    Stack::instance().globalFrame().setProcedure<Print>("print");
    Stack::instance().globalFrame().setProcedure<First>("first");
	Stack::instance().globalFrame().setProcedure<Sum>("sum");
	Stack::instance().globalFrame().setProcedure<Repeat>("repeat");

	// Turtle Graphics
	Stack::instance().globalFrame().setProcedure<Forward>("forward");
	Stack::instance().globalFrame().setProcedure<Forward>("fd");
	Stack::instance().globalFrame().setProcedure<Backward>("backward");
	Stack::instance().globalFrame().setProcedure<Backward>("bk");
	Stack::instance().globalFrame().setProcedure<Right>("right");
	Stack::instance().globalFrame().setProcedure<Right>("rt");
	Stack::instance().globalFrame().setProcedure<Left>("left");
	Stack::instance().globalFrame().setProcedure<Left>("lt");
	Stack::instance().globalFrame().setProcedure<Home>("home");
	Stack::instance().globalFrame().setProcedure<ClearScreen>("clearscreen");
	Stack::instance().globalFrame().setProcedure<ClearScreen>("cs");
}

}}
