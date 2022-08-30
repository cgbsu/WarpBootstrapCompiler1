#include "MathematicalExpressions.hpp"
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>

using namespace Warp::Testing;

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

constexpr static const auto compare_value = [](auto left, auto right) {
	return left.value == right.value;
};

template<auto TestParameterConstant>
auto parse_whole(bool debug = false)
{
	return runtime_parse<
				WholeParserType, 
				TestParameterConstant, 	
				WholeEnumType::Expression
			>(debug);
}

template<auto TestParameterConstant>
auto parse_integer(bool debug = false)
{
	return runtime_parse<
				IntegerParserType, 
				TestParameterConstant, 	
				IntegerEnumType::Expression
			>(debug);
}

template<auto TestParameterConstant>
auto parse_fixed(bool debug = false)
{
	return runtime_parse<
			FixedParserType, 
			TestParameterConstant, 	
			FixedEnumType::Expression
		>(debug);
}

template<auto TestParameterConstant>
void print_whole(bool debug = false) {
	const auto result = parse_whole<TestParameterConstant>(debug);
	std::cout << "\nWhole Result: " << TestParameterConstant.string << " = " << result.value().value << "\n\n";
}

template<auto TestParameterConstant>
void print_integer(bool debug = false) {
	const auto result = parse_integer<TestParameterConstant>(debug);
	std::cout << "\nInteger Result: " << TestParameterConstant.string << " = " << result.value().value << "\n\n";
}

template<auto TestParameterConstant>
void print_fixed(bool debug = false) {
	const auto result = parse_fixed<TestParameterConstant>(debug);
	std::cout << "\nFixed Result: " << TestParameterConstant.string << " = " << result.value().value.number << "\n\n";
}


template<auto TestParameterConstant>
void whole_test(auto expected, bool debug = false)
{
	strict_check_parse<WholeExpressionType, compare_value>(
			parse_whole<TestParameterConstant>(debug) /*Actual*/, 
			WholeExpressionType{expected} /*Expected*/
		);
}


template<auto TestParameterConstant>
void integer_test(auto expected, bool debug = false)
{
	strict_check_parse<IntegerExpressionType, compare_value>(
			parse_integer<TestParameterConstant>(debug) /*Actual*/, 
			IntegerExpressionType{expected} /*Expected*/
		);
}

template<auto TestParameterConstant> 
void fixed_test(auto expected, bool debug = false)
{ 
		strict_check_parse<FixedExpressionType, compare_value>(
			parse_fixed<TestParameterConstant>(debug) /*Actual*/, 
			FixedExpressionType{expected} /*Expected*/
		);
}

void math_check(bool value) {
	CHECK(value);
}

TEST_GROUP(MathematicalExpressions) {};

bool compare_fixed(FixedExpressionType left, FixedExpressionType right) {
	return left.value == right.value;
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
	//std::cout << "RTF: " << parse_integer<FixedString{"5i - 10i - 8i"}>(true).value().value.number << "\n";
	integer_test<FixedString{"5i - 10i - 8i"}>(-13, debug);
	fixed_test<FixedString{"16.16xp - 8.8xp"}>(FixedType{7, 36}, debug);
	//std::cout << "RTF: " << parse_fixed<FixedString{"16.16xp - 8.8xp - 1.xp - 2.2xp"}>(true).value().value.number << "\n";
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
	bool debug = false;
	whole_test<FixedString{"5u * 3u"}>(15, debug);
	whole_test<FixedString{"5u * 3u * 20u"}>(300, debug);
	whole_test<FixedString{"5u * 3u * 20u * 44u"}>(13200, debug);
};

TEST(MathematicalExpressions, InputNegation)
{
	bool debug = false;
	integer_test<FixedString{"5i * -3i * -20i * 44i"}>(13200, debug);
	integer_test<FixedString{"-44i"}>(-44, debug);
	//print_integer<FixedString{"5i * -3i * -20i * -44i"}>(debug);
	integer_test<FixedString{"5i * -3i * -20i * -44i"}>(-13200, debug);
	integer_test<FixedString{"-5i * -3i * -20i * 44i"}>(-13200, debug);
	integer_test<FixedString{"-5i * -3i * -20i * -44i"}>(13200, debug);
};

TEST(MathematicalExpressions, InputDivision)
{
	bool debug = false;
	whole_test<FixedString{"15u / 3u"}>(5, debug);
	whole_test<FixedString{"60u / 3u / 20u"}>(1, debug);
	whole_test<FixedString{"126 / 7 / 3 / 2"}>(3, debug);
};

TEST(MathematicalExpressions, MixingBasicSumsAndProducts)
{
	bool debug = false;

	whole_test<FixedString{"5u * 3u + 8u * 7u"}>(71, debug);
	whole_test<FixedString{"4u + 5u * 3u - 21u / 7u"}>(16, debug);
	whole_test<FixedString{"5u * 3u - 21u / 7u"}>(12, debug);
	whole_test<FixedString{"5u * 3u * 21u / 7u + 4u"}>(49, debug);
	whole_test<FixedString{"5u * 3u * 21u / 7u + 4u + 8u"}>(57, debug);
	whole_test<FixedString{"5u * 3u * 21u / 7u + 4u * 8u"}>(77, debug);
	whole_test<FixedString{"6u + 5u * 3u - 21u / 7u + 4u"}>(22, debug);
	whole_test<FixedString{"5u * 3u - 21u / 7u + 4u"}>(16, debug);
	whole_test<FixedString{"5u * 3u * 21u / 7u + 4u"}>(49, debug);
};

