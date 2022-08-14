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
template<
		FixedString StringParameterConstant, 
		auto ReduceToTagParameterConstant
	>
constexpr auto runtime_parse()
{
	return parser<ReduceToTagParameterConstant>.parse(
			ctpg::buffers::string_buffer(StringParameterConstant.string), 
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
using CharType = NumericLiteralParserTestType::CharacterType;
using BoolType = NumericLiteralParserTestType::BoolType;

TEST(NumericLiterals, ParseWholeAnyBase)
{
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"0d123"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"123"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"123u"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"0b1111011"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"0o173"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<NumericLiteralParserTestType::WholeType>(
			parse<FixedString{"0x7B"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
};
//
//TEST(NumericLiterals, ParseIntegers)
//{
//	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
//			parse<FixedString{"-0b1111011"}, NumericTypeTag::Integer>(), // Actual
//			-123 // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
//			parse<FixedString{"-0b1111011"}, NumericTypeTag::Integer>(), // Actual
//			-123 // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
//			parse<FixedString{"-0o173"}, NumericTypeTag::Integer>(), // Actual
//			-123 // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
//			parse<FixedString{"-123"}, NumericTypeTag::Integer>(), // Actual
//			-123 // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
//			parse<FixedString{"-123i"}, NumericTypeTag::Integer>(), // Actual
//			-123 // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::IntegerType>(
//			parse<FixedString{"123i"}, NumericTypeTag::Integer>(), // Actual
//			123 // Expected
//		);
//};
//
//TEST(NumericLiterals, ParseFixedPoints)
//{
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"123.123"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"123xp"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"123.xp"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"0x7B.123"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.291) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"0x7B.7B"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"-0x7B.7B"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(-123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"0b1111011.1111011"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"-0b1111011.1111011"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(-123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"0o173.173"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"-0o173.173"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(-123.123) // Expected
//		);
//	// Feature... not a bug... suuuuuuuuure... //
//	// TODO: FIX THIS!!!
//	//strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//	//		runtime_parse<FixedString{"0b1111011.123"}, NumericTypeTag::FixedPoint>(), // Actual
//	//		static_cast<FixedType>(123.18) // Expected
//	//	);
//	//std::cout << "HERE\n";
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"0x7B.123xp"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.291) // Expected
//		);
//	// TODO: Mix basis //
//	//strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//	//		runtime_parse<FixedString{"0x7B.0o173"}, NumericTypeTag::FixedPoint>(), // Actual
//	//		static_cast<FixedType>(123.123) // Expected
//	//	);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"123.123xp"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(123.123) // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::FixedPointType>(
//			runtime_parse<FixedString{"-123.123"}, NumericTypeTag::FixedPoint>(), // Actual
//			static_cast<FixedType>(-123.123) // Expected
//		);
//};
//
//TEST(NumericLiterals, ParseCharacters)
//{
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'q'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('q') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'a'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('a') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'q'c"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('q') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"65c"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('A') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'1'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('1') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'2'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('2') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'3'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('3') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'\\n'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('\n') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'\\0'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('\0') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"'\\\\'"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>('\\') // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::CharacterType>(
//			parse<FixedString{"' '"}, NumericTypeTag::Character>(), // Actual
//			static_cast<CharType>(' ') // Expected
//		);
//};
//
//TEST(NumericLiterals, Booleans)
//{
//	strict_check_parse<NumericLiteralParserTestType::BoolType>(
//			parse<FixedString{"true"}, NumericTypeTag::Bool>(), // Actual
//			true // Expected
//		);
//	strict_check_parse<NumericLiteralParserTestType::BoolType>(
//			parse<FixedString{"false"}, NumericTypeTag::Bool>(), // Actual
//			false // Expected
//		);
//};
//
