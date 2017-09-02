/**
 * @file: test_memory.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */

#include <gtest/gtest.h>

#include <sstream>

#include "exceptions.hpp"
#include "memory.hpp"
#include "parser.hpp"

namespace mem = mlogo::memory;

namespace {

struct Nop : mlogo::types::BasicProcedure {
    Nop() : mlogo::types::BasicProcedure(0) {}
    void operator()() const override { /* empty procedure */
    }
};

} /* ns */

TEST(Memory, globalFrameVariables) {
    auto &frame = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasVariable("test"));
    ASSERT_FALSE(frame.hasVariable("test2"));
    ASSERT_THROW(mem::Stack::instance().getVariable("test"), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().getVariable("test2"), std::logic_error);

    frame.setVariable("test", "testValue");

    ASSERT_TRUE(frame.hasVariable("test"));
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasVariable("test"));
    ASSERT_EQ(mem::ValueBox("testValue"),
              mem::Stack::instance().getVariable("test"));

    frame.setVariable("test", "newValue");
    ASSERT_EQ(mem::ValueBox("newValue"),
              mem::Stack::instance().getVariable("test"));
    ASSERT_EQ(mem::ValueBox("newValue"),
              mem::Stack::instance().globalFrame().getVariable("test"));
    ASSERT_EQ(mem::ValueBox("newValue"), frame.getVariable("test"));

    frame.setVariable("test2", "value2");
    ASSERT_EQ(mem::ValueBox("newValue"),
              mem::Stack::instance().getVariable("test"));
    ASSERT_EQ(mem::ValueBox("newValue"),
              mem::Stack::instance().globalFrame().getVariable("test"));
    ASSERT_EQ(mem::ValueBox("newValue"), frame.getVariable("test"));
    ASSERT_EQ(mem::ValueBox("value2"),
              mem::Stack::instance().getVariable("test2"));
    ASSERT_EQ(mem::ValueBox("value2"),
              mem::Stack::instance().globalFrame().getVariable("test2"));
    ASSERT_EQ(mem::ValueBox("value2"), frame.getVariable("test2"));

    auto &ref = frame.getVariable("test2");
    ref = "HelloWorld";
    ASSERT_EQ(mem::ValueBox("HelloWorld"),
              mem::Stack::instance().getVariable("test2"));

    mem::Stack::instance().openFrame().currentFrame().setVariable(
        "test3", "not visible in global");

    ASSERT_EQ(mem::ValueBox("not visible in global"),
              mem::Stack::instance().getVariable("test3"));
    ASSERT_THROW(mem::Stack::instance().globalFrame().getVariable("test3"),
                 std::out_of_range);

    // a new frame was opened, so reference to global frame is changed.
    ASSERT_TRUE(&frame != &mem::Stack::instance().globalFrame())
        << "Stack has opened e new stack causing a realloc for the stack "
        << "itself";

    mem::Stack::instance().closeFrame();
}

TEST(Memory, nonGlobalFrameVariables) {
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest"), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest2"),
                 std::logic_error);

    // open a new frame on the top of the stack
    mem::Stack::instance().openFrame();

    auto &frame = mem::Stack::instance().currentFrame();
    auto &global = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasVariable("ctest"));
    ASSERT_FALSE(frame.hasVariable("ctest2"));
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest"), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest2"),
                 std::logic_error);

    frame.setVariable("ctest", "testValue");

    ASSERT_TRUE(frame.hasVariable("ctest"));
    ASSERT_TRUE(mem::Stack::instance().currentFrame().hasVariable("ctest"));
    ASSERT_FALSE(global.hasVariable("ctest"));
    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasVariable("ctest"));

    mem::ValueBox t1 = "testValue";
    mem::ValueBox t2 = "newValue";
    ASSERT_EQ(t1, mem::Stack::instance().getVariable("ctest"));

    frame.setVariable("ctest", "newValue");
    ASSERT_EQ(t2, mem::Stack::instance().getVariable("ctest"));
    ASSERT_THROW(mem::Stack::instance().globalFrame().getVariable("ctest"),
                 std::out_of_range);
    ASSERT_EQ(t2, mem::Stack::instance().currentFrame().getVariable("ctest"));
    ASSERT_EQ(t2, frame.getVariable("ctest"));
    ASSERT_THROW(global.getVariable("ctest"), std::out_of_range);

    t2 = "value2";
    frame.setVariable("ctest2", "value2");
    ASSERT_EQ(t2, mem::Stack::instance().getVariable("ctest2"));
    ASSERT_EQ(t2, mem::Stack::instance().currentFrame().getVariable("ctest2"));
    ASSERT_EQ(t2, frame.getVariable("ctest2"));

    auto &ref = frame.getVariable("ctest2");
    ref = "HelloWorld";
    ASSERT_EQ(mem::ValueBox("HelloWorld"),
              mem::Stack::instance().getVariable("ctest2"));

    mem::Stack::instance().openFrame();

    ASSERT_TRUE(&frame != &mem::Stack::instance().currentFrame())
        << "Stack has opened e new stack causing a realloc for the stack "
        << "itself";

    mem::Stack::instance().setVariable("test101", "abc", true);
    mem::Stack::instance().setVariable("test102", "cba", false);

    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasVariable("test101"));
    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasVariable("test102"));
    ASSERT_FALSE(mem::Stack::instance().currentFrame().hasVariable("test101"));
    ASSERT_TRUE(mem::Stack::instance().currentFrame().hasVariable("test102"));

    mem::Stack::instance().setVariable("test101", "123", false);

    ASSERT_EQ("123", mem::Stack::instance().getVariable("test101"));

    mem::Stack::instance().closeFrame();

    ASSERT_EQ("abc", mem::Stack::instance().getVariable("test101"));

    mem::Stack::instance().closeFrame();
}

