#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

namespace Warp::Utilities
{
	template<typename ParameterType>
	concept NumericConcept = std::is_integral_v<ParameterType> 
			|| std::is_floating_point_v<ParameterType>;

	template<
            std::integral IntegralParameterType, 
            std::integral auto BaseParameterConstant = 10, 
            IntegralParameterType StartBaseParameterConstant = 0, 
            size_t RecursionMaxParameterConstant = 850
        >
    constexpr size_t log(const IntegralParameterType number) noexcept 
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

    template<std::integral IntegralParameterType>
    constexpr const auto raise(
            const IntegralParameterType base, 
            const size_t power
        ) noexcept 
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
        return ( left == right || ( right == true ) );
    }

}

#endif // WARP__UTILITIES__HEADER__UTILITIES__MATH__HPP

