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
		bool debug, 
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

TEST(BooleanExpressionTests, Comparisons)
{
	bool debug = false;
	bool_test<FixedString{"2u > 1u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u <= 1u"}>(WarpBool::False, debug);
	bool_test<FixedString{"1u <= 1u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u >= 1u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u >= 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 2u"}>(WarpBool::False, debug);

	bool_test<FixedString{"200u > 5498u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5498u > 200u"}>(WarpBool::True, debug);
	bool_test<FixedString{"1232u < 4895u"}>(WarpBool::True, debug);
	bool_test<FixedString{"424u <= 212u"}>(WarpBool::False, debug);
	bool_test<FixedString{"212u <= 212u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2131u <= 12131u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2131u <= 2131u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5462u >= 5783u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5462u >= 5462u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2765456u >= 221397u"}>(WarpBool::True, debug);
	bool_test<FixedString{"23947u > 23947u"}>(WarpBool::False, debug);
	bool_test<FixedString{"248u < 248u"}>(WarpBool::False, debug);

	bool_test<FixedString{"23809u = 23809u"}>(WarpBool::True, debug);
	bool_test<FixedString{"23809u = 38890u"}>(WarpBool::False, debug);
	bool_test<FixedString{"38890u = 23809u"}>(WarpBool::False, debug);
};

