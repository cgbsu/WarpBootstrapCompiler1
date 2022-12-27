#include <iostream>
#include <string>

extern "C" {
	extern int32_t my_function_1(int32_t my_parameter);
}

constexpr static const uint32_t default_arguments[] = {5, 15, 42};
const static auto exit_string = std::string{"thanks :-)"};

int main(int argc, char** args)
{
	for(uint32_t argument : default_arguments)
		std::cout << "my_function1@" << argument << ": " << my_function_1(argument) << "\n";
	while(true)
	{
		std::cout << "Testing my_function_1 (IR code created with WarpBootstrapCompiler1) "
					"enter single 32-bit integer input or \"" << exit_string << "\" to exit: \n\t--> ";
		std::string input;
		std::getline(std::cin, input);
		if(input == exit_string)
			return 0;
		uint32_t argument_for_test_function = static_cast<uint32_t>(std::stoi(input.c_str()));
		std::cout << "my_function1@" << argument_for_test_function << ": " << my_function_1(argument_for_test_function) << "\n";
	}
	return 0;
}

