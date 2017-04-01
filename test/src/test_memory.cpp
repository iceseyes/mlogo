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
}
