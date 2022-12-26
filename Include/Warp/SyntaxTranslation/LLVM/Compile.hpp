#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APFixedPoint.h>
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/Transforms/Utils/IntegerDivision.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__COMPILE__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__COMPILE__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	struct Context
	{
		std::string name;
		llvm::LLVMContext context;
		llvm::Module module;
    	llvm::IRBuilder<> builder;
    	std::unordered_map<std::string, llvm::Value*> symbol_table;
    	Context(std::string name = "WarpModule") : 
				name(name), 
				context(),
    	        module(name, context),
    	        builder(context) {}
	};

	enum class Target { LLVM };
	template<auto TargetParameterConstant, typename ReduceToParameterType, NodeType>
	struct Translator {};
	
	template<auto TargetParameterConstant, typename ReduceToParameterType>
	static auto translate(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			const BaseNode* node, 
			bool debug
		) -> std::optional<ReduceToParameterType>
	{
		return visit<
				ReduceToParameterType, 
				[](auto node, const auto* top_level_syntax_tree_context, Context* constructing_context, bool debug)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					if(debug == true)
						std::cout << "Tag: " << to_string(tag) << "\n";
					using TranslatorType = decltype(Translator<
							TargetParameterConstant, 
							ReduceToParameterType, 
							tag
						>(
								constructing_context, 
								top_level_syntax_tree_context, 
								node, 
								debug
						));
					using ReductionType = decltype(std::declval<TranslatorType>().to_value());
					if constexpr(std::is_convertible_v<std::optional<ReduceToParameterType>, ReductionType>)
					{
						std::cout << "Recducing\n";
						auto translator = TranslatorType(
								constructing_context, 
								top_level_syntax_tree_context, 
								node, 
								debug
							);
						std::cout << "Translated, returning value.\n";
						return translator.to_value();
					}
					else
					{
						std::cerr << "Error in translate, non-convertable reduction type\n";
						if(debug == true)
						{
							std::cerr << "translate(context): Type not convertable: " 
									<< friendly_type_name<ReduceToParameterType>() << " from " 
									<< friendly_type_name<ReductionType>() << "\n";
						}
						return std::nullopt;
					}
				}
			>(node->to_view(), top_level_syntax_tree_context, constructing_context, debug);
	}
}
#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__COMPILE__HPP

