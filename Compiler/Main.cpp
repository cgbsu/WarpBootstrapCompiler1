#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Testing/TestParser.hpp>
#include <Warp/SyntaxTranslation/LLVM/Translators/MathematicalExpressions.hpp>
using namespace Warp::Parsing;
using namespace Warp::Testing;
using namespace Warp::SyntaxAnalysis::SyntaxTree;
using namespace Warp::Utilities;
using namespace Warp::Runtime::Compiler;
using namespace Warp::SyntaxTranslation;

using FunctionDeclarationParserType = FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		NumericTypeTag
	>;

template<auto ParseType>
static const auto parse(std::string code, bool debug)
{
	return dynamic_runtime_parse<
			FunctionDeclarationParserType, 
			ParseType
		>(code, debug);
}

extern LLVM::Context context;

int main(int argc, char** args)
{
	ContextType test_context;
	auto parse_result = runtime_parse<
			FunctionDeclarationParserType, 
			FixedString{"40u + 2u"}, 
			FunctionDeclarationParserType::WholeMathematicalParserType::TypeSpecificMathematicalExpressionTermTags::Expression
		>(true);
	if(parse_result.has_value() == true)
	{
		LLVM::translate<LLVM::Target::LLVM, llvm::Value*>(
				&context, 
				&test_context, 
				parse_result.value().node.get(), 
				true
			);
		std::cout << "Done with translation\n";
		context.module.print(llvm::errs(), nullptr);
	}
	std::cout << "Done!\n";
	return 0;
}

LLVM::Context context = LLVM::Context();

