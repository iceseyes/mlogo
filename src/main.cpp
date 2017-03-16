/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <iostream>

#include "parser.hpp"

using namespace std;

int main(int argc, char **argv) {
	std::string str;
	while (std::getline(std::cin, str)) {
		if (str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);
		cout << "Read statement: " << stmt.name << " with arguments: ";
		for(auto a : stmt.arguments) cout << a.name << " ";
		cout << endl;
	}

    return 0;
}




