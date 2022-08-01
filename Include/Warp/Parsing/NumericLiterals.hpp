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
		Whole, 
		Integer, 
		FixedPoint, 
		//TODO: FixedPoint
		Character, 
		Minus, 
		Dot, 
		IntegerMark, 
		UnsignedMark, 
		FixedMark
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
		using Type = fpm::fixed_16_16;
	};

	template<>
	struct NumericLiteralTypeResolver<NumericLiteral::Character> {
		using Type = char;
	};

	using NumericLiteralTermsType = MakeTerms<
			TreeTerm<
					NumericLiteral::Digits, 
					RegexTerm, 
					FixedString{"[0-9]+"}, 
					FixedString{"Digits"}, 
					ctpg::associativity::no_assoc
				>, 
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
					NumericLiteral::FixedMark, 
					StringTerm, 
					FixedString{"fxp"}, 
					ctpg::associativity::no_assoc
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
		constexpr const static auto whole 
				= TermsParameterTemplate::template term<NumericLiteral::Whole>;
		constexpr const static auto integer
				= TermsParameterTemplate::template term<NumericLiteral::Integer>;
		constexpr const static auto fixed_point
				= TermsParameterTemplate::template term<NumericLiteral::FixedPoint>;

		constexpr static const auto terms = ctpg::terms(
				digits, radix, minus, unsigned_mark, integer_mark, fixed_point_mark);

		constexpr static const auto non_terminal_terms = ctpg::nterms(
				whole, integer, fixed_point);

		constexpr const static auto parse_whole 
				= whole(digits) >= [](auto digit_string) {
					return to_integral<WholeType>(digit_string);
				};
		constexpr const static auto parse_explicit_whole 
				= whole(digits, unsigned_mark) >= [](auto digit_string, auto unsigned_mark) {
					return to_integral<WholeType>(digit_string);
				};
		constexpr const static auto parse_integer
				= integer(digits, integer_mark) >= [](auto digits_string, auto integer_mark_character) {
					return to_integral<IntegerType>(digits_string);
				};
		constexpr const static auto parse_negative_whole
				= integer(minus, whole) >= [](auto minus, auto whole_value) {
					return -static_cast<IntegerType>(whole_value);
				};
		constexpr const static auto parse_negate_integer
				= integer(minus, integer) >= [](auto minus, auto integer_value) {
					return -integer_value;
				};
		constexpr const static auto parse_fixed_point
				= fixed_point(digits, radix, digits) >= [](auto major, auto radix, auto minor)
				{
					const auto minor_value 
							= to_integral<WholeType>(minor); 
					const auto major_value = to_integral<WholeType>(major);
					return FixedPointType(
							(major_value * Warp::Utilities::raise(10u, minor_value))
									+ minor_value
						);
				};
		constexpr const static auto parse_redundent_fixed_point
				= fixed_point(fixed_point, fixed_point_mark) >= [](auto fixed_point_value, auto mark) {
					return fixed_point_value;
				};
		constexpr const static auto parse_negate_fixed_point
				= fixed_point(minus, fixed_point) >= [](auto negate, auto fixed_point_value) {
					return -fixed_point_value;
				};

		consteval static const auto rules()
		{
			return ctpg::rules(
					parse_whole, 
					parse_explicit_whole, 
					parse_integer, 
					parse_negative_whole, 
					parse_negate_integer, 
					parse_fixed_point, 
					parse_negate_fixed_point
				);
		}
	};
}

#endif // WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

