/**
 * @file: types.hpp
 *
 *  Created on: 18 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <cinttypes>
#include <vector>

#include "value.hpp"

namespace mlogo {

namespace types {

class ActualArguments {
public:
	ActualArguments() {}

	ActualArguments &push_back(const ValueBox &value);
	ActualArguments &push_back(ValueBox &&value);

	const ValueBox &at(uint8_t index) const;
	ValueBox &at(uint8_t index);

private:
	std::vector<ValueBox> arguments;
};

class BasicProcedure {
public:
	BasicProcedure(uint8_t args, bool funct = false);
	virtual ~BasicProcedure() {}
	virtual void operator()() const = 0;

	uint8_t nArgs() const { return _nArgs; }
	bool isFunction() const { return _funct; }

protected:
	ValueBox &fetchArg(uint8_t index) const;
	void setReturnValue(const ValueBox &output) const;

private:
	uint8_t _nArgs;
	bool _funct;
};

} /* ns types */

} /* ns mlogo */


#endif /* TYPES_HPP_ */
