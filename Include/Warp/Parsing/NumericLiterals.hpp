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
		Character
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

	using WholeType = NumericLiteralTypeResolver<NumericLiteral::Whole>::Type;
	using NumericLiteralTermsType = MakeTerms<
			TreeTerm<
					NumericLiteral::Digits, 
					RegexTerm, 
					FixedString{"[0-9]+"}, 
					FixedString{"Digits"}, 
					ctpg::associativity::no_assoc
				>, 
			TypeTreeTerm<
					NumericLiteral::Whole, 
					NonTerminalTerm, 
					WholeType, 
					FixedString{"Whole"}
				>
		>;

	template<
			typename TermsParameterTemplate, 
			template<auto> typename ResolverParameterTemplate
		>
	struct NumericLiteralParser
	{
		using WholeType = ResolverParameterTemplate<NumericLiteral::Whole>::Type;
		constexpr const static auto digits 
				= TermsParameterTemplate::template term<NumericLiteral::Digits>;
		constexpr const static auto whole 
				= TermsParameterTemplate::template term<NumericLiteral::Whole>;
		constexpr const static auto parse_whole 
				= whole(digits) >= [](auto digit_string) {
					return from_string<WholeType>(static_cast<std::string_view>(digit_string));
				};
	};
}

#endif // WARP__PARSING__HEADER__PARSING__NUMERIC__LITERALS__HPP

