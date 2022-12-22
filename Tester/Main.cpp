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

template<auto ParseType>
static const auto parse(std::string code, bool debug)
{
	return dynamic_runtime_parse<
			FunctionDeclarationParserType, 
			ParseType
		>(code, debug);
}

template<auto ParseType>
static const bool debug_parse(std::string code, bool debug)
{
	if(auto result = parse<ParseType>(code, debug); 
			result.has_value() == true && debug == true
		)
	{
		std::cout << "Successful Parse\n";
		return true;
	}
	else if(debug == true)
		std::cerr << "Parsing failure!";
	return false;
	
}

int main(int argc, char** args)
{
	bool debug = false;
	const std::string exit_string = "thanks :-)";
	NumericTypeTag reduction_data_type = NumericTypeTag::Whole;
	ContextType test_module;
	while(true)
	{
		std::string code_buffer;
		size_t choice = 0;
		std::string choice_buffer;
		std::cout << "Please enter what you would like to do: \n"
					<< "\t[0]: PARSE prototype declaration\n"
					<< "\t[1]: PARSE alternative declaration\n"
					<< "\t[2]: PARSE function call\n"
					<< "\t[3]: PARSE constant declaration\n"
					<< "\t[4]: EVALUATE function/constant declaration\n"
					<< "\t[5]: EVALUATE expression\n" 
					<< "\t[6]: TOGGLE debug mode (currently: " << debug << ")\n"
					<< "\t[7]: EXIT\n--> ";
		std::getline(std::cin, choice_buffer);
		choice = static_cast<size_t>(std::stoi(choice_buffer));
		if(choice != 7 && choice != 6)
			std::getline(std::cin, code_buffer);
		if(code_buffer == exit_string)
			break;
		switch(choice)
		{
			case 0 : {
				debug_parse<FunctionDeclaration::Prototype>(code_buffer, debug);
				break;
			}
			case 1 
			: { 
				debug_parse<FunctionDeclaration::Alternative>(code_buffer, debug); 
				break;
			}
			case 2 : {
				debug_parse<Call::Function>(code_buffer, debug);
				break;
			}
			case 3 
			: {
				std::cerr << "Sorry, not yet implemented\n";
				//auto constant = test_module.retrieve_constant(code_buffer);
				//if(constant.has_value() == true)
				//{
				//	if(constant.value().value.has_value() == true)
				//		std::cout << "-> " << constant.value.value.value().to_string() << "\n";
				//	else {
				//		auto result = retrieve_value<NumericValue>(&test_module, constant.value().expression, debug);
				//		constant.value().value = ConstantValueType{
				//}
				//else
				//	std::cerr << "No such constant.\n";
				break;
			}
			case 4 
			: {
				auto next_module_ = parse<Construct::Context>(code_buffer, debug);
				std::cout << "Got next module.\n";
				if(next_module_.has_value() == false)
					std::cerr << "Failed to parse.\n";
				ContextType next_module = std::move(next_module_.value());
				std::cout << "Unwrapped next_module\n";
				test_module.subsume(std::move(next_module));
				std::cout << "Done\n";
				break;
			}
			case 5 
			: {
				auto call = parse<Call::Function>(code_buffer, debug);
				auto value = std::move(retrieve_value<NumericValue>(
						&test_module, 
						call.value().get(), 
						debug
					));
				if(value.has_value() == true)
					std::cout << "-> " << value.value().to_string() << "\n";
				else 
					std::cerr << "Error, failed to retrieve a value.\n";
				break;
			}
			case 6 : {
				debug = !debug;
				break;
			}
			case 7 : {
				return 0;
			}
			default : {
				std::cout << "Sorry I did not understand that choice, please try again.\n";
				break;
			}
		}
		std::cout << "Done\n";
	}
	return 0;
}

