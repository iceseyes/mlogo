/**
 * @file: geometry.cpp
 */


#include "geometry.hpp"

namespace mlogo {

namespace geometry {

namespace {
    constexpr double RADIANS4DEGREES { M_PI / 180 };
    constexpr double MAX_RAD { M_PI * 2 };

    double radNormalize(double r) {
        while(r<0) r += MAX_RAD;
        while(r>=MAX_RAD) r -= MAX_RAD;

        return r;
    }

    double deg2rad(double d) {
        return radNormalize(d*RADIANS4DEGREES);
    }
}

Angle::Angle(const Rad &angle) :
		_value(radNormalize(angle.value())) {}

Angle::Angle(Rad &&angle) :
		_value(radNormalize(angle.value())) {}

Angle::Angle(const Degrees &angle) :
		_value(deg2rad(angle.value())) {}

Angle::Angle(Degrees &&angle) :
		_value(deg2rad(angle.value())) {}

Angle::Degrees Angle::degrees() const {
	return Degrees(_value/RADIANS4DEGREES);
}

Angle::Rad Angle::radians() const {
	return Rad(_value);
}

bool Angle::equals(const Angle &angle) const {
	return std::fabs(angle._value - _value) < MAX_ERROR;
}

Angle &Angle::operator+=(const Angle &another) {
    _value = radNormalize(_value + another._value);
    return *this;
}

Angle &Angle::operator-=(const Angle &another) {
    _value = radNormalize(_value - another._value);
    return *this;
}

Angle &Angle::operator*=(double k) {
    _value = radNormalize(k*_value);
    return *this;
}

Angle &Angle::operator/=(double k) {
    _value = radNormalize(_value/k);
    return *this;
}

Angle &Angle::inv() {
    _value = radNormalize(1/_value);
    return *this;
}

double Angle::sin() const {
    double v = std::sin(_value);

    if(fabs(v)<MAX_ERROR) v = 0.0;

    return v;
}

double Angle::cos() const {
    double v = std::cos(_value);

    if(fabs(v)<MAX_ERROR) v = 0.0;

    return v;
}

double Angle::tan() const {
    double v = std::tan(_value);

    if(fabs(v)>1e4) throw std::logic_error("Tangent for right angle is undefined.");
    if(fabs(v)<MAX_ERROR) v = 0.0;

    return v;
}

bool operator==(const Angle &a, const Angle &b) {
	return a.equals(b);
}

bool operator!=(const Angle &a, const Angle &b) {
	return !a.equals(b);
}

Angle operator+(const Angle &a, const Angle &b) {
    Angle c { a };
    return (c += b);
}

Angle operator-(const Angle &a, const Angle &b) {
    Angle c { a };
    return (c -= b);
}

Angle operator*(const Angle &a, double k) {
    Angle b = a;
    return (b *= k);
}

Angle operator*(double k, const Angle &a) {
    Angle b = a;
    return (b *= k);
}

Angle operator/(const Angle &a, double k) {
    Angle b = a;
    return (b /= k);
}

Angle operator/(double k, const Angle &a) {
    Angle b = a;
    return (b.inv() * k);
}

double sin(const Angle &angle) {
    return angle.sin();
}

double cos(const Angle &angle) {
    return angle.cos();
}

double tan(const Angle &angle) {
    return angle.tan();
}

std::ostream &operator<<(std::ostream &s, const Angle::Rad &value) {
    s << value.value() << " rad";
    return s;
}

std::ostream &operator<<(std::ostream &s, const Angle::Degrees &value) {
    s << value.value() << " degrees";
    return s;
}

std::ostream &operator<<(std::ostream &s, const Angle &value) {
    s << value.radians();
    return s;
}

} /* ns: geometry */

} /* ns: mlogo */



