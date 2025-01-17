#include <Warp/Utilities.hpp>
/* This work is under the Copyright Christopher A. Greeley (2024) and it is distributed
* under the No Kill Do No Harm License, a legally non-binding sumemry is as follows: 
* 
* # No Kill Do No Harm Licence – Summary
* 
* Based on version 0.3, July 2022 of the Do No Harm License
* 
* https://github.com/raisely/NoHarm
* 
* LEGALLY NON-BINDING SUMMARY OF THE TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
* 
* ## Licence Grants
* 
* You're allowed
* 
* - to distribute the licensed work,
* - to create, publish, sublicense and patent derivative works and
* - to put your modifications or your derivative work under a seperate licence,
* 
* free of charge. Though, filing patent litigation leads to the loss of the patent licence. Also, the licence grants don't include the right to use the licensor's trademarks.
* 
* ## Unethical Behaviour
* 
* You may not use the licensed work if you engage in:
* 
* - human rights violations,
* - environmental destruction,
* - warfare,
* - addictive/destructive products or services or
* - actions that frustrate:
*   * peace,
*   * access to human rights,
*   * peaceful assembly and association,
*   * a sustainable environment or
*   * democratic processes
*   * abortion
*   * euthanasia
*   * human embryonic stem cell research (if human organisms are killed in the process)
* - except for actions that may be contrary to "human rights" (or interpretations thereof), do not kill and that frustrate 
*   * abortion
*   * euthanasia
*   * killing
* and; the software must never be used to kill, including: abortion, euthanasia, human stem cell research, in war, or law enforcement or as a part of any lethal weapon
* 
* ## Contributions
* 
* Contributions to the licensed work must be licensed under the exact same licence.
* 
* ## Licence Notice
* 
* When distributing the licensed work or your derivative work, you must
* 
* - include a copy of this licence,
* - retain attribution notices,
* - state changes that you made and
* - not use the names of the author and the contributors to promote your derivative work.
* 
* If the licensed work includes a "NOTICE" text file with attribution notices, you must copy those notices to:
* 
* - a "NOTICE" file within your derivative work,
* - a place within the source code or the documentation or
* - a place within a display generated by your derivative work.
* 
* ## No Warranty or Liability
* 
* The licensed work is offered on an as-is basis without any warranty or liability. You may choose to offer warranty or liability for your derivative work, but only fully on your own responsibility. */



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

