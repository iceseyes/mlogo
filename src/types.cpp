/**
 * @file: types.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "types.hpp"

#include "memory.hpp"

namespace mlogo {

namespace types {

ActualArguments &ActualArguments::push_back(const std::string &value) {
    arguments.push_back(value);
    return *this;
}

ActualArguments &ActualArguments::push_back(std::string &&value) {
    arguments.push_back(value);
    return *this;
}

std::string ActualArguments::at(uint8_t index) const {
    return arguments.at(index);
}

BasicProcedure::BasicProcedure(uint8_t args, bool funct) :
    _nArgs {args}, _funct {funct} {}

std::string BasicProcedure::fetchArg(uint8_t index) const {
    return memory::Stack::instance().getArgument(index);
}

} /* ns: types */

} /* ns: mlogo */
