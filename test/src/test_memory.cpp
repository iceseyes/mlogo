/**
 * @file: test_memory.cpp
 *
 *  created on:  1 aprile 2017
 *      author: Massimo Bianchi <bianchi.massimo@gmail.com>
 */


#include <gtest/gtest.h>
#include "memory.hpp"

namespace mem = mlogo::memory;

TEST(Memory, globalFrame) {
    auto &frame = mem::Stack::instance().globalFrame();

    ASSERT_FALSE(frame.hasVariable("test"));

    FAIL() << "Incomplete Test";
}
