#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__NUMBERS__HPP
#define WARP__PARSING__HEADER__PARSING__NUMBERS__HPP

namespace Warp::Parsing
{
	/*
	template<
			int LiteralPrecedence, 
			Warp::Utilities::FixedString DigitsPatternParameterConstant = "[0-9]+", 
			char RadixParameterConstant = '.', 
			char NegationParameterConstant = '-'
		>
	struct NumericLiteralRules
	{
		using DigitsTermType = RegexTerm<
				DigitsPatternParameterConstant, 
				"Digits", 
				ctpg::associativity::no_assoc, 
				LiteralPrecedence
			>;
		using NegativeTermType = CharTerm<NegationParameterConstant>;

		using WholeType = size_t;
		using IntegerType = long long signed int;
		using RationalType = double;

		using WholeTermType = NonTerminalTerm<WholeTermType, "Whole">;
		using IntegerTermType = NonTerminalTerm<IntegerType, "Integer">;
		using RationalTermType = NonTerminalTerm<RationalType, "Rational">;

		constexpr const static auto digits = DigitsTermType::term;
		constexpr const static auto negative = NegativeTermType::term;
		constexpr const static auto whole = WholeTermType::term;
		constexpr const static auto integer = IntegerTermType::term;
		constexpr const static auto rational = RationalTermType::term;

		constexpr static const auto whole_rule = whole(digits) 
			>= [](auto digits_string) {
				return from_string<WholeType>(digits_string);
			};

		constexpr static const auto integer_rule = integer(negative, whole) 
			>= [](auto negative, auto whole_value) {
				return -static_cast<IntegerType>(whole_value);
			};

		constexpr const static auto rules = ctpg::rules(
				whole_rule, 
				integer_rule
			);
		constexpr const static auto terms = ctpg::terms(digits, negative);
		constexpr const static auto non_terminal_terms = ctpg::rules(
				whole, 
				integer, 
				rational
			);
	};
	*/
}

#endif // WARP__PARSING__HEADER__PARSING__NUMBERS__HPP

