#include <Warp/Common.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SINGLE_PARAMETER__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SINGLE_PARAMETER__HPP

namespace Warp::Runtime::Compiler
{
	// This is a small file now but I think it may grow later. 2022/11/11 //
	template<
			typename ConstraintParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct SingleParameter
	{
		using ConstraintType = ConstraintParameterType;
		constexpr SingleParameter() noexcept {}
		constexpr SingleParameter(const SingleParameter& other) noexcept = default;
		constexpr SingleParameter(SingleParameter&& other) noexcept = default;
		constexpr SingleParameter& operator=(const SingleParameter& other) noexcept = default;
		constexpr SingleParameter& operator=(SingleParameter&& other) noexcept = default;
		ConstraintType constraint;
	};

}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SINGLE_PARAMETER__HPP
