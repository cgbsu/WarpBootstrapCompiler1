#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__FIXED__POINT__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__FIXED__POINT__HPP

namespace Warp::Utilities
{
	template<std::integral PartParameterType = unsigned long int>
	requires(std::is_unsigned<PartParameterType>::value == true)
	struct FixedPoint
	{
		using PartType = PartParameterType;
		constexpr const static auto base = BaseParameterConstant;
		bool negative;
		PartParameterType number, offset, base;
		// 3 * .5 = 1.5
		constexpr static void majorMinorMultiply(auto& major, auto& minor)
		{
			const auto majorMinor = major * minor;
			const auto majorPush = log<PartType, base>(majorMinor);
			const auto minorPower = log<PartType, base>(minor);
			const auto differenceFactor = raise(base, majorPush - minorPower);
			const auto majorPushFactor = raise(base, majorPush);
			const auto majorDifference = majorMinor / differenceFactor;
			major += majorDifference;
			minor += (majorMinor - (majorDifference * majorPushFactor));
		}
		// .7 + .7 = 1.4
		constexpr static void addMinor()
		//FixedPoint& operator*(const FixedPoint& other)
	};
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__FIXED__POINT__HPP

