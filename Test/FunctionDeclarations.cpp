#include <Warp/Parsing/MathematicalExpressions.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>

using namespace Warp::Utilities;
using namespace Warp::Testing;
using namespace Warp::Parsing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;

using WholeType = NumericTypeResolver<NumericTypeTag::Whole>::Type;
using IntegerType = NumericTypeResolver<NumericTypeTag::Integer>::Type;
using FixedType = NumericTypeResolver<NumericTypeTag::FixedPoint>::Type;
using CharType = NumericTypeResolver<NumericTypeTag::Character>::Type;
using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;

using NumericConstantType = Constant<SyntaxNode, NumericTypeTag>;
using NumericContexType = std::unordered_map<std::string, NumericConstantType>;

// Test instantiation //
template struct FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		NumericTypeTag
	>;

using NumericParserType = FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		NumericTypeTag
	>;

template<auto TypeTagParameterConstant>
auto retrieve_constant(const OptionalConcept auto& context, std::string name) {
	return retrieve_value<WholeType>(context.value().constants.at(name).value);
}

template<auto TypeTagParameterConstant>
auto retrieve_constant(const OptionalConcept auto& constant) {
	return retrieve_value<WholeType>(constant.value().value);
}
template<auto TypeTagParameterConstant>
auto print_constant(const OptionalConcept auto& context, std::string name, bool courtesy = true) {
	std::cout << "{" << name << ":" << retrieve_constant<TypeTagParameterConstant>(context, name) << "}"
			<< ((courtesy == false) ? "" : "\n");
}
template<auto TypeTagParameterConstant>
auto print_constant(const OptionalConcept auto& constant, bool courtesy = true) {
	std::cout << "{" << constant.name << ":" << retrieve_constant<TypeTagParameterConstant>(constant) << "}"
			<< ((courtesy == false) ? "" : "\n");
}

bool check(auto left, auto right) {
	return left == right;
}

template<
		auto TestParameterConstant, 
		auto TypeTagParameterConstant, 
		auto CheckParamterConstant = check, 
		typename ParserParameterType = NumericParserType
	>
void check_constant(
		std::string name, 
		const auto expected, 
		bool debug = false, 
		std::source_location test_location = std::source_location::current
	)
{

	const auto context = runtime_parse<
			ParserParameterType, 
			TestParameterConstant, 
			NumericParserType::UniqueProductions::Context
		>(debug);
	if(const bool parse_success = context.has_value() == false; parse_success == false)
	{
		std::cerr << test_location.file_name() << ": " << test_location.line() << ": "
				<< "Failed to parse or retrieve constant {" << name << "} with " 
				<< TestParameterConstant.string << "\n";
		CHECK((parse_success == true));
	}
	else
	{
		const auto result = retrieve_constant(context, name);
		if(const bool check_result = CheckParamterConstant(result, expected);
				check_result == false)
		{
			std::cerr << test_location.file_name() << ": " << test_location.line() << ": "
					<< "Check failed with " 
					<< name << " = " << result << " and " 
					<< expected << "\n";
			CHECK((check_result == true));
		}
	}
};



TEST_GROUP(FunctionDeclarations) {};

TEST(FunctionDeclarations, DeclareConstantFromLiteral)
{
	bool debug = false;
	runtime_parse<
			NumericParserType, 
			FixedString{"let TheQuestion = 42u"}, 
			Construct::Constant
		>(debug);
	runtime_parse<
			NumericParserType, 
			FixedString{"let TheQuestion = 2u * 21u"}, 
			Construct::Constant
		>(debug);
	auto context = runtime_parse<
			NumericParserType, 
			FixedString{"let TheQuestion = 2u * 20u +2u;"}, 
			NumericParserType::UniqueProductions::Context
		>(debug);
	print_constant<NumericTypeTag::Whole>(context, std::string{"TheQuestion"});
};

