/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <fstream>
#include <iostream>

#include "interpreter.hpp"

using namespace std;

extern "C" void initBuiltInProcedures();

int main(int argc, char **argv) {
    auto interpreter = mlogo::getInterpreter(cin, cout, cerr);
    initBuiltInProcedures();

    for (int i = 1; i < argc; ++i) {
        cout << "Loading file: " << argv[i] << endl;
        ifstream infile(argv[i]);
        auto loader = mlogo::getInterpreter(infile, cout, cerr, false);
        loader.run();
    }

    cout << "Welcome to myLogo v0.0" << endl;
    interpreter.startup();
    interpreter.run();

    return 0;
}
