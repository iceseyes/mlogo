/**
 * @file: types.hpp
 *
 *  Created on: 18 mar 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>

namespace mlogo {

namespace types {

class Value;

class ValueType {
public:
	ValueType();
	ValueType(const std::string &name);
	virtual ~ValueType();

	virtual Value evaluate() = 0;

	std::string name;
};

class Procedure : public ValueType {
public:
	Procedure();
	Procedure(const std::string &name);

	Value evaluate() override;

	Procedure &setNextArgument(const Value &v);
};

class Variable : public ValueType {
public:
	Variable();
	Variable(const std::string &name);

	Value evaluate() override;

	std::string name;
};

class Constant : public ValueType {
public:
	Constant();
	Constant(const std::string &name);

	Value evaluate() override;

	std::string name;
};

} /* ns types */

} /* ns mlogo */



#endif /* TYPES_HPP_ */
