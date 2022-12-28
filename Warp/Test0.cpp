#include <iostream>

extern "C" {
	int32_t my_function_0();
}

int main(int argc, char** args) {
	std::cout << "my_function_0(): " << my_function_0() << "\n";
	return 0;
}

