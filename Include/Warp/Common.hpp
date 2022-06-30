#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <array>
#include <algorithm>
#include <tuple>
#include <source_location>
#include <type_traits>
#include <concepts>
#include <charconv>

#include <ctpg/ctpg.hpp>

namespace Warp
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

