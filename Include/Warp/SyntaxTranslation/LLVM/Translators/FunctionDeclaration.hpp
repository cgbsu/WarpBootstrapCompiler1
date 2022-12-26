#include <Warp/SyntaxTranslation/LLVM/Translators/ConstantCall.hpp>

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
			const std::vector<FunctionParameter>& function_parameters, 
			auto generate_return_value, 
			llvm::Type* return_type
		)
	{
	std::cout << "INDEX 7\n";
        std::vector<llvm::Type*> parameter_types;
		std::vector<std::string> parameter_names;
	std::cout << "INDEX 8\n";
		for(const auto& parameter : function_parameters) {
	std::cout << "INDEX 9\n";
			parameter_types.push_back(parameter.type);
	std::cout << "INDEX 10\n";
			parameter_names.push_back(parameter.name.data());
		std::cout << "Pushed parameter with name: " << parameter_names.back() << "\n";
	std::cout << "INDEX 11\n";
		}
	std::cout << "INDEX 12\n";
        llvm::FunctionType* function_type = llvm::FunctionType::get(
				return_type, 
                parameter_types,
                false
            );
	std::cout << "INDEX 13\n";
        if(!function_type || function_type == nullptr) {
            std::cerr << "Error: Invalid function type!\n";
			return std::nullopt;
		}
	std::cout << "INDEX 14\n";
		llvm::Function* function = llvm::Function::Create(
				function_type, 
				llvm::Function::ExternalLinkage, 
				name.data(), 
				constructing_context->module
			);
	std::cout << "INDEX 15\n";
        constructing_context->symbol_table.clear();
	std::cout << "INDEX 16\n";
		size_t ii = 0;
        for(auto& parameter : function->args())
        {
	std::cout << "INDEX 17\n";
            parameter.setName(parameter_names[ii].data());
	std::cout << "INDEX 18\n";
            constructing_context->symbol_table[parameter_names[ii]] = &parameter;
	std::cout << "INDEX 19\n";
            std::cout << parameter_names[ii] << "\n";
	std::cout << "INDEX 20\n";
			++ii;
	std::cout << "INDEX 21\n";
        }
	std::cout << "INDEX 22\n";
        llvm::BasicBlock* block = llvm::BasicBlock::Create(constructing_context->context, "entry", function);
	std::cout << "INDEX 23\n";
        constructing_context->builder.SetInsertPoint(block);
	std::cout << "INDEX 24\n";
		constructing_context->builder.CreateRet(generate_return_value(constructing_context));
	std::cout << "INDEX 25\n";
        llvm::verifyFunction(*function);
	std::cout << "INDEX 26\n";
        return function;
	}
	
	llvm::Value* constraint_condition(
			Context* constructing_context,
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const std::vector<SingleParameterType>& parameters
		)
	{
	std::cout << "Const 0\n";
		llvm::Value* condition = llvm::ConstantInt::get(
				constructing_context->context, 
				llvm::APInt(1, 1, true)
			);
	std::cout << "Const 1\n";
		for(const auto& parameter : parameters)
		{
	std::cout << "Const 1.0\n";
			std::string name = parameter.name + std::string{"_constrainttmp"};
	std::cout << "Const 1.1\n";
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
						llvm::IntegerType::get(constructing_context->context, 
						1
					));
			}
		std::cout << "Const 1.1.0\n";
			if(parameter_constraint_.has_value() == false) {
				std::cerr << "Error! Failed to generate parameter constraint!\n";
				return nullptr;
			}
		std::cout << "Const 1.1.1\n";
			llvm::Value* parameter_constraint = parameter_constraint_.value();
	std::cout << "Const 1.2\n";
			condition = constructing_context->builder.CreateLogicalAnd(
					condition, 
					parameter_constraint, 
					name.c_str()
				);
	std::cout << "Const 1.3\n";
		}
	std::cout << "Const 2\n";
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
            std::cout << parameter_name << "\n";
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
	
	 auto create_replacement_table(
			const bool debug, 
			const size_t argument_count, 
			const FunctionType::AlternativeType& alternative, 
			const std::vector<std::string> replacements
		) -> std::optional<std::unordered_map<std::string, std::string>>
	{
		std::unordered_map<std::string, std::string> replace_names;
		if(alternative.get_parameters().size() != (argument_count + 1)) {
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
		for(size_t ii = 0; parameter_count; ++ii)
			parameters.push_back({parameter_names[ii], parameter_types[ii]});
		return parameters;
	}

	std::optional<llvm::Function*> translate_alternatives(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const size_t argument_count, 
			std::string name, 
			const FunctionType::AlternativesOfUniformArityRowType& alternatives, 
			std::string base_parameter_name = std::string{"parameter_"}
		)
	{
		size_t alternative_count = alternatives.size();
	std::cout << "INDEX 0\n";
		const auto alternative_functions = translate_alternative_bodies_to_functions(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				name, 
				argument_count, 
				alternatives
			);
	std::cout << "INDEX 1\n";
		const auto lookup_table = create_lookup_table_for_alternative(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				name, 
				argument_count, 
				alternative_functions
			);
	std::cout << "INDEX 2\n";
		const auto parameter_names = create_parameter_names(argument_count);
	std::cout << "INDEX 3\n";
		const auto parameter_types = list_of_type(constructing_context, argument_count);
	std::cout << "INDEX 4\n";
		const auto parameters = zip_to_paramerers(parameter_names, parameter_types);
	std::cout << "INDEX 5\n";
		if(lookup_table.has_value() == false) {
			std::cerr << "Error! No lookup table for alternative!\n";
			return std::nullopt;
		}
		size_t option = 0;
		llvm::Value* option_index = nullptr;
		std::stringstream name_buffer;
		name_buffer << name << "_" << argument_count;
		const std::string alternative_name = name_buffer.str();
	std::cout << "INDEX 6\n";
		return make_function(
				constructing_context, 
				alternative_name, 
				parameters.value(), 
				[&](Context* context) -> llvm::Value* {
						std::cout << "Gen 0!\n";
						for(const auto& alternative : alternatives)
						{
						std::cout << "Gen 0.0!\n";
							auto replacement_table = create_replacement_table(
									debug, 
									argument_count, 
									*alternative.get(), 
									parameter_names
								);
						std::cout << "Gen 0.1!\n";
							if(replacement_table.has_value() == false) {
								std::cerr << "Error! Failed to generate replacement table for parameters.\n";
								return nullptr;
							}
						std::cout << "Gen 0.2!\n";
							context->replace_names.swap(replacement_table.value());
						std::cout << "Gen 0.3!\n";
							llvm::Value* matches_constraint = constraint_condition(
									context, 
									top_level_syntax_tree_context, 
									debug, 
									alternative->get_parameters()
								);
						std::cout << "Gen 0.4!\n";
							context->replace_names.clear();
						std::cout << "Gen 0.5!\n";
							llvm::Value* next_index = context->builder.CreateShl(matches_constraint, option);
						std::cout << "Gen 0.6!\n";
							if(option_index == nullptr) {
								std::cout << "Gen 0.6.A.0!\n";
								option_index = next_index;
								std::cout << "Gen 0.6.A.1!\n";
							}
							else {
								std::cout << "Gen 0.6.B.0!\n";
								option_index = context->builder.CreateOr(option_index, next_index);
								std::cout << "Gen 0.6.B.1!\n";
							}
						std::cout << "Gen 0.7!\n";
						}
						std::cout << "Gen 1!\n";
						auto generate = context->builder.CreateExtractElement(lookup_table.value(), option_index);
						std::cout << "Gen 2\n";
						return generate;
					}, 
                llvm::Type::getInt32Ty(constructing_context->context)
			);
	}

	void translate_function(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const FunctionType& to_translate
		)
	{
		size_t argument_count = 0;
		for(const auto& alternatives : to_translate.get_alternatives())
		{
			if(alternatives.size() > 0)
			{
				translate_alternatives(
						constructing_context, 
						top_level_syntax_tree_context, 
						debug,
						argument_count, 
						to_translate.get_name(), 
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

