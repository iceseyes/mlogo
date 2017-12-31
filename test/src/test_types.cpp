/**
 * @file: test_types.cpp
 *
 *  Created on: 09 apr 2017
 *      Author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <sstream>

#include <boost/variant.hpp>

#include "parser.hpp"
#include "types.hpp"

using namespace mlogo::types;
using namespace mlogo::parser;

TEST(Value, creationAndStreaming) {
    Value word;
    ASSERT_EQ("", boost::get<WordValue>(word));

    word = "Test";
    ASSERT_EQ("Test", boost::get<WordValue>(word));
    ASSERT_THROW(boost::get<ListValue>(word), boost::bad_get);

    Value w1{"hello"}, w2{"world"}, w3{"test"};
    Value list;

    ASSERT_EQ("", boost::get<WordValue>(list));
    list = ListValue({w1, w2, w3});
    ASSERT_THROW(boost::get<WordValue>(list), boost::bad_get);

    ASSERT_EQ("world", boost::get<WordValue>(boost::get<ListValue>(list)[1]));

    std::stringstream ss;

    ss << w1 << w2 << w3 << list;

    ASSERT_EQ("helloworldtest[hello world test]", ss.str());
}

TEST(Value, equality) {
    Value word;
    Value word2;
    ASSERT_EQ(word2, word);

    word = "test";
    ASSERT_NE(word2, word);

    word2 = "test";
    ASSERT_EQ(word2, word);
    ASSERT_TRUE(word2 == word);
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

TEST(Value, toString) {
    Value w{"hello"};
    Value n{"123"};
    Value list(ListValue({w, n}));

    ASSERT_EQ("hello", toString(w));
    ASSERT_EQ("123", toString(n));
    ASSERT_EQ("[hello 123]", toString(list));
}

TEST(ValueBox, equality) {
    Value word;
    Value word2;
    ASSERT_EQ(ValueBox(word2), ValueBox(word));

    word = "test";
    ASSERT_NE(ValueBox(word2), ValueBox(word));

    word2 = "test";
    ASSERT_EQ(ValueBox(word2), ValueBox(word));
    ASSERT_TRUE(ValueBox(word2) == ValueBox(word));
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

    ValueBox strValue{"test123"};
    ASSERT_EQ("test123", strValue);

    std::string abc{"abc"};
    ValueBox abc_value{abc};
    unknown = abc;

    ASSERT_EQ("abc", unknown);
    ASSERT_EQ("abc", abc_value);
}

TEST(ValueBox, valueBoxCheckType) {
    ValueBox unknown;

    ASSERT_TRUE(unknown.isWord());
    ASSERT_FALSE(unknown.isList());

    Value word;
    ValueBox wordBox{word};

    ASSERT_TRUE(wordBox.isWord());
    ASSERT_FALSE(wordBox.isList());

    word = "word";
    ListValue v{word, "test", "hi", "logo"};
    ValueBox vbox{v};

    ASSERT_EQ("[word test hi logo]", vbox.toString(true));
    ASSERT_EQ("word test hi logo", vbox.toString());
    ASSERT_EQ(v, vbox);

    ListValue v1{v, word};
    vbox = v1;

    ASSERT_EQ(v1, vbox);
}

TEST(ValueBox, empty) {
    ValueBox word;
    ASSERT_TRUE(word.empty());

    word = "test";
    ASSERT_FALSE(word.empty());

    word = " ";
    ASSERT_FALSE(word.empty());

    word = "";
    ASSERT_TRUE(word.empty());

    ValueBox list = ListValue();
    ASSERT_TRUE(list.empty());

    list = ListValue({word.word()});
    ASSERT_FALSE(list.empty());
}

TEST(ValueBox, streamList) {
    std::stringstream ss;
    ListValue list;
    list.push_back("test");
    list.push_back("stream");

    ss << list;
    ASSERT_EQ("[test stream]", ss.str());
}

TEST(ValueBox, boolValue) {
    ValueBox b{true};

    ASSERT_TRUE(b.isWord());
    ASSERT_EQ("TRUE", b.word());
    ASSERT_TRUE(b.toBool());
    ASSERT_EQ("TRUE", b.toString());

    b = false;

    ASSERT_TRUE(b.isWord());
    ASSERT_EQ("FALSE", b.word());
    ASSERT_FALSE(b.toBool());
    ASSERT_EQ("FALSE", b.toString());

    b = "TRUE";

    ASSERT_TRUE(b.isWord());
    ASSERT_EQ("TRUE", b.word());
    ASSERT_TRUE(b.toBool());
    ASSERT_EQ("TRUE", b.toString());

    b = "true";

    ASSERT_TRUE(b.isWord());
    ASSERT_TRUE(b.toBool());

    b = "false";

    ASSERT_TRUE(b.isWord());
    ASSERT_FALSE(b.toBool());

    b = "0";

    ASSERT_TRUE(b.isWord());
    ASSERT_FALSE(b.toBool());
}

TEST(UserDefinedProcedure, buildFromParse) {
    Procedure p{parse("TO TEST")};
    p.addLine("PR [HELLO WORLD]");

    UserDefinedProcedure udp{p};
    ASSERT_EQ(0u, udp.nArgs());
    ASSERT_EQ(0u, udp.params().size());
    ASSERT_FALSE(udp.isFunction());

    p = parse("TO TEST :a :b :c");
    p.addLine("PR [:a :b :c]");

    udp = UserDefinedProcedure(p);
    ASSERT_EQ(3u, udp.nArgs());
    ASSERT_EQ(3u, udp.params().size());
    ASSERT_EQ("a", udp.params().at(0));
    ASSERT_EQ("b", udp.params().at(1));
    ASSERT_EQ("c", udp.params().at(2));
    ASSERT_FALSE(udp.isFunction());
}

TEST(UserDefinedProcedure, arguments) {
    Procedure p{parse("TO TEST :a :b :c")};
    p.addLine("PR [:a :b :c]");

    UserDefinedProcedure udp{p};
    ASSERT_EQ(3u, udp.nArgs());
    ASSERT_EQ(3u, udp.params().size());
    ASSERT_EQ("a", udp.paramName(0));
    ASSERT_EQ("b", udp.paramName(1));
    ASSERT_EQ("c", udp.paramName(2));
    ASSERT_ANY_THROW(udp.paramName(4));
    ASSERT_FALSE(udp.isFunction());

    FAIL() << "you have to test that actual arguments was passed has it is in "
              "local scope";
}
