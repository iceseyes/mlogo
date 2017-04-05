/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <cmath>
#include <iostream>

#include "parser.hpp"
#include "memory.hpp"
#include "eval.hpp"
#include "turtle.hpp"

using namespace std;

namespace mlogo { namespace builtin {

void initBuiltInProcedures();

}}


struct Forward : mlogo::types::BasicProcedure {
	Forward() : mlogo::types::BasicProcedure(1) {}
	void operator()() const override {
		int arg = std::stoi(fetchArg(0));
		mlogo::turtle::Turtle::instance().forward(arg);
	}
};

struct Right : mlogo::types::BasicProcedure {
	Right() : mlogo::types::BasicProcedure(1) {}
	void operator()() const override {
		double arg = std::stod(fetchArg(0));
		mlogo::turtle::Turtle::instance().right(arg);
	}
};

int main(int argc, char **argv) {
	mlogo::builtin::initBuiltInProcedures();
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Forward>("forward");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Forward>("fd");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Right>("right");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Right>("rt");

	string str;
	cout << endl << "? ";
	while(std::getline(std::cin, str)) {
		if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);
		auto s = mlogo::eval::make_statement(stmt);

		(*s)();

		delete s;
		cout << endl << "? ";
	}

	return 0;
}

