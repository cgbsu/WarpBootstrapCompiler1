#include <Warp/Runtime/Compiler/NumericType.hpp>
#include <Warp/Parsing/NumericLiterals.hpp>
#include <ThirdParty/fpm/ios.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <CppUTest/TestHarness.h>

#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>

using namespace Warp::Testing;
using namespace Warp::Parsing;
using namespace Warp::Utilities;

TEST_GROUP(NumericLiterals) {};

using ParserTestType = NumericLiteralParser<
		NumericLiteralTermsType, 
		NumericTypeResolver
	>;

using FixedType = ParserTestType::FixedPointType;
using CharType = ParserTestType::CharacterType;
using BoolType = ParserTestType::BoolType;

TEST(NumericLiterals, ParseWholeAnyBase)
{
	strict_check_parse<ParserTestType::WholeType>(
			parse<ParserTestType, FixedString{"0d123"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<ParserTestType::WholeType>(
			parse<ParserTestType, FixedString{"123"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<ParserTestType::WholeType>(
			parse<ParserTestType, FixedString{"123u"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<ParserTestType::WholeType>(
			parse<ParserTestType, FixedString{"0b1111011"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<ParserTestType::WholeType>(
			parse<ParserTestType, FixedString{"0o173"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
	strict_check_parse<ParserTestType::WholeType>(
			parse<ParserTestType, FixedString{"0x7B"}, NumericTypeTag::Whole>(), // Actual
			123 // Expected
		);
};

TEST(NumericLiterals, ParseIntegers)
{
	// Functionality moved. //
	//strict_check_parse<ParserTestType::IntegerType>(
	//		parse<ParserTestType, FixedString{"-0b1111011"}, NumericTypeTag::Integer>(), // Actual
	//		-123 // Expected
	//	);
	//strict_check_parse<ParserTestType::IntegerType>(
	//		parse<ParserTestType, FixedString{"-0b1111011"}, NumericTypeTag::Integer>(), // Actual
	//		-123 // Expected
	//	);
	//strict_check_parse<ParserTestType::IntegerType>(
	//		parse<ParserTestType, FixedString{"-0o173"}, NumericTypeTag::Integer>(), // Actual
	//		-123 // Expected
	//	);
	//strict_check_parse<ParserTestType::IntegerType>(
	//		parse<ParserTestType, FixedString{"-123"}, NumericTypeTag::Integer>(), // Actual
	//		-123 // Expected
	//	);
	//strict_check_parse<ParserTestType::IntegerType>(
	//		parse<ParserTestType, FixedString{"-123i"}, NumericTypeTag::Integer>(), // Actual
	//		-123 // Expected
	//	);
	strict_check_parse<ParserTestType::IntegerType>(
			parse<ParserTestType, FixedString{"123i"}, NumericTypeTag::Integer>(), // Actual
			123 // Expected
		);
};

TEST(NumericLiterals, ParseFixedPoints)
{
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"123.123"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.123) // Expected
		);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"123xp"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123) // Expected
		);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"123.xp"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123) // Expected
		);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"0x7B.123"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.291) // Expected
		);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"0x7B.7B"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.123) // Expected
		);
	//strict_check_parse<ParserTestType::FixedPointType>(
	//		runtime_parse<ParserTestType, FixedString{"-0x7B.7B"}, NumericTypeTag::FixedPoint>(), // Actual
	//		static_cast<FixedType>(-123.123) // Expected
	//	);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"0b1111011.1111011"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.123) // Expected
		);
	//strict_check_parse<ParserTestType::FixedPointType>(
	//		runtime_parse<ParserTestType, FixedString{"-0b1111011.1111011"}, NumericTypeTag::FixedPoint>(), // Actual
	//		static_cast<FixedType>(-123.123) // Expected
	//	);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"0o173.173"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.123) // Expected
		);
	//strict_check_parse<ParserTestType::FixedPointType>(
	//		runtime_parse<ParserTestType, FixedString{"-0o173.173"}, NumericTypeTag::FixedPoint>(), // Actual
	//		static_cast<FixedType>(-123.123) // Expected
	//	);
	// Feature... not a bug... suuuuuuuuure... //
	// TODO: FIX THIS!!!
	//strict_check_parse<ParserTestType::FixedPointType>(
	//		runtime_parse<ParserTestType, FixedString{"0b1111011.123"}, NumericTypeTag::FixedPoint>(), // Actual
	//		static_cast<FixedType>(123.18) // Expected
	//	);
	//std::cout << "HERE\n";
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"0x7B.123xp"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.291) // Expected
		);
	// TODO: Mix basis //
	//strict_check_parse<ParserTestType::FixedPointType>(
	//		runtime_parse<ParserTestType, FixedString{"0x7B.0o173"}, NumericTypeTag::FixedPoint>(), // Actual
	//		static_cast<FixedType>(123.123) // Expected
	//	);
	strict_check_parse<ParserTestType::FixedPointType>(
			runtime_parse<ParserTestType, FixedString{"123.123xp"}, NumericTypeTag::FixedPoint>(), // Actual
			static_cast<FixedType>(123.123) // Expected
		);
	//strict_check_parse<ParserTestType::FixedPointType>(
	//		runtime_parse<ParserTestType, FixedString{"-123.123"}, NumericTypeTag::FixedPoint>(), // Actual
	//		static_cast<FixedType>(-123.123) // Expected
	//	);
};

