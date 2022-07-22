#include <Warp/Parsing/NumericLiterals.hpp>
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
		ctpg::terms(NumericLiteralParserTestType::digits), 
		ctpg::nterms(NumericLiteralParserTestType::whole), 
		ctpg::rules(
				NumericLiteralParserTestType::parse_whole
			)
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

consteval void check_parse(auto parse_result, auto expected_result)
{
	CHECK((parse_result.has_value() == true)
			? parse_result.value() == expected_result
			: false
		);
}

template<typename ResultParameterType>
consteval void strict_check_parse(
		std::optional<ResultParameterType> parse_result, 
		ResultParameterType expected_result) {
	check_parse(parse_result, expected_result);
}
TEST(NumericLiterals, Whole) {
	check_parse(parse<FixedString{"123"}, NumericLiteral::Whole>(), 123);
};

