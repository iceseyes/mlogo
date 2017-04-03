/**
 * @file: types.hpp
 *
 *  Created on: 18 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <cinttypes>
#include <string>
#include <tuple>
#include <vector>

namespace mlogo {

namespace types {

class ActualArguments {
public:
	ActualArguments() {}

	ActualArguments &push_back(const std::string &value);
	ActualArguments &push_back(std::string &&value);

	std::string at(uint8_t index) const;

private:
	std::vector<std::string> arguments;
};

class BasicProcedure {
public:
	BasicProcedure(uint8_t args, bool funct = false);
	virtual ~BasicProcedure() {}
	virtual void operator()() const = 0;

	uint8_t nArgs() const { return _nArgs; }
	bool isFunction() const { return _funct; }

protected:
	std::string fetchArg(uint8_t index) const;
	void setReturnValue(const std::string output) const;

private:
	uint8_t _nArgs;
	bool _funct;
};

class Block {
public:
	Block();
	virtual ~Block() {}
	virtual void operator()() const;

private:
	std::vector<std::pair<std::string, ActualArguments>> statements;
};

} /* ns types */

} /* ns mlogo */



#endif /* TYPES_HPP_ */
