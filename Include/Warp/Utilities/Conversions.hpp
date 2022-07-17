#include <Warp/Common.hpp>
#include <Warp/Utilities/Math.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP
namespace Warp::Utilities
{
	template<typename CanidateParameterType>
	concept EnumConcept = std::is_enum_v<CanidateParameterType>;

	template<EnumConcept auto ParameterConstant>
	constexpr const auto enum_value = static_cast<
			std::underlying_type_t<decltype(ParameterConstant)>
		>(ParameterConstant);

	template<typename ToParameterType>
	struct FromString
	{
		constexpr static ToParameterType from_string(std::string_view from) {
			return from;
		}
	};
	
	template<NumericConcept NumericParameterType>
	struct FromString<NumericParameterType>
	{
		constexpr static NumericParameterType from_string(std::string_view from) 
		{
			NumericParameterType value;
			std::from_chars(from.begin(), from.end(), value);
			return value;
		}
	};

	template<typename ToParameterType>
	auto from_string(std::string_view from) -> ToParameterType {
		return FromString<ToParameterType>::from_string(from);
	}


}
#endif // WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP

