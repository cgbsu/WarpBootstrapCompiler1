#include <Warp/SyntaxTranslation/LLVM/Translators/ConstantCall.hpp>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_CALL__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_CALL__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	template<typename ReduceToParameterType>
	struct Translator<Target::LLVM, ReduceToParameterType, NodeType::FunctionCall>
	{
		constexpr static const auto target = Target::LLVM;
		constexpr static const auto tag = NodeType::FunctionCall;
		using ReduceToType = ReduceToParameterType;
		const size_t argument_count;
		std::string callee;
		std::optional<llvm::Function*> function;
		std::vector<llvm::Value*> arguments;
		llvm::Value* call;
		Translator(
				Context* constructing_context, // Weird bug with llvm
				const auto* top_level_syntax_context, 
				const Node<NodeType::FunctionCall>* node, 
				bool debug
			) : argument_count (node->arguments.size()), 
				callee(node->name)
		{
			function = constructing_context->retrieve_function(
					callee, 
					argument_count
				);
			if(function.has_value() == true)
			{
				for(const auto& argument : node->arguments)
				{
					auto argument_value_ = translate<Target::LLVM, llvm::Value*>(
							constructing_context, 
							top_level_syntax_context, 
							argument.get(), 
							debug
						);
					if(argument_value_.has_value() == false) {
						call = nullptr;
						break;
					}
					arguments.push_back(argument_value_.value());
				}
				if(arguments.size() == argument_count)
				{
					call = constructing_context->builder.CreateCall(
							(llvm::FunctionType*) function.value()->getType()->getPointerElementType(), 
							dynamic_cast<llvm::Value*>(function.value()), 
							llvm::ArrayRef<llvm::Value*>(arguments)
						);
				}
				else
					call = nullptr;
			}
			else
				call = nullptr;
		}
		std::optional<ReduceToType> to_value() const
		{
			if constexpr(std::is_same_v<ReduceToType, llvm::Value*> == true)
			{
				if(call == nullptr)
					return std::nullopt;
				return call;
			}
			else
				return std::nullopt;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};
}

#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_CALL__HPP

