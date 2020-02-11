/**
 * @file: test_parser.cpp
 *
 */
#include <gtest/gtest.h>

#include <string>

#include "parser_impl.hpp"

using namespace mlogo::parser;
using namespace mlogo::exceptions;

TEST(Parser, parseNumber) {
    auto f = [](const std::string &v) {
        return parse<NumberParser, Number>(v);
    };

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
    ASSERT_EQ(Word(",max.32"), f("\",max.32"));
    ASSERT_ANY_THROW(f("1c"));
    ASSERT_ANY_THROW(f("hello"));
    ASSERT_ANY_THROW(f("h4532"));
    ASSERT_ANY_THROW(f("h45.32"));
    ASSERT_ANY_THROW(f("4532 test"));
    ASSERT_ANY_THROW(f("\"4532 test"));
    ASSERT_ANY_THROW(f("\"test test"));
    ASSERT_ANY_THROW(f(";h45.32"));
    ASSERT_ANY_THROW(f("\"(max).32"));
    ASSERT_ANY_THROW(f("\"(max))(.32"));
}

TEST(Parser, parseVariable) {
    auto f = [](const std::string &v) {
        return parse<VariableParser, Variable>(v);
    };

    ASSERT_EQ(Variable("3"), f(":3"));
    ASSERT_EQ(Variable("3.14"), f(":3.14"));
    ASSERT_EQ(Variable("1c"), f(":1c"));
    ASSERT_EQ(Variable("hello"), f(":hello"));
    ASSERT_EQ(Variable("h45.32"), f(":h45.32"));
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
    ASSERT_ANY_THROW(f(";h45.32"));
}

TEST(Parser, parseProcName) {
    auto f = [](const std::string &v) {
        return parse<ProcNameParser, ProcName>(v);
    };

    ASSERT_EQ(ProcName("test"), f("test"));
    ASSERT_EQ(ProcName("forward"), f("forward"));
    ASSERT_EQ(ProcName("FD"), f("FD"));
    ASSERT_EQ(ProcName("repeat"), f("repeat"));
    ASSERT_EQ(ProcName("h264"), f("h264"));
    ASSERT_EQ(ProcName("h45.32"), f("h45.32"));
    ASSERT_EQ(ProcName("c1c"), f("c1c"));
    ASSERT_EQ(ProcName("Ob.1c.2d"), f("Ob.1c.2d"));

    ASSERT_ANY_THROW(f("1c"));
    ASSERT_ANY_THROW(f(":hello"));
    ASSERT_ANY_THROW(f("\"h4532"));
    ASSERT_ANY_THROW(f("4532"));
    ASSERT_ANY_THROW(f("4532 test"));
    ASSERT_ANY_THROW(f("test test"));
    ASSERT_ANY_THROW(f("33.a"));
    ASSERT_ANY_THROW(f("33.a34"));
    ASSERT_ANY_THROW(f("33.34a"));
}

