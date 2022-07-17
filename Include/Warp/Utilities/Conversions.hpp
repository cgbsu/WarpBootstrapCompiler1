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
	auto from_string(std::string_view from) -> ToParameterType {
		return from;
	}

	template<NumericConcept NumericParameterType>
	auto from_string<NumericParameterType>(std::string_view from) 
			-> NumericParameterType 
	{
		NumericParameterType value;
		std::from_chars(from.begin(), from.end(), value);
		return value;
	}

}
#endif // WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP

