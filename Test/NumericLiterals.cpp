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
auto parse()
{
	return parser<ReduceToTagParameterConstant>.parse(
			ctpg::buffers::string_buffer(StringParameterConstant.string), 
			std::cerr
		);
}

void check_parse(auto parse_result, auto expected_result)
{
	CHECK((parse_result.has_value() == true)
			? parse_result.value() == expected_result
			: false
		);
}

TEST(NumericLiterals, Whole)
{
	std::cout << "A\n";
	check_parse(parse<FixedString{"123"}, NumericLiteral::Whole>(), 123);
};

