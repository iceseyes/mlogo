/**
 * @file: geometry.hpp
 */

#ifndef __GEOMETRY_HPP_
#define __GEOMETRY_HPP_

#include <cmath>
#include <iostream>
#include <vector>

namespace mlogo {

namespace geometry {

struct Point;

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

    Angle &operator+=(const Angle &another);
    Angle &operator-=(const Angle &another);
    Angle &operator*=(double k);
    Angle &operator/=(double k);

    Angle &inv();

    double sin() const;
    double cos() const;
    double tan() const;

private:
    double asRadians() const;
    double asDegrees() const;

    double _value;
};

class Reference {
public:
    explicit Reference(double kx = 1, int ox = 0, double ky = 1, int oy = 0);

    Point toGPS(const Point &p) const;
    Point fromGPS(const Point &p) const;

    bool operator==(const Reference &ref) const;
    bool operator!=(const Reference &ref) const;

    bool global() const;

private:
    double kx{1};
    double ky{1};
    int ox{0};
    int oy{0};
};

struct Point {
    Point(int x, int y, const Reference &system = Reference());

    bool same(const Point &p) const;

    bool operator==(const Point &p) const;
    bool operator!=(const Point &p) const;
    bool operator<(const Point &p) const;
    bool operator<=(const Point &p) const;
    bool operator>(const Point &p) const;
    bool operator>=(const Point &p) const;

    Point &operator+=(const Point &p);
    Point &operator-=(const Point &p);
    Point &operator*=(double k);
    Point &operator/=(double k);
    Point &scale(double a, double b);

    Point toGPS() const;
    Point rotate(const Angle &a) const;

    double distance(const Point &p);

    int x;
    int y;
    Reference system;
};

class Path {
    using Points = std::vector<Point>;

public:
    using iterator = Points::iterator;
    using const_iterator = Points::const_iterator;

    Path(const Reference &system, int x, int y);
    explicit Path(const Point &p);

    Path &push_back(int x, int y);
    Path &push_back(const Point &p);
    Path &push_back(Point &&p);
    Path &pushFromLast(int offsetX, int offsetY);

    Path translate(const Point &p) const;
    Path translate(int offsetX, int offsetY) const;

    Path rotate(const Angle &a) const;

    Point last() const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    std::size_t size() const;
    bool empty() const;

private:
    Points points;
    Reference system;
};

class StraightLine {
public:
    static const double VERTICAL;

    StraightLine(double m, double q, const Reference &system = Reference());
    StraightLine(const Angle &a, double q,
                 const Reference &system = Reference());
    StraightLine(const Point &a, const Point &b);
    StraightLine(double m, const Point &a);

    Angle angle() const;
    Point whenX(int x) const;
    Point whenY(int y) const;
    Point where(const StraightLine &line) const;
    bool belongTo(const Point &p) const;

    bool operator==(const StraightLine &line) const;
    bool operator!=(const StraightLine &line) const { return !(*this == line); }

    bool parallel(const StraightLine &line) const;
    StraightLine parallel(double q) const;

    bool isVertical() const;
    bool isHorizontal() const;

private:
    double m;
    double q;
    Reference system;

    friend std::ostream &operator<<(std::ostream &s, const StraightLine &value);
};

bool operator==(const Angle &a, const Angle &b);
bool operator!=(const Angle &a, const Angle &b);

Angle operator+(const Angle &a, const Angle &b);
Angle operator-(const Angle &a, const Angle &b);
Angle operator*(const Angle &a, double k);
Angle operator*(double k, const Angle &a);
Angle operator/(const Angle &a, double k);
Angle operator/(double k, const Angle &a);

Point operator+(const Point &a, const Point &b);
Point operator-(const Point &a, const Point &b);
Point operator*(const Point &a, double k);
Point operator*(double k, const Point &a);
Point operator/(const Point &a, double k);

double sin(const Angle &angle);
double cos(const Angle &angle);
double tan(const Angle &angle);

Angle arctan(double t);

StraightLine parallel(const StraightLine &r, double q);

std::ostream &operator<<(std::ostream &s, const Angle::Rad &value);
std::ostream &operator<<(std::ostream &s, const Angle::Degrees &value);
std::ostream &operator<<(std::ostream &s, const Angle &value);
std::ostream &operator<<(std::ostream &s, const Point &value);
std::ostream &operator<<(std::ostream &s, const StraightLine &value);

}  // namespace geometry

}  // namespace mlogo

#endif /* __GEOMETRY_HPP_ */