TEST(NumericLiterals, ParseCharacters)
{
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'q'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('q') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'a'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('a') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'q'c"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('q') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"65c"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('A') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"0x41_c"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('A') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'1'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('1') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'2'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('2') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'3'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('3') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'\\n'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('\n') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'\\0'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('\0') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'\\\\'"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('\\') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"' '"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>(' ') // Expected
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"'\\0'c"}, NumericTypeTag::Character>(), // Actual
			static_cast<CharType>('\0') // Expected
		);
};

TEST(NumericLiterals, Booleans)
{
	strict_check_parse<ParserTestType::BoolType>(
			parse<ParserTestType, FixedString{"true"}, NumericTypeTag::Bool>(), // Actual
			WarpBool::True // Expected
		);
	strict_check_parse<ParserTestType::BoolType>(
			parse<ParserTestType, FixedString{"false"}, NumericTypeTag::Bool>(), // Actual
			WarpBool::False // Expected
		);
	strict_check_parse<ParserTestType::BoolType>(
			parse<ParserTestType, FixedString{"truebl"}, NumericTypeTag::Bool>(), // Actual
			WarpBool::True // Expected
		);
	strict_check_parse<ParserTestType::BoolType>(
			parse<ParserTestType, FixedString{"falsebl"}, NumericTypeTag::Bool>(), // Actual
			WarpBool::False // Expected
		);
	strict_check_parse<ParserTestType::BoolType>(
			parse<ParserTestType, FixedString{"0b1bl"}, NumericTypeTag::Bool>(), // Actual
			WarpBool::True // Expected
		);
	strict_check_parse<ParserTestType::BoolType>(
			parse<ParserTestType, FixedString{"0b0bl"}, NumericTypeTag::Bool>(), // Actual
			WarpBool::False // Expected
		);
};

template<typename ResultParameterType, auto ValueParameterConstant>
constexpr void check_varible_bit_against_value(const auto& to_check, const auto& bitness_values)
{
	for(size_t ii = 0; ii < to_check.size(); ++ii) {
		strict_check_parse<ResultParameterType>(to_check[ii], ValueParameterConstant);
		CHECK((to_check[ii].value().bits == bitness_values[ii % bitness_values.size()]));
	}
}

