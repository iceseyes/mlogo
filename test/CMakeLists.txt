# CMakeLists.txt for testing
# 0. Define test framework and sources
# 1. Build simple programs and tools for manual testing
# 2. Download Google Test and compile it
# 3. Build mlogo_test from ${TEST_SRC} sources

# Sources
set(TEST_SRC
    src/test_gtest.cpp            # test for gtest framework is working
    src/test_parser.cpp           # test for parser
    src/test_memory.cpp           # test for memory
    src/test_eval.cpp             # test for eval
    src/test_types.cpp            # test for types
    src/test_geometry.cpp         # test for geometry
    src/test_interpreter.cpp      # test for interpreter /* high level test */
    src/builtin/test_arithmetic.cpp src/builtin/test_comm.cpp)


# Set-up
set(GTEST_VERSION release-1.8.0)
set(GTEST_DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib/gtest-src)
set(GTEST_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib/gtest)

if (WIN32)
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
    set(CMAKE_USE_PTHREADS_INIT OFF)
    set(CMAKE_THREAD_LIBS_INIT OFF)
    add_definitions(-DGTEST_OS_WINDOWS_MINGW)
endif (WIN32)

set(BUILD_SHARED_LIBS no)

enable_testing()

# Download and unpack googletest at configure time
configure_file(lib/CMakeLists.txt.in lib/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${GTEST_DOWNLOAD_DIR} ${GTEST_BUILD_DIR} EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif ()

# include la directory corrente nel build tree per rendere
# accessibile test_build_config.h
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../src)

# setup mlogo_test
add_executable(mlogo_test ${TEST_SRC})
target_link_libraries(mlogo_test ${MLOGO_DEPS} ${MLOGO_LIBRARY} gcov gtest_main)
add_test(NAME "mLogo automatic test" COMMAND mlogo_test)

# setup coverage
if (ENABLE_COVERAGE)
    set(COVERAGE_EXCLUDES '*/test/*' '*/boost/*' '*/c++/*')
    setup_target_for_coverage(
            NAME mlogo_test_coverage
            EXECUTABLE mlogo_test
            DEPENDENCIES mlogo_test)
endif ()
