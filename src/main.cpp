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


int main(int argc, char **argv) {
	mlogo::builtin::initBuiltInProcedures();

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

