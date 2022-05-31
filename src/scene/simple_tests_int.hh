/**
 * @file simple_tests_int.hpp
 * @author Hyxogen
 * @brief 
 * @version 0.1
 * @date 2022-01-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SIMPLE_TESTS_INTERNAL
#define SIMPLE_TESTS_INTERNAL

#include <string>
#include <vector>

class SimpleTest {
public:	
	virtual std::string GetTestName() const = 0;

	virtual void ExecuteTest() = 0;
};

using SimpleTestList = std::vector<SimpleTest*>;

class SimpleInstance {
public:
	SimpleTestList m_TestList;

	SimpleInstance();

	void AddTest(SimpleTest *test);

	void RunAll();

	static SimpleInstance *GetInstance();
};

void	SimpleDoFail(const SimpleTest& test, std::string message);

template<typename Type>
SimpleTest *RegisterTest() {
	SimpleTest *test = new Type;
	SimpleInstance::GetInstance()->AddTest(test);
	return (test);
}

template<typename Type>
void SimpleAssertEqual(const SimpleTest& test, Type expected, Type actual, std::string file, int line, std::string message);
template<typename Type>
void SimpleAssertArrayEqual(const SimpleTest& test, Type *array1, Type *array2, int count, std::string file, int line, std::string message);

template<typename Type>
void SimpleAssertEqual(const SimpleTest& test, Type expected, Type actual, std::string file, int line, std::string message) {
	(void)file;
	(void)line;
	if (actual != expected)
		SimpleDoFail(test, file + ":" + std::to_string(line) + " Expected " + std::to_string(expected) + " actual: " + std::to_string(actual) + " " + message);
}

template<typename Type>
void SimpleAssertArrayEqual(const SimpleTest& test, const Type *array1, const Type *array2, int count, std::string file, int line, std::string message) {
	(void)message;
	for (int i = 0; i < count; i++) {
		SimpleAssertEqual<Type>(test, array1[i], array2[i], file, line,
			"at: " + std::to_string(i));
	}
}

#define _SIMPLE_TEST_FAIL(file, line, message)											SimpleDoFail(*this, #file ":" #line " - " #message);

#define _SIMPLE_ASSERT(condition, file, line, message)									{ if (condition) {} else { _SIMPLE_TEST_FAIL(file, line, message) }}

#define _SIMPLE_ASSERT_EQUAL(expected, actual, file, line, message)						SimpleAssertEqual(*this, expected, actual, file, line, message)
#define _SIMPLE_ASSERT_EQUAL_ARRAY(expected, actual, count, file, line, message)		SimpleAssertArrayEqual(*this, expected, actual, count, file, line, message);

#define _SIMPLE_RUN_ALL_TESTS()										SimpleInstance::GetInstance()->RunAll()

#define _SIMPLE_TEST_CLASSNAME(name) name##_simpletest

#define _SIMPLE_TEST(name)                                                       \
class _SIMPLE_TEST_CLASSNAME(name) : public SimpleTest {                         \
private:                                                                         \
	static SimpleTest const *executor;                                           \
                                                                                 \
public:                                                                          \
                                                                                 \
	void ExecuteTest() override;                                                 \
                                                                                 \
	std::string GetTestName() const override;                                    \
};                                                                               \
std::string _SIMPLE_TEST_CLASSNAME(name)::GetTestName() const { return #name; }  \
SimpleTest const *_SIMPLE_TEST_CLASSNAME(name)::executor =                       \
	RegisterTest<_SIMPLE_TEST_CLASSNAME(name)>();                                \
void _SIMPLE_TEST_CLASSNAME(name)::ExecuteTest()

#endif

