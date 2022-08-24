#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/Parsing/MathematicalExpressions.hpp>
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

bool compare_fixed(FixedExpressionType left, FixedExpressionType right)
{
	return left.value == right.value;
}

TEST(MathematicalExpressions, InputAddition)
{
	bool debug = false;
	strict_check_parse<WholeExpressionType, compare_value>(runtime_parse<
				WholeParserType, 
				FixedString{"1u + 1u"}, 
				WholeEnumType::Expression
			>(debug) /*Actual*/, 
			WholeExpressionType{WholeType{2u}} /*Expected*/
		);
	strict_check_parse<WholeExpressionType, compare_value>(runtime_parse<
				WholeParserType, 
				FixedString{"1 + 1"}, 
				WholeEnumType::Expression
			>(debug) /*Actual*/, 
			WholeExpressionType{WholeType{2u}} /*Expected*/
		);
	strict_check_parse<WholeExpressionType, compare_value>(runtime_parse<
				WholeParserType, 
				FixedString{"5 + 3"}, 
				WholeEnumType::Expression
			>(debug) /*Actual*/, 
			WholeExpressionType{WholeType{8u}} /*Expected*/
		);
	strict_check_parse<WholeExpressionType, compare_value>(runtime_parse<
				WholeParserType, 
				FixedString{"5u8 + 3u8"}, 
				WholeEnumType::Expression
			>(debug) /*Actual*/, 
			WholeExpressionType{WholeType{8u}} /*Expected*/
		);
	strict_check_parse<IntegerExpressionType, compare_value>(runtime_parse<
				IntegerParserType, 
				FixedString{"9i8 + 3i5"}, 
				IntegerEnumType::Expression
			>(debug) /*Actual*/, 
			IntegerExpressionType{IntegerType{12u}} /*Expected*/
		);
	strict_check_parse<IntegerExpressionType, compare_value>(runtime_parse<
				IntegerParserType, 
				FixedString{"9i8 + 3i8 + 10i"}, 
				IntegerEnumType::Expression
			>(debug) /*Actual*/, 
			IntegerExpressionType{IntegerType{22}} /*Expected*/
		);
	strict_check_parse<FixedExpressionType, compare_value>(runtime_parse<
				FixedParserType, 
				FixedString{"16.16xp + 16.16xp"}, 
				FixedEnumType::Expression
			>(debug) /*Actual*/, 
			FixedExpressionType{FixedType{32, 32}} /*Expected*/
		);
};

TEST(MathematicalExpressions, InputSubraction)
{
	bool debug = false;
	strict_check_parse<WholeExpressionType, compare_value>(runtime_parse<
				WholeParserType, 
				FixedString{"5 - 3"}, 
				WholeEnumType::Expression
			>(debug) /*Actual*/, 
			WholeExpressionType{WholeType{2}} /*Expected*/
		);
	strict_check_parse<IntegerExpressionType, compare_value>(runtime_parse<
				IntegerParserType, 
				FixedString{"5i - 10i"}, 
				IntegerEnumType::Expression
			>(debug) /*Actual*/, 
			IntegerExpressionType{IntegerType{-5}} /*Expected*/
		);
	strict_check_parse<IntegerExpressionType, compare_value>(runtime_parse<
				IntegerParserType, 
				FixedString{"5i - 10i - 8i"}, 
				IntegerEnumType::Expression
			>(debug) /*Actual*/, 
			IntegerExpressionType{IntegerType{-13}} /*Expected*/
		);
	//strict_check_parse<FixedExpressionType, compare_value>(runtime_parse<
	//			FixedParserType, 
	//			FixedString{"16.16xp - 8.8xp"}, 
	//			FixedEnumType::Expression
	//		>(debug) /*Actual*/, 
	//		FixedExpressionType{FixedType{8, 8}} /*Expected*/
	//	);
};

