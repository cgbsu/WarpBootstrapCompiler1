#include <Warp/Common.hpp>
#include <Warp/Utilities/Conversions.hpp>
#include <Warp/Utilities/TypeHash.hpp>
#include <Warp/Utilities/Templates.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__ANY__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__ANY__HPP

namespace Warp::Utilities
{
	template<typename ParameterType>
	struct Any;

	struct AnyBase
	{
		using AnyType = std::unique_ptr<AnyBase>;

		template<typename CanidateParameterType>
		using ReferenceWrappedOptionalType = std::optional<
				const std::reference_wrapper<const Any<CanidateParameterType>>
			>;

		virtual constexpr const std::partial_ordering compare(const AnyBase& other) const noexcept = 0;
		virtual constexpr const size_t* get_type_hash() const noexcept = 0;

		/*constexpr*/ const std::partial_ordering compare(const AnyType& other) const noexcept {
			return compare(*other.get()); // TODO: unique_ptr::get should be constexpr, but is not yet...
		}

		template<typename CanidateParameterType>
		constexpr const bool holds_type() const noexcept {
			return get_type_hash() == type_pointer_hash<CanidateParameterType>;
		}

		template<typename CanidateParameterType>
		constexpr auto to() const noexcept
				-> const ReferenceWrappedOptionalType<CanidateParameterType>
		{
			if(holds_type<CanidateParameterType>() == true)
				return std::optional{*dynamic_cast<const Any<CanidateParameterType>*>(this)};
			else
				return std::nullopt;
		}

		template<typename CanidateParameterType, typename FunctionPrototypeParameterType>
		constexpr auto if_type(
				const std::function<FunctionPrototypeParameterType> operate
			) const noexcept
		{
			if constexpr(const auto to_operate_on = to<CanidateParameterType>(); 
						to_operate_on != std::nullopt)
				return operate(to_operate_on.value());
			else
				return std::nullopt;
		}

		constexpr const std::partial_ordering operator<=>(const AnyBase& other) 
				const noexcept {
			return compare(other);
		}
		/*constexpr*/ const std::partial_ordering operator<=>(const AnyType& other) 
				const noexcept {
			return compare(*other.get()); // TODO: unique_ptr::get should be constexpr, but is not yet...
		}

		/* TODO: Revisit
		template<typename CanidateParameterType, typename FunctionPrototypeParameterType>
		constexpr auto first(
				const std::function<FunctionPrototypeParameterType> operate
			) const noexcept
		{
			if constexpr(
						const auto result = if_type<CanidateParameterType>(operate); 
						result != std::nullopt
					)

		}
		template<bool EnableParameterConstant, typename... ValueParameterTypes>
		struct Chain
		{
			template<typename CanidateParameterType, typename FunctionPrototypeParameterType>
			constexpr const auto then(
						const std::function<FunctionPrototypeParameterType> operate
					) const noexcept {
				return then_implementation(operate);
			}
			constexpr const auto unwrap() const noexcept {
				return std::get<sizeof...(ValueParameterTypes) - 1>(accumulated_values);
			}

			constexpr const auto unwrap_all() const noexcept {
				return accumulated_values;
			}

			protected: 
				std::tuple<ValueParameterTypes...> accumulated_values;

				template<
						typename CanidateParameterType, 
						typename FunctionPrototypeParameterType, 
						size_t... TypeIndexSequenceParameterConstants
					>
				constexpr const auto then_implementation(
						const std::function<FunctionPrototypeParameterType> operate
						std::index_sequence<TypeIndexSequenceParameterConstants...> 
								= std::make_index_sequence<sizeof...(ValueParameterTypes)>()
					) const noexcept
				{
					constexpr const auto result = if_type<CanidateParameterType>(
							[&](const CanidateParameterType& value)
							{
								return operate(
										value, 
										std::forward(std::get<
												TypeIndexSequenceParameterConstants
											>(accumulated_values))...
									);
							}
						);
					if constexpr(std::is_convertible_v<std::nullopt_t, decltype(result)> == true)
					{
						if constexpr(result != std::nullopt)
							return Chain<false>{};
					}
					else
					{
						return Chain<true>{
								std::move(std::get<
										TypeIndexSequenceParameterConstants
									>(accumulated_values))..., 
								std::move(result.value())
							};
					}
				}
		};

		template<typename... ValueParameterTypes>
		struct Chain<false, ValueParameterTypes...>
		{
			template<typename CanidateParameterType, typename FunctionPrototypeParameterType>
			constexpr const auto then(
						const std::function<FunctionPrototypeParameterType> operate
					) const noexcept {
				return Chain<false>{};
			}
			constexpr const auto unwrap() const noexcept {
				return std::nullopt;
			}
			constexpr const auto unwrap_all() const noexcept {
				return std::nullopt;
			}
		};
		*/
	};

	template<typename ParameterType>
	struct Any : public AnyBase
	{
		using Type = ParameterType;
		using AnyType = std::unique_ptr<AnyBase>;

		constexpr Any(std::in_place_type_t<ParameterType>, auto... initialization_parameters) 
				noexcept : value(initialization_parameters...) {}
		constexpr Any(auto... initialization_parameters) 
				noexcept : value(initialization_parameters...) {}
		constexpr Any(const Any& other) = default;
		constexpr Any(Any&& other) = default;

		constexpr ~Any() = default;

		constexpr Any& operator=(const Any& other) = default;
		constexpr Any& operator=(Any&& other) = default;

		virtual constexpr const std::partial_ordering compare(const AnyBase& other) 
				const noexcept final
		{
			if(other.holds_type<ParameterType>() == true)
			{
				const Any<ParameterType>& other_concrete 
						= other.to<ParameterType>().value();
				if constexpr(is_comparible<ParameterType, ParameterType> == true)
					return std::compare_strong_order_fallback(other_concrete.get_value(), value);
				else if constexpr(is_equally_comparible<ParameterType, ParameterType> == true)
					return other_concrete.get_value() == value;
				else
					return std::partial_ordering::unordered;
			}
			else
				return std::partial_ordering::unordered;
		}
		constexpr const size_t* get_type_hash() const noexcept final {
			return type_pointer_hash<ParameterType>;
		}
		constexpr const Type& get_value() const noexcept {
			return value;
		}
		constexpr Type& get_value() noexcept {
			return value;
		}
		protected: 
			Type value;
	};

	using AnyType = std::unique_ptr<AnyBase>;

	template<typename ToEraseParameterType>
	constexpr const AnyType make_any(auto... initialization_parameters) noexcept
	{
		return std::unique_ptr<AnyBase>(
				new Any<ToEraseParameterType>(
						initialization_parameters...
					)
			);
	}

	template<typename ToEraseParameterType>
	constexpr const AnyType make_clean_any(auto... initialization_parameters) noexcept {
		return make_any<CleanType<ToEraseParameterType>>(initialization_parameters...);
	}

	template<typename ToEraseParameterType>
	constexpr const AnyType make_const_clean_any(auto... initialization_parameters) noexcept {
		return make_any<const CleanType<ToEraseParameterType>>(initialization_parameters...);
	}

	constexpr const auto to_tag(auto tag) {
		return make_const_clean_any<decltype(tag)>(tag);
	}
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__ANY__HPP


