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

Reference::Reference(double kx, int ox, double ky, int oy) :
	kx(kx), ky(ky), ox(ox), oy(oy) {}

Point Reference::toGPS(const Point &p) const {
	return Point(p.x/kx + ox, p.y/ky + oy);
}

Point Reference::fromGPS(const Point &p) const {
	return Point(kx*p.x - ox, ky*p.y - oy, *this);
}

bool Reference::operator==(const Reference &ref) const {
	return ((ref.ox == ox)&&(ref.oy == oy)&&(ref.kx == kx)&&(ref.ky == ky));
}

bool Reference::operator!=(const Reference &ref) const {
	return !(*this == ref);
}

bool Reference::global() const {
	return (*this == Reference());
}

Point::Point(int x, int y, const Reference &system) :
	x(x), y(y), system(system) {}

bool Point::same(const Point &p) const {
	return this->toGPS() == p.toGPS();
}

bool Point::operator==(const Point &p) const {
	return (p.x == x) && (p.y == y) && (p.system == system);
}

bool Point::operator!=(const Point &p) const {
	return !(*this == p);
}

bool Point::operator<(const Point &p) const {
	return (system == p.system) && ((x < p.x) || ((x == p.x) && (y < p.y)));
}

bool Point::operator<=(const Point &p) const {
	return (*this < p) || (*this == p);
}

bool Point::operator>(const Point &p) const {
	return !(*this <= p);
}

bool Point::operator>=(const Point &p) const {
	return !(*this < p);
}

Point &Point::operator+=(const Point &p) {
	x += p.x;
	y += p.y;

	return *this;
}

Point &Point::operator-=(const Point &p) {
	x -= p.x;
	y -= p.y;

	return *this;
}

Point &Point::operator*=(double k) {
	x *= k;
	y *= k;

	return *this;
}

Point &Point::operator/=(double k) {
	x /= k;
	y /= k;

	return *this;
}

Point Point::toGPS() const {
	return system.toGPS(*this);
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

Point operator+(const Point &a, const Point &b) {
	Point c { a };
	return c += b;
}

Point operator-(const Point &a, const Point &b) {
	Point c { a };
	return c -= b;
}

Point operator*(const Point &a, double k) {
	Point b { a };
	return b *= k;
}

Point operator*(double k, const Point &a) {
	Point b { a };
	return b *= k;
}

Point operator/(const Point &a, double k) {
	Point b { a };
	return b /= k;
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



