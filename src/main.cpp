/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <iostream>

#include "interpreter.hpp"

using namespace std;

extern "C" void initBuiltInProcedures();

int main(int argc, char **argv) {
    auto interpreter = mlogo::getInterpreter(cin, cout, cerr);
    initBuiltInProcedures();

    cout << "Welcome to myLogo v0.0" << endl;
    interpreter.run();

    return 0;
}
