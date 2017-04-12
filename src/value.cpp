/**
 * @file: value.cpp
 */

#include "value.hpp"

using bad_get = boost::bad_get;

namespace mlogo {

namespace types {

ValueBox::ValueBox() {}

ValueBox::ValueBox(const char v[]) :
    _value(std::string(v)) {}

ValueBox::ValueBox(const std::string &v) :
        _value(v) {}

ValueBox::ValueBox(const ListValue &v) :
        _value(v) {}

ValueBox::ValueBox(const Value &v) :
        _value(v) {}

ValueBox &ValueBox::operator=(const ValueBox &v) {
    _value = v._value;
    return *this;
}

bool ValueBox::isWord() const {
	try {
		boost::get<mlogo::types::WordValue>(_value);
	} catch (bad_get &e) {
		return false;
	}

	return true;
}

bool ValueBox::operator==(const ValueBox &v1) const {
    return _value == v1._value;
}

} /* ns: types */

} /* ns: mlogo */



