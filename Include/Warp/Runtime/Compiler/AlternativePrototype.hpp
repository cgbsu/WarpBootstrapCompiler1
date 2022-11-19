#include <Warp/Common.hpp>
#include <Warp/Runtime/Compiler/SingleParameter.hpp>
#include <Warp/Utilities/ArrayView.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__ALTERNATIVE_PROTOTYPE__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__ALTERNATIVE_PROTOTYPE__HPP

namespace Warp::Runtime::Compiler
{
	template<
			size_t ParameterCountParameterConstant, 
			typename SingleParameterParameterType, 
			typename IdentifierParameterType
		>
	struct CountedParameterAlternativePrototype;

	template<
			size_t ParameterCountParameterConstant, 
			typename SingleParameterParameterType, 
			typename FunctionTreeStorageParameterType, 
			typename IdentifierParameterType
		>
	struct CountedParameterAlternative;

	template<
			typename SingleParameterParameterType, 
			typename FunctionTreeStorageParameterType, 
			typename IdentifierParameterType
		>
	struct Alternative;

	template<
			typename SingleParameterParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct AlternativePrototype
	{
		using SingleParameterType = SingleParameterParameterType;
		using IdentifierType = IdentifierParameterType;
		using ParameterView = ArrayView<SingleParameterType>;

		using AlternativePrototypeType = std::unique_ptr<AlternativePrototype>;

		using ThisType = AlternativePrototype<SingleParameterType, IdentifierType>;

		template<typename FunctionTreeStorageParameterType>
		using AlternativeType = std::unique_ptr<Alternative<
				SingleParameterType, 
				FunctionTreeStorageParameterType, 
				IdentifierType
			>>;

		template<
				size_t ParameterCountParameterConstant, 
				typename FunctionTreeStorageParameterType
			>
		using CountedParameterAlternativeType = std::unique_ptr<
				CountedParameterAlternative< 
						ParameterCountParameterConstant, 
						SingleParameterType, 
						FunctionTreeStorageParameterType, 
						IdentifierType
					>
			>;

		template<size_t ParameterCountParameterConstant>
		using CountedParameterAlternativePrototypeType = std::unique_ptr<
				CountedParameterAlternativePrototype<
						ParameterCountParameterConstant, 
						SingleParameterType, 
						IdentifierType
					>
			>;

		constexpr AlternativePrototype(const IdentifierType name) noexcept : name(name) {}

		constexpr virtual const ParameterView get_parameters() const noexcept = 0;
		constexpr virtual const size_t parameter_count() const noexcept = 0;
		constexpr virtual const ParameterView add_parameter(
				SingleParameterType parameter
			) const noexcept = 0;

		constexpr const IdentifierType& get_name() const noexcept {
			return name;
		}

		template<typename FunctionTreeStorageParameterType>
		constexpr static AlternativeType<FunctionTreeStorageParameterType> to_alternative(
				AlternativePrototypeType prototype, 
				FunctionTreeStorageParameterType function_tree
			)
		{
			return to_alternative<0, FunctionTreeStorageParameterType>(
					std::move(prototype), 
					std::move(function_tree)
				);
		}

		protected: 
			IdentifierType name;

			template<size_t ParameterCountParameterConstant, typename FunctionTreeStorageParameterType>
			constexpr static AlternativeType<FunctionTreeStorageParameterType> to_alternative(
					AlternativePrototypeType prototype, 
					FunctionTreeStorageParameterType function_tree
				)
			{
				using CountedAlternativeType = CountedParameterAlternativeType<
						ParameterCountParameterConstant, 
						FunctionTreeStorageParameterType
					>;
				using CountedAlternativePrototypeType = CountedParameterAlternativePrototypeType<
						ParameterCountParameterConstant
					>;
				if constexpr(ParameterCountParameterConstant < prototype->parameter_count())
				{
					return to_alternative<
							ParameterCountParameterConstant + 1, 
							FunctionTreeStorageParameterType
						>(
							std::move(prototype), 
							std::move(function_tree)
						);
				}
				else 
				{
					return std::make_unique<CountedAlternativeType>(
							Utilities::dynamic_cast_unique<
									CountedAlternativePrototypeType, 
									ThisType	
								>(prototype), 
							std::move(function_tree)
						);
				}
			}
	};


	template<
			size_t ParameterCountParameterConstant, 
			typename SingleParameterParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct CountedParameterAlternativePrototype : public AlternativePrototype<
			SingleParameterParameterType, 
			IdentifierParameterType
		>
	{
		constexpr const static auto parameter_count_ 
				= ParameterCountParameterConstant;
		using SingleParameterType = SingleParameterParameterType;
		using IdentifierType = IdentifierParameterType;
		using ParameterView = ArrayView<SingleParameterType>;
		using BaseType = AlternativePrototype<
				SingleParameterType, 
				IdentifierType
			>;

		using ThisType = CountedParameterAlternativePrototype<
				parameter_count_, 
				SingleParameterType, 
				IdentifierType
			>;

		using NextType = CountedParameterAlternativePrototype<
				parameter_count_ + 1, 
				SingleParameterType, 
				IdentifierType
			>;

		using AlternativePrototypeType = std::unique_ptr<BaseType>;

		using ParameterArrayType = std::array<SingleParameterType, parameter_count_>;

		constexpr CountedParameterAlternativePrototype(
				const IdentifierType name, 
				std::convertible_to<SingleParameterType> auto... parameters
			) noexcept : BaseType(name), parameters{std::move(parameters)...} {}

		template<size_t... IndexParameterConstants>
		constexpr CountedParameterAlternativePrototype(
				ThisType from, 
				SingleParameterType parameter, 
				std::index_sequence<IndexParameterConstants...> = std::make_index_sequence<parameter_count_>()
			) noexcept : 
					BaseType(from.name), 
					parameters{
							std::move(from.get_parameters()[IndexParameterConstants])..., 
							std::move(parameter)
						} {}

		constexpr virtual const ParameterView get_parameters() const noexcept final {
			return ParameterView(parameters);
		}

		constexpr const ParameterArrayType copy_parameters() const noexcept {
		}

		constexpr virtual const size_t parameter_count() const noexcept final {
			return parameter_count_;
		}

		constexpr virtual const ParameterView add_parameter(
					SingleParameterType parameter
				) const noexcept final {
			return ParameterView(parameters);
		}

		protected: 
			const ParameterArrayType parameters;
	};
	
	template<
			typename SingleParameterType, 
			typename IdentifierParameterType = std::string
		>
	constexpr auto make_alternative_prototype_with_no_parameters(IdentifierParameterType name) 
	{
		auto concrete = std::make_unique<CountedParameterAlternativePrototype<
				0, 
				SingleParameterType, 
				IdentifierParameterType
			>>(name);
		return std::unique_ptr<AlternativePrototype<
				SingleParameterType, 
				IdentifierParameterType
			>>(concrete.release());
	}

	template<typename IdentifierParameterType = std::string>
	constexpr auto make_alternative_prototype(IdentifierParameterType name, auto... parameters) 
	{
		using SingleParameterType = CleanType<
				typename TakeFirstType<decltype(parameters)...>::Type
			>;
		constexpr const size_t parameter_count = sizeof...(parameters);
		auto concrete = std::make_unique<CountedParameterAlternativePrototype<
				parameter_count, 
				SingleParameterType, 
				IdentifierParameterType
			>>(name, std::move(parameters)...);
		return std::unique_ptr<AlternativePrototype<
				SingleParameterType, 
				IdentifierParameterType
			>>(concrete.release());
	}
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__ALTERNATIVE_PROTOTYPE__HPP

