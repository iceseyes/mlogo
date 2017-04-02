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
#include <vector>
#include <string>
#include <utility>

#include "types.hpp"

namespace mlogo {

namespace memory {

using ProcedurePtr = types::BasicProcedure *;
using ActualArguments = types::ActualArguments;

class Frame {
public:
	Frame() {}

	bool hasVariable(const std::string &name) const;
	std::string &getVariable(const std::string &name) { return variables.at(name); }
	const std::string &getVariable(const std::string &name) const { return variables.at(name); }
	Frame &setVariable(const std::string &name, const std::string &value);

	bool hasProcedure(const std::string &name) const;
	ProcedurePtr getProcedure(const std::string &name) { return procedures.at(name); }
	const ProcedurePtr getProcedure(const std::string &name) const { return procedures.at(name); }
	Frame &setProcedure(const std::string &name, ProcedurePtr ptr);
	template<typename Proc, typename... Args> Frame &setProcedure(const std::string &name, Args&&... args) {
		return setProcedure(name, new Proc(std::forward<Args>(args)...));
	}

private:
	std::map<std::string, ProcedurePtr> procedures;
	std::map<std::string, std::string> variables;
};
using FrameList = std::vector<Frame>;

class Stack {
public:
	static Stack &instance() {
		static Stack _instance;
		return _instance;
	}

	void callProcedure(const std::string &name, ActualArguments args);
	std::string &getVariable(const std::string &name);
	std::string &getArgument(uint8_t index);

	Frame &globalFrame() { return frames.front(); }
	const Frame &globalFrame() const { return frames.front(); }

	Frame &currentFrame() { return frames.back(); }
	const Frame &currentFrame() const { return frames.back(); }

	Stack &openFrame();
	std::size_t nFrames() const { return frames.size(); }
	Stack &closeFrame();

private:
	static const char __ARGUMENT_PREFIX[];

	Stack();
	Stack(const Stack &) = delete;
	Stack(Stack &&) = delete;

	Stack &operator=(const Stack&) = delete;
	Stack &operator=(Stack&&) = delete;

	std::string argumentName(uint8_t index) const;

	FrameList frames;
};

} /* ns memory */

} /* ns mlogo */



#endif /* MEMORY_HPP_ */
