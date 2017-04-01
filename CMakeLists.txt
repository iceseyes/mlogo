project(mLogo CXX)
cmake_minimum_required(VERSION 3.0.2)

include (CheckFunctionExists)

# Compiling Options
option(WITH_TESTS "Enable build for mLogo Automatic Tests" ON)
option(USE_BOOST_STATIC  "Static link for boost librerees" OFF)

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
  add_definitions(-DBOOST_LOG_DYN_LINK)
endif(USE_BOOST_STATIC)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS thread log_setup log filesystem program_options system)

set(MLOGO_DEPS
    ${Boost_LIBRARIES} rt ${CMAKE_THREAD_LIBS_INIT})

include_directories(${Boost_INCLUDE_DIRS})

set(SRCS
    src/main.cpp)
    
set(LIBLOGO_SRCS
  src/parser.cpp src/memory.cpp
  src/types.cpp)

add_library(${MLOGO_LIBRARY} ${LIBLOGO_SRCS})

add_executable(${MLOGO_PROGRAM} ${SRCS})
  
target_link_libraries(${MLOGO_PROGRAM} ${MLOGO_DEPS} ${MLOGO_LIBRARY})

if(WITH_TESTS)
  add_subdirectory(test)
endif(WITH_TESTS)