TEST(Memory, globalFrameProcedureNoArgs) {
    auto &frame = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasProcedure("f_test"));
    ASSERT_FALSE(frame.hasProcedure("f_test2"));
    ASSERT_THROW(mem::Stack::instance().callProcedure("f_test", {}),
                 std::logic_error);
    ASSERT_THROW(mem::Stack::instance().callProcedure("f_test2", {}),
                 std::logic_error);

    ASSERT_EQ(1u, mem::Stack::instance().nFrames());
    mem::Stack::instance().setProcedure<Nop>("nop");
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasProcedure("nop"));
    ASSERT_NO_THROW(mem::Stack::instance().callProcedure("nop", {}));
    ASSERT_EQ(1u, mem::Stack::instance().nFrames());

    mem::Stack::instance().setVariable("globalLocalVar2", "empty");
    struct updateGlobal : mlogo::types::BasicProcedure {
        updateGlobal() : mlogo::types::BasicProcedure(0) {}
        void operator()() const override {
            ASSERT_EQ(2u, mem::Stack::instance().nFrames());
            mem::Stack::instance().setVariable("globalNewVar1", "123");
            mem::Stack::instance().setVariable("globalLocalVar2", "321");
        }
    };

    ASSERT_EQ(1u, mem::Stack::instance().nFrames());
    mem::Stack::instance().setProcedure<updateGlobal>("update_global");
    ASSERT_TRUE(
        mem::Stack::instance().globalFrame().hasProcedure("update_global"));

    ASSERT_EQ(mem::ValueBox("empty"),
              mem::Stack::instance().getVariable("globalLocalVar2"));
    ASSERT_THROW(mem::Stack::instance().getVariable("globalNewVar1"),
                 std::logic_error);
    ASSERT_NO_THROW(mem::Stack::instance().callProcedure("update_global", {}));
    ASSERT_EQ(mem::ValueBox("321"),
              mem::Stack::instance().getVariable("globalLocalVar2"));
    ASSERT_EQ(mem::ValueBox("123"),
              mem::Stack::instance().getVariable("globalNewVar1"));
    ASSERT_EQ(1u, mem::Stack::instance().nFrames());
}

TEST(Memory, ProcedureWithArgs) {
    struct SimplePrint1 : mlogo::types::BasicProcedure {
        SimplePrint1() : mlogo::types::BasicProcedure(1) {}
        void operator()() const override {
            auto arg0 = fetchArg(0);
            mem::Stack::instance().setVariable("__simple_print_result__", arg0);
        }
    };

    mem::Stack::instance().setLocalProcedure<SimplePrint1>("simple_print_1");

    ASSERT_THROW(mem::Stack::instance().getVariable("__simple_print_result__"),
                 std::logic_error);

    mem::ActualArguments args1;
    args1.push_back("test");
    mem::Stack::instance().callProcedure("simple_print_1", args1);
    ASSERT_EQ(mem::ValueBox("test"),
              mem::Stack::instance().getVariable("__simple_print_result__"));

    mem::ActualArguments args2;
    args2.push_back("tset");
    mem::Stack::instance().callProcedure("simple_print_1", args2);
    ASSERT_EQ(mem::ValueBox("tset"),
              mem::Stack::instance().getVariable("__simple_print_result__"));

    struct SimplePrint2 : mlogo::types::BasicProcedure {
        SimplePrint2() : mlogo::types::BasicProcedure(2) {}

        void operator()() const override {
            auto arg0 = fetchArg(0).toString();
            auto arg1 = fetchArg(1).toString();

            mem::Stack::instance().setVariable("__simple_print_result__",
                                               arg0 + " " + arg1);
        }
    };

    mem::Stack::instance().setLocalProcedure<SimplePrint2>("simple_print_2");

    mem::ActualArguments args3;
    args3.push_back("alpha");
    args3.push_back("beta");
    mem::Stack::instance().callProcedure("simple_print_2", args3);
    ASSERT_EQ(mem::ValueBox("alpha beta"),
              mem::Stack::instance().getVariable("__simple_print_result__"));

    mem::Stack::instance().callProcedure("simple_print_1", args3);
    ASSERT_EQ(mem::ValueBox("alpha"),
              mem::Stack::instance().getVariable("__simple_print_result__"));
    ASSERT_THROW(mem::Stack::instance().callProcedure("simple_print_2", args2),
                 std::logic_error);
}

