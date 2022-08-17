#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

namespace Warp::Utilities
{
	/* Useful for when you need an instance of something,    * 
	 * say for a constexpr/consteval function, but dont want * 
	 * to instantiate it (its just for retrieving the type)  */
	template<typename ParameterType>
	struct TypeHolder {
		using Type = ParameterType;
	};

	template<typename DoNotConvertToParameterType, typename ConvertToParameterType, typename CanidateParameterType>
	concept ConvertableToExceptConcept =
			(!std::convertible_to<CleanType<DoNotConvertToParameterType>, CleanType<CanidateParameterType>> 
					&& std::convertible_to<CleanType<ConvertToParameterType>, CleanType<CanidateParameterType>>)
			|| (!std::same_as<CleanType<DoNotConvertToParameterType>, CleanType<CanidateParameterType>> 
					&& std::convertible_to<CleanType<ConvertToParameterType>, CleanType<CanidateParameterType>>);

	template<typename, typename>
	struct TotallyOrderedWith {
		constexpr static const bool value = false;
	};

	template<typename LeftParameterType, typename RightParameterType>
	requires std::totally_ordered_with<LeftParameterType, RightParameterType>
	struct TotallyOrderedWith<LeftParameterType, RightParameterType> {
		constexpr static const bool value = true;
	};

	template<typename, typename>
	struct EquallyComparibleWith {
		constexpr static const bool value = false;
	};

	template<typename LeftParameterType, typename RightParameterType>
	requires std::equality_comparable_with<LeftParameterType, RightParameterType>
	struct EquallyComparibleWith<LeftParameterType, RightParameterType> {
		constexpr static const bool value = true;
	};

	template<typename LeftParameterType, typename RightParameterType>
	constexpr static const bool is_comparible
			= TotallyOrderedWith<LeftParameterType, RightParameterType>::value;

	template<typename LeftParameterType, typename RightParameterType>
	constexpr static const bool is_equally_comparible 
			= EquallyComparibleWith<LeftParameterType, RightParameterType>::value;
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

