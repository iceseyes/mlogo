/**
 * @file: test_geometry.cpp
 */


#include <gtest/gtest.h>

#include "geometry.hpp"


using Angle = mlogo::geometry::Angle;
using Reference = mlogo::geometry::Reference;
using Point = mlogo::geometry::Point;
using Path = mlogo::geometry::Path;
using StraightLine = mlogo::geometry::StraightLine;

constexpr double MAX_RAD_ERROR { 0.001 };
constexpr double MAX_DEG_ERROR { 0.1 };

TEST(Angle, simpleAngles) {
	Angle zero_rad { Angle::Rad(0) };
	Angle zero_deg { Angle::Degrees(0) };

	ASSERT_EQ(0.0, zero_rad.radians().value());
	ASSERT_EQ(0.0, zero_rad.degrees().value());
	ASSERT_EQ(0.0, zero_deg.radians().value());
	ASSERT_EQ(0.0, zero_deg.degrees().value());
	ASSERT_EQ(zero_rad, zero_deg);
	ASSERT_EQ(zero_deg, zero_rad);

	Angle right { Angle::Rad(M_PI/2) };
	ASSERT_NEAR(1.57, right.radians().value(), MAX_RAD_ERROR);
	ASSERT_EQ(90.0, right.degrees().value());

	right = Angle::Degrees(90);
	Angle right_deg { Angle::Degrees(90) };

	ASSERT_NEAR(1.57, right.radians().value(), MAX_RAD_ERROR);
    ASSERT_EQ(90.0, right.degrees().value());
	ASSERT_EQ(right_deg, right);

	Angle anAngle { Angle::Degrees(180) };
	ASSERT_NEAR(M_PI, anAngle.radians().value(), MAX_RAD_ERROR);
	ASSERT_EQ(180.0, anAngle.degrees().value());

	anAngle = Angle::Rad(3*M_PI/2);
	ASSERT_NEAR(3*M_PI/2, anAngle.radians().value(), MAX_RAD_ERROR);
    ASSERT_EQ(270.0, anAngle.degrees().value());

    anAngle = Angle::Rad(M_PI/4);
    ASSERT_NEAR(M_PI/4, anAngle.radians().value(), MAX_RAD_ERROR);
    ASSERT_EQ(45.0, anAngle.degrees().value());

    anAngle = right;
    ASSERT_EQ(right, anAngle);
}

TEST(Angle, strangeAngles) {
    Angle anAngle { Angle::Degrees(30) };

    ASSERT_TRUE(Angle::Rad(M_PI/6) == anAngle);
    ASSERT_DOUBLE_EQ(30.0, anAngle.degrees().value());

    anAngle = Angle::Rad(0.401426);
    ASSERT_NEAR(23.0, anAngle.degrees().value(), MAX_DEG_ERROR);
    ASSERT_EQ(Angle::Degrees(23.0), anAngle);

    anAngle = Angle::Degrees(101.3);
    ASSERT_NEAR(1.768, anAngle.radians().value(), MAX_RAD_ERROR);
    ASSERT_DOUBLE_EQ(101.3, anAngle.degrees().value());

    anAngle = Angle::Rad(0.57264253);
    ASSERT_NEAR(32.81, anAngle.degrees().value(), MAX_DEG_ERROR);
    ASSERT_EQ(Angle::Degrees(32.81), anAngle);
}

TEST(Angle, turnEquivalence) {
    Angle anAngle { Angle::Degrees(30) };

    Angle another { Angle::Degrees(360 + 30) };
    ASSERT_EQ(anAngle, another);

    another = Angle::Degrees(-330);
    ASSERT_EQ(anAngle, another);

    another = Angle::Degrees(5*360 + 30);
    ASSERT_EQ(anAngle, another);

    anAngle = Angle::Rad(0.401426);
    another = Angle::Degrees(360 + 23);
    ASSERT_EQ(anAngle, another);

    another = Angle::Degrees(-4*360 + 23);
    ASSERT_EQ(anAngle, another);

    another = Angle::Degrees(7*360 + 23);
    ASSERT_EQ(anAngle, another);

    another = Angle::Rad(6*M_PI + 0.401426);
    ASSERT_EQ(anAngle, another);

    another = Angle::Rad(-4*M_PI + 0.401426);
    ASSERT_EQ(anAngle, another);
}

