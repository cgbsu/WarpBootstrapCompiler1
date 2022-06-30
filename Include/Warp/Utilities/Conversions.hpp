#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__CONVERSIONS__HPP
#define WARP__UTILITIES__HEADER__CONVERSIONS__HPP
namespace Warp::Utilities
{
	template<typename CanidateParameterType>
	concept EnumConcept = std::is_enum_v<CanidateParameterType>;

	template<EnumConcept auto ParameterConstant>
	constexpr const auto enum_value = static_cast<std::underlying_type_t<decltype(ParameterConstant)>>(ParameterConstant);
}
#endif // WARP__UTILITIES__HEADER__CONVERSIONS__HPP
