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
		FloatingPoint, 
		//TODO: FixedPoint
		Character, 
		Minus, 
		Dot, 
		IntegerMark, 
		UnsignedMark, 
		FloatMark
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
	struct NumericLiteralTypeResolver<NumericLiteral::FloatingPoint> {
		using Type = long double;
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
					NumericLiteral::FloatMark, 
					CharTerm, 
					'f', 
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
					NumericLiteral::FloatingPoint, 
					NonTerminalTerm, 
					NumericLiteralTypeResolver<NumericLiteral::FloatingPoint>::Type, 
					FixedString{"FloatingPoint"}
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
		using FloatingPointType = ResolverParameterTemplate<NumericLiteral::FloatingPoint>::Type;
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
		constexpr const static auto float_mark
				= TermsParameterTemplate::template term<NumericLiteral::FloatMark>;
		constexpr const static auto whole 
				= TermsParameterTemplate::template term<NumericLiteral::Whole>;
		constexpr const static auto integer
				= TermsParameterTemplate::template term<NumericLiteral::Integer>;
		constexpr const static auto floating_point
				= TermsParameterTemplate::template term<NumericLiteral::FloatingPoint>;

		constexpr static const auto terms = ctpg::terms(
				digits, radix, minus, unsigned_mark, integer_mark, float_mark);

		constexpr static const auto non_terminal_terms = ctpg::nterms(
				whole, integer, floating_point);

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
		constexpr const static auto parse_floating_point
				= floating_point(digits, radix, digits) >= [](auto major, auto radix, auto minor) {
					std::string_view minor_view = minor;
					const auto minor_denomonator 
							= Warp::Utilities::raise(static_cast<WholeType>(10), minor_view.size());
					const WholeType minor_value = to_integral<WholeType>(minor);
					const auto after_radix = (static_cast<FloatingPointType>(minor_value) 
							/ static_cast<FloatingPointType>(minor_denomonator));
					return static_cast<FloatingPointType>(to_integral<WholeType>(major)) 
							+ after_radix;
				};

		consteval static const auto rules()
		{
			return ctpg::rules(
					parse_whole, 
					parse_explicit_whole, 
					parse_integer, 
					parse_negative_whole, 
					parse_negate_integer, 
					parse_floating_point
				);
		}
	};
}

#endif // WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

