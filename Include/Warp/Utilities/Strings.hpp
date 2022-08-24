#include <Warp/Common.hpp>

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