TEST(Memory, ProcedureWithArgsAndReturnValue) {
    struct Sum : mlogo::types::BasicProcedure {
        Sum() : mlogo::types::BasicProcedure(2, true) {}
        void operator()() const override {
            std::stringstream ss;
            auto arg0 = fetchArg(0).asInteger();
            auto arg1 = fetchArg(1).asInteger();
            ss << (arg0 + arg1);
            mem::Stack::instance().storeResult(ss.str());
        }
    };

    struct SimplePrint3 : mlogo::types::BasicProcedure {
        SimplePrint3() : mlogo::types::BasicProcedure(0) {}
        void operator()() const override {
            mem::ActualArguments args;
            args.push_back("2");
            args.push_back("3");

            mem::Stack::instance().callProcedure("sum", args, "res");
            auto arg0 = mem::Stack::instance().getVariable("res");
            mem::Stack::instance().setVariable("__simple_print_result__", arg0);
        }
    };

    mem::Stack::instance()
        .setLocalProcedure<Sum>("sum")
        .setLocalProcedure<SimplePrint3>("simple_print_3");

    mem::ActualArguments args;
    mem::Stack::instance().callProcedure("simple_print_3", args);
    ASSERT_EQ(mem::ValueBox("5"),
              mem::Stack::instance().getVariable("__simple_print_result__"));
}

TEST(Memory, ignoreCase) {
    struct Sum : mlogo::types::BasicProcedure {
        Sum() : mlogo::types::BasicProcedure(0, true) {}
        void operator()() const override {}
    };

    mem::Stack::instance().setVariable("UPPERcase", "123");
    ASSERT_EQ("123", mem::Stack::instance().getVariable("uppercase"));
    ASSERT_EQ("123", mem::Stack::instance().getVariable("uppERcase"));
    ASSERT_EQ("123", mem::Stack::instance().getVariable("UPPERCASE"));
    ASSERT_EQ("123", mem::Stack::instance().getVariable("UPPERCaSE"));
    ASSERT_EQ("123", mem::Stack::instance().getVariable("UPPERcase"));

    auto ptr = std::make_shared<Sum>();
    mem::Stack::instance().setProcedure("UPPERcaseSum", ptr);
    ASSERT_EQ(ptr, mem::Stack::instance().getProcedure("uppercaseSUM"));
    ASSERT_EQ(ptr, mem::Stack::instance().getProcedure("uppERcaseSUM"));
    ASSERT_EQ(ptr, mem::Stack::instance().getProcedure("UPPERCASEsUM"));
    ASSERT_EQ(ptr, mem::Stack::instance().getProcedure("UPPERCaSESUm"));
    ASSERT_EQ(ptr, mem::Stack::instance().getProcedure("UPPERcaseSuM"));
}

TEST(Memory, frameGetVariable) {
    mem::Frame f;

    f.setVariable("test", "123");
    ASSERT_EQ("123", f.getVariable("test"));

    const mem::Frame &ref = f;
    ASSERT_EQ("123", ref.getVariable("test"));
}

TEST(Memory, frameGetProcedure) {
    using InvalidProcedureError = mlogo::exceptions::InvalidProcedureBody;
    mem::Frame f;

    ASSERT_THROW(f.setProcedure("exception", nullptr), InvalidProcedureError);

    f.setProcedure<Nop>("noexcept");
    ASSERT_TRUE(f.getProcedure("noexcept"));

    const mem::Frame &ref = f;
    ASSERT_TRUE(ref.getProcedure("noexcept"));
}

TEST(Memory, noReturnedValue) {
    using NoReturnValueException = mlogo::exceptions::NoReturnValueException;

    mem::Frame f, one;
    one.waitForValueIn("test");
    ASSERT_THROW(one.setResultVariable(f), NoReturnValueException);
}

