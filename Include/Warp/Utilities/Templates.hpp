#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

namespace Warp::Utilities
{
	template<typename DoNotConvertToParameterType, typename ConvertToParameterType, typename CanidateParameterType>
	concept ConvertableToExceptConcept =
			(!std::convertible_to<CleanType<DoNotConvertToParameterType>, CleanType<CanidateParameterType>> 
					&& std::convertible_to<CleanType<ConvertToParameterType>, CleanType<CanidateParameterType>>)
			|| (!std::same_as<CleanType<DoNotConvertToParameterType>, CleanType<CanidateParameterType>> 
					&& std::convertible_to<CleanType<ConvertToParameterType>, CleanType<CanidateParameterType>>);
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

