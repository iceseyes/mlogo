/**
 * @file: test_geometry.cpp
 */


#include <gtest/gtest.h>

#include "geometry.hpp"


using Angle = mlogo::geometry::Angle;

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
