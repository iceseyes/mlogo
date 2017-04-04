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

using namespace std;

namespace mlogo { namespace builtin {

using BuiltinProcedure = types::BasicProcedure;
using Stack = memory::Stack;

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

void initBuiltInProcedures() {
    Stack::instance().globalFrame().setProcedure<Print>("print");
	Stack::instance().globalFrame().setProcedure<Sum>("sum");
}

}}
