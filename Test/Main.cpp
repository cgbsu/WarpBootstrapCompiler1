#include <iostream>
#include "CppUTest/CommandLineTestRunner.h"

int main(int argc, char** args) {
	const auto result = CommandLineTestRunner::RunAllTests(argc, args);
	return 0;
}

