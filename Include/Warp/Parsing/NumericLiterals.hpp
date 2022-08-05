#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP
#define WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

namespace Warp::Parsing
{
	using namespace Warp::Utilities;

	enum class NumericLiteral
	{
		Digits, 
		Base10Digits, 
		Base16Digits, 
		Base2Digits, 
		Base8Digits, 
		Whole, 
		Integer, 
		FixedPoint, 
		Character, 
		CharacterLiteral, 
		EscapeCharacterLiteral, 
		Minus, 
		Dot, 
		IntegerMark, 
		UnsignedMark, 
		FixedMark, 
		AnyDecimalDigits, 
		AnyDecimalDigitsReduction, 
		CharacterMark 
	};

	//template<typename NumericParameterType>
	//struct NumericType
	//{
	//	using UnderylingType = NumericType;
	//	constexpr static const bool fixed_point_type = false;
	//	const UnderylingType number;
	//	const size_t bits = sizeof(UnderylingType) * 8;//* CHAR_BIT;
	//};

	//template<
	//		size_t WholePartBitsParameterConstant, 
	//		size_t DecimalPartBitsParameterConstant
	//	>
	//struct NumericType<numeric::fixed<
	//		WholePartBitsParameterConstant, 
	//		DecimalPartBitsParameterConstant
	//	>>
	//{
	//	constexpr const static auto whole_part_bits = WholePartBitsParameterConstant;
	//	constexpr const static auto decimal_part_bits = DecimalPartBitsParameterConstant;
	//	using UnderylingType = numeric::fixed<whole_part_bits, decimal_part_bits>;
	//	constexpr static const bool fixed_point_type = true;
	//	const UnderylingType number;
	//	const size_t bits = whole_part_bits + decimal_part_bits;
	//};
	
	struct Digits
	{
		const std::string digits;
		const size_t base;
		constexpr Digits(std::string digits, size_t base) noexcept : digits(digits), base(base) {}
		constexpr Digits(std::string_view digits) noexcept : digits(digits), base(base) {}
		constexpr operator std::string_view() const {
			return digits;
		}
		constexpr operator std::string() const {
			return digits;
		}
	};

	template<auto NumericalTypeTag>
	struct NumericLiteralTypeResolver {};
	
