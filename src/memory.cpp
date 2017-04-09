/**
 * @file: memory.cpp
 * Implements memory.hpp
 *
 * created on: 23 mar 2017
 *     author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "memory.hpp"

#include <sstream>
#include <iostream>
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

Frame &Frame::setVariable(const std::string &name, const Value &value) {
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

Frame &Frame::storeResult(const Value &result) {
    hasResultSetted = true;
    _lastResult = result;
    return *this;
}

Frame &Frame::setResultVariable(const Frame &child) {
    if(!_lastResultVariable.empty()) {
        if(!child.hasResultSetted) throw std::logic_error("Current Frame does not mantain a return value.");
        setVariable(_lastResultVariable, child._lastResult);
        child.hasResultSetted = false;
        _lastResultVariable = "";
    }

    return *this;
}

Stack::Stack() :
        frames(1) {
    // Populate global frame with internal symbols.
    // initInternalSymbols();
}

void Stack::callProcedure(const std::string &name, ActualArguments args, const std::string &returnIn) {
    auto iter = find_if(
            frames.rbegin(), frames.rend(),
            [this, &name](Frame &f) {return f.hasProcedure(name);});

    if(iter != frames.rend()) {
        auto &func = *iter->getProcedure(name);

        if(func.isFunction()) currentFrame().waitForValueIn(returnIn);

        // open a new frame and store arguments
        auto &f = openFrame().currentFrame();
        for (int i = 0; i < func.nArgs(); ++i) {
            f.setVariable(argumentName(i), args.at(i));
        }

        // call the procedure
        func();

        // destroy function frame
        closeFrame();
    } else
        throw std::logic_error("Procedure Undefined or invalid arguments");
}

ProcedurePtr Stack::getProcedure(const std::string &name) {
    auto iter = find_if(
        frames.rbegin(), frames.rend(),
        [this, &name](Frame &f) {return f.hasProcedure(name);});

    if(iter != frames.rend()) {
        return iter->getProcedure(name);
    } else
        throw std::logic_error("Procedure Undefined");
}

std::size_t Stack::getProcedureNArgs(const std::string &name) {
    auto iter = find_if(
        frames.rbegin(), frames.rend(),
        [this, &name](Frame &f) {return f.hasProcedure(name);});

    if(iter != frames.rend()) {
        auto &func = *iter->getProcedure(name);
        return func.nArgs();
    } else
        throw std::logic_error("Procedure Undefined");
}

Value &Stack::getVariable(const std::string &name) {
    auto iter = find_if(
            frames.rbegin(), frames.rend(),
            [this, &name](Frame &f) {return f.hasVariable(name);});

    if(iter != frames.rend()) {
        return iter->getVariable(name);
    }

    throw std::logic_error("Variable Undefined");
}

Value &Stack::getArgument(uint8_t index) {
    return currentFrame().getVariable(argumentName(index));
}

Stack &Stack::openFrame() {
    frames.push_back(Frame());
    return *this;
}

Stack &Stack::closeFrame() {
    if(nFrames() <= 1) {  // current frame is global frame or no frames at all!
        throw std::logic_error("Global Frame cannot be closed.");
    }

    auto &current = frames[frames.size()-1];
    auto &parent = frames[frames.size()-2];

    if(current.hasResult() && parent.waitForValue()) {
        parent.setResultVariable(current);
    } else if(current.hasResult()) {
        throw std::logic_error("Procedure can not return a value to none.");
    } else if(parent.waitForValue()) {
        throw std::logic_error("Expected a function, found procedure instead.");
    }

    frames.pop_back();
    return *this;
}

std::string Stack::argumentName(uint8_t index) const {
    stringstream ss;
    ss << __ARGUMENT_PREFIX << index;

    return ss.str();
}

Stack &Stack::storeResult(const Value &result) {
    currentFrame().storeResult(result);
    return *this;
}

} /* ns: memory */
} /* ns: mlogo */
