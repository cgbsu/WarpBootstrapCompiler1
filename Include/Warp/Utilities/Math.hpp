#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

namespace Warp::Utilities
{
	template<typename ParameterType>
	concept NumericConcept = std::is_integral_v<ParameterType> 
			|| std::is_floating_point_v<ParameterType>;
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

