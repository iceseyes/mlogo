/**
 * @file: memory.hpp
 *
 *  Created on: 18 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <cinttypes>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "defines.hpp"
#include "parser.hpp"
#include "types.hpp"

namespace mlogo {

namespace memory {

using ValueBox = types::ValueBox;
using ProcedurePtr = std::shared_ptr<types::BasicProcedure>;
using ActualArguments = types::ActualArguments;

class Frame {
public:
    Frame() {}

    bool hasVariable(const std::string &name) const;
    ValueBox &getVariable(const std::string &name);
    const ValueBox &getVariable(const std::string &name) const;
    Frame &setVariable(const std::string &name, const ValueBox &value);

    bool hasProcedure(const std::string &name) const;
    ProcedurePtr getProcedure(const std::string &name);
    const ProcedurePtr getProcedure(const std::string &name) const;
    Frame &setProcedure(const std::string &name, ProcedurePtr ptr);
    template <typename Proc, typename... Args>
    Frame &setProcedure(const std::string &name, Args &&... args) {
        return setProcedure(
            name, std::make_shared<Proc>(std::forward<Args>(args)...));
    }

    Frame &storeResult(const ValueBox &result);
    Frame &setResultVariable(const Frame &child);
    Frame &waitForValueIn(const std::string &varName) {
        _lastResultVariable = varName;
        return *this;
    }

    bool hasResult() const { return hasResultSetted; }
    bool waitForValue() const { return !_lastResultVariable.empty(); }

    /**
     * Delete every variable and procedure in this frame.
     * Restore frame in its just-created state.
     *
     * @return a reference to this Frame.
     */
    Frame &clear();

private:
    std::map<std::string, ProcedurePtr> procedures;
    std::map<std::string, ValueBox> variables;
    ValueBox _lastResult;
    std::string _lastResultVariable;
    mutable bool hasResultSetted{false};
};
using FrameList = std::vector<Frame>;

class Stack {
public:
    static Stack &instance() {
        static Stack _instance;
        return _instance;
    }

    void callProcedure(
        const std::string &name, ActualArguments args,
        const std::string &returnIn = "___discard_return_value__");
    ProcedurePtr getProcedure(const std::string &name);
    bool hasProcedure(const std::string &name);
    std::size_t getProcedureNArgs(const std::string &name);
    ValueBox &getVariable(const std::string &name);
    ValueBox &getArgument(uint8_t index);

    Stack &setVariable(const std::string &name, const ValueBox &v,
                       bool global = true);
    Stack &setLocalVariable(const std::string &name, const ValueBox &v);

    Stack &setProcedure(const std::string &name, ProcedurePtr v,
                        bool global = true);
    template <typename Proc, typename... Args>
    Stack &setProcedure(const std::string &name, Args &&... args) {
        return setProcedure(
            name, std::make_shared<Proc>(std::forward<Args>(args)...));
    }
    Stack &setProcedure(const parser::Procedure &definition);

    template <typename Proc, typename... Args>
    Stack &setLocalProcedure(const std::string &name, Args &&... args) {
        return setProcedure(
            name, std::make_shared<Proc>(std::forward<Args>(args)...), false);
    }

    Frame &globalFrame() { return frames.front(); }
    const Frame &globalFrame() const { return frames.front(); }

    Frame &currentFrame() { return frames.back(); }
    const Frame &currentFrame() const { return frames.back(); }

    Stack &openFrame();
    std::size_t nFrames() const { return frames.size(); }
    Stack &closeFrame();

    Stack &storeResult(const ValueBox &result);

    /**
     * Delete every frame but the global and clear global frame.
     * This method restore as-just-created state the Stack.
     *
     * @return a reference to this Stack
     */
    Stack &clear();

private:
    static const char __ARGUMENT_PREFIX[];

    Stack();
    Stack(const Stack &) = delete;
    Stack(Stack &&) = delete;

    Stack &operator=(const Stack &) = delete;
    Stack &operator=(Stack &&) = delete;

    std::string argumentName(uint8_t index) const;

    FrameList frames;
};

} /* ns memory */

} /* ns mlogo */

#endif /* MEMORY_HPP_ */
