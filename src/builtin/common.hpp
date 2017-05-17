/**
 * @file: common.hpp
 *
 *  Created on: 17 mag 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef BUILTIN_COMMON_HPP_
#define BUILTIN_COMMON_HPP_


#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "../types.hpp"
#include "../memory.hpp"
#include "../parser.hpp"
#include "../eval.hpp"
#include "../turtle.hpp"

using namespace std;

namespace mlogo { namespace builtin {

using Value = types::Value;
using ListValue = types::ListValue;
using ValueBox = types::ValueBox;

using BuiltinProcedure = types::BasicProcedure;
using Stack = memory::Stack;
using Turtle = turtle::Turtle;

using types::toString;

void initArithmeticBuiltInProcedures();
void initDataBuiltInProcedures();
void initCommBuiltInProcedures();
void initMemoryBuiltInProcedures();
void initControlBuiltInProcedures();
void initGraphicsBuiltInProcedures();

}}

#endif /* BUILTIN_COMMON_HPP_ */
