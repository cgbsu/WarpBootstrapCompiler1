#include <Warp/Parsing/BooleanExpressions.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>

using namespace Warp::Utilities;
using namespace Warp::Parsing;
using namespace Warp::Testing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;

using TestParserType = BooleanExpressionParser<
		BooleanExpressionTermsType, 
		NumericTypeResolver
	>;

using UnderlyingBoolType = typename NumericTypeResolver<NumericTypeTag::Bool>::Type::UnderylingType;
using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;

constexpr static const bool from_warp_bool(WarpBool from) {
	return from == WarpBool::True;
}

constexpr static const auto compare_value = [](
		const auto& left, 
		const auto& right, 
		bool debug = true, 
		std::source_location location
	)
{
	if(debug == true)
		std::cout << "Start Retrieving Value, with Expected: " << from_warp_bool(right) << "\n";
	const auto actual = retrieve_value<
			typename NumericTagResolver<CleanType<decltype(right)>>::NumericType
		>(left.get(), debug).value();
	bool result = (actual == right);
	if(result == false) {
		std::cout << location.file_name() << ": " << location.line() 
				<< ": Comparision failure! With Actual: " << result 
				<< " vs. Expected: " << from_warp_bool(right) << "\n";
	}
	return result;
};

template<auto TestParameterConstant>
auto parse_bool(bool debug = false, std::source_location location = std::source_location::current())
{
	if(debug == true)
		std::cout << "(Bool) Parsing: " << TestParameterConstant.string << "\n";
	auto result = runtime_parse<
				TestParserType, 
				TestParameterConstant, 	
				BooleanExpression::Expression
			>(debug);
	if(result.has_value() == false) {
		std::cout << location.file_name() << ": " << location.line() 
				<< ": Failed to parse Whole with: " << TestParameterConstant.string << "\n";
	}
	return result;
}

template<auto TestParameterConstant>
void bool_test(
		UnderlyingBoolType expected, 
		bool debug = false, 
		std::source_location location = std::source_location::current()
	)
{
	check_parse<compare_value>(
			parse_bool<TestParameterConstant>(debug, location) /*Actual*/, 
			expected, /*Expected*/
			debug, 
			location
		);
}

TEST_GROUP(BooleanExpressionTests) {};

TEST(BooleanExpressionTests, Comparisons) {
	bool debug = true;
	bool_test<FixedString{"2u > 1u"}>(WarpBool::True, debug);
};

