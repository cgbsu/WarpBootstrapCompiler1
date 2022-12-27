#include <Warp/SyntaxTranslation/LLVM/Translators/MathematicalExpressions.hpp>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__CONSTANT_CALL__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__CONSTANT_CALL__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	template<typename ReduceToParameterType, NodeType ConstantCallNodeTypeParameterConstant>
	requires(ConstantCallNodeTypeParameterConstant == NodeType::ConstantCall)
	struct Translator<Target::LLVM, ReduceToParameterType, ConstantCallNodeTypeParameterConstant>
	{
		constexpr static const auto target = Target::LLVM;
		constexpr static const auto tag = NodeType::ConstantCall;
		using ReduceToType = ReduceToParameterType;
		llvm::Value* value;
		Translator(
				Context* constructing_context, // Weird bug with llvm
				const auto* top_level_syntax_context, 
				const Node<ConstantCallNodeTypeParameterConstant>* node, 
				bool debug
			)
		{
			std::cout << "Got name: " << node->name << "\n";
			for(const auto& parameter : constructing_context->symbol_table)
				std::cout << "In table: " << parameter.first << "\n";
			std::cout << "To Name: " << constructing_context->to_name(node->name) << "\n";
			value = constructing_context->symbol_table.at(constructing_context->to_name(node->name));
		}
		std::optional<ReduceToType> to_value() const
		{
			if constexpr(std::is_same_v<ReduceToType, llvm::Value*> == true)
			{
				if(value == nullptr)
					return std::nullopt;
				return value;
			}
			else
				return std::nullopt;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};

}

#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__CONSTANT_CALL__HPP

