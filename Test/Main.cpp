#include <Warp/Utilities.hpp>

using namespace Warp::Utilities;

constexpr static const char test[] = "ABC123";
int main(int argc, char** args)
{
	using X = TemplateStringType<test>;
	std::cout << X::string << "\n";
	return 0;
}

