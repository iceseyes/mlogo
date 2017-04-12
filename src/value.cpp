/**
 * @file: value.cpp
 */

#include "value.hpp"

using bad_get = boost::bad_get;

namespace mlogo {

namespace types {

ValueBox::ValueBox() {}

ValueBox::ValueBox(const Value &v)
	: _value(v) {}

bool ValueBox::isWord() const {
	try {
		boost::get<mlogo::types::WordValue>(_value);
	} catch (bad_get &e) {
		return false;
	}

	return true;
}

} /* ns: types */

} /* ns: mlogo */



