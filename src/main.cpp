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

	string str;
	cout << endl << "? ";
	while(std::getline(std::cin, str)) {
		if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);
		cout << endl << "? ";
	}

	return 0;
}

