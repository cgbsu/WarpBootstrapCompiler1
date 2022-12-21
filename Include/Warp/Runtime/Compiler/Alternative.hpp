#include <Warp/Runtime/Compiler/AlternativePrototype.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__ALTERNATIVE__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__ALTERNATIVE__HPP

namespace Warp::Runtime::Compiler
{
	template<
			typename SingleParameterParameterType, 
			typename FunctionTreeStorageParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct Alternative
	{
		using SingleParameterType = SingleParameterParameterType;
		using FunctionTreeStorageType = FunctionTreeStorageParameterType;
		using IdentifierType = IdentifierParameterType;

		constexpr Alternative(
				AlternativePrototype<
						SingleParameterParameterType, 
						IdentifierParameterType
				>&& from, 
				FunctionTreeStorageParameterType function_body
			) : 
					name(from.name), 
					parameter_count_(parameters.size()), 
					parameters(std::move(from.parameters)), 
					function_body(std::move(function_body)) {}

		constexpr const IdentifierType get_name() const noexcept {
			return name;
		}
		constexpr const FunctionTreeStorageType& get_function_body() const noexcept {
			return function_body;
		}
		inline constexpr const size_t parameter_count() const noexcept {
			return parameter_count_;
		}
		constexpr const std::vector<SingleParameterType>& get_parameters() const noexcept {
			return parameters;
		}

		protected: 
			IdentifierType name;
			size_t parameter_count_;
			std::vector<SingleParameterType> parameters;
			FunctionTreeStorageType function_body;
	};

	//template<
	//		typename SingleParameterParameterType, 
	//		typename FunctionTreeStorageParameterType, 
	//		typename IdentifierParameterType = std::string
	//	>
	//struct Alternative
	//{
	//	using SingleParameterType = SingleParameterParameterType;
	//	using FunctionTreeStorageType = FunctionTreeStorageParameterType;
	//	using IdentifierType = IdentifierParameterType;
	//	using ParameterView = ArrayView<SingleParameterType>;
	//	using AlternativeType = std::unique_ptr<Alternative>;

	//	constexpr Alternative(const IdentifierType name) noexcept : name(name) {}

	//	constexpr virtual const ParameterView get_parameters() const noexcept = 0;
	//	constexpr virtual const size_t parameter_count() const noexcept = 0;
	//	constexpr virtual const FunctionTreeStorageType& function_tree() const noexcept = 0;

	//	constexpr const IdentifierType& get_name() const noexcept {
	//		return name;
	//	}

	//	protected: 
	//		const IdentifierType name;
	//};

	//template<
	//		size_t ParameterCountParameterConstant, 
	//		typename SingleParameterParameterType, 
	//		typename FunctionTreeStorageParameterType, 
	//		typename IdentifierParameterType = std::string
	//	>
	//struct CountedParameterAlternative : public Alternative<
	//		SingleParameterParameterType, 
	//		FunctionTreeStorageParameterType, 
	//		IdentifierParameterType
	//	>
	//{
	//	constexpr static const size_t parameter_count_ = ParameterCountParameterConstant; 

	//	using SingleParameterType = SingleParameterParameterType;
	//	using FunctionTreeStorageType = FunctionTreeStorageParameterType;
	//	using IdentifierType = IdentifierParameterType;
	//	using ParameterView = ArrayView<SingleParameterType>;

	//	using BaseType = Alternative<
	//			SingleParameterType, 
	//			FunctionTreeStorageType, 
	//			IdentifierType
	//		>;

	//	using ThisType = CountedParameterAlternative<
	//			parameter_count_, 
	//			SingleParameterType, 
	//			FunctionTreeStorageType, 
	//			IdentifierType
	//		>;

	//	using PrototypeType = CountedParameterAlternativePrototype<
	//			parameter_count_, 
	//			SingleParameterType, 
	//			IdentifierType
	//		>;

	//	using AlternativeType = std::unique_ptr<BaseType>;

	//	using AlternativePrototypeType = std::unique_ptr<AlternativeType>;

	//	constexpr CountedParameterAlternative(
	//			std::unique_ptr<PrototypeType> from, 
	//			FunctionTreeStorageType function_tree_
	//		) noexcept :
	//				BaseType(from.get_name()), 
	//				prototype(std::move(prototype)), 
	//				function_tree_(std::move(function_tree_)) 
	//		{}
	//				
	//	constexpr CountedParameterAlternative(
	//			IdentifierType name, 
	//			FunctionTreeStorageType function_tree_, 
	//			std::convertible_to<SingleParameterType> auto... parameters_
	//		) noexcept : 
	//				BaseType(name), 
	//				function_tree_(std::move(function_tree_)), 
	//				prototype(std::make_unique<PrototypeType>(name, std::move(parameters_)...)) {}

	//	constexpr virtual const ParameterView get_parameters() const noexcept final {
	//		prototype->get_parameters();
	//	}

	//	constexpr virtual const size_t parameter_count() const noexcept final {
	//		return parameter_count_;
	//	}

	//	constexpr virtual const FunctionTreeStorageType& function_tree() const noexcept final {
	//		return function_tree_;
	//	}

	//	protected: 
	//		const std::unique_ptr<PrototypeType> prototype;
	//		const FunctionTreeStorageType function_tree_;
	//};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__ALTERNATIVE__HPP