TEST(Angle, basicOperations) {
    Angle anAngle { Angle::Degrees(30) };
    Angle another { Angle::Degrees(45) };

    Angle sum = anAngle + another;
    ASSERT_EQ(Angle::Degrees(75), sum);

    sum += Angle::Degrees(15);
    ASSERT_EQ(Angle::Degrees(90), sum);
    ASSERT_EQ(Angle::Rad(M_PI/2), sum);

    sum -= Angle::Degrees(25);
    ASSERT_EQ(Angle::Degrees(65), sum);

    another = anAngle - sum + Angle::Rad(M_PI/6);
    ASSERT_EQ(Angle::Degrees(355), another);

    anAngle = Angle::Degrees(45);
    ASSERT_EQ(Angle::Rad(M_PI), anAngle*4);

    another = 2 * anAngle;
    ASSERT_EQ(Angle::Rad(M_PI/2), another);

    another *= 2;
    ASSERT_EQ(Angle::Rad(M_PI), another);

    ASSERT_EQ(Angle::Rad(M_PI/2), another/2);

    anAngle = another/6;
    ASSERT_EQ(Angle::Degrees(30), anAngle);
    ASSERT_EQ(Angle::Degrees(30), anAngle+Angle::Degrees(360));
    ASSERT_EQ(Angle::Degrees(30), anAngle-Angle::Degrees(360));

    ASSERT_EQ(Angle::Degrees(3), anAngle/10);
    ASSERT_EQ(Angle::Degrees(30), 13*anAngle);
}

TEST(Angle, basicTrigonometry) {
    Angle anAngle { Angle::Degrees(0) };

    ASSERT_EQ(0.0, anAngle.sin());
    ASSERT_EQ(0.0, sin(anAngle));
    ASSERT_EQ(1.0, anAngle.cos());
    ASSERT_EQ(1.0, cos(anAngle));

    anAngle += Angle::Rad(M_PI);

    ASSERT_EQ(0.0, anAngle.sin());
    ASSERT_EQ(0.0, sin(anAngle));
    ASSERT_EQ(-1.0, anAngle.cos());
    ASSERT_EQ(-1.0, cos(anAngle));

    anAngle += Angle::Rad(M_PI);

    ASSERT_EQ(0.0, anAngle.sin());
    ASSERT_EQ(0.0, sin(anAngle));
    ASSERT_EQ(1.0, anAngle.cos());
    ASSERT_EQ(1.0, cos(anAngle));

    anAngle = Angle::Rad(M_PI/2);

    ASSERT_EQ(1.0, anAngle.sin());
    ASSERT_EQ(1.0, sin(anAngle));
    ASSERT_EQ(0.0, anAngle.cos());
    ASSERT_EQ(0.0, cos(anAngle));

    anAngle += Angle::Degrees(180);

    ASSERT_EQ(-1.0, anAngle.sin());
    ASSERT_EQ(-1.0, sin(anAngle));
    ASSERT_EQ(0.0, anAngle.cos());
    ASSERT_EQ(0.0, cos(anAngle));

    anAngle += Angle::Degrees(180);

    ASSERT_EQ(1.0, anAngle.sin());
    ASSERT_EQ(1.0, sin(anAngle));
    ASSERT_EQ(0.0, anAngle.cos());
    ASSERT_EQ(0.0, cos(anAngle));

    ASSERT_THROW(anAngle.tan(), std::logic_error);
    ASSERT_THROW(tan(anAngle), std::logic_error);
    ASSERT_THROW((anAngle*3).tan(), std::logic_error);

    anAngle = Angle::Degrees(0);
    ASSERT_EQ(0.0, tan(anAngle));

    anAngle = Angle::Degrees(180);
    ASSERT_EQ(0.0, tan(anAngle));

    anAngle = Angle::Degrees(89);
    ASSERT_NEAR(57.29, tan(anAngle), 0.001);

    anAngle = Angle::Degrees(45);
    ASSERT_NEAR(1, tan(anAngle), 0.001);
}

TEST(Reference, globalReference) {
    Reference global;

    ASSERT_TRUE(global.global());
    ASSERT_EQ(Reference(1, 0, 1, 0), global);
    ASSERT_NE(Reference(1, 0, -1, 0), global);
}

