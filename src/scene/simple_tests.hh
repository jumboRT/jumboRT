/**
 * @file simple_tests.hpp
 * @author Hyxogen
 * @brief 
 * @version 0.1
 * @date 2022-01-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SIMPLE_TESTS_H
#define SIMPLE_TESTS_H

#include "simple_tests_int.hh"

#define SIMPLE_TEST(test_name)							_SIMPLE_TEST(test_name)

#define ASSERT_TRUE(condition)							_SIMPLE_ASSERT(condition, __FILE__, __LINE__, "Expected: TRUE actual: FALSE")
#define ASSERT_FALSE(condition)							_SIMPLE_ASSERT(!(condition), __FILE__, __LINE__, "Expected: FALSE actual: TRUE")
#define ASSERT_EQUAL(expected, actual)					_SIMPLE_ASSERT_EQUAL(expected, actual, __FILE__, __LINE__, "")
#define ASSERT_EQUAL_ARRAY(expected, actual, count)		_SIMPLE_ASSERT_EQUAL_ARRAY(expected, actual, count, __FILE__, __LINE__, "")

#define SIMPLE_INIT(argc, argv)							(void)argc;(void)argv;
#define SIMPLE_RUN_ALL_TESTS()							_SIMPLE_RUN_ALL_TESTS()
#define SIMPLE_CLEANUP()

#endif

