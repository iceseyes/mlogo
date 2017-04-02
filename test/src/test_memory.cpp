/**
 * @file: test_memory.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include <gtest/gtest.h>
#include "memory.hpp"

namespace mem = mlogo::memory;

TEST(Memory, globalFrameVariables) {
    auto &frame = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasVariable("test"));
    ASSERT_FALSE(frame.hasVariable("test2"));
    ASSERT_THROW(mem::Stack::instance().getVariable("test"), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().getVariable("test2"), std::logic_error);

    frame.setVariable("test", "testValue");

    ASSERT_TRUE(frame.hasVariable("test"));
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasVariable("test"));
    ASSERT_EQ("testValue", mem::Stack::instance().getVariable("test"));

    frame.setVariable("test", "newValue");
    ASSERT_EQ("newValue", mem::Stack::instance().getVariable("test"));
    ASSERT_EQ("newValue", mem::Stack::instance().globalFrame().getVariable("test"));
    ASSERT_EQ("newValue", frame.getVariable("test"));

    frame.setVariable("test2", "value2");
    ASSERT_EQ("newValue", mem::Stack::instance().getVariable("test"));
    ASSERT_EQ("newValue", mem::Stack::instance().globalFrame().getVariable("test"));
    ASSERT_EQ("newValue", frame.getVariable("test"));

    ASSERT_EQ("value2", mem::Stack::instance().getVariable("test2"));
    ASSERT_EQ("value2", mem::Stack::instance().globalFrame().getVariable("test2"));
    ASSERT_EQ("value2", frame.getVariable("test2"));

    std::string &ref = frame.getVariable("test2");
    ref = "HelloWorld";
    ASSERT_EQ("HelloWorld", mem::Stack::instance().getVariable("test2"));

    mem::Stack::instance()
        .openFrame().currentFrame()
        .setVariable("test3", "not visible in global");

    ASSERT_EQ("not visible in global", mem::Stack::instance().getVariable("test3"));
    ASSERT_THROW(mem::Stack::instance().globalFrame().getVariable("test3"), std::out_of_range);

    // a new frame was opened, so reference to global frame is changed.
    ASSERT_TRUE(&frame != &mem::Stack::instance().globalFrame())
        << "Stack has opened e new stack causing a realloc for the stack itself";

    mem::Stack::instance().closeFrame();
}

TEST(Memory, nonGlobalFrameVariables) {
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest"), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest2"), std::logic_error);

    // open a new frame on the top of the stack
    mem::Stack::instance().openFrame();

    auto &frame = mem::Stack::instance().currentFrame();
    auto &global = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasVariable("ctest"));
    ASSERT_FALSE(frame.hasVariable("ctest2"));
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest"), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().getVariable("ctest2"), std::logic_error);

    frame.setVariable("ctest", "testValue");

    ASSERT_TRUE(frame.hasVariable("ctest"));
    ASSERT_TRUE(mem::Stack::instance().currentFrame().hasVariable("ctest"));
    ASSERT_FALSE(global.hasVariable("ctest"));
    ASSERT_FALSE(mem::Stack::instance().globalFrame().hasVariable("ctest"));

    ASSERT_EQ("testValue", mem::Stack::instance().getVariable("ctest"));

    frame.setVariable("ctest", "newValue");
    ASSERT_EQ("newValue", mem::Stack::instance().getVariable("ctest"));
    ASSERT_THROW(mem::Stack::instance().globalFrame().getVariable("ctest"), std::out_of_range);
    ASSERT_EQ("newValue", mem::Stack::instance().currentFrame().getVariable("ctest"));
    ASSERT_EQ("newValue", frame.getVariable("ctest"));
    ASSERT_THROW(global.getVariable("ctest"), std::out_of_range);

    frame.setVariable("ctest2", "value2");
    ASSERT_EQ("value2", mem::Stack::instance().getVariable("ctest2"));
    ASSERT_EQ("value2", mem::Stack::instance().currentFrame().getVariable("ctest2"));
    ASSERT_EQ("value2", frame.getVariable("ctest2"));

    std::string &ref = frame.getVariable("ctest2");
    ref = "HelloWorld";
    ASSERT_EQ("HelloWorld", mem::Stack::instance().getVariable("ctest2"));

    mem::Stack::instance().openFrame();

    ASSERT_TRUE(&frame != &mem::Stack::instance().currentFrame())
        << "Stack has opened e new stack causing a realloc for the stack itself";

    mem::Stack::instance().closeFrame();
    mem::Stack::instance().closeFrame();
}

TEST(Memory, globalFrameProcedureNoArgs) {
    auto &frame = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasProcedure("f_test"));
    ASSERT_FALSE(frame.hasProcedure("f_test2"));
    ASSERT_THROW(mem::Stack::instance().callProcedure("f_test", {}), std::logic_error);
    ASSERT_THROW(mem::Stack::instance().callProcedure("f_test2", {}), std::logic_error);

    struct Nop : mlogo::types::BasicProcedure {
        Nop() : mlogo::types::BasicProcedure(0) {}
        void operator()() const override { /* empty procedure */ }
    };

    ASSERT_EQ(1u, mem::Stack::instance().nFrames());
    mem::Stack::instance().globalFrame().setProcedure<Nop>("nop");
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasProcedure("nop"));
    ASSERT_NO_THROW(mem::Stack::instance().callProcedure("nop", {}));
    ASSERT_EQ(1u, mem::Stack::instance().nFrames());

    mem::Stack::instance().globalFrame().setVariable("globalLocalVar2", "empty");
    struct updateGlobal : mlogo::types::BasicProcedure {
        updateGlobal() : mlogo::types::BasicProcedure(0) {}
        void operator()() const override {
            ASSERT_EQ(2u, mem::Stack::instance().nFrames());
            mem::Stack::instance().globalFrame().setVariable("globalNewVar1", "123");
            mem::Stack::instance().globalFrame().setVariable("globalLocalVar2", "321");
        }
    };

    ASSERT_EQ(1u, mem::Stack::instance().nFrames());
    mem::Stack::instance().globalFrame().setProcedure<updateGlobal>("update_global");
    ASSERT_TRUE(mem::Stack::instance().globalFrame().hasProcedure("update_global"));

    ASSERT_EQ("empty", mem::Stack::instance().getVariable("globalLocalVar2"));
    ASSERT_THROW(mem::Stack::instance().getVariable("globalNewVar1"), std::logic_error);
    ASSERT_NO_THROW(mem::Stack::instance().callProcedure("update_global", {}));
    ASSERT_EQ("321", mem::Stack::instance().getVariable("globalLocalVar2"));
    ASSERT_EQ("123", mem::Stack::instance().getVariable("globalNewVar1"));
    ASSERT_EQ(1u, mem::Stack::instance().nFrames());
}

TEST(Memory, ProcedureWithArgs) {
    struct SimplePrint1 : mlogo::types::BasicProcedure {
        SimplePrint1() : mlogo::types::BasicProcedure(1) {}
        void operator()() const override {
            std::string arg0 = fetchArg(0);
            mem::Stack::instance().globalFrame()
                .setVariable("__simple_print_result__", arg0);
        }
    };

    mem::Stack::instance().currentFrame()
        .setProcedure<SimplePrint1>("simple_print_1");

    ASSERT_THROW(mem::Stack::instance().getVariable("__simple_print_result__"), std::logic_error);

    mem::ActualArguments args1;
    args1.push_back("test");
    mem::Stack::instance().callProcedure("simple_print_1", args1);
    ASSERT_EQ("test", mem::Stack::instance().getVariable("__simple_print_result__"));

    FAIL() << "Incomplete Test";
}
