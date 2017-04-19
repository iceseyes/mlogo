/**
 * @file: geometry.hpp
 */

#ifndef __GEOMETRY_HPP_
#define __GEOMETRY_HPP_

#include <cmath>

namespace mlogo {

namespace geometry {

constexpr double RADIANS4DEGREES { M_PI / 180 };

class Angle {
public:
	struct Rad {
		explicit Rad(double value) : _value(value) {}
		double value() const { return _value; }

	private:
		double _value;
	};

	struct Degrees {
		explicit Degrees(double value) : _value(value) {}
		double value() const { return _value; }

	private:
		double _value;
	};

	Angle(const Rad &angle);
	Angle(Rad &&angle);
	Angle(const Degrees &angle);
	Angle(Degrees &&angle);

	Degrees degrees() const;
	Rad radians() const;

	bool equals(const Angle &angle) const;
private:
	double _value;
};

bool operator==(const Angle &a, const Angle &b);
bool operator!=(const Angle &a, const Angle &b);

} /* ns: geometry */

} /* ns: mlogo */

#endif /* __GEOMETRY_HPP_ */
