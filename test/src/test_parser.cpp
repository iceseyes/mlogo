/**
 * @file: test_parser.cpp
 *
 */

#include "gtest/gtest.h"

#include <string>

#include "parser_impl.hpp"

using namespace mlogo::parser;

#ifdef TODO
TEST(Parser, parseNumber) {
	ASSERT_EQ("3", parse<NumberParser>("3"));
	ASSERT_EQ("12345", parse<NumberParser>("12345"));
	ASSERT_ANY_THROW(parse<NumberParser>("1c"));
	ASSERT_ANY_THROW(parse<NumberParser>("hello"));
	ASSERT_ANY_THROW(parse<NumberParser>("h4532"));
	ASSERT_ANY_THROW(parse<NumberParser>("h 4532"));
	ASSERT_ANY_THROW(parse<NumberParser>("4532 test"));
}

TEST(Parser, parseWord) {
	ASSERT_EQ("3", parse<WordParser>("\"3"));
	ASSERT_EQ("1c", parse<WordParser>("\"1c"));
	ASSERT_EQ("hello", parse<WordParser>("\"hello"));
	ASSERT_EQ("h45.32", parse<WordParser>("\"h45.32"));
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
