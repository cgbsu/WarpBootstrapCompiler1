#include <Warp/Parsing/NumericLiterals.hpp>
#include <ThirdParty/fpm/ios.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Parsing;
using namespace Warp::Utilities;

TEST_GROUP(NumericLiterals)
{
};

using NumericLiteralParserTestType = NumericLiteralParser<
		NumericLiteralTermsType, 
		NumericLiteralTypeResolver
	>;

template<auto ReduceToTagParameterConstant>
constexpr const static auto parser = ctpg::parser(
		NumericLiteralTermsType::term<ReduceToTagParameterConstant>, 
		NumericLiteralParserTestType::terms, 
		NumericLiteralParserTestType::non_terminal_terms, 
		NumericLiteralParserTestType::rules()
	);

template<
		FixedString StringParameterConstant, 
		auto ReduceToTagParameterConstant
	>
consteval auto parse()
{
	return parser<ReduceToTagParameterConstant>.parse(
			ctpg::buffers::cstring_buffer(StringParameterConstant.string), 
			std::cerr
		);
}

constexpr static const auto equal 
		= [](auto left, auto right) { return left == right; };

template<auto CompareParameterConstant = equal>
constexpr void check_parse(auto parse_result, auto expected_result)
{
	CHECK(parse_result.has_value() == true);
	CHECK((parse_result.has_value() == true)
			? CompareParameterConstant(parse_result.value(), expected_result)
			: false
		);
}

template<typename ResultParameterType, auto CompareParameterConstant = equal>
constexpr void strict_check_parse(
		std::optional<ResultParameterType> parse_result, 
		ResultParameterType expected_result) {
	check_parse(parse_result, expected_result);
}
using FixedType = NumericLiteralParserTestType::FixedPointType;

TEST(NumericLiterals, Parse)
{
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"123"}, NumericLiteral::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"123u"}, NumericLiteral::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
			parse<FixedString{"-123"}, NumericLiteral::Integer>(), // Actual
			-123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
			parse<FixedString{"-123i"}, NumericLiteral::Integer>(), // Actual
			-123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
			parse<FixedString{"123i"}, NumericLiteral::Integer>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
			parse<FixedString{"123.123"}, NumericLiteral::FixedPoint>(), // Actual
			static_cast<FixedType>(123.123) // Expected
		);
	//strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
	//		parse<FixedString{"123.123fxp"}, NumericLiteral::FixedPoint>(), // Actual
	//		static_cast<FixedType>(123.123) // Expected
	//	);
	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
			parse<FixedString{"-123.123"}, NumericLiteral::FixedPoint>(), // Actual
			static_cast<FixedType>(-123.123) // Expected
		);
};

