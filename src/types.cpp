/**
 * @file: types.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "types.hpp"

#include <sstream>
#include <algorithm>

#include "memory.hpp"

namespace mlogo {

namespace types {


std::string toString(const Value &v) {
    std::stringstream ss;
    ss << v;
    return ss.str();
}


ActualArguments &ActualArguments::push_back(const Value &value) {
    arguments.push_back(value);
    return *this;
}

ActualArguments &ActualArguments::push_back(Value &&value) {
    arguments.push_back(value);
    return *this;
}

const Value &ActualArguments::at(uint8_t index) const {
    return arguments.at(index);
}

Value &ActualArguments::at(uint8_t index) {
    return arguments.at(index);
}

BasicProcedure::BasicProcedure(uint8_t args, bool funct) :
        _nArgs { args }, _funct { funct } {}

Value &BasicProcedure::fetchArg(uint8_t index) const {
    return memory::Stack::instance().getArgument(index);
}

void BasicProcedure::setReturnValue(const Value &output) const {
    if(!_funct) {
        throw std::logic_error("Pure procedure can not return a value");
    }

    memory::Stack::instance().storeResult(output);
}

} /* ns: types */

} /* ns: mlogo */
