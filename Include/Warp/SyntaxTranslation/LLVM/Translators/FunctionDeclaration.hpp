#include <Warp/SyntaxTranslation/LLVM/Translators/FunctionCall.hpp>

#ifndef WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_DECLARATION__HPP
#define WARP__SYNTAX__TRANSLATTION__HEADER__WARP__SYNTAX_TRANSLATION__LLVM__TRANSLATORS__FUNCTION_DECLARATION__HPP

namespace Warp::SyntaxTranslation::LLVM
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	std::optional<llvm::Function*> make_function(
			Context* constructing_context, 
			std::string name, 
			const std::vector<FunctionParameter>& function_parameters, // TODO: Make this optional? Split into names and types and make types optional?
			auto generate_return_value, 
			llvm::Type* return_type // TODO: Make this optional?
		)
	{
		std::vector<llvm::Type*> parameter_types;
		std::vector<std::string> parameter_names;
		for(const auto& parameter : function_parameters) {
			parameter_types.push_back(parameter.type);
			parameter_names.push_back(parameter.name.data());
		}
		llvm::Function* function = constructing_context->module.getFunction(name);
		if(function == nullptr)
		{
        	llvm::FunctionType* function_type = llvm::FunctionType::get(
					return_type, 
        	        parameter_types,
        	        false
        	    );
        	if(!function_type || function_type == nullptr) {
        	    std::cerr << "Error: Invalid function type!\n";
				return std::nullopt;
			}
			function = llvm::Function::Create(
					function_type, 
					llvm::Function::ExternalLinkage, 
					name.data(), 
					constructing_context->module
				);
		}
        constructing_context->symbol_table.clear();
		size_t ii = 0;
        for(auto& parameter : function->args())
        {
            parameter.setName(parameter_names[ii].data());
            constructing_context->symbol_table[parameter_names[ii]] = &parameter;
			++ii;
        }
        llvm::BasicBlock* block = llvm::BasicBlock::Create(constructing_context->context, "entry", function);
        constructing_context->builder.SetInsertPoint(block);
		constructing_context->builder.CreateRet(generate_return_value(
				constructing_context, 
				function, 
				parameter_types, 
				parameter_names, 
				return_type
			));
        llvm::verifyFunction(*function);
        return function;
	}
	
	llvm::Value* constraint_condition(
			Context* constructing_context,
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const std::vector<SingleParameterType>& parameters
		)
	{
		llvm::Type* bool_type = llvm::IntegerType::get(
				constructing_context->context, 
				1
			);
		llvm::Value* condition = llvm::ConstantInt::getTrue(
				constructing_context->context
			);
		for(const auto& parameter : parameters)
		{
			std::string name = parameter.name + std::string{"_constrainttmp"};
			std::optional<llvm::Value*> parameter_constraint_ = std::nullopt;
			if(parameter.constraint.constraint.get() != nullptr)
			{
				parameter_constraint_ = translate<Target::LLVM, llvm::Value*>(
						constructing_context,
						top_level_syntax_tree_context, 
						parameter.constraint.constraint.get(), 
						debug
					);
			}
			else // Writing a bug rn, potential for multiple indicies to work. //
			{
				parameter_constraint_ = llvm::ConstantInt::getTrue(
						constructing_context->context
					);
			}
			if(parameter_constraint_.has_value() == false) {
				std::cerr << "Error! Failed to generate parameter constraint!\n";
				return nullptr;
			}
			llvm::Value* parameter_constraint = parameter_constraint_.value();
			condition = constructing_context->builder.CreateLogicalAnd(
					condition, 
					parameter_constraint, 
					name.c_str()
				);
		}
		return condition;
	}



	std::optional<llvm::Function*> translate_alternative_body_to_function(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const std::unique_ptr<AlternativeType>& alternative, 
			size_t alternative_index
		)
	{
		const size_t parameter_count = alternative->parameter_count();
		std::stringstream name_buffer;
		name_buffer << alternative->get_name() << "_" << parameter_count << "_" << alternative_index;
		const std::string name = name_buffer.str();

        std::vector<llvm::Type*> parameter_types(parameter_count, llvm::Type::getInt32Ty(constructing_context->context));
        llvm::FunctionType* function_type = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(constructing_context->context),    
                parameter_types,
                false
            );
        if(!function_type || function_type == nullptr) {
            std::cerr << "Error: Invalid function type!\n";
			return std::nullopt;
		}
		llvm::Function* function = llvm::Function::Create(
				function_type, 
				llvm::Function::ExternalLinkage, 
				name.data(), 
				constructing_context->module
			);
        constructing_context->symbol_table.clear();
		size_t ii = 0;
        for(auto& parameter : function->args())
        {
			const std::string parameter_name = alternative->get_parameters()[ii].name;
            parameter.setName(parameter_name.data());
            constructing_context->symbol_table[parameter_name] = &parameter;
			++ii;
        }
        llvm::BasicBlock* block = llvm::BasicBlock::Create(constructing_context->context, "entry", function);
        constructing_context->builder.SetInsertPoint(block);
		const auto body_ = translate<Target::LLVM, llvm::Value*>(
				constructing_context, 
				top_level_syntax_tree_context, 
				alternative->get_function_body().get(), 
				debug
			);
		if(body_.has_value() == false) {
			std::cerr << "Error: Could not parse function body.\n";
			return std::nullopt;
		}
		constructing_context->builder.CreateRet(body_.value());
        llvm::verifyFunction(*function);
        return function;
	}

	
	std::vector<std::optional<llvm::Function*>> translate_alternative_bodies_to_functions(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			std::string name, 
			const size_t argument_count, 
			const FunctionType::AlternativesOfUniformArityRowType& alternatives
		)
	{
		std::vector<std::optional<llvm::Function*>> compiled_alternatives;
		for(size_t ii = 0; ii < alternatives.size(); ++ii)
		{
			compiled_alternatives.push_back(translate_alternative_body_to_function(
					constructing_context, 
					top_level_syntax_tree_context, 
					debug, 
					alternatives[ii], 
					ii
				));
		}
		return compiled_alternatives;
	}

	  std::optional<llvm::Constant*> create_lookup_table_for_alternative(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			std::string name, 
			const size_t argument_count, 
			const std::vector<std::optional<llvm::Function*>>& alternatives
		)
	{
		if(alternatives.size() <= 0)
			return std::nullopt;
		std::vector<llvm::Constant*> alternative_functions;
		size_t alternative_index = 0;
		for(const auto& alternative : alternatives)
		{
			if(alternative.has_value() == false)
			{
				std::cerr << "Error! Alternative " << name 
						<< ":" << argument_count << ":" 
						<< alternative_index << " does not have a body!\n";
				return std::nullopt;
			}
			alternative_functions.push_back(alternative.value());
			++alternative_index;
		}
		llvm::ArrayType* array_type = llvm::ArrayType::get(
				alternative_functions[0]->getType(), 
				alternative_functions.size()
			);
		return llvm::ConstantArray::get(
				array_type, 
				alternative_functions
			);
	}

	llvm::Value* create_lookup_table_global(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			std::string name, 
			const size_t argument_count, 
			const std::vector<std::optional<llvm::Function*>>& alternative_functions
		)
	{
		const auto lookup_table = create_lookup_table_for_alternative(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				name, 
				argument_count, 
				alternative_functions
			);
		if(lookup_table.has_value() == false) {
			std::cerr << "Error! No lookup table for alternative!\n";
			return nullptr;
		}
		std::stringstream lookup_table_name_buffer;
		lookup_table_name_buffer << name << "_" << argument_count << "_table";
		std::string lookup_table_name = lookup_table_name_buffer.str();
		constructing_context->module.getOrInsertGlobal(
				lookup_table_name.data(), 
				lookup_table.value()->getType()
			);
		llvm::GlobalVariable* lookup_table_global = constructing_context->module.getNamedGlobal(lookup_table_name.data());
		lookup_table_global->setInitializer(lookup_table.value());
		lookup_table_global->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
		return lookup_table_global;
	}
	
	 auto create_replacement_table(
			const bool debug, 
			const size_t argument_count, 
			const FunctionType::AlternativeType& alternative, 
			const std::vector<std::string> replacements
		) -> std::optional<std::unordered_map<std::string, std::string>>
	{
		std::unordered_map<std::string, std::string> replace_names;
		if(alternative.get_parameters().size() != argument_count) {
			std::cerr << "Internal Error: Parameter count does not match argument count\n";
			return std::nullopt;
		}
		for(size_t ii = 0; ii < argument_count; ++ii)
			replace_names.insert({alternative.get_parameters()[ii].name, replacements[ii]});
		return replace_names;
	}
	
	template<auto TypeGeneratorParameterConstant = &llvm::Type::getInt32Ty>
	auto list_of_type(
			Context* constructing_context, 
			size_t length
		) -> std::vector<llvm::Type*>
	{
		return std::vector<llvm::Type*>(
				length, 
				TypeGeneratorParameterConstant(constructing_context->context)
			);
	}

	std::vector<std::string> create_parameter_names(
			const size_t parameter_count, 
			const std::string base_name = std::string{"parameter_"}
		)
	{
		std::vector<std::string> parameter_names;
		for(size_t ii = 0; ii < parameter_count; ++ii)
		{
			std::stringstream buffer;
			buffer << base_name << (parameter_count + 1);
			parameter_names.push_back(buffer.str());
		}
		return parameter_names;
	}

	 std::optional<std::vector<FunctionParameter>> zip_to_paramerers(
			const std::vector<std::string>& parameter_names, 
			const std::vector<llvm::Type*>& parameter_types
		)
	{
		const size_t parameter_count = parameter_names.size();
		if(parameter_count != parameter_types.size()) {
			std::cerr << "Internal Error! Mismatch in actual vs. expected number of parameters.\n";
			return std::nullopt;
		}
		std::vector<FunctionParameter> parameters;
		for(size_t ii = 0; ii < parameter_count; ++ii) {
			parameters.push_back({parameter_names[ii], parameter_types[ii]});
		}
		return parameters;
	}

	llvm::Value* call_function_pointer_from_array(
			Context* context, 
			llvm::Function* function, 
			llvm::FunctionType* function_type, 
			llvm::Value* function_pointer_array, 
			std::vector<llvm::Value*> index_array
		)
	{
		llvm::Type* function_pointer_type = (llvm::Type*) function->getType();
		llvm::ArrayType* size_1_array_type = llvm::ArrayType::get(
				function->getType(), 
				1
			);
		auto function_pointer_array_type = llvm::PointerType::get(
				size_1_array_type, 
				context->module.getDataLayout().getDefaultGlobalsAddressSpace()
			);
		auto cast_to_array = context->builder.CreatePointerBitCastOrAddrSpaceCast(
				function_pointer_array, 
				function_pointer_array_type, 
				"cast_to_array"
			);
		auto option_address = context->builder.CreateGEP( 
				size_1_array_type, 
				cast_to_array, 
				index_array, 
				"option_address"
			);
		auto cast_from_array = context->builder.CreatePointerBitCastOrAddrSpaceCast(
				option_address, 
				function_pointer_type, 
				"cast_from_array"
			);
		auto loaded_option = context->builder.CreateLoad(
				function_pointer_type, 
				cast_from_array, 
				"loaded_option"
			);
		std::vector<llvm::Value*> arguments;
		for(auto& parameter : context->symbol_table)
			arguments.push_back(parameter.second);
		return context->builder.CreateCall(
				function_type, 
				loaded_option, 
				llvm::ArrayRef(arguments)
			);
	}
	
	llvm::Value* function_index_value(
			Context* context, 
			const auto* top_level_syntax_tree_context, 
			llvm::Function* function, 
			std::vector<llvm::Type*> parameter_types, 
			std::vector<std::string> parameter_names, 
			llvm::Type* return_type, 
			llvm::Type* index_type, 
			const FunctionType::AlternativesOfUniformArityRowType& alternatives, 
			const size_t argument_count, 
			bool debug
		)
	{
		size_t option = 0;
		llvm::Value* option_index = nullptr;
		for(const auto& alternative : alternatives)
		{
			auto replacement_table = create_replacement_table(
					debug, 
					argument_count, 
					*alternative.get(), 
					parameter_names
				);
			if(replacement_table.has_value() == false) {
				std::cerr << "Error! Failed to generate replacement table for parameters.\n";
				return nullptr;
			}
			context->replace_names.swap(replacement_table.value());
			llvm::Value* matches_constraint = constraint_condition(
					context, 
					top_level_syntax_tree_context, 
					debug, 
					alternative->get_parameters()
				);
			context->replace_names.clear();
			llvm::Value* index_matches = context->builder.CreateIntCast(
					matches_constraint, 
					index_type, 
					false, 
					"index_matches"
				);
			llvm::Value* select_next_index = context->builder.CreateMul(
					index_matches, 
					llvm::ConstantInt::get(index_type, option), 
					"select_next_option"
				);
			if(option_index == nullptr) {
				option_index = select_next_index;
			}
			else
			{
				llvm::Value* invert_index_matches = context->builder.CreateIntCast(
						context->builder.CreateNot(matches_constraint), 
						index_type, 
						false, 
						"invert_index_matches"
					);
				llvm::Value* select_previous_index = context->builder.CreateMul(
						invert_index_matches, 
						option_index, 
						"should_select_previous_option"
					);
				option_index = context->builder.CreateAdd(
						select_next_index, 
						select_previous_index, 
						"perform_selection"
					);
			}
			++option;
		}
		return option_index;
	}

	std::optional<llvm::Function*> translate_alternatives(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const size_t argument_count, 
			std::string base_name, 
			std::string alternative_name, 
			const FunctionType::AlternativesOfUniformArityRowType& alternatives, 
			std::string base_parameter_name = std::string{"parameter_"}, 
			size_t bits_in_index = 32
		)
	{
		size_t alternative_count = alternatives.size();
		if(alternative_count <= 0) {
			return std::nullopt;
		}
		llvm::Type* return_type = llvm::Type::getInt32Ty(constructing_context->context);
		llvm::Type* index_type = llvm::IntegerType::get(
				constructing_context->context, 
				bits_in_index
			);
		const auto alternative_functions = translate_alternative_bodies_to_functions(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				base_name, 
				argument_count, 
				alternatives
			);
		const auto parameter_names = create_parameter_names(argument_count);
		const auto parameter_types = list_of_type(constructing_context, argument_count);
		const auto parameters = zip_to_paramerers(parameter_names, parameter_types);
		llvm::Value* lookup_table_global = create_lookup_table_global(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				base_name, 
				argument_count, 
				alternative_functions
			);
		llvm::FunctionType* indexing_function_type = llvm::FunctionType::get(
				index_type, 
				llvm::ArrayRef(parameter_types), 
				false
			);
		llvm::FunctionType* function_type = llvm::FunctionType::get(
				return_type, 
				llvm::ArrayRef(parameter_types), 
				false
			);
		const std::string alternative_selection_function_name = alternative_name + std::string{"_select_index"};
		std::optional<llvm::Function*> selection_function_ = make_function(
				constructing_context, 
				alternative_selection_function_name, 
				parameters.value(), 
				[&](
						Context* context, 
						llvm::Function* function, 
						std::vector<llvm::Type*> parameter_types, 
						std::vector<std::string> parameter_names, 
						llvm::Type* return_type
					) -> llvm::Value*
				{
					llvm::Value* option_index = function_index_value(
							context, 
							top_level_syntax_tree_context, 
							function, 
							parameter_types, 
							parameter_names, 
							index_type, // Return Type
							index_type, 
							alternatives, 
							argument_count, 
							debug
						);
					return option_index;
				}, 
				index_type
			);
		if(selection_function_.has_value() == false)
			return std::nullopt;
		llvm::Function* selection_function = selection_function_.value();
		//if(llvm::verifyFunction(*selection_function) == false)
		//{
		//	std::cerr << "Error: Failed to verify selection function " 
		//				<< alternative_selection_function_name 
		//				<< "\n";
		//	return std::nullopt;
		//}
		auto alternative_function_ = make_function(
				constructing_context, 
				alternative_name, 
				parameters.value(), 
				[&](
						Context* context, 
						llvm::Function* function, 
						std::vector<llvm::Type*> parameter_types, 
						std::vector<std::string> parameter_names, 
						llvm::Type* return_type
					) -> llvm::Value*
				{
					std::vector<llvm::Value*> indexing_arguments;
					for(std::string argument_name : parameter_names)
						indexing_arguments.push_back(context->symbol_table.at(context->to_name(argument_name)));
					llvm::Value* option_index = context->builder.CreateCall(
							indexing_function_type, 
							(llvm::Value*) selection_function, 
							indexing_arguments
						);
					return call_function_pointer_from_array(
							context, 
							function, 
							function_type, 
							lookup_table_global, 
							std::vector<llvm::Value*>{option_index}
						);
				}, 
                llvm::Type::getInt32Ty(constructing_context->context)
			);
		if(alternative_function_.has_value() == false)
			return std::nullopt;
		llvm::Function* alternative_function = alternative_function_.value();
		//if(llvm::verifyFunction(*alternative_function_.value()) == false)
		//{
		//	std::cerr << "Error: Failed to verify alternative function " 
		//				<< alternative_function
		//				<< "\n";
		//	return std::nullopt;
		//}
		return alternative_function;
	}

	struct WarpLLVMFunction {
		const std::string name;
		std::vector<llvm::Function*> alternatives;
	};

	const static std::string default_entry_point_function_name = std::string{"warp_main"}; /* There is a problem with the 
			Warp parser where identifiers must be a certain length. */
	const static std::string default_output_entry_point_function_name = std::string{"main"}; /* Because of the limits on the length of 
			identifiers this is needed for the time being. */

	std::optional<WarpLLVMFunction> translate_function(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const FunctionType& to_translate, 
			std::string entry_point_function_name = default_entry_point_function_name, 
			std::string output_entry_point_function_name = default_output_entry_point_function_name, 
			bool keep_entry_point_name_same = true
		)
	{
		WarpLLVMFunction function{to_translate.get_name()};
		size_t argument_count = 0;
		for(const auto& alternatives : to_translate.get_alternatives())
		{
			function.alternatives.push_back(nullptr);
			if(alternatives.size() > 0)
			{
				std::string base_name = to_translate.get_name();
				std::string alternative_name 
						= base_name
						+ std::string{"_"} 
						+ std::to_string(argument_count);
				if(base_name == default_entry_point_function_name 
						&& keep_entry_point_name_same == true)
					alternative_name = output_entry_point_function_name;
				const auto parameter_types = list_of_type(constructing_context, argument_count);
				llvm::Function* alternative_declaration = (llvm::Function*)
						constructing_context->module.getOrInsertFunction( // Declares but does not define the function.
								alternative_name, 
								llvm::FunctionType::get(
										llvm::Type::getInt32Ty(constructing_context->context), 
										parameter_types, 
										false 
									)
							).getCallee();
				std::cout << "Submitted {" << alternative_name << "}\n";
				constructing_context->function_table.insert({ // This is so it can be references by alternative options .//
						alternative_name, 
						alternative_declaration	
					});
				auto new_alternative_ = translate_alternatives(
						constructing_context, 
						top_level_syntax_tree_context, 
						debug,
						argument_count, 
						base_name, 
						alternative_name, 
						alternatives
					);
				if(new_alternative_.has_value() == false)
					return std::nullopt;
				auto new_alternative = new_alternative_.value();
				function.alternatives[argument_count] = new_alternative;
			}
			++argument_count;
		}
		return function;
	}

	template<typename ReduceToParameterType, NodeType NotImplementedTagParameterConstant>
	struct Translator<Target::LLVM, ReduceToParameterType, NotImplementedTagParameterConstant> {
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

