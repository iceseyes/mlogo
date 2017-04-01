/**
 * @file: types.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include "types.hpp"

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

} /* ns: types */

} /* ns: mlogo */
