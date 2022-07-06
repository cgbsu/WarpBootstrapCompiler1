#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__TERMS__HPP
#define WARP__UTILITIES__HEADER__TERMS__HPP

namespace Warp::Utilities
{
	template<typename TermParameterType>
	concept TermConcept = TermParameterType::term;
	
	template<
			const char* PatternParameterConstant,
			const char* NameParameterConstant, 
			int PrecedenceParameterConstant = 0, 
			ctpg::associativity AssociativityParameterConstant 
					= ctpg::associativity::no_assoc
		>
	struct RegexTerm
	{
	    constexpr static const char* pattern = PatternParameterConstant;
	    constexpr static const char* name = NameParameterConstant;
		constexpr static int precedence = PrecedenceParameterConstant;
		constexpr static ctpg::associativity associativity = AssociativityParameterConstant;

	    constexpr static ctpg::regex_term<pattern> term(name, precedence, associativity);
	};
	
	template<
			const char* StringParameterConstant,
			const char* NameParameterConstant
			int PrecedenceParameterConstant = 0, 
			ctpg::associativity AssociativityParameterConstant 
					= ctpg::associativity::no_assoc
		>
	struct StringTerm
	{
	    constexpr static const char* string = StringParameterConstant;
	    constexpr static const char* name = NameParameterConstant;
		constexpr static int precedence = PrecedenceParameterConstant;
		constexpr static ctpg::associativity associativity = AssociativityParameterConstant;

	    constexpr static ctpg::string_term<string> term(name, precedence, associativity);
	};
	
	template<
			char ParameterConstant, 
			int PrecedenceParameterConstant = 0, 
			ctpg::associativity AssociativityParameterConstant 
					= ctpg::associativity::no_assoc
		>
	struct CharTerm
	{
		constexpr static const char character = ParameterConstant;
		constexpr static int precedence = PrecedenceParameterConstant;
		constexpr static ctpg::associativity associativity = AssociativityParameterConstant;

		constexpr static ctpg::char_term term(character, precedence, associativity);
	};

	template<typename ValueParameterType, const char* NameParameterConstant>
	struct NonTerminalTerm
	{
		constexpr
	}

}  // namespace Warp::Utilities

#endif	// WARP__UTILITIES__HEADER__TERMS__HPP


