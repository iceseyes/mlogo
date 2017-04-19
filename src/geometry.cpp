/**
 * @file: geometry.cpp
 */


#include "geometry.hpp"

namespace mlogo {

namespace geometry {

Angle::Angle(const Rad &angle) :
		_value(angle.value()) {}

Angle::Angle(Rad &&angle) :
		_value(angle.value()) {}

Angle::Angle(const Degrees &angle) :
		_value(angle.value()*RADIANS4DEGREES) {}

Angle::Angle(Degrees &&angle) :
		_value(angle.value()*RADIANS4DEGREES) {}

Angle::Degrees Angle::degrees() const {
	return Degrees(_value/RADIANS4DEGREES);
}

Angle::Rad Angle::radians() const {
	return Rad(_value);
}

bool Angle::equals(const Angle &angle) const {
	return angle._value == _value;
}

bool operator==(const Angle &a, const Angle &b) {
	return a.equals(b);
}

bool operator!=(const Angle &a, const Angle &b) {
	return !a.equals(b);
}

} /* ns: geometry */

} /* ns: mlogo */



