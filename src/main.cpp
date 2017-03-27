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

int main(int argc, char **argv) {
	mlogo::memory::Frame f;
	f.procedures["print"] = [](mlogo::memory::Frame *f) {
		cout << f->arguments.front() << endl;
	};

	string str;
	cout << endl << "? ";
	while(std::getline(std::cin, str)) {
		if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);

		f.arguments.clear();
		for (auto a : stmt.arguments) {
			f.arguments.push_back(
				boost::apply_visitor(
					mlogo::parser::DisplayArgumentVisitor(), a));
		}

		f.procedures[stmt.name.name](&f);

		cout << endl << "? ";
	}

	return 0;
}

