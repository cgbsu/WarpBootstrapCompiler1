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
            std::integral IntegralParameterType, 
            std::integral auto BaseParameterConstant = 10, 
            IntegralParameterType StartBaseParameterConstant = 0, 
            std::unsigned_integral auto RecursionMaxParameterConstant = default_max_recursion
        >
    constexpr std::unsigned_integral auto log(const IntegralParameterType number) noexcept 
            // -> const IntegralParameterType
    {
        if constexpr(RecursionMaxParameterConstant > 0)
        {
            const size_t lowered = static_cast<const IntegralParameterType>(
					number / BaseParameterConstant
				);
            if(lowered < BaseParameterConstant)
                return StartBaseParameterConstant;
            return log<
                    IntegralParameterType, 
                    BaseParameterConstant, 
                    StartBaseParameterConstant + 1, 
                    RecursionMaxParameterConstant - 1
                >(lowered);
        }
        else
            return number;
    }

	template<
            std::integral IntegralParameterType, 
            IntegralParameterType StartBaseParameterConstant = 0, 
            std::unsigned_integral auto RecursionMaxParameterConstant = default_max_recursion
        >
    constexpr std::unsigned_integral auto log(
			const IntegralParameterType number, const std::integral auto base) noexcept 
    {
        if constexpr(RecursionMaxParameterConstant > 0)
        {
            const auto lowered 
					= static_cast<const IntegralParameterType>(number / base);
            if(lowered < base)
                return StartBaseParameterConstant;
            return log<
                    IntegralParameterType, 
                    StartBaseParameterConstant + 1, 
                    RecursionMaxParameterConstant - 1
                >(lowered, base);
        }
        else
            return number;
    }
	
    template<std::integral IntegralParameterType>
    constexpr const auto raise(
            const IntegralParameterType base, 
            const std::unsigned_integral auto power
        ) noexcept -> IntegralParameterType 
    {
        if(power <= 0)
            return 1;
        return base * raise(base, power - 1);
    }

    template<
			std::integral IntegralParameterType, 
			std::integral auto PowerParameterConstant, 
			std::integral auto BaseParameterConstant = 10
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


    constexpr const bool logical_implies( const bool left, const bool right ) { 
        return (left == right || ( right == true ));
    }

	constexpr const auto absolute_value(auto value) {
		return (value < 0) ? -value : value;
	}

	const auto static extract_digits(
			/*const std::integral auto*/ unsigned number, 
			/*const std::unsigned_integral auto*/ unsigned index,
			/*const std::unsigned_integral auto*/ unsigned length = 1,  
			/*const std::unsigned_integral auto*/ unsigned base = 10
		)
	{
		const auto positive_number = absolute_value(number);
		const std::unsigned_integral auto front_divisor = raise(
				base, 
				index //+ (length)// - 1)
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
		const auto r = (positive_number - (front_to_remove + back_to_remove)) / front_divisor;
		std::cout << "FR: " << front_to_remove << " BR: " << back_to_remove << " R: " << r << "\n";
		return r;
	}
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

