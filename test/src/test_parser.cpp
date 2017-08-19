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
	ASSERT_EQ(Word("(max).32"), f("\"(max).32"));
	ASSERT_EQ(Word("(max))(.32"), f("\"(max))(.32"));
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
	ASSERT_EQ(Variable("PI"), f(":PI"));
	ASSERT_EQ(Variable("var+1"), f(":var+1"));
	ASSERT_ANY_THROW(f("1c"));
	ASSERT_ANY_THROW(f("hello"));
	ASSERT_ANY_THROW(f("h4532"));
	ASSERT_ANY_THROW(f("h45.32"));
	ASSERT_ANY_THROW(f("4532 test"));
	ASSERT_ANY_THROW(f(":4532 test"));
	ASSERT_ANY_THROW(f(":test test"));
	ASSERT_ANY_THROW(f(":test)"));
	ASSERT_ANY_THROW(f(":(test)"));
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

TEST(Parser, parseExpr) {
    auto f = [](const std::string &v) { return parse<ExpressionParser, Expression>(v); };

    ASSERT_EQ(Expression("2"), f("2"));
    ASSERT_EQ(Expression("1+2"), f("1+2"));
    ASSERT_EQ(Expression("3+4"), f("3 + 4"));
    ASSERT_EQ(Expression("3+4"), f("3+ 4"));
    ASSERT_EQ(Expression("3+4"), f("3 +4"));
    ASSERT_EQ(Expression("5*6"), f("5 * 6"));
    ASSERT_EQ(Expression("5/6"), f("5 / 6"));
    ASSERT_EQ(Expression("5-6"), f("5 - 6"));
    ASSERT_EQ(Expression("(1+2)"), f("(1+2)"));
    ASSERT_EQ(Expression("(1+2)*3"), f("(1+2)*3"));
    ASSERT_EQ(Expression("4-(1+2)*3"), f("4-(1+2)*3"));
    ASSERT_EQ(Expression("4-(1+2)*4"), f("4-( 1+2 )*4"));
    ASSERT_EQ(Expression("(1+2)*(3)"), f("(1+2)*(3)"));
    ASSERT_EQ(Expression("((1+2)*(3))/4"), f("((1+2)*(3)) / 4"));
    ASSERT_EQ(Expression("((1.5+2)*(3.1415))/4.23"), f("((1.5+2)*(3.1415))/4.23"));
    ASSERT_EQ(Expression("1+var"), f("1+:var"));
    ASSERT_EQ(Expression("1+var"), f("1 + :var"));
    ASSERT_EQ(Expression("var+1"), f(":var + 1"));
    ASSERT_EQ(Expression("(var)+1"), f("(:var)+1"));
    ASSERT_EQ(Expression("((1.5+2)*PI)/4.23"), f("((1.5+2)*:PI)/4.23"));
    ASSERT_EQ(Expression("((abcd+2)*PI)/4.23"), f("((:abcd +2)*:PI)/4.23"));
    ASSERT_EQ(Expression("-var+1"), f("-:var + 1"));
    ASSERT_EQ(Expression("var+-1"), f(":var + -1"));

    ASSERT_ANY_THROW(f(":var+ 1"));

    FAIL() << "Incomplete Test";
}

TEST(Parser, parseList) {
	auto f = [](const std::string &v) { return parse<ListParser, List>(v); };
	List tmp;

	tmp.push_back(Word("test"));
	ASSERT_EQ(tmp, f("[test]"));
	ASSERT_EQ(tmp, f("[ test ]"));
	ASSERT_EQ(tmp, f("[ test]"));
	ASSERT_EQ(tmp, f("[test ]"));

	tmp.push_back(Word("123"));
	ASSERT_EQ(tmp, f("[test 123]"));
	ASSERT_EQ(tmp, f("[ test 123 ]"));
	ASSERT_EQ(tmp, f("[ test 123]"));
	ASSERT_EQ(tmp, f("[test 123 ]"));

	tmp.push_back(Word(":v.123"));
	ASSERT_EQ(tmp, f("[test 123 :v.123]"));

	List tmp2;
	tmp2.push_back(Word("100"));
	tmp2.push_back(Word("100"));
    ASSERT_EQ(tmp2, f("[100 100]"));
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
	ASSERT_EQ(ProcName("print"), stmt.name);
	ASSERT_EQ(1u, stmt.arguments.size());
	ASSERT_EQ(Word("Hello"), boost::get<Word>(stmt.arguments[0]));

	stmt = parse("print SUM 1 2");
	ASSERT_EQ(ProcName("print"), stmt.name);
	ASSERT_EQ(3u, stmt.arguments.size());
	ASSERT_EQ(ProcName("SUM"), boost::get<ProcName>(stmt.arguments[0]));

	List list;
	list.push_back({"Hello"});
	list.push_back({"World"});

	stmt = parse("print [Hello World]");
	ASSERT_EQ(ProcName("print"), stmt.name);
	ASSERT_EQ(1u, stmt.arguments.size());
	ASSERT_EQ(list, boost::get<List>(stmt.arguments[0]));
}

//TEST(Parser, parseExprStatement) {
//    auto stmt = parse("fd 10/2");
//    ASSERT_EQ(ProcName("fd"), stmt.name);
//    ASSERT_EQ(1u, stmt.arguments.size());
//
//    FAIL() << "Incomplete test";
//}
