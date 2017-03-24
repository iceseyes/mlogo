/**
 * @file: test_parser.cpp
 *
 */

#include "gtest/gtest.h"

#include <string>

#include "parser_impl.hpp"

using namespace mlogo::parser;

TEST(Parser, parseNumber) {
	auto f = [](const std::string &v) { return parse<NumberParser, Number>(v); };
	ASSERT_EQ(Number("3"), f("3"));
	ASSERT_EQ(Number("12345"), f("12345"));
	ASSERT_EQ(Number("12.345"), f("12.345"));
	ASSERT_EQ(Number(".345"), f(".345"));
	ASSERT_ANY_THROW(f("1c"));
	ASSERT_ANY_THROW(f("hello"));
	ASSERT_ANY_THROW(f("h4532"));
	ASSERT_ANY_THROW(f("h 4532"));
	ASSERT_ANY_THROW(f("4532 test"));
}

#ifdef TODO
TEST(Parser, parseWord) {
	ASSERT_EQ("3", parse<WordParser>("\"3"));
	ASSERT_EQ("1c", parse<WordParser>("\"1c"));
	ASSERT_EQ("hello", parse<WordParser>("\"hello"));
	ASSERT_EQ("h45.32", parse<WordParser>("\"h45.32"));
	ASSERT_EQ(";h45.32", parse<WordParser>("\";h45.32"));
	ASSERT_EQ(",max.32", parse<WordParser>("\",max.32"));
	ASSERT_ANY_THROW(parse<WordParser>("1c"));
	ASSERT_ANY_THROW(parse<WordParser>("hello"));
	ASSERT_ANY_THROW(parse<WordParser>("h4532"));
	ASSERT_ANY_THROW(parse<WordParser>("h45.32"));
	ASSERT_ANY_THROW(parse<WordParser>("4532 test"));
	ASSERT_ANY_THROW(parse<WordParser>("\"4532 test"));
	ASSERT_ANY_THROW(parse<WordParser>("\"test test"));
}

TEST(Parser, parseVariable) {
	ASSERT_EQ("3", parse<VariableParser>(":3"));
	ASSERT_EQ("1c", parse<VariableParser>(":1c"));
	ASSERT_EQ("hello", parse<VariableParser>(":hello"));
	ASSERT_EQ("h45.32", parse<VariableParser>(":h45.32"));
	ASSERT_ANY_THROW(parse<VariableParser>("1c"));
	ASSERT_ANY_THROW(parse<VariableParser>("hello"));
	ASSERT_ANY_THROW(parse<VariableParser>("h4532"));
	ASSERT_ANY_THROW(parse<VariableParser>("h45.32"));
	ASSERT_ANY_THROW(parse<VariableParser>("4532 test"));
	ASSERT_ANY_THROW(parse<VariableParser>(":4532 test"));
	ASSERT_ANY_THROW(parse<VariableParser>(":test test"));
}

TEST(Parser, parseProcName) {
	ASSERT_EQ("test", parse<ProcNameParser>("test"));
	ASSERT_EQ("forward", parse<ProcNameParser>("forward"));
	ASSERT_EQ("FD", parse<ProcNameParser>("FD"));
	ASSERT_EQ("repeat", parse<ProcNameParser>("repeat"));
	ASSERT_ANY_THROW(parse<ProcNameParser>("1c"));
	ASSERT_ANY_THROW(parse<ProcNameParser>(":hello"));
	ASSERT_ANY_THROW(parse<ProcNameParser>("\"h4532"));
	ASSERT_ANY_THROW(parse<ProcNameParser>("h45.32"));
	ASSERT_ANY_THROW(parse<ProcNameParser>("4532 test"));
	ASSERT_ANY_THROW(parse<ProcNameParser>("test test"));
}
#endif
