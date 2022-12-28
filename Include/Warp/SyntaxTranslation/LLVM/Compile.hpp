#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <filesystem>

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
#include <llvm/Target/TargetOptions.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>
//#include <clang/Driver/Driver.h>
//#include <clang/Driver/Compilation.h>
//#include <clang/Frontend/TextDiagnosticPrinter.h>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__COMPILE__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__COMPILE__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	struct FunctionParameter {
		std::string name;
		llvm::Type* type;
	};

	struct Context
	{
		std::string name;
		llvm::LLVMContext context;
		llvm::Module module;
    	llvm::IRBuilder<> builder;
    	std::unordered_map<std::string, llvm::Value*> symbol_table;
    	std::unordered_map<std::string, llvm::Function*> function_table;
		std::unordered_map<std::string, std::string> replace_names;
    	Context(std::string name = "WarpModule") : 
				name(name), 
				context(),
    	        module(name, context),
    	        builder(context) {}
		std::string to_name(const std::string from_name) const noexcept
		{
			if(replace_names.contains(from_name) == true)
				return replace_names.at(from_name);
			return from_name;
		}
		std::optional<llvm::Function*> retrieve_function(std::string name, const size_t argument_count)
		{
			std::string alternative_name = name + std::string{"_"} + std::to_string(argument_count);
			if(function_table.contains(alternative_name) == true)
				return function_table.at(alternative_name);
			return std::nullopt;
		}
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
						auto translator = TranslatorType(
								constructing_context, 
								top_level_syntax_tree_context, 
								node, 
								debug
							);
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

	const static std::string default_cpu = "generic";
	const static std::string default_features = "";
	const static llvm::TargetOptions default_options;
	const static llvm::Optional<llvm::Reloc::Model> default_reloc_model = llvm::Optional<llvm::Reloc::Model>();
	const static bool default_debug = false;

	void initialize_llvm();

	std::optional<llvm::TargetMachine*> initialize_target(
			llvm::Module& to_compile, 
			std::string target_triple, 
			std::string target_cpu = default_cpu, 
			std::string target_features = default_features, 
			llvm::TargetOptions target_options = default_options, 
			llvm::Optional<llvm::Reloc::Model> target_reloc_model = default_reloc_model
		);

	bool write_object_file(
			std::filesystem::path output_file_path, 
			llvm::Module& to_write, 
			llvm::TargetMachine* target_machine
		);

	bool compile(
			Context* context, 
			std::string code, 
			std::filesystem::path output_file_path, 
			std::string target_triple, 
			std::string target_cpu = default_cpu, 
			std::string target_features = default_features, 
			llvm::TargetOptions target_options = default_options, 
			llvm::Optional<llvm::Reloc::Model> target_reloc_model = default_reloc_model, 
			bool debug = default_debug
		);
}
#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__COMPILE__HPP

