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

TEST(Parser, parseWord) {
	auto f = [](const std::string &v) { return parse<WordParser, Word>(v); };
	ASSERT_EQ(Word("3"), f("\"3"));
	ASSERT_EQ(Word("3.14"), f("\"3.14"));
	ASSERT_EQ(Word("1c"), f("\"1c"));
	ASSERT_EQ(Word("hello"), f("\"hello"));
	ASSERT_EQ(Word("h45.32"), f("\"h45.32"));
	ASSERT_EQ(Word(";h45.32"), f("\";h45.32"));
	ASSERT_EQ(Word(",max.32"), f("\",max.32"));
	ASSERT_ANY_THROW(f("1c"));
	ASSERT_ANY_THROW(f("hello"));
	ASSERT_ANY_THROW(f("h4532"));
	ASSERT_ANY_THROW(f("h45.32"));
	ASSERT_ANY_THROW(f("4532 test"));
	ASSERT_ANY_THROW(f("\"4532 test"));
	ASSERT_ANY_THROW(f("\"test test"));
}

TEST(Parser, parseVariable) {
	auto f = [](const std::string &v) { return parse<VariableParser, Variable>(v); };
	ASSERT_EQ(Variable("3"), f(":3"));
	ASSERT_EQ(Variable("3.14"), f(":3.14"));
	ASSERT_EQ(Variable("1c"), f(":1c"));
	ASSERT_EQ(Variable("hello"), f(":hello"));
	ASSERT_EQ(Variable("h45.32"), f(":h45.32"));
	ASSERT_EQ(Variable(";h45.32"), f(":;h45.32"));
	ASSERT_EQ(Variable(",max.32"), f(":,max.32"));
	ASSERT_ANY_THROW(f("1c"));
	ASSERT_ANY_THROW(f("hello"));
	ASSERT_ANY_THROW(f("h4532"));
	ASSERT_ANY_THROW(f("h45.32"));
	ASSERT_ANY_THROW(f("4532 test"));
	ASSERT_ANY_THROW(f(":4532 test"));
	ASSERT_ANY_THROW(f(":test test"));
}

TEST(Parser, parseProcName) {
	auto f = [](const std::string &v) { return parse<ProcNameParser, ProcName>(v); };

	ASSERT_EQ(ProcName("test"), f("test"));
	ASSERT_EQ(ProcName("forward"), f("forward"));
	ASSERT_EQ(ProcName("FD"), f("FD"));
	ASSERT_EQ(ProcName("repeat"), f("repeat"));
	ASSERT_EQ(ProcName("h264"), f("h264"));
	ASSERT_ANY_THROW(f("1c"));
	ASSERT_ANY_THROW(f(":hello"));
	ASSERT_ANY_THROW(f("\"h4532"));
	ASSERT_ANY_THROW(f("h45.32"));
	ASSERT_ANY_THROW(f("4532 test"));
	ASSERT_ANY_THROW(f("test test"));
}

TEST(Parser, parseStatement) {
	auto stmt = parse("fd 10");
	ASSERT_EQ(ProcName("fd"), stmt.name);
	ASSERT_EQ(1u, stmt.arguments.size());
	ASSERT_EQ(Number("10"), boost::get<Number>(stmt.arguments[0]));

	stmt = parse("fd 10 rt 90 fd 10 rt 90 fd 10 rt 90 fd 10");
	ASSERT_EQ(ProcName("fd"), stmt.name);
	ASSERT_EQ(13u, stmt.arguments.size());

	stmt = parse("print \"Hello");
	ASSERT_EQ(ProcName("fd"), stmt.name);
	ASSERT_EQ(1u, stmt.arguments.size());
	ASSERT_EQ(Word("Hello"), boost::get<Word>(stmt.arguments[0]));

	stmt = parse("print SUM 1 2");
	ASSERT_EQ(ProcName("fd"), stmt.name);
	ASSERT_EQ(3u, stmt.arguments.size());
	ASSERT_EQ(ProcName("SUM"), boost::get<ProcName>(stmt.arguments[0]));

	FAIL() << "Incomplete Test";
}
