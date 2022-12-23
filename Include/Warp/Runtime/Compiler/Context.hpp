#include <Warp/Runtime/Compiler/Function.hpp>
#include <Warp/Runtime/Compiler/Constant.hpp>
#include <Warp/Runtime/Compiler/Constraint.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/Utilities/Any.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__CONTEXT__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__CONTEXT__HPP

namespace Warp::Runtime::Compiler
{
	struct ConstantValueType {
		SyntaxNode expression;
		std::optional<NumericValue> value;
	};

	template<
			typename TypeTagParameterType = AnyType, 
			typename ValueStorageParameterType = ConstantValueType, 
			typename FunctionTreeStorageParameterType = SyntaxNode, 
			typename ConstraintStorageParameterType = SyntaxNode, 
			typename SingleParameterStorageParameterType = SyntaxNode, 
			typename IdentifierParameterType = std::string, 
			template<typename, typename> typename HashTableParameterTemplate 
					= std::unordered_map
		>
	struct Context
	{
		using IdentifierType = IdentifierParameterType;
		using ValueStorageType = ValueStorageParameterType;
		using TypeTagType = TypeTagParameterType;
		using ConstraintStorageType = ConstraintStorageParameterType;
		using FunctionTreeStorageType = FunctionTreeStorageParameterType;
		using SingleParameterStorageType = SingleParameterStorageParameterType;

		template<typename KeyParameterType, typename ValueParameterType>
		using HashMap = HashTableParameterTemplate<KeyParameterType, ValueParameterType>;

		using ThisType = Context<
				TypeTagType, 
				ValueStorageType, 
				ConstraintStorageType, 
				FunctionTreeStorageType, 
				SingleParameterStorageType, 
				IdentifierType,
				HashMap
			>;

		using ConstantType = Constant<ValueStorageType, TypeTagType, IdentifierType>;
		using ConstraintType = Constraint<ConstraintStorageType, IdentifierType>;
		using SingleParameterType = SingleParameter<ConstraintType, IdentifierType>;
		using AlternativePrototypeType = AlternativePrototype<SingleParameterType, IdentifierType>;
		using AlternativeType = Alternative<
				SingleParameterType, 
				FunctionTreeStorageType, 
				IdentifierType
			>;
		using FunctionType = Function<AlternativeType, IdentifierType>;

		HashMap<IdentifierType, ConstantType> constants;
		HashMap<IdentifierType, std::unique_ptr<FunctionType>> functions;
		
		constexpr Context(ThisType* parent) : parent(parent) {}
		constexpr Context() : parent(std::nullopt) {}

		void subsume(Context other)
		{
			for(auto&& function : other.functions)
				functions.insert(std::move(function));
			for(auto&& constant : other.constants)
				constants.insert(std::move(constant));
		}

		Context inject(std::unique_ptr<AlternativeType> alternative)
		{
			const auto name = alternative->get_name();
			if(functions.contains(name) == false)
			{
				auto new_pair = std::make_pair(
						name, 
						std::move(std::make_unique<FunctionType>(std::move(alternative)))
					);
				functions.insert(std::move(new_pair));
			}
			else
				functions.at(name)->add_alternative(std::move(alternative));
			return std::move(*this);
		}
		Context inject(ConstantType constant)
		{
			auto name = constant.name;
			constants.insert({name, std::move(constant)});
			return std::move(*this);
		}
		template<size_t CurrentDepthParameterConstant = 0, size_t MaxDepthParameterConstant = 10>
		const OptionalReference<FunctionType> retrieve_function(IdentifierType name) const noexcept
		{
			if(functions.contains(name) == true)
				return OptionalReference<FunctionType>(*functions.at(name).get());
			if constexpr(CurrentDepthParameterConstant < MaxDepthParameterConstant)
			{
				if(parent.has_value() == true)
				{
					return parent.value()->template retrieve_function<
							CurrentDepthParameterConstant + 1, 
							MaxDepthParameterConstant
						>(name);
				}
			}
			return OptionalReference<FunctionType>(std::nullopt);
		}
		template<size_t CurrentDepthParameterConstant = 0, size_t MaxDepthParameterConstant = 10>
		const OptionalReference<ConstantType> retrieve_constant(IdentifierType name) const noexcept
		{
			if(constants.contains(name) == true)
				return OptionalReference<ConstantType>(constants.at(name));
			if constexpr(CurrentDepthParameterConstant < MaxDepthParameterConstant)
			{
				if(parent.has_value() == true)
				{
					return parent.value()->template retrieve_constant<
							CurrentDepthParameterConstant + 1, 
							MaxDepthParameterConstant
						>(name);
				}
			}
			return OptionalReference<ConstantType>{std::nullopt};
		}
		const auto get_parent() const noexcept {
			return parent;
		}
		protected: 
		std::optional<ThisType*> parent;
	};

	using DefaultContextType = Context<
			AnyType, 
			ConstantValueType, 
			SyntaxNode, 
			SyntaxNode, 
			SyntaxNode, 
			std::string, 
			std::unordered_map
		>;

	using ConstantType = DefaultContextType::ConstantType;
	using ConstraintType = DefaultContextType::ConstraintType;
	using AlternativePrototypeType = DefaultContextType::AlternativePrototypeType;
	using AlternativeType = DefaultContextType::AlternativeType;
	using SingleParameterType = DefaultContextType::SingleParameterType;
	using FunctionType = DefaultContextType::FunctionType;
	using ContextType = DefaultContextType;
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__CONTEXT__HPP

