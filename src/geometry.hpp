/**
 * @file: geometry.hpp
 */

#ifndef __GEOMETRY_HPP_
#define __GEOMETRY_HPP_

#include <cmath>
#include <iostream>

namespace mlogo {

namespace geometry {

class Angle {
public:
    constexpr static double MAX_ERROR = 1e-5;

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

	Angle &operator+=(const Angle &another);
	Angle &operator-=(const Angle &another);
	Angle &operator*=(double k);
	Angle &operator/=(double k);

	Angle &inv();

	double sin() const;
	double cos() const;
	double tan() const;

private:
	double _value;
};

bool operator==(const Angle &a, const Angle &b);
bool operator!=(const Angle &a, const Angle &b);

Angle operator+(const Angle &a, const Angle &b);
Angle operator-(const Angle &a, const Angle &b);
Angle operator*(const Angle &a, double k);
Angle operator*(double k, const Angle &a);
Angle operator/(const Angle &a, double k);
Angle operator/(double k, const Angle &a);

double sin(const Angle &angle);
double cos(const Angle &angle);
double tan(const Angle &angle);

std::ostream &operator<<(std::ostream &s, const Angle::Rad &value);
std::ostream &operator<<(std::ostream &s, const Angle::Degrees &value);
std::ostream &operator<<(std::ostream &s, const Angle &value);

} /* ns: geometry */

} /* ns: mlogo */

#endif /* __GEOMETRY_HPP_ */
