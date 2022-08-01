#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

namespace Warp::Utilities
{
	template<typename ParameterType>
	concept NumericConcept = std::is_integral_v<ParameterType> 
			|| std::is_floating_point_v<ParameterType>;

	constexpr static const size_t default_max_recursion = 850;
	template<
            std::unsigned_integral auto BaseParameterConstant = 10, 
            std::unsigned_integral auto StartBaseParameterConstant = 0, 
            std::unsigned_integral auto RecursionMaxParameterConstant = default_max_recursion
        >
    constexpr std::unsigned_integral auto log(const std::integral auto number) noexcept 
    {
        if constexpr(RecursionMaxParameterConstant > 0)
        {
            const auto lowered = static_cast<decltype(number)>(
					number / BaseParameterConstant
				);
            if(lowered < BaseParameterConstant)
                return StartBaseParameterConstant;
            return log<
                    BaseParameterConstant, 
                    StartBaseParameterConstant + 1, 
                    RecursionMaxParameterConstant - 1
                >(lowered);
        }
        else
            return number;
    }

	template<
            std::unsigned_integral auto StartBaseParameterConstant = 0u, 
            std::unsigned_integral auto RecursionMaxParameterConstant = default_max_recursion
        >
    constexpr std::unsigned_integral auto log(
			const std::integral auto number, const std::unsigned_integral auto base) noexcept 
    {
		using UnsignedNumberType = std::make_unsigned_t<decltype(number)>;
        if constexpr(RecursionMaxParameterConstant > 0)
        {
            const auto lowered 
					= static_cast<decltype(number)>(number / base);
            if(lowered < base)
                return static_cast<UnsignedNumberType>(StartBaseParameterConstant);
           return log<
                    static_cast<UnsignedNumberType>(StartBaseParameterConstant) + UnsignedNumberType{1}, 
                    RecursionMaxParameterConstant - 1
                >(lowered, base);
        }
        else
            return static_cast<
	std::make_unsigned_t<decltype(number)>>(number);
    }
	
    constexpr auto raise(
            const std::unsigned_integral auto base, 
            const std::unsigned_integral auto power
        ) noexcept
    {
        if(power <= 0)
            return decltype(base){1};
        return base * raise(base, power - 1);
    }

    template<
			std::unsigned_integral auto PowerParameterConstant, 
			std::unsigned_integral auto BaseParameterConstant = 10
		>
    consteval const auto raise_constexpr() noexcept 
    {
        if constexpr(PowerParameterConstant <= 0)
            return 1;
        return BaseParameterConstant * raise_constexpr(
				BaseParameterConstant, 
				PowerParameterConstant - 1
			);
    }


    constexpr const bool logical_implies(const bool left, const bool right) { 
        return (left == right || (right == true));
    }

	constexpr const auto absolute_value(auto value) {
		return (value < 0) ? -value : value;
	}

	constexpr const auto difference(auto first, auto second) {
		return absolute_value(absolute_value(second) - absolute_value(first));
	}
	
	const auto static extract_digits(
			std::integral auto number, 
			std::unsigned_integral auto index,
			std::unsigned_integral auto length, // = 1u TODO: Does not seem to be working on gcc 12.1
			std::unsigned_integral auto base // = 10u TODO: Does not seem to be working on gcc 12.1
		)
	{
		const auto positive_number = absolute_value(number);
		const std::unsigned_integral auto front_divisor = raise(
				base, 
				index 
			);
		const std::unsigned_integral auto back_divisor = raise(
				base, 
				index + length
			);
		const std::unsigned_integral auto front_to_remove = positive_number - (
				(positive_number / front_divisor) * front_divisor
			);
		const std::unsigned_integral auto back_to_remove = (
				(positive_number / back_divisor) * back_divisor
			);
		return (positive_number - (front_to_remove + back_to_remove)) / front_divisor;
	}

	const auto static extract_digits(std::integral auto number, 
			std::unsigned_integral auto index) { // Defaults for concept arguments dont seem to be working GCC 12.1
		return extract_digits(number, index, 1u, 10u);
	}

	const auto static extract_digits(
				std::integral auto number, 
				std::unsigned_integral auto index, 
				std::unsigned_integral auto length
			) { // Defaults for concept arguments dont seem to be working GCC 12.1
		return extract_digits(number, index, length, 10u);
	}
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

