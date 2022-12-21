#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

namespace Warp::Utilities
{
	struct PlaceHolder {};

	/* Useful for when you need an instance of something,    * 
	 * say for a constexpr/consteval function, but dont want * 
	 * to instantiate it (its just for retrieving the type)  */
	template<typename ParameterType>
	struct TypeHolder {
		using Type = ParameterType;
	};

	template<
			typename DoNotConvertToParameterType, 
			typename ConvertToParameterType, 
			typename CanidateParameterType
		>
	concept ConvertableToExceptConcept =
			(!std::convertible_to<
							CleanType<DoNotConvertToParameterType>, 
							CleanType<CanidateParameterType>
						> 
					&& std::convertible_to<
							CleanType<ConvertToParameterType>, 
							CleanType<CanidateParameterType>
						>
				)
			|| (!std::same_as<
							CleanType<DoNotConvertToParameterType>, 
							CleanType<CanidateParameterType>
						> 
					&& std::convertible_to<
							CleanType<ConvertToParameterType>, 
							CleanType<CanidateParameterType>
						>
				);

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

	template<typename CanidateParameterType>
	concept HasTypeConcept = requires(CanidateParameterType) { typename CanidateParameterType::Type; };

	template<typename ParserParameterType>
	concept ParserConcept = requires(ParserParameterType) {
			ParserParameterType::rules();
			ParserParameterType::terms;
			ParserParameterType::non_terminal_terms;
		};

	template<size_t ToIgnoreParmeterConstant, size_t... IndexParameterConstants>
	constexpr static const auto take_after_implentation(auto from, std::index_sequence<IndexParameterConstants...>) {
		return std::tuple{std::get<IndexParameterConstants + ToIgnoreParmeterConstant>(from)...};
	}

	template<size_t ToIgnoreParmeterConstant>
	constexpr static const auto take_after(auto from) {
		return take_implentation(from, (std::tuple_size_v<decltype(from)> - ToIgnoreParmeterConstant));
	}

	template<typename TupleParameterType, size_t ToIgnoreParmeterConstant>
	using TupleAfterType = decltype(take_after<ToIgnoreParmeterConstant>(std::declval<TupleParameterType>()));

	template<typename TupleParameterType>
	using TupleAfterFirstType = TupleAfterType<TupleParameterType, 1>;

	constexpr const auto concatinate_tuples(auto current_tuple, auto last_tuple) {
		return std::tuple_cat(current_tuple, last_tuple);
	}

	constexpr const auto concatinate_tuples(auto current_tuple, auto... tuples)
			requires(sizeof...(tuples) > 0) {
		return std::tuple_cat(current_tuple, concatinate_tuples(tuples...));
	}
	// Shameless yoink from cppreference.com/reference/en/cpp/utility/variant/visit.html
	template<class... Ts> struct OverloadedVisit : Ts... { using Ts::operator()...; };
	template<class... Ts> OverloadedVisit(Ts...) -> OverloadedVisit<Ts...>;

	template<typename ParameterType>
	struct IsOptionalHelper {
		constexpr static const bool value = false;
		constexpr IsOptionalHelper(ParameterType) {}
	};
	
	template<typename ParameterType>
	struct IsOptionalHelper<std::optional<ParameterType>> {
		constexpr static const bool value = true;
		constexpr IsOptionalHelper(std::optional<ParameterType>) {}
	};
	
	template<typename ParameterType>
	constexpr static const auto is_optional = IsOptionalHelper<ParameterType>::value;
	
	template<typename ParameterType>
	concept OptionalConcept = is_optional<ParameterType>;

	template<typename ParameterType>
	using OptionalReference = std::optional<std::reference_wrapper<ParameterType>>;
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

