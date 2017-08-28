project(mLogo C CXX)
cmake_minimum_required(VERSION 3.0.2)

include (CheckFunctionExists)

# Compiling Options
option(WITH_TESTS "Enable build for mLogo Automatic Tests" ON)
option(USE_BOOST_STATIC  "Static link for boost librerees" OFF)
option(WITH_WINDOW_ALWAYS_ON_TOP "Requires that graphics window is always on top" ON)
option(WITH_COVERAGE "Run coverage for tests" OFF)
option(WITH_BOOST_SPIRIT_DEBUG "Enable Boost Spirit DEBUG informations" OFF)

# Basic Configuration
set(MLOGO_PROGRAM mlogo)
set(MLOGO_LIBRARY logo)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

# bug GCC
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-result")

if(USE_BOOST_STATIC)
  message(STATUS "Linking Boost Statically")
  set(Boost_USE_STATIC_LIBS        ON)
  set(Boost_USE_STATIC_RUNTIME     ON)
else(USE_BOOST_STATIC)
  message(STATUS "Linking Boost dynamically")
endif(USE_BOOST_STATIC)

find_package(Threads)
find_package(Boost REQUIRED)
find_package(SDL2 REQUIRED)

if(WITH_COVERAGE)
    find_program(GCOV_PATH gcov)
    find_program(LCOV_PATH lcov)

    if(GCOV_PATH AND LCOV_PATH)
      include(CodeCoverage)
      APPEND_COVERAGE_COMPILER_FLAGS()
      set(ENABLE_COVERAGE YES)
    else()
      message(WARNING "Coverage disabled: install gcov/lcov")
    endif()
endif()

if(WITH_WINDOW_ALWAYS_ON_TOP)
    add_definitions(-DWITH_WINDOW_ALWAYS_ON_TOP)
endif()

if(WITH_BOOST_SPIRIT_DEBUG)
    add_definitions(-DBOOST_SPIRIT_DEBUG)
endif()

# Workaround for libstdc++ using clang and gdb
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D_GLIBCXX_DEBUG)
endif()

set(MLOGO_DEPS
    ${Boost_LIBRARIES} rt ${CMAKE_THREAD_LIBS_INIT} ${SDL2_LIBRARY})

include_directories(${Boost_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR})

set(SRCS
    src/main.cpp)
    
set(LIBLOGO_SRCS
  src/parser.cpp src/memory.cpp
  src/types.cpp src/value.cpp src/eval.cpp
  src/geometry.cpp src/graphics.cpp src/turtle.cpp)

set(LIBLOGO_BUILTIN_SRCS
  src/builtin/arithmetic.cpp
  src/builtin/comm.cpp
  src/builtin/data.cpp
  src/builtin/control.cpp
  src/builtin/graphics.cpp
  src/builtin/memory.cpp
  src/builtin/main.cpp
)
  
add_library(${MLOGO_LIBRARY} ${LIBLOGO_SRCS} ${LIBLOGO_BUILTIN_SRCS})

add_executable(${MLOGO_PROGRAM} ${SRCS})
  
target_link_libraries(${MLOGO_PROGRAM} ${MLOGO_DEPS} ${MLOGO_LIBRARY})
target_link_libraries(${MLOGO_LIBRARY} ${MLOGO_DEPS})

if(WITH_TESTS)
  add_subdirectory(test)
endif(WITH_TESTS)

