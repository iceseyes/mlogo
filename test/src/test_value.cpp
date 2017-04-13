/**
 * @file: test_value.cpp
 *
 *  Created on: 09 apr 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <sstream>

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

    std::stringstream ss;

    ss << w1 << w2 << w3 << list;

    ASSERT_EQ("helloworldtesthello world test", ss.str());
}

TEST(Value, equality) {
    Value word;
    Value word2;
    ASSERT_EQ(word2, word);

    word="test";
    ASSERT_NE(word2, word);

    word2="test";
    ASSERT_EQ(word2, word);
    ASSERT_TRUE(word2==word);
    ASSERT_EQ(Value("test"), word);

    Value list;
    Value list2;
    ASSERT_NE(word2, list);
    ASSERT_EQ(list2, list);

    list = ListValue({word, word2});
    ASSERT_NE(list2, list);

    list2 = ListValue({word, word2});
    ASSERT_EQ(list2, list);
}

TEST(ValueBox, equality) {
    Value word;
    Value word2;
    ASSERT_EQ(ValueBox(word2), ValueBox(word));

    word="test";
    ASSERT_NE(ValueBox(word2), ValueBox(word));

    word2="test";
    ASSERT_EQ(ValueBox(word2), ValueBox(word));
    ASSERT_TRUE(ValueBox(word2)==ValueBox(word));
    ASSERT_EQ(ValueBox(Value("test")), ValueBox(word2));

    Value list;
    Value list2;
    ASSERT_NE(ValueBox(word2), ValueBox(list));
    ASSERT_EQ(ValueBox(list2), ValueBox(list));

    list = ListValue({word, word2});
    ASSERT_NE(ValueBox(list2), ValueBox(list));

    list2 = ListValue({word, word2});
    ASSERT_EQ(ValueBox(list2), ValueBox(list));
}

TEST(ValueBox, valueBoxFromString) {
    ValueBox unknown;

    unknown = "test";
    ASSERT_EQ("test", unknown);

    ValueBox strValue { "test123" };
    ASSERT_EQ("test123", strValue);

    std::string abc { "abc" };
    ValueBox abc_value { abc };
    unknown = abc;

    ASSERT_EQ("abc", unknown);
    ASSERT_EQ("abc", abc_value);
}

TEST(ValueBox, valueBoxCheckType) {
	ValueBox unknown;

	ASSERT_TRUE(unknown.isWord());
	ASSERT_FALSE(unknown.isList());

	Value word;
	ValueBox wordBox { word };

	ASSERT_TRUE(wordBox.isWord());
	ASSERT_FALSE(wordBox.isList());

	word = "word";
	ListValue v {word, "test", "hi", "logo"};
	ValueBox vbox { v };

    ASSERT_EQ("word test hi logo", vbox.toString());
    ASSERT_EQ(v, vbox);

    ListValue v1 {v, word};
    vbox = v1;

    ASSERT_EQ(v1, vbox);
}


