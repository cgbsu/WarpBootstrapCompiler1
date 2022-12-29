#include <iostream>

extern "C" {
	extern int32_t factorial_1(int32_t to_factor);
	extern int32_t my_function_1(int32_t to_factor);
	extern int32_t my_other_function_1(int32_t to_factor);
}

int main(int argc, char** args)
{
		int32_t to_factor;
		std::cin >> to_factor;
		std::cout << "factorial_1: " << factorial_1(to_factor) << "\n";
		std::cout << "my_function_1: " << my_function_1(to_factor) << "\n";
		std::cout << "my_other_function_1: " << my_other_function_1(to_factor) << "\n";
		return 0;
}

