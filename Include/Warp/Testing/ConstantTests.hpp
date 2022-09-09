#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Testing/ParseTestingUtilities.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__CONSTANT__TESTS__HPP
#define WARP__PARSING__HEADER__PARSING__CONSTANT__TESTS__HPP

namespace Warp::Testing
{
	using namespace Utilities;
	using namespace Parsing;
	using namespace Runtime::Compiler::SimpleExecutor;
	
	using WholeType = NumericTypeResolver<NumericTypeTag::Whole>::Type;
	using IntegerType = NumericTypeResolver<NumericTypeTag::Integer>::Type;
	using FixedType = NumericTypeResolver<NumericTypeTag::FixedPoint>::Type;
	using CharType = NumericTypeResolver<NumericTypeTag::Character>::Type;
	using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;
	
	using NumericConstantType = Constant<SyntaxNode, NumericTypeTag>;
	using NumericContexType = std::unordered_map<std::string, NumericConstantType>;
	
	template<
			auto TypeTagParameterConstant, 
			template<auto> typename TypeResolverParameterTemplate = NumericTypeResolver
		>
	auto retrieve_constant(const OptionalConcept auto& context, std::string name, bool debug)
	{
		if(debug == true)
		{
			if(context.has_value() == false)
				std::cerr << "retrieve_constant::Error: NO CONTEXT\n";
			std::cout << "retrieve_constant: Requested constant: " << name << "\n";
			if(context.value().constants.contains(name) == false)
				std::cerr << "retrieve_constant::Error: No constant with requested name!\n";
		}
		const auto value = retrieve_value<
				typename TypeResolverParameterTemplate<TypeTagParameterConstant>::Type
			>(&context.value(), context.value().constants.at(name).value.get(), debug);
		if(debug == true && value.has_value() == false)
			std::cerr << "retrieve_constant::Error: No value for constant retrieved!\n";
		return value.value();
	}
	
	template<
			auto TypeTagParameterConstant,
			template<auto> typename TypeResolverParameterTemplate = NumericTypeResolver
		>
	auto retrieve_constant(const OptionalConcept auto& constant, bool debug)
	{
		return retrieve_value<
				typename NumericTypeResolver<TypeTagParameterConstant>::Type
			>(debug, constant.value().value).value();
	}
	template<
			auto TypeTagParameterConstant, 
			template<auto> typename TypeResolverParameterTemplate = NumericTypeResolver
		>
	auto print_constant(const OptionalConcept auto& context, std::string name, bool debug, bool courtesy = true)
	{
		std::cout << "{" << name << ":" << to_string(retrieve_constant<
						TypeTagParameterConstant
					>(context, name, debug)) << "}"
				<< ((courtesy == false) ? "" : "\n");
	}

	template<
			auto TypeTagParameterConstant, 
			template<auto> typename TypeResolverParameterTemplate = NumericTypeResolver
		>
	auto print_constant(const OptionalConcept auto& constant, bool debug, bool courtesy = true) {
		std::cout << "{" << constant.name << ":" << to_string(retrieve_constant<
						TypeTagParameterConstant, 
						TypeResolverParameterTemplate
					>(constant, debug)) << "}"
				<< ((courtesy == false) ? "" : "\n");
	}
	
	bool check(auto left, auto right, bool debug)
	{
		bool equal = (left == right);
		if(equal == false)
			std::cout << "Check failed, with Actual: " << left << " vs Expected: " << right << "\n";
		return equal;
	}
	
	template<
			auto TestParameterConstant, 
			auto TypeTagParameterConstant, 
			typename ParserParameterType = NumericParserType, 
			template<auto> typename ResolverParameterTemplate = NumericTypeResolver
		>
	void check_context_constant(
			std::vector<std::string> names, 
			const auto expected, 
			bool debug = false, 
			std::source_location test_location = std::source_location::current()
		)
	{
		auto context = runtime_parse<
				ParserParameterType, 
				TestParameterConstant, 
				Construct::Context
			>(debug);
		if(const bool parse_success = (context.has_value() == true); parse_success == false)
		{
			std::cerr << "\n" << test_location.file_name() << ": " << test_location.line() << ": "
					<< "Failed to parse or retrieve constants from \"" 
					<< TestParameterConstant.string << "\"\n";
			CHECK((parse_success == true));
		}
		else
		{
			for(size_t ii = 0; ii < names.size(); ++ii)
			{
				auto result = retrieve_constant<
							TypeTagParameterConstant, 
							ResolverParameterTemplate
					>(
						context, 
						names[ii], 
						debug
					);
				if(debug == true)
					std::cout << "{" << names[ii] << ":" << to_string(result) << "}\n";
				if(const bool check_result = (result == expected[ii]);
						check_result == false)
				{
					std::cerr << "\n" << test_location.file_name() << ": " << test_location.line() << ": "
							<< "Check failed with " 
							<< names[ii] << " = " << to_string(result) << " and " 
							<< to_string(expected[ii]) << "\n";
					CHECK((check_result == true));
				}
			}
		}
	}
}

#endif // WARP__PARSING__HEADER__PARSING__CONSTANT__TESTS__HPP

