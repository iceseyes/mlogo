/**
 * @file: test_value.cpp
 *
 *  Created on: 09 apr 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <boost/variant.hpp>

#include "value.hpp"

TEST(Value, creationAndStreaming) {
    mlogo::types::Value word;
    ASSERT_EQ("", boost::get<mlogo::types::WordValue>(word));

    word = "Test";
    ASSERT_EQ("Test", boost::get<mlogo::types::WordValue>(word));
    ASSERT_THROW(boost::get<mlogo::types::ListValue>(word), boost::bad_get);

    FAIL() << "Incomplete Test";
}


