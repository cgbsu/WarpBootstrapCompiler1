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
			|| CanidateParameterConstant == NodeType::Subtract;

	constexpr static const char integral_multiply_operation_name[] = "multmp";
	constexpr static const char integral_add_operation_name[] = "addtmp";
	constexpr static const char integral_sub_operation_name[] = "subtmp";

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
						integral_multiply_operation_name
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
						integral_add_operation_name
					);
			}
			case NodeType::Subtract
			: {
				return context->builder.CreateSub(
						left, 
						right, 
						integral_sub_operation_name
					);
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
		std::optional<ReduceToType> to_value() const {return std::nullopt;}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};

	template<typename ReduceToParameterType, NodeType NotImplementedTagParameterConstant>
	struct Translator<Target::LLVM, ReduceToParameterType, NotImplementedTagParameterConstant>
	{
		Translator(
				Context* constructing_context, // Weird bug with llvm
				const auto* top_level_syntax_context, 
				const Node<NotImplementedTagParameterConstant>* node, 
				bool debug
			) { std::cerr << "Target LLVM: Not yet implemented\n"; }
		std::optional<ReduceToParameterType> to_value() const {std::cerr << "NOT IMPLEMENTED!\n"; return std::nullopt; }
	};
}

#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__MATHEMATICAL_EXPRESSIONS_TRANSLATOR__HPP