TEST(NumericLiterals, WholeVariableBitArity)
{
	auto whole = parse<ParserTestType, FixedString{"0d123"}, NumericTypeTag::Whole>().value();
	whole.bits = 8;
	CHECK((whole == 123u));
	auto whole2 = parse<ParserTestType, FixedString{"0d123"}, NumericTypeTag::Whole>().value();
	CHECK((whole == whole2));
	check_parse(
			parse<ParserTestType, FixedString{"0d123u8"}, NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123"}, NumericTypeTag::Whole>()
		);
	check_parse(
			parse<ParserTestType, FixedString{"0d123u8"}, NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123u16"}, NumericTypeTag::Whole>()
		);
	check_parse(
			parse<ParserTestType, FixedString{"0d123u16"}, NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123u8"}, NumericTypeTag::Whole>()
		);
	check_parse(
			parse<ParserTestType, FixedString{"0d123u27"}, NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123u12"}, NumericTypeTag::Whole>()
		);

	constexpr const auto to_check = std::array{
			parse<ParserTestType, FixedString{"0d123u8"},      NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123u16"},     NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123u27"},     NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0d123u35"},     NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0x7Bu8"},       NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0x7Bu16"},      NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0x7Bu27"},      NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0x7Bu35"},      NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0o173u8"},      NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0o173u16"},     NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0o173u27"},     NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0o173u35"},     NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0b1111011u8"},  NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0b1111011u16"}, NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0b1111011u27"}, NumericTypeTag::Whole>(), 
			parse<ParserTestType, FixedString{"0b1111011u35"}, NumericTypeTag::Whole>()
		};

	constexpr const auto bit_values = std::array{8, 16, 27, 35};

	check_varible_bit_against_value<ParserTestType::WholeType, 123>(to_check, bit_values);
};


TEST(NumericLiterals, IntegerVariableBitArity)
{
	auto integer = parse<ParserTestType, FixedString{"0d123i"}, NumericTypeTag::Integer>().value();
	integer.bits = 8;
	CHECK((integer == 123));
	auto integer2 = parse<ParserTestType, FixedString{"0d123i"}, NumericTypeTag::Integer>().value();
	CHECK((integer == integer2));
	check_parse(
			parse<ParserTestType, FixedString{"0d123i8"}, NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i"}, NumericTypeTag::Integer>()
		);
	check_parse(
			parse<ParserTestType, FixedString{"0d123i8"}, NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i16"}, NumericTypeTag::Integer>()
		);
	check_parse(
			parse<ParserTestType, FixedString{"0d123i16"}, NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i8"}, NumericTypeTag::Integer>()
		);
	check_parse(
			parse<ParserTestType, FixedString{"0d123i27"}, NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i12"}, NumericTypeTag::Integer>()
		);

	constexpr const auto to_check = std::array{
			parse<ParserTestType, FixedString{"0d123i8"},      NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i16"},     NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i27"},     NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0d123i35"},     NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0x7Bi8"},       NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0x7Bi16"},      NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0x7Bi27"},      NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0x7Bi35"},      NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0o173i8"},      NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0o173i16"},     NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0o173i27"},     NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0o173i35"},     NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0b1111011i8"},  NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0b1111011i16"}, NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0b1111011i27"}, NumericTypeTag::Integer>(), 
			parse<ParserTestType, FixedString{"0b1111011i35"}, NumericTypeTag::Integer>()
		};

	constexpr const auto bit_values = std::array{8, 16, 27, 35};

	check_varible_bit_against_value<ParserTestType::IntegerType, 123>(to_check, bit_values);
};

TEST(NumericLiterals, CharacterVariableBitArity)
{
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"0d123c8"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c"}, NumericTypeTag::Character>().value()
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"0d123c8"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c16"}, NumericTypeTag::Character>().value()
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"0d123c37"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c16"}, NumericTypeTag::Character>().value()
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"0d123c37"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c21"}, NumericTypeTag::Character>().value()
		);
	strict_check_parse<ParserTestType::CharacterType>(
			parse<ParserTestType, FixedString{"0d123c8"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c21"}, NumericTypeTag::Character>().value()
		);

	const auto to_check = std::array{
			parse<ParserTestType, FixedString{"0d123c8"},      NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c16"},     NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c27"},     NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0d123c35"},     NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0x7B_c8"},       NumericTypeTag::Character>(), /*This test prompted the introduction of the numerical * 
																					   * delinator '_' only supported here so far, see *******
																					   * NumericLiterals.hpp for details ********************/
			parse<ParserTestType, FixedString{"0x7B_c16"},      NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0x7B_c27"},      NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0x7B_c35"},      NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0o173c8"},      NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0o173c16"},     NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0o173c27"},     NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0o173c35"},     NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0b1111011c8"},  NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0b1111011c16"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0b1111011c27"}, NumericTypeTag::Character>(), 
			parse<ParserTestType, FixedString{"0b1111011c35"}, NumericTypeTag::Character>()
		};

	constexpr const auto bit_values = std::array{8, 16, 27, 35};

	check_varible_bit_against_value<ParserTestType::CharacterType, 123>(to_check, bit_values);
};

