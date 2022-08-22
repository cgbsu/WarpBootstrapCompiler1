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

//TEST(MathematicalExpressions, Addition)
//{
//	check_parse(runtime_parse<
//			 ParserTestTemplate<NumericTypeTag::Whole, NumericTypeTag::Whole>, 
//			 FixedString{"1u + 1u"}, 
//			 NumericTypeTag::Whole
//		>() /*Actual*/, WholeType{2u} /*Expected*/);
//};

