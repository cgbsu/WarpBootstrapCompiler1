#include <Warp/Utilities/Logging.hpp>

using namespace Warp::Utilities;

int main(int argc, char** args)
{
	Log<LogType::Warning>()[std::cout, "Hello"];
	return 0;
}

