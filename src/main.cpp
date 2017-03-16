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
    mlogo::parser::parse("hello world");
    return 0;
}




