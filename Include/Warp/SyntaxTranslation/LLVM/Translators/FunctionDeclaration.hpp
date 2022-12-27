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
        std::vector<llvm::Type*> parameter_types;
		std::vector<std::string> parameter_names;
		for(const auto& parameter : function_parameters) {
			parameter_types.push_back(parameter.type);
			parameter_names.push_back(parameter.name.data());
		}
        llvm::FunctionType* function_type = llvm::FunctionType::get(
				return_type, 
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

	std::optional<llvm::Function*> translate_alternatives(
			Context* constructing_context, 
			const auto* top_level_syntax_tree_context, 
			bool debug, 
			const size_t argument_count, 
			std::string name, 
			const FunctionType::AlternativesOfUniformArityRowType& alternatives, 
			std::string base_parameter_name = std::string{"parameter_"}, 
			size_t bits_in_index = 32
		)
	{
		size_t alternative_count = alternatives.size();
		if(alternative_count <= 0) {
			return std::nullopt;
		}
		llvm::Type* index_type = llvm::IntegerType::get(
				constructing_context->context, 
				bits_in_index
			);
		const auto alternative_functions = translate_alternative_bodies_to_functions(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				name, 
				argument_count, 
				alternatives
			);
		const auto lookup_table = create_lookup_table_for_alternative(
				constructing_context, 
				top_level_syntax_tree_context, 
				debug, 
				name, 
				argument_count, 
				alternative_functions
			);
		const auto parameter_names = create_parameter_names(argument_count);
		const auto parameter_types = list_of_type(constructing_context, argument_count);
		const auto parameters = zip_to_paramerers(parameter_names, parameter_types);
		if(lookup_table.has_value() == false) {
			std::cerr << "Error! No lookup table for alternative!\n";
			return std::nullopt;
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
		size_t option = 0;
		llvm::Value* option_index = nullptr;
		std::stringstream name_buffer;
		name_buffer << name << "_" << argument_count;
		const std::string alternative_name = name_buffer.str();
		return make_function(
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
						llvm::FunctionType* function_type = llvm::FunctionType::get(
								return_type, 
								llvm::ArrayRef(parameter_types), 
								false
							);
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
							llvm::Value* next_index = context->builder.CreateMul(
									context->builder.CreateIntCast(
											matches_constraint, 
											index_type, 
											false
										), 
									llvm::ConstantInt::get(index_type, option)
								);
							if(option_index == nullptr) {
								option_index = next_index;
							}
							else {
								option_index = context->builder.CreateAdd(
										option_index, 
										next_index
									);
							}
						++option;
						}
						auto table_pointer_type = llvm::PointerType::get(
								lookup_table.value()->getType(), 
								constructing_context->module.getDataLayout().getDefaultGlobalsAddressSpace()
							);
						auto opauqe_pointer_type = llvm::PointerType::get(
								constructing_context->context, 
								constructing_context->module.getDataLayout().getDefaultGlobalsAddressSpace()
							);
						auto function_pointer_type = llvm::PointerType::get(
								function_type, //alternative_functions[0].value()->getType(), 
								constructing_context->module.getDataLayout().getDefaultGlobalsAddressSpace()
							);
						auto type_from_gep = llvm::GetElementPtrInst::getTypeAtIndex(
								//table_pointer_type, 

								((llvm::ArrayType*) lookup_table_global->getType()->getElementType()), 
								static_cast<uint64_t>(0)
							);
						//auto element_pointer_type = alternative_functions[0].value()->getType();
						//std::cout << "Valid ElementType: " << llvm::PointerType::isValidElementType(element_pointer_type) << "\n";
						std::cout << "Hello\n";
						std::cout << "GEP NUll? " << (type_from_gep == nullptr) << "\n";
						//std::cout << "Valid ElementType: " << llvm::PointerType::isValidElementType(type_from_gep) << "\n";
						//std::cout << "Valid ElementType: " << llvm::PointerType::isValidElementType(lookup_table_global->getType()) << "\n";
						auto index_array = std::vector<llvm::Value*>{option_index};
						std::cout << "Hello 0\n";
						//auto generate = context->builder.CreateGEP( // TODO: Stop using a depracted function
						//		//lookup_table_global->getElementType(), 
						//		//function_type, 
						//		//type_from_gep, 
						//		//function->getType(), 
						//		//element_pointer_type, 
						//		//alternative_functions[0].value()->getType(), 

						//		//function_pointer_type, 
						//		//opauqe_pointer_type, 
						//		((llvm::ArrayType*) lookup_table_global->getType()->getElementType()), 
						//		//((llvm::ArrayType*) type_from_gep)->getElementType(),
						//		//function_pointer_type, 
						//		lookup_table_global, 
						//		index_array, 
						//		"option_array_address"
						//	);
						//std::cout << "Hello 1\n";
						//auto element = context->builder.CreateGEP( // TODO: Stop using a depracted function
						//		//function_type, 
						//		//type_from_gep, 
						//		function->getType(), 
						//		//((llvm::ArrayType*) lookup_table_global->getType()->getElementType())->getElementType(), 
						//		//element_pointer_type, 
						//		//alternative_functions[0].value()->getType(), 
						//		//opauqe_pointer_type, 

						//		//function_pointer_type, 
						//		//opauqe_pointer_type, 
						//		//((llvm::ArrayType*) type_from_gep)->getElementType(),
						//		//function_pointer_type, 
						//		generate, 
						//		index_array, 
						//		"option_address"
						//	);
						//std::cout << "Hello 2\n";
						//std::vector<unsigned> first_element_index{0};
						//std::cout << "ABOUT TO MAKE FE\n";
						//auto first_element = context->builder.CreateExtractValue(
						//		lookup_table_global, 
						//		first_element_index
						//	);
						//std::cout << "First element\n";
						llvm::ArrayType* size_1_array_type = llvm::ArrayType::get(
								function->getType(), 
								1
							);
						auto first_element = context->builder.CreatePointerBitCastOrAddrSpaceCast(
								(llvm::Value*) lookup_table_global, 
								(llvm::Type*) function->getType(), 
								"cast"
							);
						std::cout << "CAST\n";
						auto function_pointer_array_type = llvm::PointerType::get(
								size_1_array_type, 
								constructing_context->module.getDataLayout().getDefaultGlobalsAddressSpace()
							);
						auto first_element_array = context->builder.CreatePointerBitCastOrAddrSpaceCast(
								(llvm::Value*) lookup_table_global, 
								function_pointer_array_type, 
								"cast_to_array"
							);
						std::cout << "CAST 2\n";
						auto element = context->builder.CreateGEP( // TODO: Stop using a depracted function
								size_1_array_type, 
								//first_element, 
								//lookup_table_global, 
								first_element_array, 
								index_array, 
								"option_address"
							);
						std::cout << "GEP!!!\n";
						auto element_from_array = context->builder.CreatePointerBitCastOrAddrSpaceCast(
								element, 
								(llvm::Type*) function->getType(), 
								"cast_from_array"
							);
						std::cout << "CAST 3\n";
						//auto element = context->builder.Insert(
						//		llvm::GetElementPtrInst::Create(
						//				function->getType(), 
						//				//(llvm::Type*) function->getType(), 
						//				//lookup_table_global->getType(), 
						//				//alternative_functions[0].value()->getType(), 
						//				//(llvm::ArrayType*) lookup_table_global, 
						//				//((llvm::ArrayType*) lookup_table_global)->getElementType(), 
						//				first_element, 
						//				index_array
						//			)
						//		);
						//std::cout << "Element\n";
						///////std::vector<llvm::Value*> zero_value{
						///////		llvm::ConstantInt::get(
						///////				context->context, 
						///////				llvm::APInt(
						///////						32, 
						///////						0, 
						///////						false
						///////					)
						///////			)
						///////	};
						///////auto size_pointer = context->builder.CreateGEP( 
						///////		(llvm::Type*) function->getType(), 
						///////		llvm::ConstantPointerNull::get(function->getType()), 
						///////		zero_value, 
						///////		"size_pointer"
						///////	);
						///////std::cout << "Zero GEP\n";
						///////auto integer_size = context->builder.CreatePtrToInt( 
						///////		size_pointer, 
						///////		llvm::Type::getInt32Ty(context->context), 
						///////		"size"
						///////	);
						///////auto offset = context->builder.CreateMul(
						///////		integer_size, 
						///////		option_index
						///////	);
						///////auto function_address = context->builder.CreateAdd(
						///////		offset, 
						///////		context->builder.CreatePtrToInt(
						///////				first_element, 
						///////				llvm::Type::getInt32Ty(context->context), 
						///////				"base_address"
						///////			), 
						///////		"function_address"
						///////	);
						/////////auto element = context->builder.CreateGEP( 
						/////////		(llvm::Type*) function->getType(), 
						/////////		first_element, 
						/////////		index_array
						/////////	);
						///////std::cout << "GEP\n";
						///////auto loaded_option = context->builder.CreateLoad(
						///////		//element_pointer_type, 
						///////		//opauqe_pointer_type, 
						///////		function_type, //(llvm::Type*) function->getType(), 
						///////		//first_element, 
						///////		//element, 
						///////		//first_element, 
						///////		context->builder.CreateIntToPtr(
						///////				function_address, 
						///////				(llvm::Type*) function->getType()
						///////			), 
						///////		"option"
						///////	);
						///////std::cout << "Load\n";
						//auto loaded_function_casted_pointer = context->builder.CreatePointerBitCastOrAddrSpaceCast(
						//		loaded_option, 
						//		opauqe_pointer_type, 
						//		"option_function_pointer"
						//	);
						auto loaded_option = context->builder.CreateLoad(
								(llvm::Type*) function->getType(), 
								element_from_array, 
								"option"
							);
						std::vector<llvm::Value*> arguments;
						for(auto& parameter : constructing_context->symbol_table)
							arguments.push_back(parameter.second);
						return context->builder.CreateCall(
								function_type, 
								loaded_option, 
								llvm::ArrayRef(arguments)
							);
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
			}
			++argument_count;
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

