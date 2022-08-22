#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Parsing/TypeTerms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP
#define WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

namespace Warp::Parsing
{
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	enum class NumericLiteral
	{
		Digits, 
		Base10Digits, 
		Base16Digits, 
		Base2Digits, 
		Base8Digits, 
		CharacterLiteral, 
		EscapeCharacterLiteral, 
		Minus, 
		Dot, 
		IntegerMark, 
		UnsignedMark, 
		FixedMark, 
		BoolMark, 
		AnyDecimalDigits, 
		AnyDecimalDigitsReduction, 
		CharacterMark, 
		BooleanLiteral, 
		NumericalDelinator /* TODO: Add more support for this 100_000, and 0x_7D or 0x8A.0o_71 or 0x3B.1F_fxp, 
							* support currently limited to integral char. */
	};
	

	using NumericLiteralTermsType = TypeTerms::FlatMerge<MakeTerms<
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
			TreeTerm<
					NumericLiteral::BoolMark, 
					StringTerm, 
					FixedString{"bl"}, 
					ctpg::associativity::no_assoc
				>, 
			TypeTreeTerm<
					NumericLiteral::Digits, 
					NonTerminalTerm, 
					std::string, 
					FixedString{"Digits"}
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
					FixedString{"('[a-zA-Z~`!@#$%^&*()-=_+<>,\\.\"/?;:|{}]')|(' ')"}, //(' ') is a quick fix for now.
					FixedString{"CharacterLiteral"}, 
					ctpg::associativity::no_assoc
				>,
			TreeTerm<
					NumericLiteral::BooleanLiteral, 
					RegexTerm, 
					FixedString{"(true)|(false)"}, 
					FixedString{"BooleanLiteral"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::EscapeCharacterLiteral, 
					RegexTerm, 
					FixedString{"'\\\\[0nt'\\\\]'"}, 
					FixedString{"CharacterLiteral"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::NumericalDelinator, 
					CharTerm, 
					'_', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					NumericLiteral::AnyDecimalDigits, 
					RegexTerm, 
					FixedString{"\\.[0-9A-Fa-f]+"}, 
					FixedString{"AnyDecimalDigits"}, 
					ctpg::associativity::no_assoc
				>
			>>;

	template<typename TermsParameterType = NumericLiteralTermsType, 
			template<auto> typename TypeResolverParameterTemplate = NumericTypeResolver>
	struct DigitParser
	{
		using TermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = typename TypeResolverParameterTemplate<NonTerminalTypeTagParameterConstant>::Type;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		using WholeType = TypeResolverTemplate<NumericTypeTag::Whole>;
		
		constexpr const static auto base_10_digits = term<NumericLiteral::Base10Digits>;
		constexpr const static auto base_16_digits = term<NumericLiteral::Base16Digits>;
		constexpr const static auto base_8_digits = term<NumericLiteral::Base8Digits>;
		constexpr const static auto base_2_digits = term<NumericLiteral::Base2Digits>;
		constexpr const static auto any_decimal_digits = term<NumericLiteral::AnyDecimalDigits>;
		constexpr const static auto any_decimal_digits_reduction = term<NumericLiteral::AnyDecimalDigitsReduction>;
		constexpr const static auto digits = term<NumericLiteral::Digits>;
		constexpr const static auto fixed_point_mark = term<NumericLiteral::FixedMark>;

		constexpr static const auto terms = ctpg::terms(
				base_10_digits, 
				base_16_digits, 
				base_8_digits, 
				base_2_digits, 
				any_decimal_digits, 
				fixed_point_mark
			);
		constexpr static const auto non_terminal_terms = ctpg::nterms(
				digits, 
				any_decimal_digits_reduction
			);

		constexpr const static auto parse_base_10_digits
				= digits(base_10_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					if(digit_string_view.size() > 2) {
						if(digit_string_view.substr(0, 2) == "0d") // TODO: Derive offsets and string constants "0d" from terms //
							return digit_string_view.substr(2);
					}
					return std::string_view{digit_string};
				};
		constexpr const static auto parse_base_2_digits
				= digits(base_2_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					return integral_to_string(
							to_integral<typename WholeType::UnderylingType, 2>(digit_string_view.substr(2))
						);
				};
		constexpr const static auto parse_base_8_digits
				= digits(base_8_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					return integral_to_string(
							to_integral<typename WholeType::UnderylingType, 8>(digit_string_view.substr(2))
						);
				};
		constexpr const static auto parse_base_16_digits
				= digits(base_16_digits) >= [](auto digit_string)
				{
					const std::string_view digit_string_view = digit_string;
					return integral_to_string(
							base_16_to_integral<typename WholeType::UnderylingType>(digit_string_view.substr(2))
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

		static consteval const auto rules()
		{
			return ctpg::rules(
					parse_base_10_digits, 
					parse_base_2_digits, 
					parse_base_8_digits, 
					parse_base_16_digits, 
					parse_any_decimal_digits_reduction, 
					parse_explicit_any_decimal_digits_reduction
				);
		}
	};

	//template<size_t LeftIndexParameterConstant, size_t... RightIndexParameterConstants>
	//constexpr static const auto merge_tuples_detect_duplicate_helper(
	//		const auto left_canidate, 
	//		const auto right_tuple, 
	//		std::index_sequence<RightIndexParameterConstants...>
	//	)
	//{
	//	LeftIndexParameterConstant
	//}

	template<
			typename TermsParameterType = NumericLiteralTermsType, 
			template<auto> typename TypeResolverParameterTemplate = NumericTypeResolver
		>
	struct NumericLiteralParser 
			: public DigitParser<TermsParameterType, TypeResolverParameterTemplate>
	{
		using TermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<NonTerminalTypeTagParameterConstant>::Type;

		using BaseType = DigitParser<TermsType, TypeResolverParameterTemplate>;

		using WholeType = TypeResolverTemplate<NumericTypeTag::Whole>;
		using IntegerType = TypeResolverTemplate<NumericTypeTag::Integer>;
		using FixedPointType = TypeResolverTemplate<NumericTypeTag::FixedPoint>;
		using CharacterType = TypeResolverTemplate<NumericTypeTag::Character>;
		using BoolType = TypeResolverTemplate<NumericTypeTag::Bool>;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr const static auto base_10_digits = term<NumericLiteral::Base10Digits>;
		constexpr const static auto base_16_digits = term<NumericLiteral::Base16Digits>;
		constexpr const static auto base_8_digits = term<NumericLiteral::Base8Digits>;
		constexpr const static auto base_2_digits = term<NumericLiteral::Base2Digits>;
		constexpr const static auto any_decimal_digits = term<NumericLiteral::AnyDecimalDigits>;
		constexpr const static auto any_decimal_digits_reduction = term<NumericLiteral::AnyDecimalDigitsReduction>;
		constexpr const static auto digits = term<NumericLiteral::Digits>;
		constexpr const static auto fixed_point_mark = term<NumericLiteral::FixedMark>;
		constexpr const static auto character_literal = term<NumericLiteral::CharacterLiteral>;
		constexpr const static auto escape_character_literal = term<NumericLiteral::EscapeCharacterLiteral>;
		constexpr const static auto boolean_literal = term<NumericLiteral::BooleanLiteral>;
		constexpr const static auto radix = term<NumericLiteral::Dot>;
		constexpr const static auto minus = term<NumericLiteral::Minus>;
		constexpr const static auto unsigned_mark = term<NumericLiteral::UnsignedMark>;
		constexpr const static auto integer_mark = term<NumericLiteral::IntegerMark>;
		constexpr const static auto bool_mark = term<NumericLiteral::BoolMark>;
		constexpr const static auto numerical_delinator = term<NumericLiteral::NumericalDelinator>;
		constexpr const static auto character_mark = term<NumericLiteral::CharacterMark>;
		constexpr const static auto whole = term<NumericTypeTag::Whole>;
		constexpr const static auto integer = term<NumericTypeTag::Integer>;
		constexpr const static auto fixed_point = term<NumericTypeTag::FixedPoint>;
		constexpr const static auto character = term<NumericTypeTag::Character>;
		constexpr const static auto boolean = term<NumericTypeTag::Bool>;

		//constexpr static const auto terms = TermsType::template create_terms<false>();
		constexpr static const auto terms //= TermsType::template create_terms<false>();
				= std::tuple_cat(BaseType::terms, ctpg::terms(
						character_literal, 
						escape_character_literal, 
						boolean_literal, 
						radix, 
						minus, 
						unsigned_mark, 
						integer_mark, 
						fixed_point_mark, 
						character_mark, 
						numerical_delinator, 
						bool_mark
					));

		//constexpr static const auto non_terminal_terms = TermsType::template create_terms<true>();
		constexpr static const auto non_terminal_terms 
				= std::tuple_cat(BaseType::non_terminal_terms, ctpg::nterms(
						whole, 
						integer, 
						fixed_point, 
						character, 
						any_decimal_digits_reduction, 
						boolean
					));

		template<std::unsigned_integral auto BaseParameterConstant>
		constexpr static const FixedPointType make_fixed_point_from_base(
				std::string_view major, 
				std::string_view minor
			)
		{
			if constexpr(BaseParameterConstant == 16)
			{
				return FixedPointType{
						base_16_to_integral<size_t>(major), 
						base_16_to_integral<size_t>(minor.substr(1))
					};
			}
			else
			{
				return FixedPointType{
						to_integral<size_t, BaseParameterConstant>(major), 
						to_integral<size_t, BaseParameterConstant>(minor.substr(1))
					};
			}
		}
		constexpr static const CharacterType character_to_escape_character(char character)
		{
			switch(character)
			{
				case 'n':
					return CharacterType{'\n'};
				case 't': 
					return CharacterType{'\t'};
				case '\\': 
					return CharacterType{'\\'};
				case '\'': 
					return CharacterType{'\''};
				default:
					return CharacterType{'\0'};
			}
		}
		
		constexpr const static auto parse_whole 
				= whole(digits) >= [](auto digit_string) {
					return WholeType{digit_string};
				};
		constexpr const static auto parse_explicit_whole 
				= whole(digits, unsigned_mark) 
				>= [](auto digit_string, auto unsigned_mark) {
					return WholeType{digit_string};
				};
		constexpr const static auto parse_whole_with_bit_precision
				= whole(digits, unsigned_mark, digits) 
				>= [](auto digit_string, auto unsigned_mark, auto bit_precision_digits) {
					return WholeType{digit_string, to_integral<size_t>(std::string_view{bit_precision_digits})};
				};
		constexpr const static auto parse_integer
				= integer(digits, integer_mark) 
				>= [](auto digits_string, auto integer_mark_character) {
					return IntegerType{digits_string};
				};
		constexpr const static auto parse_integer_with_bit_precision
				= integer(digits, integer_mark, digits) 
				>= [](auto digits_string, auto integer_mark_character, auto bit_precision_digits) {
					return IntegerType{digits_string, to_integral<size_t>(std::string_view{bit_precision_digits})};
				};
		constexpr const static auto parse_negative_whole
				= integer(minus, digits) >= [](auto minus, auto digits_string) {
					return -IntegerType{digits_string};
				};
		constexpr const static auto parse_negate_integer
				= integer(minus, integer) >= [](auto minus, auto integer_value) {
					return -integer_value;
				};
		constexpr const static auto parse_fixed_point_explicit
				= fixed_point(digits, fixed_point_mark) 
				>= [](auto digits_string, auto fixed_point_mark_string) {
					return FixedPointType{digits_string};
				};
		constexpr const static auto parse_fixed_point_radix_explicit
				= fixed_point(digits, radix, fixed_point_mark) 
				>= [](auto digits_string, auto radix_character, auto fixed_point_mark_string) {
					return FixedPointType{digits_string};
				};
		constexpr const static auto parse_fixed_point
				= fixed_point(digits, radix, digits) 
				>= [](auto major, auto radix, auto minor) {
					return FixedPointType{major, minor};
				};
		// TODO: Possible representation change 
		// BASIS_MARK DIGITS RADIX DIGITS
		// and
		// BASIS_MARK DIGITS RADIX BASIS_MARK DIGITS
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
							to_string_view(major).substr(2), 
							to_string_view(minor)
						);
				};
		constexpr const static auto parse_base_8_fixed_point
				= fixed_point(base_8_digits, any_decimal_digits_reduction) 
				>= [](auto major, auto minor)
				{
					return make_fixed_point_from_base<8u>(
							to_string_view(major).substr(2), 
							to_string_view(minor)
						);
				};
		constexpr const static auto parse_base_16_fixed_point
				= fixed_point(base_16_digits, any_decimal_digits_reduction) 
				>= [](auto major, auto minor)
				{
					return make_fixed_point_from_base<16u>(
							to_string_view(major).substr(2), 
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
		constexpr const static auto parse_character_with_bit_precision
				= character(character_literal, character_mark, digits) 
				>= [](auto character_literal_string, auto character_mark, auto bit_precision_digits)
				{
					return CharacterType{
							std::string_view{character_literal_string}[1], 
							to_integral<size_t>(bit_precision_digits)
						};
				};
		constexpr const static auto parse_escape_character
				= character(escape_character_literal) 
				>= [](auto escape_character_string) {
					const char character = std::string_view{escape_character_string}[2];
					return character_to_escape_character(character);
				};
		constexpr const static auto parse_marked_escape_character
				= character(escape_character_literal, character_mark) 
				>= [](auto escape_character_string, auto character_mark) {
					const char character = std::string_view{escape_character_string}[2];
					return character_to_escape_character(character);
				};
		constexpr const static auto parse_escape_character_with_bit_precision
				= character(escape_character_literal, character_mark, digits) 
				>= [](auto escape_character_string, auto character_mark, auto bit_precision_digits)
				{
					const char character = std::string_view{escape_character_string}[2];
					return CharacterType{
							character_to_escape_character(character), 
							to_integral<size_t>(bit_precision_digits)
						};
				};
		constexpr const static auto parse_marked_character_number
				= character(digits, character_mark) 
				>= [](auto character_number, auto character_mark) {
					return CharacterType{character_number}; 
			};
		constexpr const static auto parse_marked_character_number_with_deliniator
				= character(digits, numerical_delinator, character_mark) 
				>= [](auto character_number, auto numerical_delinator, auto character_mark) {
					return CharacterType{character_number}; 
			};
		constexpr const static auto parse_marked_character_number_with_bit_precision
				= character(digits, character_mark, digits) 
				>= [](auto character_number, auto character_mark, auto bit_precision_digits) {
					return CharacterType{character_number, to_integral<size_t>(bit_precision_digits)}; 
			};

		constexpr const static auto parse_marked_character_number_with_bit_precision_with_delinator
				= character(digits, numerical_delinator, character_mark, digits) 
				>= [](auto character_number, auto delinator, auto character_mark, auto bit_precision_digits) {
					return CharacterType{character_number, to_integral<size_t>(bit_precision_digits)}; 
			};

		constexpr const static auto parse_boolean_value
				= boolean(boolean_literal) 
				>= [](auto boolean_literal_string) {
					return BoolType{to_warp_bool(to_bool(boolean_literal_string).value())};
				};

		constexpr const static auto parse_explicit_boolean_value
				= boolean(boolean_literal, bool_mark) 
				>= [](auto boolean_literal_string, auto bool_mark_string) {
					return BoolType{to_warp_bool(to_bool(boolean_literal_string).value())};
				};

		constexpr const static auto parse_integral_boolean_value
				= boolean(base_2_digits, bool_mark) 
				>= [](auto boolean_literal_string, auto bool_mark_string) {
					return BoolType{to_warp_bool( // TODO: Derive offset and string constants "0" and "1" from terms //
							to_bool<FixedString{"1"}, FixedString{"0"}>( 
									std::string_view{boolean_literal_string}.substr(2) 
								).value()
						)};
				};


		consteval static const auto rules()
		{
			return std::tuple_cat(
					BaseType::rules(), 
					ctpg::rules(
							parse_whole, 
							parse_explicit_whole, 
							parse_whole_with_bit_precision, 
							parse_integer, 
							parse_integer_with_bit_precision, 
							parse_negative_whole, 
							parse_negate_integer, 
							parse_base_16_fixed_point, 
							parse_base_10_fixed_point, 
							parse_base_8_fixed_point, 
							parse_base_2_fixed_point, 
							parse_fixed_point, 
							parse_fixed_point_explicit, 
							parse_fixed_point_radix_explicit, 
							parse_redundent_fixed_point, 
							parse_negate_fixed_point, 
							parse_character, 
							parse_marked_character, 
							parse_character_with_bit_precision, 
							parse_escape_character, 
							parse_marked_escape_character, 
							parse_escape_character_with_bit_precision, 
							parse_marked_character_number, 
							parse_marked_character_number_with_deliniator, 
							parse_marked_character_number_with_bit_precision, 
							parse_marked_character_number_with_bit_precision_with_delinator, 
							parse_boolean_value, 
							parse_explicit_boolean_value, 
							parse_integral_boolean_value
						)
				);
		}
	};
}

#endif // WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

