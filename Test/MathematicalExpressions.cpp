#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/Parsing/MathematicalExpressions.hpp>
#include <Warp/Utilities.hpp>
#include <ThirdParty/fpm/ios.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>

using namespace Warp::Testing;
using namespace Warp::Parsing;
using namespace Warp::Utilities;

template<auto ResultTagParameterConstant, auto InputTagParameterConstant>
using ParserTestTemplate = MathematicalExpressionParser<
		MathematicalExpressionTermsType, 
		NumericTypeResolver, 
		ResultTagParameterConstant, 
		InputTagParameterConstant
	>;


using WholeType = NumericTypeResolver<NumericTypeTag::Whole>::Type;
using IntegerType = NumericTypeResolver<NumericTypeTag::Integer>::Type;
using FixedType = NumericTypeResolver<NumericTypeTag::FixedPoint>::Type;
using CharType = NumericTypeResolver<NumericTypeTag::Character>::Type;
using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;

using WholeParserType = ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>;
using IntegerParserType = ParserTestTemplate<NumericTypeTag::Integer, NumericTypeTag::Integer>;
using FixedParserType = ParserTestTemplate<NumericTypeTag::FixedPoint, NumericTypeTag::FixedPoint>;

using WholeEnumType = WholeParserType::TypeSpecificMathematicalExpressionTermTags;
using IntegerEnumType = IntegerParserType::TypeSpecificMathematicalExpressionTermTags;
using FixedEnumType = FixedParserType::TypeSpecificMathematicalExpressionTermTags;

using WholeExpressionType = WholeParserType::Expression;
using IntegerExpressionType = IntegerParserType::Expression;
using FixedExpressionType = FixedParserType::Expression;

constexpr const auto compare_value = [](auto left, auto right) 
		{ return left.value == right.value; };

void math_check(bool value) {
	CHECK(value);
}

TEST_GROUP(MathematicalExpressions) {};

bool compare_fixed(FixedExpressionType left, FixedExpressionType right) {
	return left.value == right.value;
}

template<auto TestParameterConstant>
void whole_test(auto expected, bool debug = false)
{
	strict_check_parse<WholeExpressionType, compare_value>(runtime_parse<
				WholeParserType, 
				TestParameterConstant, 	
				WholeEnumType::Expression
			>(debug) /*Actual*/, 
			WholeExpressionType{expected} /*Expected*/
		);
}

template<auto TestParameterConstant>
void integer_test(auto expected, bool debug = false)
{
	strict_check_parse<IntegerExpressionType, compare_value>(runtime_parse<
				IntegerParserType, 
				TestParameterConstant, 	
				IntegerEnumType::Expression
			>(debug) /*Actual*/, 
			IntegerExpressionType{expected} /*Expected*/
		);
}

template<auto TestParameterConstant>
void fixed_test(auto expected, bool debug = false)
{
	strict_check_parse<FixedExpressionType, compare_value>(runtime_parse<
				FixedParserType, 
				TestParameterConstant, 	
				FixedEnumType::Expression
			>(debug) /*Actual*/, 
			FixedExpressionType{expected} /*Expected*/
		);
}

TEST(MathematicalExpressions, InputAddition)
{
	bool debug = false;
	whole_test<FixedString{"1u + 1u"}>(2u, debug);
	whole_test<FixedString{"1 + 1"}>(2u, debug);
	whole_test<FixedString{"5 + 3"}>(8u, debug);
	whole_test<FixedString{"5u8 + 3u8"}>(8u, debug);
	integer_test<FixedString{"9i8 + 3i5"}>(12u, debug);
	integer_test<FixedString{"9i8 + 3i8 + 10i"}>(22, debug);
	fixed_test<FixedString{"16.16xp + 16.16xp"}>(FixedType{32, 32}, debug);
};

TEST(MathematicalExpressions, InputSubraction)
{
	bool debug = false;
	whole_test<FixedString{"5 - 3"}>(2, debug);
	integer_test<FixedString{"5i - 10i"}>(-5, debug);
	integer_test<FixedString{"5i - 10i - 8i"}>(-13, debug);
	fixed_test<FixedString{"16.16xp - 8.8xp"}>(FixedType{7, 36}, debug);
	fixed_test<FixedString{"16.16xp - 8.8xp - 1.xp - 2.2xp"}>(FixedType{4, 16}, debug);
};

TEST(MathematicalExpressions, InputSums)
{
	bool debug = false;
	integer_test<FixedString{"5i - 10i + 14i - 1i"}>(8, debug);
	integer_test<FixedString{"5i - 10i + 14i - 1i - 23i"}>(-15, debug);
};

TEST(MathematicalExpressions, InputMultiplication)
{
	bool debug = true;
	whole_test<FixedString{"5u * 3u"}>(15, debug);
};

