#include <Warp/SyntaxTranslation/LLVM/Translators/ConstantCall.hpp>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_DECLARATION__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_DECLARATION__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	llvm::Value* constraint_condition(
			Context* constructing_context,
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const std::vector<SingleParameterType>& parameters
		)
	{
		llvm::Value* condition = llvm::ConstantInt::get(
				constructing_context->context, 
				llvm::APInt(1, 1, true)
			);
		for(const auto& parameter : parameters)
		{
			std::string name = parameter.name + std::string{"_constrainttmp"};
			llvm::Value* parameter_constraint = translate<Target::LLVM, llvm::Value*>(
					constructing_context,
					top_level_syntax_tree_context, 
					parameter.constraint.constraint, 
					debug
				).value();
			condition = constructing_context->builder.CreateLogicalAnd(
					condition, 
					parameter_constraint, 
					name.c_str()
				);
		}
		return condition;
	}


	llvm::Function* translate_alternatives(
			Context* constructing_context, 
			std::string name, 
			const size_t argument_count, 
			const FunctionType::AlternativesOfUniformArityRowType& alternatives
		)
	{
		std::vector<std::string> parameter_names;

///////////////////////////////
        std::vector<llvm::Type*> parameter_types(argument_count, llvm::Type::getInt32Ty(constructing_context->context));
        llvm::FunctionType* function_type = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(constructing_context->context),    
                parameter_types,
                false
            );
        if(!function_type || function_type == nullptr)
            std::cerr << "Error: Invalid function type!\n";
		llvm::Function* function = llvm::Function::Create(
				function_type, 
				llvm::Function::ExternalLinkage, 
				name.data(), 
				constructing_context->module
			);
        size_t ii = 0;
        constructing_context->symbol_table.clear();
        for(auto& parameter : function->args())
        {
			std::stringstream buffer;
			buffer << name << "_" << argument_count << "_parameter_" << ii;
			parameter_names.push_back(buffer.str());
            parameter.setName(parameter_names[ii].data());
            constructing_context->symbol_table[parameter_names[ii]] = &parameter;
            std::cout << parameter_names[ii] << "\n";
        }
        llvm::BasicBlock* block = llvm::BasicBlock::Create(constructing_context->context, "entry", function);
        constructing_context->builder.SetInsertPoint(block);
		//context->builder->CreateRet(body->codegen());
        llvm::verifyFunction(*function);
        return function;
/////////////////////////////
	}


	//std::pair<llvm::Value*, llvm::BasicBlock*> translate_alternative(
	//		llvm::Function* function, 
	//		Context* constructing_context, 
	//		const size_t argument_count, 
	//		const AlternativeType::AlternativesOfUniformArityRowType& alterantives, 
	//		size_t alternative_index = 0
	//	)
	//{
	//	size_t alternative_count = alternatives.size();

	//	std::stringstream name_buffer;
	//	name_buffer << name << "_" << argument_count << "_" << alternative_index << "_condition";
	//	llvm::Value* condition = constraint_condition(
	//			constructing_context,
	//			top_level_syntax_tree_context, 
	//			debug, 
	//			alternatives[alternative_index]->get_parameters()
	//		)
	//	if(
	//	llvm::BasicBlock* then = llvm::BasicBlock::Create(
	//			constructing_context->context, 
	//			name_buffer.str().c_str()
	//		);
	//		if(branches == nullptr)
	//		{
	//			if(previous_then == nullptr) {
	//				previous_then = then;
	//				previous_condition = condition;
	//			}
	//			else
	//			{
	//				branches = constructing_context->CreateCondBr(
	//						previous_condition, 
	//						previous_then, 
	//						
	//						
	//			}
	//}

	void translate_function(Context* constructing_context, const FunctionType& to_translate)
	{
		size_t argument_count = 0;
		for(const auto& alternatives : to_translate.get_alternatives())
		{
			if(alternatives.size() > 0)
			{
				translate_alternatives(
						constructing_context, 
						to_translate.get_name(), 
						argument_count, 
						alternatives
					);
				++argument_count;
			}
		}
	}

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

#endif // WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_DECLARATION__HPP

