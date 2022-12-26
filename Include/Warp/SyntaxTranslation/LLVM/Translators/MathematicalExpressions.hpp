#include <Warp/SyntaxTranslation/LLVM/Translators/LiteralTranslator.hpp>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__MATHEMATICAL_EXPRESSIONS_TRANSLATOR__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__MATHEMATICAL_EXPRESSIONS_TRANSLATOR__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	template<NodeType CanidateParameterConstant>
	constexpr static bool is_binary_arithmatic_operator 
			= CanidateParameterConstant == NodeType::Multiply 
			|| CanidateParameterConstant == NodeType::Divide 
			|| CanidateParameterConstant == NodeType::Add 
			|| CanidateParameterConstant == NodeType::Subtract
			|| CanidateParameterConstant == NodeType::Equal
			|| CanidateParameterConstant == NodeType::LessThan 
			|| CanidateParameterConstant == NodeType::GreaterThan 
			|| CanidateParameterConstant == NodeType::LessThankOrEqualTo 
			|| CanidateParameterConstant == NodeType::GreaterThanOrEqualTo;

	template<NodeType AritmaticOperationTag>
	requires(is_binary_arithmatic_operator<AritmaticOperationTag> == true)
	llvm::Value* to_binary_arithmatic_operation(Context* context, llvm::Value* left, llvm::Value* right)
	{
		switch(AritmaticOperationTag)
		{
			case NodeType::Multiply 
			: {
				return context->builder.CreateMul(
						left, 
						right, 
						"multmp"
					);
			}
			case NodeType::Divide : {
				std::cout << "DIVISION NOT YET IMPLEMENTED!\n"; //TODO
				return nullptr;
				//return llvm::generateSignedRemainderCode(left, right, context->builder);
			}
			case NodeType::Add 
			: {
				return context->builder.CreateAdd(
						left, 
						right, 
						"addtmp"
					);
			}
			case NodeType::Subtract
			: {
				return context->builder.CreateSub(
						left, 
						right, 
						"subtmp"
					);
			}
			case NodeType::GreaterThan
			: {
				llvm::Value* comparison = context->builder.CreateICmpSGT(
						left, 
						right, 
						"tmpgt"
				);
				return comparison;
			}
			case NodeType::LessThan
			: {
				llvm::Value* comparison = context->builder.CreateICmpSLT(
						left, 
						right, 
						"tmplt"
				);
				return comparison;
			}
			case NodeType::LessThankOrEqualTo // TODO: Rename to LessThankOrEqualTo
			: {
				llvm::Value* comparison = context->builder.CreateICmpSLE(
						left, 
						right, 
						"tmpleq"
				);
				return comparison;
			}
			case NodeType::GreaterThanOrEqualTo
			: {
				llvm::Value* comparison = context->builder.CreateICmpSGE(
						left, 
						right, 
						"tmpgeq"
				);
				return comparison;
			}
			case NodeType::Equal
			: {
				llvm::Value* comparison = context->builder.CreateICmpEQ(
						left, 
						right, 
						"tmpeq"
				);
				return comparison;
			}
			case NodeType::LogicalAnd
			: {
				llvm::Value* comparison = context->builder.CreateLogicalAnd(
						left, 
						right, 
						"tmpand"
				);
				return comparison;
			}
			case NodeType::LogicalOr
			: {
				llvm::Value* comparison = context->builder.CreateLogicalOr(
						left, 
						right, 
						"tmpor"
				);
				return comparison;
			}
			default : {
				return nullptr;
			}
		}
	}

	template<typename ReduceToParameterType, NodeType BinaryOperationTagParameterConstant>
	requires(is_binary_arithmatic_operator<BinaryOperationTagParameterConstant> == true)
	struct Translator<Target::LLVM, ReduceToParameterType, BinaryOperationTagParameterConstant>
	{
		constexpr static const auto target = Target::LLVM;
		constexpr static const auto tag = BinaryOperationTagParameterConstant;
		using ReduceToType = ReduceToParameterType;
		const SyntaxNode& left;
		const SyntaxNode& right;
		using ValueType = llvm::Value*; // TODO: Change this to account for mutable variables
		ValueType value;
		Translator(
				Context* constructing_context, // Weird bug with llvm
				const auto* top_level_syntax_context, 
				const Node<BinaryOperationTagParameterConstant>* node, 
				bool debug
			) : 
				left(node->left), 
				right(node->right), 
				value(to_binary_arithmatic_operation<BinaryOperationTagParameterConstant>(
						constructing_context, 
						translate<target, ValueType>(
								constructing_context, 
								top_level_syntax_context, 
								left.get(), 
								debug
							).value(), 
						translate<target, ValueType>(
								constructing_context, 
								top_level_syntax_context, 
								right.get(), 
								debug
							).value()
					)) {}
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

#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__MATHEMATICAL_EXPRESSIONS_TRANSLATOR__HPP


