#include <Warp/Cache/FunctionParser.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Testing/TestParser.hpp>
using namespace Warp::Parsing;
using namespace Warp::Testing;

using FunctionDeclarationParserType = FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		NumericTypeTag
	>;

std::optional<Warp::Runtime::Compiler::ContextType> parse_context(std::string code, bool debug = false)
{
	return dynamic_runtime_parse<
			FunctionDeclarationParserType, 
			Warp::Parsing::Construct::Context
		>(code, debug);
}