TEST(Reference, turtleLikeReference) {
    Reference turtle { 1, 320, -1, 240 };

    ASSERT_FALSE(turtle.global());
    ASSERT_NE(Reference(1, 0, 1, 0), turtle);
    ASSERT_EQ(Reference(1, 320, -1, 240), turtle);
    ASSERT_NE(Reference(1, 0, -1, 0), turtle);

    Point p = turtle.toGPS({0, 0});
    ASSERT_EQ(320, p.x);
    ASSERT_EQ(240, p.y);
    ASSERT_TRUE(p.system.global());

    p = turtle.toGPS({10, 10});
    ASSERT_EQ(330, p.x);
    ASSERT_EQ(230, p.y);
    ASSERT_TRUE(p.system.global());

    p = turtle.toGPS({-10, -10});
    ASSERT_EQ(310, p.x);
    ASSERT_EQ(250, p.y);
    ASSERT_TRUE(p.system.global());

    p = turtle.fromGPS({ 320, 240 });
    ASSERT_EQ(0, p.x);
    ASSERT_EQ(0, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(turtle, p.system);

    p = turtle.fromGPS({ 310, 250 });
    ASSERT_EQ(-10, p.x);
    ASSERT_EQ(-10, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(turtle, p.system);

    p = turtle.fromGPS({ 330, 230 });
    ASSERT_EQ(10, p.x);
    ASSERT_EQ(10, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(turtle, p.system);

    p = turtle.fromGPS({ 330, 250 });
    ASSERT_EQ(10, p.x);
    ASSERT_EQ(-10, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(turtle, p.system);
}

TEST(Reference, halfReference) {
    Reference ref { 0.5, -50, -0.5, 100 };

    ASSERT_FALSE(ref.global());

    Point p = ref.toGPS({0, 0});
    ASSERT_EQ(-50, p.x);
    ASSERT_EQ(100, p.y);
    ASSERT_TRUE(p.system.global());

    p = ref.toGPS({10, 10});
    ASSERT_EQ(-30, p.x);
    ASSERT_EQ(80, p.y);
    ASSERT_TRUE(p.system.global());

    p = ref.toGPS({-10, -10});
    ASSERT_EQ(-70, p.x);
    ASSERT_EQ(120, p.y);
    ASSERT_TRUE(p.system.global());

    p = ref.fromGPS({ -50, 100 });
    ASSERT_EQ(0, p.x);
    ASSERT_EQ(0, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(ref, p.system);

    p = ref.fromGPS({ -70, 120 });
    ASSERT_EQ(-10, p.x);
    ASSERT_EQ(-10, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(ref, p.system);

    p = ref.fromGPS({-30, 80 });
    ASSERT_EQ(10, p.x);
    ASSERT_EQ(10, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(ref, p.system);

    p = ref.fromGPS({ -30, 120 });
    ASSERT_EQ(10, p.x);
    ASSERT_EQ(-10, p.y);
    ASSERT_FALSE(p.system.global());
    ASSERT_EQ(ref, p.system);
}

TEST(Point, globalPoints) {
    Point p { 100, 200 };

    ASSERT_EQ(100, p.x);
    ASSERT_EQ(200, p.y);
    ASSERT_TRUE(p.system.global());

    p = { 320, 240 };
    ASSERT_EQ(320, p.x);
    ASSERT_EQ(240, p.y);
    ASSERT_TRUE(p.system.global());

    ASSERT_EQ(Point({320, 240}), p);
    ASSERT_TRUE(p.same({320, 240}));
}

TEST(Point, globalOrderedPoints) {
    Point p1 { 100, 200 };
    Point p2 { 320, 240 };
    Point p3 { 320, -200 };
    Point p4 { -100, 0 };

    ASSERT_TRUE(p1 < p2);
    ASSERT_TRUE(p1 <= p2);
    ASSERT_TRUE(p3 <= p2);
    ASSERT_TRUE(p2 > p3);
    ASSERT_TRUE(p4 < p1);
    ASSERT_TRUE(p4 <= Point({-100, 0}));
    ASSERT_TRUE(Point({-100, 0}) >= p4);
}

TEST(Point, globalOperations) {
    Point p1 { 100, 200 };
    Point t = p1 - Point({-50, 100});

    ASSERT_EQ(Point({150, 100}), t);

    t += { 50, 50 };

    ASSERT_EQ(Point({200, 150}), t);

    t = t * 2;

    ASSERT_EQ(Point({400, 300}), t);

    t /= 2;

    ASSERT_EQ(Point({200, 150}), t);

    t = 2 * t;

    ASSERT_EQ(Point({400, 300}), t);
}

TEST(Point, localPosition) {
    Reference system { 1, 320, -1, 240 };
    Point p1 { 0, 0, system };

    ASSERT_EQ(0, p1.x);
    ASSERT_EQ(0, p1.y);

    ASSERT_TRUE(Point({320, 240}).same(p1));
    ASSERT_TRUE(p1.same(Point({320, 240})));

    ASSERT_NE(Point({320, 240}), p1);

    Point globalP1 { p1.toGPS() };
    ASSERT_EQ(Point({320, 240}), globalP1);

    Point p2 { p1 + Point {100, -25} };
    ASSERT_EQ(100, p2.x);
    ASSERT_EQ(-25, p2.y);

    ASSERT_TRUE(p1 < p2);
    ASSERT_TRUE(p1 <= p2);
    ASSERT_TRUE(p2 > p1);
    ASSERT_TRUE(p2 >= p1);

    // p1, p2 using different reference system
    // so are incomparable with globalP1
    ASSERT_FALSE(p1 > globalP1);
    ASSERT_FALSE(p1 >= globalP1);
    ASSERT_FALSE(p2 > globalP1);
    ASSERT_FALSE(p2 >= globalP1);

    ASSERT_FALSE(p1 < globalP1);
    ASSERT_FALSE(p1 <= globalP1);
    ASSERT_FALSE(p2 < globalP1);
    ASSERT_FALSE(p2 <= globalP1);

    p2 /= 2;
    ASSERT_EQ(50, p2.x);
    ASSERT_EQ(-12, p2.y); // TRUNC-ed!
    ASSERT_EQ(system, p2.system);

    p2 *= 2;
    ASSERT_EQ(100, p2.x);
    ASSERT_EQ(-24, p2.y);
    ASSERT_EQ(system, p2.system);
}

TEST(Path, turtle) {
    Reference ref { 1, 320, -1, 240 };
    Path turtle { ref, 8, 0 };

    ASSERT_TRUE(turtle.empty());
    ASSERT_EQ(1u, turtle.size());
    ASSERT_EQ(Point(8, 0, ref), turtle.last());

    turtle.push_back(-8, 5);

    ASSERT_FALSE(turtle.empty());
    ASSERT_EQ(2u, turtle.size());
    ASSERT_EQ(Point(-8, 5, ref), turtle.last());

    turtle.push_back(-8, -5);

    ASSERT_FALSE(turtle.empty());
    ASSERT_EQ(3u, turtle.size());
    ASSERT_EQ(Point(-8, -5, ref), turtle.last());

    turtle.push_back(8, 0);

    ASSERT_FALSE(turtle.empty());
    ASSERT_EQ(4u, turtle.size());
    ASSERT_EQ(Point(8, 0, ref), turtle.last());

    auto i = turtle.begin();
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(Point(-8, 5, ref), *(i++));
    ASSERT_EQ(Point(-8, -5, ref), *(i++));
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(i, turtle.end());
}

TEST(Path, turtleRotate) {
    Reference ref { 1, 320, -1, 240 };
    Path turtle { ref, 8, 0 };
    turtle.push_back(-8, 5);
    turtle.push_back(-8, -5);
    turtle.push_back(8, 0);

    auto t1 = turtle.rotate(Angle::Degrees(90));

    auto i = t1.begin();
    ASSERT_EQ(Point(0, 8, ref), *(i++));
    ASSERT_EQ(Point(-5, -8, ref), *(i++));
    ASSERT_EQ(Point(5, -8, ref), *(i++));
    ASSERT_EQ(Point(0, 8, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    i = turtle.begin();
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(Point(-8, 5, ref), *(i++));
    ASSERT_EQ(Point(-8, -5, ref), *(i++));
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(i, turtle.end());

    t1 = t1.rotate(Angle(Angle::Degrees(-45)));

    i = t1.begin();
    ASSERT_EQ(Point(6, 6, ref), *(i++));
    ASSERT_EQ(Point(-9, -2, ref), *(i++));
    ASSERT_EQ(Point(-2, -9, ref), *(i++));
    ASSERT_EQ(Point(6, 6, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    t1 = t1.rotate(Angle(Angle::Degrees(-45)));

    i = t1.begin();
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(Point(-8, 5, ref), *(i++));
    ASSERT_EQ(Point(-8, -5, ref), *(i++));
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(i, t1.end());
}

TEST(Path, turtleTranslate) {
    Reference ref { 1, 320, -1, 240 };
    Path turtle { ref, 8, 0 };
    turtle.push_back(-8, 5);
    turtle.push_back(-8, -5);
    turtle.push_back(8, 0);

    auto t1 = turtle.translate(100, 0);

    auto i = t1.begin();
    ASSERT_EQ(Point(108, 0, ref), *(i++));
    ASSERT_EQ(Point(92, 5, ref), *(i++));
    ASSERT_EQ(Point(92, -5, ref), *(i++));
    ASSERT_EQ(Point(108, 0, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    i = turtle.begin();
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(Point(-8, 5, ref), *(i++));
    ASSERT_EQ(Point(-8, -5, ref), *(i++));
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(i, turtle.end());

    t1 = t1.translate(0, 100);

    i = t1.begin();
    ASSERT_EQ(Point(108, 100, ref), *(i++));
    ASSERT_EQ(Point(92, 105, ref), *(i++));
    ASSERT_EQ(Point(92, 95, ref), *(i++));
    ASSERT_EQ(Point(108, 100, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    t1 = t1.translate(-100, -100);

    i = t1.begin();
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(Point(-8, 5, ref), *(i++));
    ASSERT_EQ(Point(-8, -5, ref), *(i++));
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(i, t1.end());
}

TEST(Path, turtleRotateAndTranslate) {
    Reference ref { 1, 320, -1, 240 };
    Path turtle { ref, 8, 0 };
    turtle.push_back(-8, 5);
    turtle.push_back(-8, -5);
    turtle.push_back(8, 0);

    auto t1 = turtle.rotate(Angle::Degrees(30));

    auto i = t1.begin();
    ASSERT_EQ(Point(7, 4, ref), *(i++));
    ASSERT_EQ(Point(-9, 0, ref), *(i++));
    ASSERT_EQ(Point(-4, -8, ref), *(i++));
    ASSERT_EQ(Point(7, 4, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    t1 = t1.translate(100, 100);

    i = t1.begin();
    ASSERT_EQ(Point(107, 104, ref), *(i++));
    ASSERT_EQ(Point(91, 100, ref), *(i++));
    ASSERT_EQ(Point(96, 92, ref), *(i++));
    ASSERT_EQ(Point(107, 104, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    t1 = t1.translate(-100, -100);

    i = t1.begin();
    ASSERT_EQ(Point(7, 4, ref), *(i++));
    ASSERT_EQ(Point(-9, 0, ref), *(i++));
    ASSERT_EQ(Point(-4, -8, ref), *(i++));
    ASSERT_EQ(Point(7, 4, ref), *(i++));
    ASSERT_EQ(i, t1.end());

    i = turtle.begin();
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(Point(-8, 5, ref), *(i++));
    ASSERT_EQ(Point(-8, -5, ref), *(i++));
    ASSERT_EQ(Point(8, 0, ref), *(i++));
    ASSERT_EQ(i, turtle.end());
}

TEST(StraightLine, basicLine) {
    StraightLine line {1, 0};  // from m and q

    ASSERT_EQ(Point(1, 1), line.whenX(1));
    ASSERT_EQ(Point(1, 1), line.whenY(1));
    ASSERT_EQ(Point(5, 5), line.whenX(5));
    ASSERT_EQ(Point(5, 5), line.whenY(5));

    StraightLine lineTurtle = {1, 0, Reference (1, 320, -1, 240)};  // from m and q in a different system

    ASSERT_EQ(Point(1, 1), line.whenX(1));
    ASSERT_EQ(Point(1, 1), line.whenY(1));
    ASSERT_EQ(Point(5, 5), line.whenX(5));
    ASSERT_EQ(Point(5, 5), line.whenY(5));

    ASSERT_NE(line, lineTurtle);
    ASSERT_NE(lineTurtle, line);

    StraightLine line2 {1, 0};  // from m and q
    ASSERT_EQ(line, line2);
    ASSERT_EQ(line2, line);

    line2 = StraightLine(Angle::Degrees(45), 0);
    ASSERT_EQ(line, line2);
    ASSERT_EQ(line2, line);

    ASSERT_EQ(Point(1, 1), line.whenX(1));
    ASSERT_EQ(Point(1, 1), line.whenY(1));
    ASSERT_EQ(Point(5, 5), line.whenX(5));
    ASSERT_EQ(Point(5, 5), line.whenY(5));

    line2 = StraightLine(Point(3,4), Point(4, 5));
    line = StraightLine(1, 1);
    ASSERT_EQ(line, line2);
    ASSERT_EQ(line2, line);
    ASSERT_TRUE(line.belongTo({1,2}));
    ASSERT_TRUE(line.belongTo({3,4}));
    ASSERT_TRUE(line.belongTo({4,5}));

    line = StraightLine(1, Point(4, 5));
    ASSERT_EQ(line, line2);
    ASSERT_EQ(line2, line);
    ASSERT_TRUE(line.belongTo({1,2}));
    ASSERT_TRUE(line.belongTo({3,4}));
    ASSERT_TRUE(line.belongTo({4,5}));
}
