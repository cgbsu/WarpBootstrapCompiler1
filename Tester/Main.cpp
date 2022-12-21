#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <filesystem>
//auto debug_log = [](bool condition, std::source_location location = std::source_location::current())
//{
//	if(condition == true)
//	{
//		std::stringstream buffer;
//		Log<std::stringstream>(location)[buffer, "Failure!"];
//		std::cerr << buffer.str() << "\n";
//	}
//}
//
//#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED debug_log
//#include <Warp/Testing/ConstantTests.hpp>
#include <Warp/Testing/TestParser.hpp>

using namespace Warp::Utilities;
using namespace Warp::Parsing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;
using namespace Warp::Runtime::Compiler;
using namespace Warp::Testing;

using FunctionDeclarationParserType = FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		NumericTypeTag
	>;

static const auto parse(std::string code, bool debug)
{
	return dynamic_runtime_parse<
			FunctionDeclarationParserType, 
			Call::Function
		>(code, debug);
}

int main(int argc, char** args)
{
	bool debug = false;
	std::string code_buffer;
	while(true)
	{
		std::getline(std::cin, code_buffer);
		parse(code_buffer, debug);
		std::cout << "Done\n";
		code_buffer = "";
	}
	return 0;
}

