/**
 * @file: memory.cpp
 * Implements memory.hpp
 *
 * created on: 23 mar 2017
 *     author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include "memory.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "exceptions.hpp"

using namespace std;
using namespace mlogo::exceptions;

using boost::to_lower_copy;

namespace mlogo {

namespace memory {

const char Stack::__ARGUMENT_PREFIX[]{"_p"};

namespace {

std::string formatName(const std::string &name) { return to_lower_copy(name); }

}  // namespace

bool Frame::hasVariable(const std::string &name) const {
    auto iter = variables.find(formatName(name));
    return iter != variables.end();
}

Frame &Frame::setVariable(const std::string &name, const ValueBox &value) {
    variables[formatName(name)] = value;
    return *this;
}

ValueBox &Frame::getVariable(const std::string &name) {
    return variables.at(formatName(name));
}

const ValueBox &Frame::getVariable(const std::string &name) const {
    return variables.at(formatName(name));
}

bool Frame::hasProcedure(const std::string &name) const {
    auto iter = procedures.find(formatName(name));
    return iter != procedures.end();
}

Frame &Frame::setProcedure(const std::string &name, ProcedurePtr ptr) {
    if (ptr) {
        procedures[formatName(name)] = ptr;
        return *this;
    }

    throw InvalidProcedureBody(name, InvalidProcedureBody::NULL_PTR);
}

ProcedurePtr Frame::getProcedure(const std::string &name) {
    return procedures.at(formatName(name));
}

const ProcedurePtr Frame::getProcedure(const std::string &name) const {
    return procedures.at(formatName(name));
}

Frame &Frame::storeResult(const ValueBox &result) {
    hasResultSetted = true;
    _lastResult = result;
    return *this;
}

Frame &Frame::setResultVariable(const Frame &child) {
    if (!_lastResultVariable.empty()) {
        if (!child.hasResultSetted) throw NoReturnValueException();
        setVariable(_lastResultVariable, child._lastResult);
        child.hasResultSetted = false;
        _lastResultVariable = "";
    }

    return *this;
}

Frame &Frame::clear() {
    procedures.clear();
    variables.clear();
    _lastResultVariable.clear();
    hasResultSetted = false;

    return *this;
}

Stack::Stack() : frames(1) {
    // Populate global frame with internal symbols.
    // initInternalSymbols();
}

void Stack::callProcedure(const std::string &name, ActualArguments args,
                          const std::string &returnIn) {
    auto iter =
        find_if(frames.rbegin(), frames.rend(),
                [this, &name](Frame &f) { return f.hasProcedure(name); });

    if (iter != frames.rend()) {
        auto &func = *iter->getProcedure(name);

        if (func.isFunction()) currentFrame().waitForValueIn(returnIn);

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
        throw UndefinedProcedure(name);
}

ProcedurePtr Stack::getProcedure(const std::string &name) {
    auto iter =
        find_if(frames.rbegin(), frames.rend(),
                [this, &name](Frame &f) { return f.hasProcedure(name); });

    if (iter != frames.rend()) {
        return iter->getProcedure(name);
    } else
        throw UndefinedProcedure(name);
}

bool Stack::hasProcedure(const std::string &name) {
    auto iter =
        find_if(frames.rbegin(), frames.rend(),
                [this, &name](Frame &f) { return f.hasProcedure(name); });

    return iter != frames.rend();
}

std::size_t Stack::getProcedureNArgs(const std::string &name) {
    auto iter =
        find_if(frames.rbegin(), frames.rend(),
                [this, &name](Frame &f) { return f.hasProcedure(name); });

    if (iter != frames.rend()) {
        auto &func = *iter->getProcedure(name);
        return func.nArgs();
    } else
        throw UndefinedProcedure(name);
}

ValueBox &Stack::getVariable(const std::string &name) {
    auto iter =
        find_if(frames.rbegin(), frames.rend(),
                [this, &name](Frame &f) { return f.hasVariable(name); });

    if (iter != frames.rend()) {
        return iter->getVariable(name);
    }

    throw std::logic_error("Variable Undefined");
}

ValueBox &Stack::getArgument(uint8_t index) {
    return currentFrame().getVariable(argumentName(index));
}

Stack &Stack::setVariable(const std::string &name, const ValueBox &v,
                          bool global) {
    if (global) {
        globalFrame().setVariable(name, v);
    } else {
        currentFrame().setVariable(name, v);
    }

    return *this;
}

Stack &Stack::setLocalVariable(const std::string &name, const ValueBox &v) {
    return setVariable(name, v, false);
}

Stack &Stack::setProcedure(const std::string &name, ProcedurePtr v,
                           bool global) {
    if (global) {
        globalFrame().setProcedure(name, v);
    } else {
        currentFrame().setProcedure(name, v);
    }

    return *this;
}

Stack &Stack::setProcedure(const parser::Procedure &definition) {
    setProcedure(definition.name(),
                 std::make_shared<types::UserDefinedProcedure>(definition),
                 true);

    return *this;
}

Stack &Stack::openFrame() {
    frames.push_back(Frame());
    return *this;
}

Stack &Stack::closeFrame() {
    if (nFrames() <= 1) {  // current frame is global frame or no frames at all!
        throw UnclosableFrameException();
    }

    auto &current = frames[frames.size() - 1];
    auto &parent = frames[frames.size() - 2];

    if (current.hasResult() && parent.waitForValue()) {
        parent.setResultVariable(current);
    } else if (current.hasResult()) {
        throw InvalidReturnValue();
    } else if (parent.waitForValue()) {
        throw ExpectedReturnValue();
    }

    frames.pop_back();
    return *this;
}

std::string Stack::argumentName(uint8_t index) const {
    stringstream ss;
    ss << __ARGUMENT_PREFIX << index;

    return ss.str();
}

Stack &Stack::storeResult(const ValueBox &result) {
    currentFrame().storeResult(result);
    return *this;
}

Stack &Stack::clear() {
    while (nFrames() > 1) frames.pop_back();
    globalFrame().clear();

    return *this;
}

}  // namespace memory

}  // namespace mlogo
