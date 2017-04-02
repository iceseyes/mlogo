/**
 * @file: memory.cpp
 * Implements memory.hpp
 *
 * created on: 23 mar 2017
 *     author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "memory.hpp"

#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace mlogo {
namespace memory {

const char Stack::__ARGUMENT_PREFIX[] { "_p" };

bool Frame::hasVariable(const std::string &name) const {
    auto iter = variables.find(name);
    return iter != variables.end();
}

Frame &Frame::setVariable(const std::string &name, const std::string &value) {
    variables[name] = value;
    return *this;
}

bool Frame::hasProcedure(const std::string &name) const {
    auto iter = procedures.find(name);
    return iter != procedures.end();
}

Frame &Frame::setProcedure(const std::string &name, ProcedurePtr ptr) {
    if(ptr) {
        procedures[name] = ptr;
        return *this;
    }

    throw invalid_argument("Function Pointer Must be a not null.");
}

Stack::Stack() : frames(1) {
    // Populate global frame with internal symbols.
    // initInternalSymbols();
}

void Stack::callProcedure(const std::string &name, ActualArguments args) {
    auto iter = find_if(
        frames.rbegin(), frames.rend(),
        [this, &name](Frame &f) { return f.hasProcedure(name); });

    if(iter!=frames.rend()) {
        auto &func = *iter->getProcedure(name);

        // open a new frame and store arguments
        auto f = openFrame().currentFrame();
        for(int i=0; i<func.nArgs(); ++i) {
            f.setVariable(argumentName(i), args.at(i));
        }

        // call the procedure
        func();

        // destroy function frame
        closeFrame();
    } else
        throw std::logic_error("Procedure Undefined or invalid arguments");
}

std::string &Stack::getVariable(const std::string &name) {
    auto iter = find_if(
        frames.rbegin(), frames.rend(),
        [this, &name](Frame &f) { return f.hasVariable(name); });

    if(iter!=frames.rend()) {
        return iter->getVariable(name);
    }

    throw std::logic_error("Variable Undefined");
}

std::string &Stack::getArgument(uint8_t index) {
    return currentFrame().getVariable(argumentName(index));
}

Stack &Stack::openFrame() {
    frames.push_back(Frame());
    return *this;
}

Stack &Stack::closeFrame() {
    if(nFrames()>1) { // current frame is not global frame
        frames.pop_back();
        return *this;
    }

    throw std::logic_error("Global Frame cannot be closed.");
}

std::string Stack::argumentName(uint8_t index) const {
    stringstream ss;
    ss << __ARGUMENT_PREFIX << index;

    return ss.str();
}

} /* ns: memory */
} /* ns: mlogo */
