#include <Warp/SyntaxTranslation/LLVM/Compile.hpp>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__LITERAL_TRANSLATOR__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__LITERAL_TRANSLATOR__HPP
namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;
	
	template<NodeType CanidateParameterConstant>
	constexpr static bool is_literal_node = 
			CanidateParameterConstant == NodeType::LiteralWhole 
					|| CanidateParameterConstant == NodeType::LiteralInteger 
					|| CanidateParameterConstant == NodeType::LiteralCharacter 
					|| CanidateParameterConstant == NodeType::LiteralFixed 
					|| CanidateParameterConstant == NodeType::LiteralBool;

	template<NodeType LiteralTagParameterConstant>
	llvm::Value* to_llvm_value(Context* context, NumericValue from)
	{
		if constexpr(LiteralTagParameterConstant == NodeType::LiteralWhole)
		{
			auto value = from.to<NumericTypeResolver<NumericTypeTag::Whole>::Type>().value();
			return llvm::ConstantInt::get(
					context->context, 
					llvm::APInt(
							static_cast<unsigned>(value.bits), 
							static_cast<uint64_t>(value.number), 
							false
						)
				);
		}
		else if constexpr(LiteralTagParameterConstant == NodeType::LiteralInteger)
		{
			auto value = from.to<NumericTypeResolver<NumericTypeTag::Integer>::Type>().value();
			return llvm::ConstantInt::get(
					context->context, 
					llvm::APInt(
							static_cast<unsigned>(value.bits), 
							static_cast<uint64_t>(value.number), 
							true
						)
				);
		}
		else if constexpr(LiteralTagParameterConstant == NodeType::LiteralCharacter)
		{
			auto value = from.to<NumericTypeResolver<NumericTypeTag::Character>::Type>().value();
			return llvm::ConstantInt::get(
					context->context, 
					llvm::APInt(
							static_cast<unsigned>(value.bits), 
							static_cast<uint64_t>(value.number), 
							false
						)
				);
		}
		else if constexpr(LiteralTagParameterConstant == NodeType::LiteralFixed)
		{
			std::cerr << "Compiling Fixed Point Numbers Not Yet Supported!\n";
			return nullptr;
			//auto value = from.to<NumericTypeResolver<NumericTypeTag::FixedPoint>::Type>().value();
			//auto fixed = llvm::APFixedPoint(
			//		value.number, 
			//		llvm::FixedPointSemantics(
			//				value.whole_part_bits, 
			//				value.decimal_part_bits, 
			//				true, // Signed
			//				true, // Saturated (something to do with rounding?)
			//				false // Unsigned/Unsigned Padding
			//			)
			//	);
			//llvm::APFixedPoint& fixed_reference = fixed;
			//return llvm::ConstantInt::get(
			//		context->context
			//		static_cast<llvm::APInt&>(static_cast<llvm::APSInt&>(fixed_reference))
			//	);
		}
		else if constexpr(LiteralTagParameterConstant == NodeType::LiteralBool)
		{
			auto value = from.to<NumericTypeResolver<NumericTypeTag::Bool>::Type>().value();
			return llvm::ConstantInt::get(
					context->context, 
					llvm::APInt(
							static_cast<unsigned>(value.bits), 
							static_cast<uint64_t>(value.number), 
							false
						)
				);
		}
		else
			return nullptr;
	}

	template<typename ReduceToParameterType, NodeType LiteralTagParameterConstant>
	requires(is_literal_node<LiteralTagParameterConstant> == true)
	struct Translator<Target::LLVM, ReduceToParameterType, LiteralTagParameterConstant>
	{
		constexpr static const auto target = Target::LLVM;
		constexpr static const auto tag = LiteralTagParameterConstant;
		using ReduceToType = ReduceToParameterType;
		using NumericType = Node<tag>::NumericType;
		llvm::Value* value;
		Translator(
				Context* constructing_context, // Weird bug with llvm
				const auto* top_level_syntax_context, 
				const Node<LiteralTagParameterConstant>* node, 
				bool debug
			) : value(to_llvm_value<LiteralTagParameterConstant>(constructing_context, node->value)) {}
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
#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__LITERAL_TRANSLATOR__HPP

