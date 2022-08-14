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
		//Whole, 
		//Integer, 
		//FixedPoint, 
		//Character, 
		CharacterLiteral, 
		EscapeCharacterLiteral, 
		Minus, 
		Dot, 
		IntegerMark, 
		UnsignedMark, 
		FixedMark, 
		AnyDecimalDigits, 
		AnyDecimalDigitsReduction, 
		CharacterMark, 
		//Bool, 
		BooleanLiteral
	};

	enum class WarpBool : unsigned char {
		True = 1, 
		False = 0 
	};

	//constexpr const WarpBool& operator=(WarpBool& to, bool from) {
	//	return from == true ? (to = WarpBool::True) : (to = WarpBool::False);
	//}
	
	constexpr WarpBool to_warp_bool(const bool from) {
		return from == true ? WarpBool::True : WarpBool::False;
	}

	enum class NumericTypeTag {
		Whole, Integer, FixedPoint, Character, Bool
	};

	template<NumericTypeTag ParameterTypeConstant, typename NumericParameterType>
	struct NumericType
	{
		using UnderylingType = NumericParameterType;
		constexpr static const NumericTypeTag type = ParameterTypeConstant;
		constexpr static const bool fixed_point_type = false;
		constexpr static const size_t bits = sizeof(UnderylingType) * CHAR_BIT;
		using ThisType = NumericType<type, UnderylingType>;
		UnderylingType number; // This would be const, but CTPG requires it this object be writable. //

		constexpr NumericType(const UnderylingType number) noexcept 
				: number(number) {}
		constexpr NumericType(const std::string_view numeric_string_representation) noexcept 
				: number(to_integral<UnderylingType>(numeric_string_representation)) {}

		constexpr NumericType(const NumericType& other) noexcept = default;
		constexpr NumericType(NumericType&& other) noexcept = default;
		constexpr NumericType& operator=(const NumericType& other) noexcept = default;
		constexpr NumericType& operator=(NumericType&& other) noexcept = default;

		constexpr std::strong_ordering operator<=>(const NumericType& other) const noexcept = default;

		constexpr std::strong_ordering operator<=>(const UnderylingType& other) const noexcept
		{
			return (other < number) 
					? std::strong_ordering::less
					: ((other > number) 
							? std::strong_ordering::greater 
							: std::strong_ordering::equivalent);
		}

		constexpr operator UnderylingType() const noexcept {
			return number;
		}
		constexpr NumericType operator+(const NumericType& other) const noexcept {
			return number + other.number;
		}
		constexpr NumericType operator*(const NumericType& other) const noexcept {
			return number * other.number;
		}
		constexpr NumericType operator-(const NumericType& other) const noexcept {
			return number - other.number;
		}
		constexpr NumericType operator/(const NumericType& other) const noexcept {
			return number / other.number;
		}
		constexpr NumericType operator-() const noexcept {
			//static_assert(std::is_unsigned_v<UnderylingType> == true);
			return -number;
		}
	};

	template<
			NumericTypeTag ParameterTypeConstant, 
			size_t WholePartBitsParameterConstant, 
			size_t DecimalPartBitsParameterConstant
		>
	struct NumericType<
			ParameterTypeConstant, 
			numeric::fixed<
					WholePartBitsParameterConstant, 
					DecimalPartBitsParameterConstant
				>
		>
	{
		constexpr static const NumericTypeTag type = ParameterTypeConstant;
		constexpr const static auto whole_part_bits = WholePartBitsParameterConstant;
		constexpr const static auto decimal_part_bits = DecimalPartBitsParameterConstant;
		using UnderylingType = numeric::fixed<whole_part_bits, decimal_part_bits>;
		using ThisType = NumericType<type, UnderylingType>;
		constexpr static const bool fixed_point_type = true;
		constexpr static const size_t bits = whole_part_bits + decimal_part_bits;
		UnderylingType number; // This would be const, but CTPG requires it this object be writable. //

		constexpr NumericType(const UnderylingType number) noexcept 
				: number(number) {}
		constexpr NumericType(const std::string_view whole_part, const std::string_view decimal_part) noexcept
				: number(to_fixed_point_integral<size_t, UnderylingType>(
							to_integral<size_t>(whole_part), 
							to_integral<size_t>(decimal_part)
						)) {}
		constexpr NumericType(const std::string_view whole_part) noexcept
				: number(to_fixed_point_integral<size_t, UnderylingType>(to_integral<size_t>(whole_part), 0u)) {}
		constexpr NumericType(const size_t whole_part, const size_t decimal_part) noexcept
				: number(to_fixed_point_integral<size_t, UnderylingType>(whole_part, decimal_part)) {}

		constexpr NumericType(const NumericType& other) noexcept = default;
		constexpr NumericType(NumericType&& other) noexcept = default;
		constexpr NumericType& operator=(const NumericType& other) noexcept = default;
		constexpr NumericType& operator=(NumericType&& other) noexcept = default;


		constexpr std::strong_ordering operator<=>(const NumericType& other) const noexcept = default;
		constexpr std::strong_ordering operator<=>(const UnderylingType& other) const noexcept
		{
			return (other < number) 
					? std::strong_ordering::less
					: ((other > number) 
							? std::strong_ordering::greater 
							: std::strong_ordering::equivalent);
		}
		constexpr operator UnderylingType() const noexcept {
			return number;
		}
		constexpr NumericType operator+(const NumericType& other) const noexcept {
			return number + other.number;
		}
		constexpr NumericType operator*(const NumericType& other) const noexcept {
			return number * other.number;
		}
		constexpr NumericType operator-(const NumericType& other) const noexcept {
			return number - other.number;
		}
		constexpr NumericType operator/(const NumericType& other) const noexcept {
			return number / other.number;
		}
		constexpr NumericType operator-() const noexcept {
			return -number;
		}
	};
	
	template<auto NumericalTypeTag>
	struct NumericLiteralTypeResolver {};
	
	template<>
	struct NumericLiteralTypeResolver<NumericTypeTag::Whole> {
		using Type = NumericType<NumericTypeTag::Whole, size_t>;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericTypeTag::Integer> {
		using Type = NumericType<NumericTypeTag::Integer, long long signed int>;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericTypeTag::FixedPoint> {
		using Type = NumericType<NumericTypeTag::FixedPoint, numeric::fixed<16, 16>>;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericTypeTag::Character> {
		using Type = NumericType<NumericTypeTag::Character, char>;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericTypeTag::Bool> {
		using Type = NumericType<NumericTypeTag::Bool, WarpBool>;
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
					NumericTypeTag::FixedPoint, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericTypeTag::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Whole, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericTypeTag::Whole>::Type, 
					FixedString{"Whole"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Integer, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericTypeTag::Integer>::Type, 
					FixedString{"Integer"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::FixedPoint, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericTypeTag::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Character, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericTypeTag::Character>::Type, 
					FixedString{"Character"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Bool, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericTypeTag::Bool>::Type, 
					FixedString{"Bool"}
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
		using WholeType = ResolverParameterTemplate<NumericTypeTag::Whole>::Type;
		using IntegerType = ResolverParameterTemplate<NumericTypeTag::Integer>::Type;
		using FixedPointType = ResolverParameterTemplate<NumericTypeTag::FixedPoint>::Type;
		using CharacterType = ResolverParameterTemplate<NumericTypeTag::Character>::Type;
		using BoolType = ResolverParameterTemplate<NumericTypeTag::Bool>::Type;

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
		constexpr const static auto boolean_literal
				= TermsParameterTemplate::template term<NumericLiteral::BooleanLiteral>;
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
				= TermsParameterTemplate::template term<NumericTypeTag::Whole>;
		constexpr const static auto integer
				= TermsParameterTemplate::template term<NumericTypeTag::Integer>;
		constexpr const static auto fixed_point
				= TermsParameterTemplate::template term<NumericTypeTag::FixedPoint>;
		constexpr const static auto character
				= TermsParameterTemplate::template term<NumericTypeTag::Character>;
		constexpr const static auto boolean
				= TermsParameterTemplate::template term<NumericTypeTag::Bool>;

		constexpr static const auto terms = ctpg::terms(
				character_literal, 
				escape_character_literal, 
				base_10_digits, 
				base_16_digits, 
				base_8_digits, 
				base_2_digits, 
				any_decimal_digits, 
				boolean_literal, 
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
				any_decimal_digits_reduction, 
				boolean
			);

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
		constexpr const static auto parse_whole 
				= whole(digits) >= [](auto digit_string) {
					return WholeType{digit_string};
				};
		constexpr const static auto parse_explicit_whole 
				= whole(digits, unsigned_mark) 
				>= [](auto digit_string, auto unsigned_mark) {
					return WholeType{digit_string};
				};
		constexpr const static auto parse_integer
				= integer(digits, integer_mark) 
				>= [](auto digits_string, auto integer_mark_character) {
					return IntegerType{digits_string};
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
		constexpr const static auto parse_escape_character
				= character(escape_character_literal) 
				>= [](auto escape_character_string) {
					const char character = std::string_view{escape_character_string}[2];
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
				};
		constexpr const static auto parse_marked_character_number
				= character(digits, character_mark) 
				>= [](auto character_number, auto character_mark) {
					return CharacterType{character_number}; 
			};

		constexpr const static auto parse_boolean_value
				= boolean(boolean_literal) 
				>= [](auto boolean_literal_string) {
					return BoolType{to_warp_bool(to_bool(boolean_literal_string).value())};
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
					parse_marked_character, 
					parse_boolean_value
				);
		}
	};
}

#endif // WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

