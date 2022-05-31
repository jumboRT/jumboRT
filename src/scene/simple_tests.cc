/**
 * @file simple_tests.cpp
 * @author Hyxogen
 * @brief 
 * @version 0.1
 * @date 2022-01-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "simple_tests.hh"
#include "simple_tests_int.hh"
#include <iostream>

void SimpleDoFail(const SimpleTest& test,std::string message) {
	std::cout << "Failed test[" << test.GetTestName() << "]: " << message << std::endl;
}

SimpleInstance::SimpleInstance() : m_TestList(SimpleTestList()) {

}

void SimpleInstance::AddTest(SimpleTest *test) {
	m_TestList.push_back(test);
}

SimpleInstance *SimpleInstance::GetInstance() {
	static SimpleInstance *instance = nullptr;
	if (instance == nullptr)
		instance = new SimpleInstance();
	return (instance);
}

void SimpleInstance::RunAll() {
	for (SimpleTest *test : m_TestList)
		test->ExecuteTest();
}

