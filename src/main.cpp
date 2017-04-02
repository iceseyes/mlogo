/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <iostream>

#include "parser.hpp"
#include "memory.hpp"

using namespace std;

struct Print : mlogo::types::BasicProcedure {
	Print() : BasicProcedure(1) {}
	void operator()() const override {
		std::string arg = fetchArg(0);
		cout << arg << endl;
	}
};

int main(int argc, char **argv) {
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Print>("print");

	string str;
	cout << endl << "? ";
	while(std::getline(std::cin, str)) {
		if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);
		mlogo::memory::ActualArguments args;
		for(auto a : stmt.arguments) args.push_back(boost::apply_visitor(mlogo::parser::DisplayArgumentVisitor(), a));
		mlogo::memory::Stack::instance().callProcedure(stmt.name.name, args);
		cout << endl << "? ";
	}

	return 0;
}

