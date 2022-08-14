#include <Warp/Common.hpp>

#include <Warp/Utilities/Math.hpp>
#include <Warp/Utilities/Logging.hpp>
#include <Warp/Utilities/Strings.hpp>
#include <Warp/Utilities/Conversions.hpp>
#include <Warp/Utilities/Templates.hpp>

#ifndef WARP__UTILITIES__HEADER__HPP
#define WARP__UTILITIES__HEADER__HPP

namespace Warp::Utilities
{
	/* Useful for when you need an instance of something,    * 
	 * say for a constexpr/consteval function, but dont want * 
	 * to instantiate it (its just for retrieving the type)  */
	template<typename ParameterType>
	struct TypeHolder {
		using Type = ParameterType;
	};
}

#endif // WARP__UTILITIES__HEADER__HPP