	template<>
	struct NumericLiteralTypeResolver<NumericLiteral::Whole> {
		using Type = size_t;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericLiteral::Integer> {
		using Type = long long signed int;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericLiteral::FixedPoint> {
		using Type = numeric::fixed<16, 16>;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericLiteral::Character> {
		using Type = char;
	};

	using NumericLiteralTermsType = MakeTerms<
			TreeTerm<
					NumericLiteral::Minus, 
					CharTerm, 
					'-', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::Dot, 
					CharTerm, 
					'.', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::IntegerMark, 
					CharTerm, 
					'i', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::UnsignedMark, 
					CharTerm, 
					'u', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::CharacterMark, 
					CharTerm, 
					'c', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::FixedMark, 
					StringTerm, 
					FixedString{"xp"}, 
					ctpg::associativity::no_assoc
				>, 
			TypeTreeTerm<
					NumericLiteral::Digits, 
					NonTerminalTerm, 
					std::string, 
					FixedString{"Digits"}
				>, 
			TypeTreeTerm<
					NumericLiteral::FixedPoint, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericLiteral::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericLiteral::Whole, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericLiteral::Whole>::Type, 
					FixedString{"Whole"}
				>, 
			TypeTreeTerm<
					NumericLiteral::Integer, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericLiteral::Integer>::Type, 
					FixedString{"Integer"}
				>, 
			TypeTreeTerm<
					NumericLiteral::FixedPoint, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericLiteral::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericLiteral::Character, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericLiteral::Character>::Type, 
					FixedString{"Character"}
				>, 
			TypeTreeTerm<
					NumericLiteral::AnyDecimalDigitsReduction, 
					NonTerminalTerm, 
					std::string, 
					FixedString{"AnyDecimalDigitsReduction"}
				>, 
			TreeTerm<
					NumericLiteral::Base10Digits, 
					RegexTerm, 
					FixedString{"([0-9]+)|(0d[0-9]+)"}, 
					FixedString{"Base10Digits"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::Base2Digits, 
					RegexTerm, 
					FixedString{"0b[0-1]+"}, 
					FixedString{"Base2Digits"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::Base16Digits, 
					RegexTerm, 
					FixedString{"0x[0-9A-Fa-f]+"}, 
					FixedString{"Base16Digits"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::Base8Digits, 
					RegexTerm, 
					FixedString{"0o[0-7]+"}, 
					FixedString{"Base8Digits"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::CharacterLiteral, 
					RegexTerm, 
					FixedString{"'[a-zA-Z~`!@#$%^&*()-=_+<>,\\.\"/?;:\\s|{}]'"}, 
					FixedString{"CharacterLiteral"}, 
					ctpg::associativity::no_assoc
				>,
			TreeTerm<
					NumericLiteral::EscapeCharacterLiteral, 
					RegexTerm, 
					FixedString{"'\\\\[0nt'\\\\]'"}, 
					FixedString{"CharacterLiteral"}, 
					ctpg::associativity::no_assoc
				>
			>::AddOnePriority<
					TreeTerm<
							NumericLiteral::AnyDecimalDigits, 
							RegexTerm, 
							FixedString{"\\.[0-9A-Fa-f]+"}, 
							FixedString{"AnyDecimalDigits"}, 
							ctpg::associativity::no_assoc
						>
					>;

	template<
			typename TermsParameterTemplate, 
			template<auto> typename ResolverParameterTemplate
		>
	struct NumericLiteralParser
	{
		using WholeType = ResolverParameterTemplate<NumericLiteral::Whole>::Type;
		using IntegerType = ResolverParameterTemplate<NumericLiteral::Integer>::Type;
		using FixedPointType = ResolverParameterTemplate<NumericLiteral::FixedPoint>::Type;
		using CharacterType = ResolverParameterTemplate<NumericLiteral::Character>::Type;

		constexpr const static auto character_literal
				= TermsParameterTemplate::template term<NumericLiteral::CharacterLiteral>;
		constexpr const static auto escape_character_literal
				= TermsParameterTemplate::template term<NumericLiteral::EscapeCharacterLiteral>;
		constexpr const static auto base_10_digits 
				= TermsParameterTemplate::template term<NumericLiteral::Base10Digits>;
		constexpr const static auto base_16_digits 
				= TermsParameterTemplate::template term<NumericLiteral::Base16Digits>;
		constexpr const static auto base_8_digits 
				= TermsParameterTemplate::template term<NumericLiteral::Base8Digits>;
		constexpr const static auto base_2_digits 
				= TermsParameterTemplate::template term<NumericLiteral::Base2Digits>;
		constexpr const static auto any_decimal_digits 
				= TermsParameterTemplate::template term<NumericLiteral::AnyDecimalDigits>;
		constexpr const static auto any_decimal_digits_reduction 
				= TermsParameterTemplate::template term<NumericLiteral::AnyDecimalDigitsReduction>;
		constexpr const static auto digits 
				= TermsParameterTemplate::template term<NumericLiteral::Digits>;
		constexpr const static auto radix
				= TermsParameterTemplate::template term<NumericLiteral::Dot>;
		constexpr const static auto minus
				= TermsParameterTemplate::template term<NumericLiteral::Minus>;
		constexpr const static auto unsigned_mark
				= TermsParameterTemplate::template term<NumericLiteral::UnsignedMark>;
		constexpr const static auto integer_mark
				= TermsParameterTemplate::template term<NumericLiteral::IntegerMark>;
		constexpr const static auto fixed_point_mark
				= TermsParameterTemplate::template term<NumericLiteral::FixedMark>;
		constexpr const static auto character_mark
				= TermsParameterTemplate::template term<NumericLiteral::CharacterMark>;
		constexpr const static auto whole 
				= TermsParameterTemplate::template term<NumericLiteral::Whole>;
		constexpr const static auto integer
				= TermsParameterTemplate::template term<NumericLiteral::Integer>;
		constexpr const static auto fixed_point
				= TermsParameterTemplate::template term<NumericLiteral::FixedPoint>;
		constexpr const static auto character
				= TermsParameterTemplate::template term<NumericLiteral::Character>;

		constexpr static const auto terms = ctpg::terms(
				character_literal, 
				escape_character_literal, 
				base_10_digits, 
				base_16_digits, 
				base_8_digits, 
				base_2_digits, 
				any_decimal_digits, 
				radix, 
				minus, 
				unsigned_mark, 
				integer_mark, 
				fixed_point_mark, 
				character_mark
			);

		constexpr static const auto non_terminal_terms = ctpg::nterms(
				digits, 
				whole, 
				integer, 
				fixed_point, 
				character, 
				any_decimal_digits_reduction
			);

		template<std::unsigned_integral auto BaseParameterConstant>
		constexpr static const FixedPointType make_fixed_point_from_base(
				std::string_view major, 
				std::string_view minor
			)
		{
			if constexpr(BaseParameterConstant == 16)
			{
				return to_fixed_point_integral<WholeType, FixedPointType>(
						base_16_to_integral<WholeType>(major), 
						base_16_to_integral<WholeType>(minor.substr(1))
					);
			}
			else
			{
				return to_fixed_point_integral<WholeType, FixedPointType>(
						to_integral<WholeType, BaseParameterConstant>(major), 
						to_integral<WholeType, BaseParameterConstant>(minor.substr(1))
					);
			}
		}

		constexpr const static auto parse_base_10_digits
				= digits(base_10_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					if(digit_string_view.size() > 2) {
						if(digit_string_view.substr(0, 2) == "0d")
							return digit_string_view.substr(2);
					}
					return std::string_view{digit_string};
				};
		constexpr const static auto parse_base_2_digits
				= digits(base_2_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					return integral_to_string(
							to_integral<WholeType, 2>(digit_string_view.substr(2))
						);
				};
		constexpr const static auto parse_base_8_digits
				= digits(base_8_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					return integral_to_string(
							to_integral<WholeType, 8>(digit_string_view.substr(2))
						);
				};
		constexpr const static auto parse_base_16_digits
				= digits(base_16_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					return integral_to_string(
							base_16_to_integral<WholeType>(digit_string_view.substr(2))
						);
				};
		constexpr const static auto parse_whole 
				= whole(digits) >= [](auto digit_string) {
					return to_integral<WholeType>(digit_string);
				};
		constexpr const static auto parse_explicit_whole 
				= whole(digits, unsigned_mark) 
				>= [](auto digit_string, auto unsigned_mark) {
					return to_integral<WholeType>(digit_string);
				};
		constexpr const static auto parse_integer
				= integer(digits, integer_mark) 
				>= [](auto digits_string, auto integer_mark_character) {
					return to_integral<IntegerType>(digits_string);
				};
		constexpr const static auto parse_negative_whole
				= integer(minus, digits) >= [](auto minus, auto digits_string) {
					return -to_integral<IntegerType>(digits_string);
				};
		constexpr const static auto parse_negate_integer
				= integer(minus, integer) >= [](auto minus, auto integer_value) {
					return -integer_value;
				};
		constexpr const static auto parse_fixed_point_explicit
				= fixed_point(digits, fixed_point_mark) 
				>= [](auto digits_string, auto fixed_point_mark_string) {
					return FixedPointType{to_integral<WholeType>(digits_string)};
				};
		constexpr const static auto parse_fixed_point_radix_explicit
				= fixed_point(digits, radix, fixed_point_mark) 
				>= [](auto digits_string, auto radix_character, auto fixed_point_mark_string) {
					return FixedPointType{to_integral<WholeType>(digits_string)};
				};
		constexpr const static auto parse_fixed_point
				= fixed_point(digits, radix, digits) 
				>= [](auto major, auto radix, auto minor) {
					return to_fixed_point_integral<WholeType, FixedPointType>(
							to_integral<WholeType>(major), 
							to_integral<WholeType>(minor)
						);
				};
		constexpr const static auto parse_any_decimal_digits_reduction
				= any_decimal_digits_reduction(any_decimal_digits)
				>= [](auto any_decimal_digits_string) {
					return std::string{any_decimal_digits_string};
				};
		constexpr const static auto parse_explicit_any_decimal_digits_reduction
				= any_decimal_digits_reduction(any_decimal_digits, fixed_point_mark)
				>= [](auto any_decimal_digits_string, auto fixed_point_mark_string) {
					return std::string{any_decimal_digits_string};
				};
		constexpr const static auto parse_base_10_fixed_point
				= fixed_point(base_10_digits, any_decimal_digits_reduction) 
				>= [](auto major, auto minor)
				{
					return make_fixed_point_from_base<10u>(
							to_string_view(major), 
							to_string_view(minor)
						);
				};
		constexpr const static auto parse_base_2_fixed_point
				= fixed_point(base_2_digits, any_decimal_digits_reduction) 
				>= [](auto major, auto minor)
				{
					return make_fixed_point_from_base<2u>(
							to_string_view(major), 
							to_string_view(minor)
						);
				};
		constexpr const static auto parse_base_8_fixed_point
				= fixed_point(base_8_digits, any_decimal_digits_reduction) 
				>= [](auto major, auto minor)
				{
					return make_fixed_point_from_base<8u>(
							to_string_view(major), 
							to_string_view(minor)
						);
				};
		constexpr const static auto parse_base_16_fixed_point
				= fixed_point(base_16_digits, any_decimal_digits_reduction) 
				>= [](auto major, auto minor)
				{
					return make_fixed_point_from_base<16u>(
							to_string_view(major), 
							to_string_view(minor)
						);
				};
		constexpr const static auto parse_redundent_fixed_point
				= fixed_point(fixed_point, fixed_point_mark) 
				>= [](auto fixed_point_value, auto mark) {
					return fixed_point_value;
				};
		constexpr const static auto parse_negate_fixed_point
				= fixed_point(minus, fixed_point) 
				>= [](auto negate, auto fixed_point_value) {
					return -fixed_point_value;
				};
		constexpr const static auto parse_character
				= character(character_literal) 
				>= [](auto character_literal_string) {
					return std::string_view{character_literal_string}[1];
				};
		constexpr const static auto parse_marked_character
				= character(character_literal, character_mark) 
				>= [](auto character_literal_string, auto character_mark) {
					return std::string_view{character_literal_string}[1];
				};
		constexpr const static auto parse_escape_character
				= character(escape_character_literal) 
				>= [](auto escape_character_string) {
					const char character = std::string_view{escape_character_string}[2];
					switch(character)
					{
						case 'n':
							return '\n';
						case 't': 
							return '\t';
						case '\\': 
							return '\\';
						case '\'': 
							return '\'';
						default:
							return '\0';
					}
				};
		constexpr const static auto parse_marked_character_number
				= character(digits, character_mark) 
				>= [](auto character_number, auto character_mark) {
					return to_integral<CharacterType>(character_number); 
			};

		consteval static const auto rules()
		{
			return ctpg::rules(
					parse_base_10_digits, 
					parse_base_2_digits, 
					parse_base_8_digits, 
					parse_base_16_digits, 
					parse_whole, 
					parse_explicit_whole, 
					parse_integer, 
					parse_negative_whole, 
					parse_negate_integer, 
					parse_fixed_point, 
					parse_any_decimal_digits_reduction, 
					parse_explicit_any_decimal_digits_reduction, 
					parse_fixed_point_explicit, 
					parse_fixed_point_radix_explicit, 
					parse_base_16_fixed_point, 
					parse_base_10_fixed_point, 
					parse_base_8_fixed_point, 
					parse_base_2_fixed_point, 
					parse_redundent_fixed_point, 
					parse_negate_fixed_point, 
					parse_character, 
					parse_escape_character, 
					parse_marked_character_number, 
					parse_marked_character
				);
		}
	};
}

#endif // WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

