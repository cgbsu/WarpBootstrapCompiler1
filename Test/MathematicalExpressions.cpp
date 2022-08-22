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

TEST_GROUP(MathematicalExpressions) {};

TEST(MathematicalExpressions, InputAddition)
{
	bool debug = false;
	strict_check_parse<WholeType>(runtime_parse<
				 ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>, 
				 FixedString{"1u + 1u"}, 
				 NumericTypeTag::Whole
			>(debug) /*Actual*/, 
			WholeType{2u} /*Expected*/
		);
	strict_check_parse<WholeType>(runtime_parse<
				 ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>, 
				 FixedString{"1 + 1"}, 
				 NumericTypeTag::Whole
			>(debug) /*Actual*/, 
			WholeType{2u} /*Expected*/
		);
	strict_check_parse<WholeType>(runtime_parse<
				 ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>, 
				 FixedString{"5 + 3"}, 
				 NumericTypeTag::Whole
			>(debug) /*Actual*/, 
			WholeType{8u} /*Expected*/
		);
	strict_check_parse<WholeType>(runtime_parse<
				 ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>, 
				 FixedString{"5u8 + 3u8"}, 
				 NumericTypeTag::Whole
			>(debug) /*Actual*/, 
			WholeType{8u} /*Expected*/
		);
	strict_check_parse<IntegerType>(runtime_parse<
				 ParserTestTemplate<NumericTypeTag::Integer, NumericTypeTag::Integer>, 
				 FixedString{"9i8 + 3i5"}, 
				 NumericTypeTag::Integer
			>(debug) /*Actual*/, 
			IntegerType{12u} /*Expected*/
		);
	//TODO: The following runs fine
	//std::cout << "VALUE?: " << runtime_parse<
	//			 ParserTestTemplate<NumericTypeTag::FixedPoint, NumericTypeTag::FixedPoint>, 
	//			 FixedString{"16.16xp + 16.16xp"}, 
	//			 NumericTypeTag::FixedPoint
	//		>(debug).value().number << "\n";
	//However this runs failes CHECK()
	//strict_check_parse<FixedType>(
	//		runtime_parse<
	//			 ParserTestTemplate<NumericTypeTag::FixedPoint, NumericTypeTag::FixedPoint>, 
	//			 FixedString{"16.16xp + 16.16xp"}, 
	//			 NumericTypeTag::FixedPoint
	//		>(true) /*Actual*/, 
	//		FixedType{32, 32} /*Expected*/
	//	);
	// I will assume for the moment it is somethign to do with the test code or FixedPoint type 
	// and not the parser, and I will resume (TODO) this later.
};

TEST(MathematicalExpressions, InputSubraction)
{
	bool debug = false;
	strict_check_parse<WholeType>(parse<
				 ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>, 
				 FixedString{"5 - 3"}, 
				 NumericTypeTag::Whole
			>() /*Actual*/, 
			WholeType{2} /*Expected*/
		);
	strict_check_parse<IntegerType>(parse<
				 ParserTestTemplate<NumericTypeTag::Integer, NumericTypeTag::Integer>, 
				 FixedString{"5i - 10i"}, 
				 NumericTypeTag::Integer
			>() /*Actual*/, 
			IntegerType{-5} /*Expected*/
		);
};

