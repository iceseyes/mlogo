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
#include "turtle.hpp"

using namespace std;

namespace mlogo { namespace builtin {

using BuiltinProcedure = types::BasicProcedure;
using Stack = memory::Stack;
using Turtle = turtle::Turtle;

struct Print : BuiltinProcedure {
	Print() : BuiltinProcedure(1) {}
	void operator()() const override {
		string arg = fetchArg(0);
		cout << arg << endl;
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
    Stack::instance().globalFrame().setProcedure<Print>("print");
	Stack::instance().globalFrame().setProcedure<Sum>("sum");

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
