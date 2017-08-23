/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <cmath>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "eval.hpp"
#include "memory.hpp"
#include "parser.hpp"
#include "turtle.hpp"

using namespace std;
using namespace boost;

extern "C" void initBuiltInProcedures();

int main(int argc, char **argv) {
    initBuiltInProcedures();

    string str;
    cout << "Welcome to myLogo v0.0" << endl;

    cerr << endl << "? ";
    while (std::getline(std::cin, str)) {
        if (to_lower_copy(str) == "bye") break;

        mlogo::eval::AST ast;
        try {
            auto stmt = mlogo::parser::parse(str);
            ast = mlogo::eval::make_ast(stmt);

            ast();
        } catch (std::logic_error &e) {
            cerr << "I don't know how to " << str << " (" << e.what() << ")"
                 << endl;
        }

        cerr << "? ";
    }

    return 0;
}
