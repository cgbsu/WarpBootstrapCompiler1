#include <Warp/Utilities.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__TERM__WRAPPERS__HPP
#define WARP__PARSING__HEADER__PARSING__TERM__WRAPPERS__HPP

namespace Warp::Parsing
{
	using namespace Warp::Utilities;

	constexpr const static auto no_associativity = ctpg::associativity::no_assoc; // Shorthand

	template<auto ValueParameterConstant>
	struct Value {
		constexpr static const decltype(ValueParameterConstant) value 
				= ValueParameterConstant;
	};
	
	template<typename TermParameterType>
	concept TermConcept = TermParameterType::term;
	
	template<
			FixedString PatternParameterConstant, 
			FixedString NameParameterConstant, 
			ctpg::associativity AssociativityParameterConstant, 
			int PrecedenceParameterConstant = 0
		>
	struct RegexTerm
	{
	    constexpr static const FixedString pattern = PatternParameterConstant;
	    constexpr static const FixedString name = NameParameterConstant;
		constexpr static const int precedence = PrecedenceParameterConstant;
		constexpr static ctpg::associativity associativity = AssociativityParameterConstant;

	    constexpr static auto term = ctpg::regex_term<pattern.string>(
				name.string, 
				precedence, 
				associativity
			);
	};

	template<
			FixedString StringParameterConstant,
			ctpg::associativity AssociativityParameterConstant, 
			int PrecedenceParameterConstant = 0
		>
	struct StringTerm
	{
	    constexpr static const FixedString string = StringParameterConstant;
		constexpr static const int precedence = PrecedenceParameterConstant;
		constexpr static ctpg::associativity associativity = AssociativityParameterConstant;

	    constexpr static auto term = ctpg::string_term(
				string.string, 
				precedence, 
				associativity
			);
	};

	template<
			char CharacterParameterConstant, 
			ctpg::associativity AssociativityParameterConstant, 
			int PrecedenceParameterConstant = 0
		>
	struct CharTerm
	{
		constexpr static const char character = CharacterParameterConstant;
		constexpr static int precedence = PrecedenceParameterConstant;
		constexpr static ctpg::associativity associativity = AssociativityParameterConstant;

		constexpr static const auto term = ctpg::char_term(
				character, 
				precedence, 
				associativity
			);
	};

	template<typename ValueParameterType, FixedString NameParameterConstant>
	struct NonTerminalTerm
	{
		using ValueType = ValueParameterType;
		constexpr static const FixedString name = NameParameterConstant;
		constexpr static auto term = ctpg::nterm<ValueType>(name.string);
	};
}
#endif // WARP__PARSING__HEADER__PARSING__TERM__WRAPPERS__HPP

