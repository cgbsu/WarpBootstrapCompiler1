#include <iostream>
#include "CppUTest/CommandLineTestRunner.h"

int main(int argc, char** args)
{
	std::cout << "GLaDOS: <Initiating test proper>...\n";
	const auto result = CommandLineTestRunner::RunAllTests(argc, args);
		std::cout << "GLaDOS: </Test proper complete> Please proceede through the material emancipation grid to the next test chamber, then there will be... cake\n";
}

