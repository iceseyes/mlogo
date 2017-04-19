/**
 * @file: test_geometry.cpp
 */


#include <gtest/gtest.h>

#include "geometry.hpp"


using Angle = mlogo::geometry::Angle;

constexpr double MAX_RAD_ERROR { 0.001 };

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

