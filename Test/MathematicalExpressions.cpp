#include <Warp/Runtime/Compiler/NumericType.hpp>
#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/Parsing/MathematicalExpressions.hpp>
#include <Warp/Utilities.hpp>
#include <ThirdParty/fpm/ios.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Testing/TestParser.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>

using namespace Warp::Testing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;
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

using UnderlyingWholeType = typename NumericTypeResolver<NumericTypeTag::Whole>::Type::UnderylingType;
using UnderlyingIntegerType = typename NumericTypeResolver<NumericTypeTag::Integer>::Type::UnderylingType;
using UnderlyingFixedType = typename NumericTypeResolver<NumericTypeTag::FixedPoint>::Type::UnderylingType;
using UnderlyingCharType = typename NumericTypeResolver<NumericTypeTag::Character>::Type::UnderylingType;
using UnderlyingBoolType = typename NumericTypeResolver<NumericTypeTag::Bool>::Type::UnderylingType;

using WholeParserType = ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>;
using IntegerParserType = ParserTestTemplate<NumericTypeTag::Integer, NumericTypeTag::Integer>;
using FixedParserType = ParserTestTemplate<NumericTypeTag::FixedPoint, NumericTypeTag::FixedPoint>;

using WholeEnumType = WholeParserType::TypeSpecificMathematicalExpressionTermTags;
using IntegerEnumType = IntegerParserType::TypeSpecificMathematicalExpressionTermTags;
using FixedEnumType = FixedParserType::TypeSpecificMathematicalExpressionTermTags;

using WholeExpressionType = WholeParserType::Expression;
using IntegerExpressionType = IntegerParserType::Expression;
using FixedExpressionType = FixedParserType::Expression;

constexpr static const auto compare_value = [](const auto& left, const auto& right) {
	return retrieve_value<typename NumericTagResolver<CleanType<decltype(right)>>::NumericType>(left.node.get()).value() == right;
};

template<auto TestParameterConstant>
auto parse_whole(bool debug = false)
{
	return runtime_parse<
				WholeParserType, 
				TestParameterConstant, 	
				WholeParserType::TypeSpecificMathematicalExpressionTermTags::Expression
			>(debug);
}

template<auto TestParameterConstant>
auto parse_integer(bool debug = false)
{
	return runtime_parse<
				IntegerParserType, 
				TestParameterConstant, 	
				IntegerParserType::TypeSpecificMathematicalExpressionTermTags::Expression
			>(debug);
}

template<auto TestParameterConstant>
auto parse_fixed(bool debug = false)
{
	return runtime_parse<
			FixedParserType, 
			TestParameterConstant, 	
			FixedParserType::TypeSpecificMathematicalExpressionTermTags::Expression
		>(debug);
}

template<auto TestParameterConstant>
void print_whole(bool debug = false)
{
	const auto result = parse_whole<TestParameterConstant>(debug);
	std::cout << "\nWhole Result: " << TestParameterConstant.string 
			<< " = " << retrieve_value<NumericTypeResolver<NumericTypeTag::Whole>::Type>(
					result.value().node
				).value() << "\n\n";
}

template<auto TestParameterConstant>
void print_integer(bool debug = false)
{
	const auto result = parse_integer<TestParameterConstant>(debug);
	std::cout << "\nInteger Result: " << TestParameterConstant.string 
			<< " = " << retrieve_value<NumericTypeResolver<NumericTypeTag::Integer>::Type>(
					result.value().node
				).value() << "\n\n";
}

template<auto TestParameterConstant>
void print_fixed(bool debug = false)
{
	const auto result = parse_fixed<TestParameterConstant>(debug);
	std::cout << "\nFixed Result: " << TestParameterConstant.string 
			<< " = " << retrieve_value<NumericTypeResolver<NumericTypeTag::FixedPoint>::Type>(
					result.value().node
				).value() << "\n\n";
}


template<auto TestParameterConstant>
void whole_test(UnderlyingWholeType expected, bool debug = false)
{
	check_parse<compare_value>(
			parse_whole<TestParameterConstant>(debug) /*Actual*/, 
			expected /*Expected*/
		);
}


template<auto TestParameterConstant>
void integer_test(UnderlyingIntegerType expected, bool debug = false)
{
	check_parse<compare_value>(
			parse_integer<TestParameterConstant>(debug) /*Actual*/, 
			expected /*Expected*/
		);
}

template<auto TestParameterConstant> 
void fixed_test(UnderlyingFixedType expected, bool debug = false)
{ 
		check_parse<compare_value>(
			parse_fixed<TestParameterConstant>(debug) /*Actual*/, 
			expected /*Expected*/
		);
}

void math_check(bool value) {
	CHECK(value);
}

TEST_GROUP(MathematicalExpressions) {};

bool compare_fixed(const SyntaxNode& left, FixedType right) {
	return retrieve_value<FixedType>(left.get()).value() == right;
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
	integer_test<FixedString{"5i - 3i"}>(2, debug);
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
	integer_test<FixedString{"-44i"}>(-44, debug);
	integer_test<FixedString{"5i * -3i * -20i * 44i"}>(13200, debug);
	integer_test<FixedString{"5i * -3i * -20i * 44i"}>(13200, debug);
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

TEST(MathematicalExpressions, Parenthesis)
{
	bool debug = false;

	whole_test<FixedString{"(1)"}>(1, debug);
	whole_test<FixedString{"6u + 5u * 3u - (21u / 7u + 4u)"}>(14, debug);
	whole_test<FixedString{"(6u + 5u) * 3u - 21u / 7u + 4u"}>(34, debug);
	whole_test<FixedString{"(6u + 5u) * 3u - 33u / (7u + 4u)"}>(30, debug);
	integer_test<FixedString{"6i + (5i * 3i - 3102i) / 7i + 4i"}>(-431, debug);
	whole_test<FixedString{"5u * (3u * 21u / 7u) + 4u"}>(49, debug);
	whole_test<FixedString{"5u * 3u * 21u / (7u + 4u + 10u)"}>(15, debug);
};


TEST(MathematicalExpressions, NestedParenthesis)
{
	bool debug = false;

	whole_test<FixedString{"3 * (4 * (4 + 4) + 6 * (2 + 3))"}>(186, debug);
	whole_test<FixedString{"3 * (4 * (4 + 4) + 6 * (2 + ((2 + 4) / 2)))"}>(186, debug);
	whole_test<FixedString{"3 * (4 * (4 + 4) + (2 + 3) * (2 + ((2 + 4) / 2)))"}>(171, debug);
	whole_test<FixedString{"3* ((4 * (4 + 4) + (2 + 3)) * (2 + ((2 + 4) / 2)))"}>(555, debug);
};

