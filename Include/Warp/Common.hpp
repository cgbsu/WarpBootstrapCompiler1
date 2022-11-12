#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <any>
#include <sstream>
#include <array>
#include <algorithm>
#include <tuple>
#include <source_location>
#include <type_traits>
#include <concepts>
#include <charconv>
#include <initializer_list>
#include <limits>
#include <compare>
#include <memory>
#include <functional>
#include <utility>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <climits>

#include <ctpg/ctpg.hpp>
#include <ctre.hpp>

#include <ThirdParty/cpp-utilities/fixed.hpp>

#ifndef WARP__UTILITIES__HEADER__COMMON__HPP
#define WARP__UTILITIES__HEADER__COMMON__HPP

namespace Warp
{
	namespace Utilities::Detail
	{
		#ifdef NDEBUG
			constexpr static const bool debug = true;
		#else
			#ifdef WARP_BOOSTRAP_COMPILER_DEBUG_ON
				constexpr static const bool debug = true;
			#else
				constexpr static const bool debug = false;
			#endif
		#endif
	}
	using size_t = std::size_t; // Getting some warnings, this is to silence them
}

#endif // WARP__UTILITIES__HEADER__COMMON__HPP

