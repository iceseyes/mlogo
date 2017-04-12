/**
 * @file: test_value.cpp
 *
 *  Created on: 09 apr 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <boost/variant.hpp>

#include "value.hpp"

using namespace mlogo::types;

TEST(Value, creationAndStreaming) {
    Value word;
    ASSERT_EQ("", boost::get<WordValue>(word));

    word = "Test";
    ASSERT_EQ("Test", boost::get<WordValue>(word));
    ASSERT_THROW(boost::get<ListValue>(word), boost::bad_get);

    Value w1 { "hello" }, w2 { "world" }, w3 { "test" };
    Value list;

    ASSERT_EQ("", boost::get<WordValue>(list));
    list = ListValue({ w1, w2, w3 });
    ASSERT_THROW(boost::get<WordValue>(list), boost::bad_get);

    ASSERT_EQ("world", boost::get<WordValue>(boost::get<ListValue>(list)[1]));

    FAIL() << "Incomplete Test";
}

TEST(ValueBox, valueBoxCheckType) {
	ValueBox unknown;

	ASSERT_TRUE(unknown.isWord());
	ASSERT_FALSE(unknown.isList());

	Value word;
	ValueBox wordBox { word };

	ASSERT_TRUE(wordBox.isWord());
	ASSERT_FALSE(wordBox.isList());

	FAIL() << "Incomplete Test";
}


