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

#include "types.hpp"

namespace mlogo {

namespace memory {

class Frame {
	using ProcedurePtr = types::BasicProcedure *;
	using ActualArguments = types::ActualArguments;

public:
	Frame();
	Frame(Frame &parent);

	void pushProcedure(const std::string &name, ProcedurePtr *procObj);
	void callProcedure(const std::string &name, ActualArguments args);

	std::string &getVariable(const std::string &name);
	const std::string &getVariable(const std::string &name) const;

protected:
	ProcedurePtr _procByName(const std::string &name);

private:
	std::map<std::string, ProcedurePtr> procedures;
	std::map<std::string, std::string> variables;

	Frame *_parent;
};

} /* ns memory */

} /* ns mlogo */



#endif /* MEMORY_HPP_ */