TEST(Memory, undefinedProcedure) {
    using UndefinedProcedure = mlogo::exceptions::UndefinedProcedure;

    auto &stack = mem::Stack::instance();
    ASSERT_THROW(stack.getProcedure("undefined"), UndefinedProcedure);
    ASSERT_THROW(stack.getProcedureNArgs("undefined"), UndefinedProcedure);
}

TEST(Memory, closingFrameException) {
    using UnclosableFrameException =
        mlogo::exceptions::UnclosableFrameException;
    using InvalidReturnValue = mlogo::exceptions::InvalidReturnValue;
    using ExpectedReturnValue = mlogo::exceptions::ExpectedReturnValue;

    mem::Stack::instance().closeFrame();  // close working frame
    ASSERT_THROW(mem::Stack::instance().closeFrame(), UnclosableFrameException);

    auto &f = mem::Stack::instance().openFrame().currentFrame();
    mem::Stack::instance().openFrame();
    mem::Stack::instance().storeResult("byebye");
    ASSERT_THROW(mem::Stack::instance().closeFrame(), InvalidReturnValue);
    f.waitForValueIn("test");
    mem::Stack::instance().closeFrame();

    mem::Stack::instance().openFrame();
    f.waitForValueIn("test");
    ASSERT_THROW(mem::Stack::instance().closeFrame(), ExpectedReturnValue);
    mem::Stack::instance().storeResult("byebye");
    mem::Stack::instance().closeFrame();
}

TEST(Memory, procedure2Stack) {
    mlogo::parser::Procedure definition{
        mlogo::parser::parse("TO SQUARE :side")};
    definition.addLine("nop");
    definition.addLine("end");

    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasProcedure("SQUARE"));
    mem::Stack::instance().setProcedure(definition);
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasProcedure("SQUARE"));

    definition = mlogo::parser::parse("TO SQUARE2 :side");
    definition.addLine("square :side");
    definition.addLine("end");

    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasProcedure("SQUARE2"));
    mem::Stack::instance().setProcedure(definition);
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasProcedure("SQUARE2"));
}

TEST(Memory, frameClear) {
    struct Sum : mlogo::types::BasicProcedure {
        Sum() : mlogo::types::BasicProcedure(0, true) {}
        void operator()() const override {}
    };

    mem::Frame testFrame;
    testFrame.setVariable("one", "test1");
    testFrame.setVariable("two", "test2");

    auto ptr = std::make_shared<Sum>();
    testFrame.setProcedure("oneProc", ptr);

    ASSERT_TRUE(testFrame.hasVariable("one"));
    ASSERT_TRUE(testFrame.hasVariable("two"));
    ASSERT_TRUE(testFrame.hasProcedure("oneProc"));

    testFrame.clear();

    ASSERT_FALSE(testFrame.hasVariable("one"));
    ASSERT_FALSE(testFrame.hasVariable("two"));
    ASSERT_FALSE(testFrame.hasProcedure("oneProc"));
}

TEST(Memory, clearStack) {
    struct Sum : mlogo::types::BasicProcedure {
        Sum() : mlogo::types::BasicProcedure(0, true) {}
        void operator()() const override {}
    };

    mem::Stack::instance().openFrame();
    ASSERT_TRUE(mem::Stack::instance().nFrames() > 1);

    mem::Stack::instance().currentFrame().setVariable("one", "test1");
    mem::Stack::instance().currentFrame().setVariable("two", "test2");

    auto ptr = std::make_shared<Sum>();
    mem::Stack::instance().currentFrame().setProcedure("oneProc", ptr);

    mem::Stack::instance().globalFrame().setVariable("three", "test3");
    mem::Stack::instance().globalFrame().setVariable("four", "test4");

    ptr = std::make_shared<Sum>();
    mem::Stack::instance().globalFrame().setProcedure("twoProc", ptr);

    ASSERT_TRUE(mem::Stack::instance().currentFrame().hasVariable("one"));
    ASSERT_TRUE(mem::Stack::instance().currentFrame().hasVariable("two"));
    ASSERT_TRUE(mem::Stack::instance().currentFrame().hasProcedure("oneProc"));
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasVariable("three"));
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasVariable("four"));
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasProcedure("twoProc"));

    mem::Stack::instance().clear();

    ASSERT_FALSE(mem::Stack::instance().currentFrame().hasVariable("one"));
    ASSERT_FALSE(mem::Stack::instance().currentFrame().hasVariable("two"));
    ASSERT_FALSE(mem::Stack::instance().currentFrame().hasProcedure("oneProc"));
    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasVariable("three"));
    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasVariable("four"));
    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasProcedure("twoProc"));
}
