/**
 * @file: test_geometry.cpp
 */


#include <gtest/gtest.h>

#include "geometry.hpp"


using Angle = mlogo::geometry::Angle;

TEST(Angle, creationAndConversion) {
	Angle zero_rad { Angle::Rad(0) };
	Angle zero_deg { Angle::Degrees(0) };

	ASSERT_EQ(0.0, zero_rad.radians().value());
	ASSERT_EQ(0.0, zero_rad.degrees().value());
	ASSERT_EQ(0.0, zero_deg.radians().value());
	ASSERT_EQ(0.0, zero_deg.degrees().value());
	ASSERT_EQ(zero_rad, zero_deg);
	ASSERT_EQ(zero_deg, zero_rad);

	FAIL() << "Incomplete Test";
}

