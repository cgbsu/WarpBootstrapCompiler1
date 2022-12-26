#include <Warp/Common.hpp>
#define WARP__BOOTSTRAP__COMPILER__COMPILE__LATEST__PARSER__CACHE
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/Cache/FunctionParser.hpp>
#include <Warp/SyntaxTranslation/LLVM/Translators/FunctionDeclaration.hpp>
//using namespace Warp::Parsing;
//using namespace Warp::Testing;
//using namespace Warp::SyntaxAnalysis::SyntaxTree;
using namespace Warp::Utilities;
using namespace Warp::Runtime::Compiler;
using namespace Warp::SyntaxTranslation;


extern LLVM::Context context;

int main(int argc, char** args)
{
	auto parse_result_ = parse_context(
			std::string{"let my_function() { 42u * 42u };"}, 
			true
		);
	std::string test_function_name = "my_function";
	if(parse_result_.has_value() == true)
	{
		const auto parsed_context = std::move(parse_result_.value());
		std::cout << "Have parse result got " << parsed_context.functions.size() << " functions\n";
		for(const auto& function : parsed_context.functions)
		{
			std::string name = function.second->get_name();
			std::cout << "Function: <" << name << "> (" << name.size() << " character name): \n";
			size_t parameter_count = 0;
			for(const auto& alternatives : function.second->get_alternatives()) {
				std::cout << "\tHas " << alternatives.size() << " alternatives with " << parameter_count << " parameters.\n";
				++parameter_count;
			}
		}
		std::cout << "Parsed context has function <" << test_function_name << ">: " 
				<< std::boolalpha << (parsed_context.functions.contains(test_function_name) == true) << "\n";
		auto test_function_ = parsed_context.retrieve_function(test_function_name);
		std::cout << "Retrieved test function? " << std::boolalpha << (test_function_.has_value() == true) << "\n";
		if(test_function_.has_value() == false) {
			std::cout << "FAILURE TO RETRIEVE FUNCTION\n";
			return 1;
		}
		LLVM::translate_function(&context, &parsed_context, true, test_function_.value());
		//LLVM::translate<LLVM::Target::LLVM, llvm::Value*>(
		//		&context, 
		//		&test_context, 
		//		parse_result.value().node.get(), 
		//		true
		//	);
		std::cout << "Done with translation\n";
		context.module.print(llvm::errs(), nullptr);
	}
	else
		std::cout << "PARSING FAILURE\n";
	std::cout << "Done!\n";
	return 0;
}

LLVM::Context context = LLVM::Context();

