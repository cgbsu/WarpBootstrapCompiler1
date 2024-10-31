#include <Warp/Common.hpp>
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



#ifndef WARP__UTILITIES__HEADER__UTILITIES__STRINGS__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__STRINGS__HPP
namespace Warp::Utilities
{
	template<const auto& StringParameterConstant, size_t IndexParameterConstant = 0>
	consteval size_t string_length()
	{
		if constexpr(StringParameterConstant[IndexParameterConstant] != '\0')
			return string_length<StringParameterConstant, IndexParameterConstant + 1>();
		else
			return IndexParameterConstant;
	}

	template<char... CharacterParameterConstants>
	struct TemplateString {
		constexpr static const size_t length = sizeof...(CharacterParameterConstants);
		constexpr static const char array[length] = { CharacterParameterConstants... };
	};

	template<
			const auto& StringParameterConstant, 
			size_t IndexParameterConstant, 
			size_t LengthParameterConstant, 
			char... CharacterParameterConstants
		>
	consteval auto to_template_string()
	{
		if constexpr(IndexParameterConstant == LengthParameterConstant)
		{
			return to_template_string<
					StringParameterConstant, 
					IndexParameterConstant + 1,
					LengthParameterConstant, 
					CharacterParameterConstants..., 
					StringParameterConstant[IndexParameterConstant]
				>();
		}
		else
			return TemplateString<CharacterParameterConstants...>{}; 
	}

	template<const auto& StringParameterConstant>
	using TemplateStringType = decltype(to_template_string<
			StringParameterConstant, 
			0, 
			string_length<StringParameterConstant>()
		>);

	template<size_t LengthParameterConstant>
	constexpr const auto& copy_array(const auto (&from)[LengthParameterConstant], auto (&to)[LengthParameterConstant])
	{
		for(size_t ii = 0; ii < LengthParameterConstant; ++ii)
			to[ii] = from[ii];
		return to;
	}

	template<size_t FromLengthParameterConstant, size_t ToLengthParameterConstant>
	requires(ToLengthParameterConstant > FromLengthParameterConstant)
	constexpr const auto& copy_array(const auto (&from)[FromLengthParameterConstant], auto (&to)[ToLengthParameterConstant])
	{
		for(size_t ii = 0; ii < FromLengthParameterConstant; ++ii)
			to[ii] = from[ii];
		return to;
	}

	template<size_t LengthParameterConstant>
	struct FixedString
	{
		char string[LengthParameterConstant];
		constexpr static const size_t size = LengthParameterConstant;
		constexpr FixedString(const auto (&string_)[LengthParameterConstant]) { copy_array(string_, string); }
		operator std::string_view() {
			return std::string_view{string};
		}
		//template<size_t ToAppendLengthParameterConstant>
		//constexpr const auto append(const auto (&to_append)[ToAppendLengthParameterConstant]) const noexcept 
		//{
		//	char appended[LengthParameterConstant + ToAppendLengthParameterConstant];
		//	return FixedString{copy_array<ToAppendLengthParameterConstant>(
		//			copy_array(string, appended)
		//			appended[LengthParameterConstant], 
		//		)};
		//}
	};
}
#endif // WARP__UTILITIES__HEADER__UTILITIES__STRINGS__HPP

