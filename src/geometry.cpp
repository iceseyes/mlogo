/**
 * @file: geometry.cpp
 */

#include "geometry.hpp"

using namespace std;

namespace mlogo {

namespace geometry {

namespace {
constexpr double EPSILON{1e-5};
constexpr double M{1e4};
constexpr double RADIANS4DEGREES{M_PI / 180};
constexpr double MAX_RAD{M_PI * 2};

double radNormalize(double r) {
    while (r < 0) r += MAX_RAD;
    while (r >= MAX_RAD) r -= MAX_RAD;

    return r;
}

double deg2rad(double d) { return radNormalize(d * RADIANS4DEGREES); }

int myround(double v) {
    int sgn = v / fabs(v);

    return sgn * round(fabs(v));
}

bool isZero(double d) { return fabs(d) < EPSILON; }

bool isInf(double d) { return fabs(d) > M; }

double square(double x) { return x * x; }
}

const double StraightLine::VERTICAL{std::tan(M_PI / 2)};

Angle::Angle(const Rad &angle) : _value(radNormalize(angle.value())) {}

Angle::Angle(Rad &&angle) : _value(radNormalize(angle.value())) {}

Angle::Angle(const Degrees &angle) : _value(deg2rad(angle.value())) {}

Angle::Angle(Degrees &&angle) : _value(deg2rad(angle.value())) {}

Angle::Degrees Angle::degrees() const {
    return Degrees(_value / RADIANS4DEGREES);
}

Angle::Rad Angle::radians() const { return Rad(_value); }

bool Angle::equals(const Angle &angle) const {
    return isZero(angle._value - _value);
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
    _value = radNormalize(k * _value);
    return *this;
}

Angle &Angle::operator/=(double k) {
    _value = radNormalize(_value / k);
    return *this;
}

Angle &Angle::inv() {
    _value = radNormalize(1 / _value);
    return *this;
}

double Angle::sin() const {
    double v = std::sin(_value);

    if (isZero(v)) v = 0.0;

    return v;
}

double Angle::cos() const {
    double v = std::cos(_value);

    if (isZero(v)) v = 0.0;

    return v;
}

double Angle::tan() const {
    double v = std::tan(_value);

    if (isInf(v)) throw logic_error("Tangent for right angle is undefined.");
    if (isZero(v)) v = 0.0;

    return v;
}

Reference::Reference(double kx, int ox, double ky, int oy)
    : kx(kx), ky(ky), ox(ox), oy(oy) {}

Point Reference::toGPS(const Point &p) const {
    return Point(p.x / kx + ox, p.y / ky + oy);
}

Point Reference::fromGPS(const Point &p) const {
    return Point(kx * (p.x - ox), ky * (p.y - oy), *this);
}

bool Reference::operator==(const Reference &ref) const {
    return ((ref.ox == ox) && (ref.oy == oy) && (ref.kx == kx) &&
            (ref.ky == ky));
}

bool Reference::operator!=(const Reference &ref) const {
    return !(*this == ref);
}

bool Reference::global() const { return (*this == Reference()); }

Point::Point(int x, int y, const Reference &system)
    : x(x), y(y), system(system) {}

bool Point::same(const Point &p) const { return this->toGPS() == p.toGPS(); }

bool Point::operator==(const Point &p) const {
    return (p.x == x) && (p.y == y) && (p.system == system);
}

bool Point::operator!=(const Point &p) const { return !(*this == p); }

bool Point::operator<(const Point &p) const {
    return (system == p.system) && ((x < p.x) || ((x == p.x) && (y < p.y)));
}

bool Point::operator<=(const Point &p) const {
    return (*this < p) || (*this == p);
}

bool Point::operator>(const Point &p) const {
    return (system == p.system) && !(*this <= p);
}

bool Point::operator>=(const Point &p) const {
    return (system == p.system) && !(*this < p);
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

Point &Point::scale(double a, double b) {
    x *= a;
    y *= b;

    return *this;
}

Point Point::toGPS() const { return system.toGPS(*this); }

Point Point::rotate(const Angle &a) const {
    double sin_a = sin(a);
    double cos_a = cos(a);

    Point p{*this};
    p.x = myround(x * cos_a - y * sin_a);
    p.y = myround(x * sin_a + y * cos_a);

    return p;
}

double Point::distance(const Point &p) {
    return sqrt(square(x - p.x) + square(y - p.y));
}

Path::Path(const Reference &system, int x, int y) : system(system) {
    push_back(x, y);
}

Path::Path(const Point &p) : system(p.system) { push_back(p); }

Path &Path::push_back(int x, int y) {
    points.emplace_back(x, y, system);
    return *this;
}

Path &Path::push_back(const Point &p) {
    points.push_back(p);
    return *this;
}

Path &Path::push_back(Point &&p) {
    points.push_back(p);
    return *this;
}

Path &Path::push_from_last(int offsetX, int offsetY) {
    Point p = last() + Point{offsetX, offsetY};
    push_back(move(p));
    return *this;
}

Path Path::translate(const Point &p) const {
    Path path{*this};

    for (auto &point : path) point += p;

    return path;
}

Path Path::translate(int offsetX, int offsetY) const {
    Point p{offsetX, offsetY};
    return translate(p);
}

Path Path::rotate(const Angle &a) const {
    Path p{*this};

    for (auto &point : p) point = point.rotate(a);

    return p;
}

Point Path::last() const { return points.back(); }

Path::iterator Path::begin() noexcept { return points.begin(); }

Path::const_iterator Path::begin() const noexcept { return points.begin(); }

Path::iterator Path::end() noexcept { return points.end(); }

Path::const_iterator Path::end() const noexcept { return points.end(); }

size_t Path::size() const { return points.size(); }

bool Path::empty() const {
    return size() < 2;  // at least 2 points are needed by a path
}

StraightLine::StraightLine(double m, double q, const Reference &system)
    : m(m), q(q), system(system) {}

StraightLine::StraightLine(const Angle &a, double q, const Reference &system) {
    if (a != Angle::Degrees(90))
        m = tan(a);
    else
        m = VERTICAL;

    this->q = q;
    this->system = system;
}

StraightLine::StraightLine(const Point &a, const Point &b) {
    if (a.system != b.system)
        throw std::logic_error("Ambiguous Reference System.");

    int dx = (b.x - a.x);
    if (dx == 0) {
        m = VERTICAL;
        q = b.x;
    } else {
        m = (1.0 * b.y - a.y) / dx;
        q = 1.0 * b.y - m * b.x;
    }

    system = a.system;
}

StraightLine::StraightLine(double m, const Point &a)
    : m(m), q(a.y - m * a.x), system(a.system) {}

Angle StraightLine::angle() const { return Angle::Rad(atan(m)); }

Point StraightLine::whenX(int x) const {
    if (isVertical())
        throw logic_error(
            "straight line is vertical and Y coordinate cannot be computed "
            "from X.");
    return Point(x, myround(m * x + q), system);
}

Point StraightLine::whenY(int y) const {
    if (isHorizontal())
        throw logic_error(
            "straight line is horizontal and X coordinate cannot be computed "
            "from Y.");
    if (isVertical()) return Point(myround(q), y, system);

    return Point(myround((y - q) / m), y, system);
}

Point StraightLine::where(const StraightLine &line) const {
    if (line.system != system)
        throw logic_error("Straight lines in different reference system");
    if (parallel(line)) throw logic_error("Straight lines are parallels");

    if (isVertical()) {
        return line.whenX(q);
    } else if (line.isVertical()) {
        return whenX(line.q);
    }

    return whenX(myround((q - line.q) / (line.m - m)));
}

bool StraightLine::belongTo(const Point &p) const {
    Point p1 = system.fromGPS(p.toGPS());

    if (isInf(m)) return q == p1.x;

    int y = myround(m * p1.x + q);

    return !(p1.y - y);
}

bool StraightLine::operator==(const StraightLine &line) const {
    return (line.system == system) && isZero(line.m - m) && isZero(line.q - q);
}

bool StraightLine::parallel(const StraightLine &line) const {
    return isZero(line.m - m);
}

StraightLine StraightLine::parallel(double q) const {
    return StraightLine(m, q, system);
}

bool StraightLine::isVertical() const { return isInf(m); }

bool StraightLine::isHorizontal() const { return isZero(m); }

bool operator==(const Angle &a, const Angle &b) { return a.equals(b); }

bool operator!=(const Angle &a, const Angle &b) { return !a.equals(b); }

Angle operator+(const Angle &a, const Angle &b) {
    Angle c{a};
    return (c += b);
}

Angle operator-(const Angle &a, const Angle &b) {
    Angle c{a};
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
    Point c{a};
    return c += b;
}

Point operator-(const Point &a, const Point &b) {
    Point c{a};
    return c -= b;
}

Point operator*(const Point &a, double k) {
    Point b{a};
    return b *= k;
}

Point operator*(double k, const Point &a) {
    Point b{a};
    return b *= k;
}

Point operator/(const Point &a, double k) {
    Point b{a};
    return b /= k;
}

double sin(const Angle &angle) { return angle.sin(); }

double cos(const Angle &angle) { return angle.cos(); }

double tan(const Angle &angle) { return angle.tan(); }

StraightLine parallel(const StraightLine &r, double q) { return r.parallel(q); }

ostream &operator<<(ostream &s, const Angle::Rad &value) {
    s << value.value() << " rad";
    return s;
}

ostream &operator<<(ostream &s, const Angle::Degrees &value) {
    s << value.value() << " degrees";
    return s;
}

ostream &operator<<(ostream &s, const Angle &value) {
    s << value.radians();
    return s;
}

ostream &operator<<(ostream &s, const Point &value) {
    s << "(" << value.x << "," << value.y << ")";
    return s;
}

std::ostream &operator<<(std::ostream &s, const StraightLine &value) {
    if (isInf(value.m))
        s << "x = " << value.q;
    else
        s << "y = " << value.m << "x + " << value.q;

    return s;
}

} /* ns: geometry */

} /* ns: mlogo */
