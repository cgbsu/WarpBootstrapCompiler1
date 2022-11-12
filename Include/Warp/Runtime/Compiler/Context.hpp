#include <Warp/Runtime/Compiler/Function.hpp>
#include <Warp/Runtime/Compiler/Constant.hpp>
#include <Warp/Runtime/Compiler/Constraint.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/Utilities/Any.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__CONTEXT__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__CONTEXT__HPP

namespace Warp::Runtime::Compiler
{
	template<
			typename TypeTagParameterType = AnyType, 
			typename ValueStorageParameterType = SyntaxNode, 
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

		using ConstantType = Constant<ValueStorageType, TypeTagType, IdentifierType>;
		using ConstraintType = Constraint<ConstraintStorageType, IdentifierType>;
		using SingleParameterType = SingleParameter<ConstraintType, IdentifierType>;
		using AlternativeType = Alternative<
				SingleParameterType, 
				FunctionTreeStorageType, 
				IdentifierType
			>;
		using FunctionType = Function<AlternativeType, IdentifierType>;

		HashMap<IdentifierType, ConstantType> constants;
		HashMap<IdentifierType, std::unique_ptr<FunctionType>> functions;
		Context inject(std::unique_ptr<AlternativeType> alternative)
		{
			if(functions.contains(alternative->get_name()) == false)
			{
				auto new_pair = std::make_pair(
						alternative->get_name(), 
						std::move(std::make_unique<FunctionType>(std::move(alternative)))
					);
				functions.insert(std::move(new_pair));
			}
			else
				functions.at(alternative->get_name())->add_alternative(std::move(alternative));
			return std::move(*this);
		}
		Context inject(ConstantType constant) {
			constants.insert({constant.name, std::move(constant)});
			return std::move(*this);
		}
	};

	using DefaultContextType = Context<
			AnyType, 
			SyntaxNode, 
			SyntaxNode, 
			SyntaxNode, 
			SyntaxNode, 
			std::string, 
			std::unordered_map
		>;

	using ConstantType = DefaultContextType::ConstantType;
	using ContextType = DefaultContextType;
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__CONTEXT__HPP