TEST(Parser, parseExpr) {
    auto f = [](const std::string &v) {
        return parse<ExpressionParser, Expression>(v);
    };

    // Check for Not Equal Operator in Expression
    ASSERT_NE(Expression(Number("2")), Expression(Number("3")));
    ASSERT_NE(Expression('/')
                  << Expression(Number("2")) << Expression(Number("3")),
              Expression(Number("3")));
    ASSERT_NE(Expression('/')
                  << Expression(Number("2")) << Expression(Number("3")),
              Expression(Number("2")));
    ASSERT_NE(Expression('/')
                  << Expression(Number("2")) << Expression(Number("3")),
              Expression('/'));

    // a Number is an Expression.
    ASSERT_EQ(Expression(Number("2")), f("2"));

    // SUM of expressions is an expression.
    ASSERT_EQ(Expression('+') << Number("1") << Number("2"), f("1+2"));
    ASSERT_EQ(Expression('+') << Number("3") << Number("4"), f("3 + 4"));
    ASSERT_EQ(Expression('+') << Number("3") << Number("4"), f("3+ 4"));
    ASSERT_EQ(Expression('+') << Number("3") << Number("4"), f("3 +4"));

    // other operations
    ASSERT_EQ(Expression('*') << Number("5") << Number("6"), f("5 * 6"));
    ASSERT_EQ(Expression('/') << Number("5") << Number("6"), f("5 / 6"));
    ASSERT_EQ(Expression('-') << Number("5") << Number("6"), f("5 - 6"));

    // priority
    ASSERT_EQ(Expression('+') << (Expression('+') << Number("4") << Number("5"))
                              << Number("6"),
              f("4 + 5 + 6"));
    ASSERT_EQ(Expression('-') << (Expression('+') << Number("4") << Number("5"))
                              << Number("6"),
              f("4 + 5 - 6"));
    ASSERT_EQ(Expression('+')
                  << Number("4")
                  << (Expression('*') << Number("5") << Number("6")),
              f("4 + 5 * 6"));
    ASSERT_EQ(Expression('+')
                  << Number("4")
                  << (Expression('/') << Number("5") << Number("6")),
              f("4 + 5 / 6"));

    ASSERT_EQ(Expression('+') << (Expression('*') << Number("4") << Number("5"))
                              << Number("6"),
              f("4 * 5 + 6"));
    ASSERT_EQ(Expression('+') << (Expression('/') << Number("4") << Number("5"))
                              << Number("6"),
              f("4 / 5 + 6"));
    ASSERT_EQ(Expression('/') << (Expression('/') << Number("4") << Number("5"))
                              << Number("6"),
              f("4 / 5 / 6"));

    // Brackets and operators priority
    ASSERT_EQ(Expression('+') << Number("1") << Number("2"), f("(1+2)"));
    ASSERT_EQ(Expression('*') << (Expression('+') << Number("1") << Number("2"))
                              << Number("3"),
              f("(1+2)*3"));
    ASSERT_EQ(Expression('-')
                  << Number("4")
                  << (Expression('*')
                      << (Expression('+') << Number("1") << Number("2"))
                      << Number("3")),
              f("4-(1+2)*3"));
    ASSERT_EQ(Expression('-')
                  << Number("4")
                  << (Expression('*')
                      << (Expression('+') << Number("1") << Number("2"))
                      << Number("4")),
              f("4-( 1+2 )*4"));
    ASSERT_EQ(Expression('*') << (Expression('+') << Number("1") << Number("2"))
                              << Number("3"),
              f("(1+2)*(3)"));
    ASSERT_EQ(Expression('/')
                  << (Expression('*')
                      << (Expression('+') << Number("1") << Number("2"))
                      << Number("3"))
                  << Number("4"),
              f("((1+2)*(3)) / 4"));
    ASSERT_EQ(Expression('/')
                  << (Expression('*')
                      << (Expression('+') << Number("1.5") << Number("2"))
                      << Number("3.1415"))
                  << Number("4.23"),
              f("((1.5+2)*(3.1415))/4.23"));

    // Variables
    ASSERT_EQ(Expression('+') << Number("1") << Variable("var"), f("1+:var"));
    ASSERT_EQ(Expression('+') << Number("1") << Variable("var"), f("1 + :var"));
    ASSERT_EQ(Expression('+') << Variable("var") << Number("1"), f(":var + 1"));
    ASSERT_EQ(Expression('+') << Variable("var") << Number("1"), f("(:var)+1"));

    // All-in
    ASSERT_EQ(Expression('/')
                  << (Expression('*')
                      << (Expression('+') << Number("1.5") << Number("2"))
                      << Variable("PI"))
                  << Number("4.23"),
              f("((1.5+2)*:PI)/4.23"));
    ASSERT_EQ(Expression('/')
                  << (Expression('*')
                      << (Expression('+') << Variable("abcd") << Number("2"))
                      << Variable("PI"))
                  << Number("4.23"),
              f("((:abcd +2)*:PI)/4.23"));
    ASSERT_EQ(Expression('+')
                  << (Expression(Expression::MINUS) << Variable("var"))
                  << Number("1"),
              f("-:var + 1"));
    ASSERT_EQ(Expression('+') << Variable("var")
                              << (Expression(Expression::MINUS) << Number("1")),
              f(":var + -1"));

    // Statement
    Statement stmt("sqrt");
    stmt.arguments.push_back(Expression('/') << Number("10") << Number("5"));
    ASSERT_EQ(Expression(stmt), f("sqrt 10/5"));

    stmt = Statement("sqrt");
    stmt.arguments.push_back(ProcName("ln"));
    stmt.arguments.push_back(Expression(Variable("var")));

    ASSERT_EQ(Expression('/') << stmt << Number("5"), f("(sqrt ln :var)/5"));

    ASSERT_ANY_THROW(f(":var+ 1"));
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
    ASSERT_EQ(Expression(Number("10")),
              boost::get<Expression>(stmt.arguments[0]));

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

TEST(Parser, parseExprStatement) {
    auto stmt = parse("fd 10/2");
    ASSERT_EQ(ProcName("fd"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression('/') << Number("10") << Number("2"),
              boost::get<Expression>(stmt.arguments[0]));

    stmt = parse("fd 10 / :var");
    ASSERT_EQ(ProcName("fd"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression('/') << Number("10") << Variable("var"),
              boost::get<Expression>(stmt.arguments[0]));

    Statement sqrt5("sqrt");
    sqrt5.arguments.push_back(Expression(Number("5")));
    stmt = parse("rt (2*:PI * (360/2*:PI)) / sqrt 5");
    ASSERT_EQ(ProcName("rt"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression('/')
                  << (Expression('*')
                      << (Expression('*') << Number("2") << Variable("PI"))
                      << (Expression('*')
                          << (Expression('/') << Number("360") << Number("2"))
                          << Variable("PI")))
                  << Expression(sqrt5),
              boost::get<Expression>(stmt.arguments[0]));

    stmt = parse("rt sqrt 5 * 2");
    ASSERT_EQ(ProcName("rt"), stmt.name);
    ASSERT_EQ(2u, stmt.arguments.size());
    ASSERT_EQ(ProcName("sqrt"), boost::get<ProcName>(stmt.arguments[0]));
    ASSERT_EQ(Expression('*') << Number("5") << Number("2"),
              boost::get<Expression>(stmt.arguments[1]));

    stmt = parse("func sqrt 5 * 2 ln :var / 2 (-6)");
    ASSERT_EQ(ProcName("func"), stmt.name);
    ASSERT_EQ(5u, stmt.arguments.size());
    ASSERT_EQ(ProcName("sqrt"), boost::get<ProcName>(stmt.arguments[0]));
    ASSERT_EQ(Expression('*') << Number("5") << Number("2"),
              boost::get<Expression>(stmt.arguments[1]));
    ASSERT_EQ(ProcName("ln"), boost::get<ProcName>(stmt.arguments[2]));
    ASSERT_EQ(Expression('/') << Variable("var") << Number("2"),
              boost::get<Expression>(stmt.arguments[3]));
    ASSERT_EQ(Expression(Expression::MINUS) << Number("6"),
              boost::get<Expression>(stmt.arguments[4]));
}

TEST(Parser, expressionPushedOnlyForFunctions) {
    Expression e;
    ASSERT_THROW(e << Expression(Number(5)), std::logic_error);
}

TEST(Parser, expression2statement) {
    Expression e;
    ASSERT_THROW(e.statement(), std::logic_error);

    e = Expression(parse("sum 99 1"));
    ASSERT_ANY_THROW(e.number());
    ASSERT_ANY_THROW(e.variable());
    ASSERT_ANY_THROW(e.functor());

    Statement stmt{e.statement()};

    ASSERT_EQ("sum", stmt.name);
    ASSERT_EQ(2u, stmt.arguments.size());
}

TEST(Parser, expression2number) {
    Expression e;
    Number n{e.number()};
    ASSERT_EQ("0", n.value);

    e = Expression(parse("sum 99 1"));
    ASSERT_THROW(e.number(), std::logic_error);

    e = Expression(Variable("var"));
    ASSERT_THROW(e.number(), std::logic_error);

    e = Expression(Number(99));
    ASSERT_ANY_THROW(e.statement());
    ASSERT_ANY_THROW(e.variable());
    ASSERT_ANY_THROW(e.functor());

    n = e.number();
    ASSERT_EQ("99", n.value);
}

TEST(Parser, expressionMoveCtor) {
    Expression e{Number(100)};
    ASSERT_EQ("100", e.number().value);

    Expression e1{std::move(e)};
    ASSERT_EQ("100", e1.number().value);

    e = Expression(parse("sum 1 1"));
    ASSERT_EQ("sum", e.statement().name);

    Expression e2{std::move(e)};
    ASSERT_EQ("sum", e2.statement().name);
    ASSERT_THROW(e.statement(), std::logic_error);
}

TEST(Parser, boolExprParsing) {
    Expression e = boost::get<Expression>(parse("print 1 = 3").arguments[0]);
    ASSERT_EQ(Expression('=') << Number(1) << Number(3), e);

    /*
    e = boost::get<Expression>(parse("print 1 * 4 = 12 / 3").arguments[0]);
    auto e1 = Expression('=') << Expression('*') << Number(1) << Number(4)
                              << Expression('/') << Number(12) << Number(3);

    std::cout << e.debug() << std::endl << e1.debug();

    ASSERT_EQ(e1, e);
    */
}

TEST(Parser, expressionUnsupportedInfixOperator) {
    ASSERT_THROW(Expression('~'), std::logic_error);
    ASSERT_THROW(Expression('!'), std::logic_error);
    ASSERT_THROW(Expression('%'), std::logic_error);
}

TEST(Parser, parseComments) {
    auto stmt = parse("; This is a comment line");
    ASSERT_TRUE(stmt.name.name.empty());
    ASSERT_FALSE(stmt);
    ASSERT_FALSE(stmt.name);

    stmt = parse(
        "func sqrt 5 * 2 ln :var / 2 (-6)     ;;; This  iS an   expression");
    ASSERT_EQ(ProcName("func"), stmt.name);
    ASSERT_EQ(5u, stmt.arguments.size());
    ASSERT_EQ(Expression(Expression::MINUS) << Number("6"),
              boost::get<Expression>(stmt.arguments[4]));

    stmt = parse("print \"hello;world");
    ASSERT_EQ(ProcName("print"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Word("hello"), boost::get<Word>(stmt.arguments[0]));

    stmt = parse("print :hello;world");
    ASSERT_EQ(ProcName("print"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression(Variable("hello")),
              boost::get<Expression>(stmt.arguments[0]));

    stmt = parse("print 10;world");
    ASSERT_EQ(ProcName("print"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression(Number("10")),
              boost::get<Expression>(stmt.arguments[0]));

    stmt = parse("print 10;");
    ASSERT_EQ(ProcName("print"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression(Number("10")),
              boost::get<Expression>(stmt.arguments[0]));

    stmt = parse("print [hello;world]");
    ASSERT_EQ(ProcName("print"), stmt.name);
    ASSERT_EQ(1u, stmt.arguments.size());
    List tmp;
    tmp.push_back(Word("hello;world"));
    ASSERT_EQ(tmp, boost::get<List>(stmt.arguments[0]));
}

TEST(Parser, parseProcedureDef) {
    auto stmt = parse("TO SQUARE :side");
    ASSERT_EQ(ProcName("TO"), stmt.name);
    ASSERT_TRUE(stmt.isStartProcedure());
    ASSERT_FALSE(stmt.isEndProcedure());
    ASSERT_EQ(2u, stmt.arguments.size());
    ASSERT_EQ(ProcName("SQUARE"), boost::get<ProcName>(stmt.arguments[0]));
    ASSERT_EQ(Expression(Variable("side")),
              boost::get<Expression>(stmt.arguments[1]));

    stmt = parse("REPEAT 4 [FD 10 RT 90]");
    ASSERT_EQ(ProcName("REPEAT"), stmt.name);
    ASSERT_FALSE(stmt.isStartProcedure());
    ASSERT_FALSE(stmt.isEndProcedure());
    ASSERT_EQ(2u, stmt.arguments.size());
    ASSERT_EQ(Expression(Number("4")),
              boost::get<Expression>(stmt.arguments[0]));
    List tmp;
    tmp.push_back(Word("FD"));
    tmp.push_back(Word("10"));
    tmp.push_back(Word("RT"));
    tmp.push_back(Word("90"));
    ASSERT_EQ(tmp, boost::get<List>(stmt.arguments[1]));

    stmt = parse("END");
    ASSERT_EQ(ProcName("END"), stmt.name);
    ASSERT_FALSE(stmt.isStartProcedure());
    ASSERT_TRUE(stmt.isEndProcedure());
    ASSERT_EQ(0u, stmt.arguments.size());

    stmt = parse("TO CENTER :side :beside :x :y");
    ASSERT_EQ(ProcName("TO"), stmt.name);
    ASSERT_TRUE(stmt.isStartProcedure());
    ASSERT_FALSE(stmt.isEndProcedure());
    ASSERT_EQ(5u, stmt.arguments.size());

    ASSERT_ANY_THROW(parse("TO :VAR"));
    ASSERT_ANY_THROW(parse("TO 12"));
    ASSERT_ANY_THROW(parse("TO 12+2"));
    ASSERT_ANY_THROW(parse("TO 12 :VAR"));
    ASSERT_ANY_THROW(parse("TO SUM 2+2 :VAR"));
    ASSERT_ANY_THROW(parse("END 2+2"));
}

TEST(Parser, parseUserProcedure) {
    auto udp = Procedure(parse("TO SQUARE :side"));
    ASSERT_FALSE(udp.addLine("repeat 4 [fd :side RT 90]"));
    ASSERT_TRUE(udp.addLine("end"));

    ASSERT_EQ("SQUARE", udp.name());
    ASSERT_EQ(1u, udp.nParams());
    ASSERT_EQ(1u, udp.lines.size());

    udp = Procedure(parse("TO RECTANGLE :side1 :side2"));
    ASSERT_ANY_THROW(udp.addLine("fd :side1 RT 90]"));
    ASSERT_THROW(udp.addLine("TO fd :side1"), std::logic_error);

    udp = Procedure(parse("TO RECTANGLE :side1 :side2"));
    ASSERT_ANY_THROW(udp.addLine("TO side1 :RT]"));
    ASSERT_ANY_THROW(Procedure(parse("fd :side1 RT 90]")));

    udp = Procedure(parse("TO RECTANGLE :side1 :side2"));
    ASSERT_FALSE(udp.addLine("fd :side1 RT 90"));
    ASSERT_FALSE(udp.addLine("fd :side2 RT 90"));
    ASSERT_FALSE(udp.addLine("fd :side1 RT 90"));
    ASSERT_FALSE(udp.addLine("fd :side2 RT 90"));
    ASSERT_TRUE(udp.addLine("end"));

    ASSERT_EQ("RECTANGLE", udp.name());
    ASSERT_EQ(2u, udp.nParams());
    ASSERT_EQ(4u, udp.lines.size());

    ASSERT_ANY_THROW(udp.addLine("fd 100"));
    ASSERT_THROW(Procedure(parse("fd 100")), std::logic_error);

    udp = Procedure(parse("TO MOVE"));
    ASSERT_FALSE(udp.addLine("RT 90 FD 60 LT 90"));
    ASSERT_TRUE(udp.addLine("END"));
}

TEST(Parser, streamList) {
    std::stringstream ss;
    List l;

    ss << l;
    ASSERT_EQ("[]", ss.str());

    l.push_back(Word("2+3"));

    ss = std::stringstream();
    ss << l;
    ASSERT_EQ("[2+3]", ss.str());

    l.push_back(Word("hello"));

    ss = std::stringstream();
    ss << l;
    ASSERT_EQ("[2+3 hello]", ss.str());

    l.push_back(Word("AlOhA"));

    ss = std::stringstream();
    ss << l;
    ASSERT_EQ("[2+3 hello AlOhA]", ss.str());
    ss.flush();
}

TEST(Parser, streamProcedure) {
    std::stringstream ss;
    auto udp = Procedure(parse("TO SQUARE :side"));
    udp.addLine("repeat 4 [fd :side RT 90]");
    udp.addLine("end");

    ss << udp;
    ASSERT_EQ("TO SQUARE :side\nrepeat 4 [fd :side RT 90]\nEND", ss.str());
}

TEST(Parser, statementMove) {
    Statement stmt{parse("FD 100")};
    ASSERT_EQ(1u, stmt.arguments.size());
    ASSERT_EQ(Expression(Number(100)),
              boost::get<Expression>(stmt.arguments[0]));

    Statement s1{std::move(stmt)};

    ASSERT_EQ(1u, s1.arguments.size());
    ASSERT_TRUE(stmt.arguments.empty());
    ASSERT_EQ(Expression(Number(100)), boost::get<Expression>(s1.arguments[0]));
}

TEST(Parser, streamNumber) {
    std::stringstream ss;

    ss << Number("3.14");
    ASSERT_EQ("3.14", ss.str());

    ss = std::stringstream();
    ss << Number(100);
    ASSERT_EQ("100", ss.str());
}

TEST(Parser, streamStatementInExpression) {
    std::stringstream ss;
    Expression e;
    e.node = Expression::Node::STATEMENT;
    ASSERT_ANY_THROW(ss << e);

    Statement stmt = parse("fd 100");
    e = stmt;
    ss << e;
    ASSERT_EQ("fd 100", ss.str());
}

TEST(Parser, procNameEquality) {
    ProcName a{"hi"};
    ProcName b("hi");
    ProcName c("Hi");
    ProcName d("sum");

    ASSERT_EQ(a, b);
    ASSERT_NE(a, c);
    ASSERT_NE(a, d);
    ASSERT_EQ(b, a);
    ASSERT_NE(b, c);
    ASSERT_NE(b, d);
    ASSERT_NE(c, d);
    ASSERT_EQ(c, c);
    ASSERT_EQ(d, d);
}

TEST(Parser, procedure) {
    Procedure p{parse("TO TEST")};
    p.addLine("PR [HELLO WORLD]");

    ASSERT_EQ(0u, p.nParams());
    ASSERT_EQ("TEST", p.name());
    ASSERT_TRUE(p.parameters().empty());
    ASSERT_EQ(1u, p.lines.size());
}

TEST(Parser, syntaxError) {
    ASSERT_THROW(parse("TO SUM 2+2 :a"), SyntaxError);
    ASSERT_THROW(parse("TO X :a :b :c 7"), SyntaxError);
    ASSERT_THROW(parse("END X :a :b :c 7"), SyntaxError);
    ASSERT_THROW(parse("END X"), SyntaxError);
    ASSERT_THROW(parse("print 7 + + 3"), SyntaxError);
}
