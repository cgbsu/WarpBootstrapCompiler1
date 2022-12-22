#include <Warp/Testing/Enable.hpp>
#define WARP__TESTING__HEADER__TESTING__TEST__FUNCTION__DECLARATIONS__HPP__DEBUG__ON
#ifdef WARP__TESTING__HEADER__TESTING__TEST__FUNCTION__DECLARATIONS__HPP__DEBUG__ON
#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <Warp/Runtime/Compiler/Context.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ConstantTests.hpp>

using namespace Warp::Utilities;
using namespace Warp::Parsing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;
using namespace Warp::Runtime::Compiler;
using namespace Warp::Testing;

#define WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON

using FunctionDeclarationParserType = FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		NumericTypeTag
	>;

TEST_GROUP(FunctionDeclarations) {};

#ifdef TEST_CONSTANTS

TEST(FunctionDeclarations, DeclareConstantFromLiteral)
{
	bool debug = false;
	// For postarity
	//check_constant<
	//		FixedString{"let TheQuestion = 42u"}, 
	//		Construct::Constant
	//	>(debug);
	//runtime_parse<
	//		NumericParserType, 
	//		FixedString{"let TheQuestion = 2u * 21u"}, 
	//		Construct::Constant
	//	>(debug);
	//auto context = runtime_parse<
	//		NumericParserType, 
	//		FixedString{"let TheQuestion = 2u * 20u +2u;"}, 
	//		NumericParserType::UniqueProductions::Context
	//	>(debug);
	//print_constant<NumericTypeTag::Whole>(context, std::string{"TheQuestion"});
	//TestingParser<NumericParserType>::parser<
	//		NumericParserType::UniqueProductions::Context
	//	>.write_diag_str(std::cerr);
	check_context_constant<
			FixedString{"let TheAnswer = 42u;"}, 
			NumericTypeTag::Whole
		>({"TheAnswer"}, std::vector{42u}, debug);
	check_context_constant<
			FixedString{"let TheAnswer = 2u * 20u + 2u;"}, 
			NumericTypeTag::Whole
		>({"TheAnswer"}, std::vector{42u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer"}, std::vector{42u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
				}, 
			NumericTypeTag::Whole
		>({"PieDay"}, std::vector{314u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"PieDay"}, std::vector{314u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"BakerStreet"}, std::vector{221u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer"}, std::vector{42u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "PieDay", "BakerStreet"}, std::vector{42u, 314u, 221u}, debug);
};
#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON
TEST(FunctionDeclarations, MultiTypeDeclareConstantFromLiteral)
{
	check_context_constant<
			FixedString{
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Character
		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let LetterOfTheDay = 'A';"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer"}, std::vector{42u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let LetterOfTheDay = 'A';"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "PieDay", "BakerStreet"}, std::vector{42u, 314u, 221u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let LetterOfTheDay = 'A';"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Character
		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
	check_context_constant<
			FixedString{
					"let LetterOfTheDay = 'A';"
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Character
		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Character
		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
	check_context_constant<
			FixedString{
					"let LetterOfTheDay = 'A';"
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
				}, 
			NumericTypeTag::Whole
		>({"PieDay"}, std::vector{314u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let BakerStreet = 221;\n"
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Whole
		>({"PieDay"}, std::vector{314u}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let NextLetter = 'B';"
					"let BakerStreet = 221;\n"
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Character
		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let NextLetter = 'B';"
					"let BakerStreet = 221;\n"
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Character
		>({"NextLetter"}, std::vector{'B'}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let NextLetter = 'B';"
					"let BakerStreet = 221;\n"
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Character
		>({"LetterOfTheDay", "NextLetter"}, std::vector{'A', 'B'}, debug);
	check_context_constant<
			FixedString{
					"let TheAnswer = 2u * 20u + 2u;\n"
					"let PieDay = 314u;\n"
					"let NextLetter = 'B';"
					"let BakerStreet = 221;\n"
					"let LetterOfTheDay = 'A';"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "PieDay", "BakerStreet"}, std::vector{42u, 314u, 221u}, debug);
	//check_context_constant<
	//		FixedString{
	//				"let TheAnswer = 2u * 20u + 2u;\n"
	//				//"let Cake = false;"
	//				"let LetterOfTheDay = 'A';"
	//			}, 
	//		NumericTypeTag::Whole
	//	>({"TheAnswer"}, std::vector{42u}, debug);
	//check_context_constant<
	//		FixedString{
	//				"let TheAnswer = 2u * 20u + 2u;\n"
	//				"let LetterOfTheDay = 'A';"
	//			}, 
	//		NumericTypeTag::Character
	//	>({"LetterOfTheDay"}, std::vector{'A'}, debug);
};

#endif // WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON

TEST(FunctionDeclarations, UseConstansInConstants)
{
	bool debug = false;
	//NumericParserType::parser.write_diag_str(std::cerr);
	check_context_constant<
			FixedString{
					"let Fourty = 2u * 20u;\n"
					"let NotTheAnswer = Fourty;\n"
				}, 
			NumericTypeTag::Whole
		>({"Fourty"}, std::vector{40u}, debug);
	check_context_constant<
			FixedString{
					"let Fourty = 2u * 20u;\n"
					"let NotTheAnswer = Fourty;\n"
				}, 
			NumericTypeTag::Whole
		>({"Fourty", "NotTheAnswer"}, std::vector{40u, 40u}, debug);
	debug = false;
	check_context_constant<
			FixedString{
					"let HalfAnswer = 21u;\n"
					"let TheAnswer = 2u * HalfAnswer;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "HalfAnswer"}, std::vector{42u, 21u}, debug);
	check_context_constant<
			FixedString{
					"let HalfAnswer = 21u;\n"
					"let TheAnswer = HalfAnswer * 2u;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "HalfAnswer"}, std::vector{42u, 21u}, debug);
	check_context_constant<
			FixedString{
					"let Fourty = 2u * 20u;\n"
					"let TheAnswer = Fourty + 2u;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "Fourty"}, std::vector{42u, 40u}, debug);
	check_context_constant<
			FixedString{
					"let Fourty = 2u * 20u;\n"
					"let TheAnswer = 2u + Fourty;\n"
				}, 
			NumericTypeTag::Whole
		>({"TheAnswer", "Fourty"}, std::vector{42u, 40u}, debug);
};

#endif // TEST_CONSTANTS

constexpr static const auto compare_single_parameter = [](
			const auto& left, 
			const auto& right, 
			bool debug, 
			std::source_location location
		) {
	return left.name == right.name;
};

constexpr static const auto compare_prototype = [](
		const auto& left, 
		const auto& right, 
		bool debug, 
		std::source_location location
	)
{
	if(left->parameters.size() == right->parameters.size())
	{
		const auto& left_parameters = left->parameters;
		const auto& right_parameters = right->parameters;
		bool same = true;
		for(size_t ii = 0; ii < left->parameters.size(); ++ii)
		{
			same = same && compare_single_parameter(
					left_parameters[ii], 
					right_parameters[ii], 
					debug, 
					location
				);
		}
		return same;
	}
	else
		return false;
};

template<auto TestParameterConstant>
constexpr static const auto parse_prototype(bool debug)
{
	return runtime_parse<
			FunctionDeclarationParserType, 
			TestParameterConstant, 
			FunctionDeclaration::Prototype
		>(debug);
}

template<auto TestParameterConstant>
constexpr static const auto parse_context(bool debug)
{
	return runtime_parse<
			FunctionDeclarationParserType, 
			TestParameterConstant, 
			Construct::Context
		>(debug);
}

template<auto TestParameterConstant>
constexpr static const auto parse_call(bool debug)
{
	return runtime_parse<
			FunctionDeclarationParserType, 
			TestParameterConstant, 
			Call::Function
		>(debug);
}

template<auto TestParameterConstant>
constexpr static const bool check_function_call_value(
		const auto& context, 
		auto expected, 
		bool debug, 
		std::source_location source_location = std::source_location::current()
	)
{
	const auto call = parse_call<FixedString{TestParameterConstant}>(debug);
	if(call.has_value() == false)
	{
		if(debug == true)
		{
			std::cerr << "Failed to parse function call " 
					<< source_location.file_name() 
					<< ":" 
					<< source_location.line() 
					<< "\n";
		}
		return false;
	}
	const auto result = retrieve_value<NumericValue>(
			&context.value(), 
			call.value(), 
			debug
		);
	if(result.has_value() == false)
	{
		if(debug == true)
		{
			std::cerr << "Failed to retrieve value " 
					<< source_location.file_name() 
					<< ":" 
					<< source_location.line() 
					<< "\n";
		}
		return false;
	}
	bool success = (expected == reult);
	if(success == true && debug == true)
	{
		std::cerr << "Expected not equal to actual " 
				<< "Expected: " << result.value().to_string() 
				<< "Actual: " << expected.to_string() 
				<< source_location.file_name() 
				<< ":" 
				<< source_location.line() 
				<< "\n";
	}
	return success;
}

TEST(FunctionDeclarations, Prototype)
{
	bool debug = false;
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function()"}>(debug) /*Actual*/, 
			make_alternative_prototype_with_no_parameters<SingleParameterType>("my_function"), /*Expected*/
			debug
		);
};

#define TEST_0
#ifndef TEST_0
TEST(FunctionDeclarations, PrototypeWithUnconstrainedParameters)
{
	bool debug = false;
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter)"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(SingleParameterType{
							std::string{"my_first_parameter"}, 
							ConstraintType()
						})
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter, my_second_parameter)"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(SingleParameterType{
							std::string{"my_first_parameter"}, 
							ConstraintType()
						}), 
					std::move(SingleParameterType{
							std::string{"my_second_parameter"}, 
							ConstraintType()
						})
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter, my_second_parameter, my_third_parameter)"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(SingleParameterType{
							std::string{"my_first_parameter"}, 
							ConstraintType()
						}), 
					std::move(SingleParameterType{
							std::string{"my_second_parameter"}, 
							ConstraintType()
						}), 
					std::move(SingleParameterType{
							std::string{"my_third_parameter"}, 
							ConstraintType()
						})
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter, my_second_parameter, my_third_parameter, my_fourth_parameter)"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(SingleParameterType{
							std::string{"my_first_parameter"}, 
							ConstraintType()
						}), 
					std::move(SingleParameterType{
							std::string{"my_second_parameter"}, 
							ConstraintType()
						}), 
					std::move(SingleParameterType{
							std::string{"my_third_parameter"}, 
							ConstraintType()
						}), 
					std::move(SingleParameterType{
							std::string{"my_fourth_parameter"}, 
							ConstraintType()
						})
				)), /*Expected*/
			debug
		);
}
#endif // TEST_0
template<auto TestParameterConstant>
constexpr auto parse_boolean_expression(bool debug)
{
	return runtime_parse<
			FunctionDeclarationParserType, 
			TestParameterConstant, 	
			BooleanExpression::Expression
		>(debug);
}

template<auto TestParameterConstant>
constexpr auto constraint_from_boolean_expression(bool debug)
{
	return ConstraintType{
			std::move(parse_boolean_expression<TestParameterConstant>(debug).value())
		};
}

template<auto TestParameterConstant>
constexpr auto single_parameter_from_boolean_expression(std::string name, bool debug)
{
	return SingleParameterType{
			name, 
			ConstraintType{std::move(
					parse_boolean_expression<TestParameterConstant>(debug).value()
				)}
		};
}
#define TEST_1
#ifndef TEST_1
TEST(FunctionDeclarations, PrototypeWithOneConstrainedParameter)
{
	bool debug = false;
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {1u > 0u})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1u > 0u"}>(
							std::string{"my_first_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {203u < (test_constant)})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"203u < (test_constant)"}>(
							std::string{"my_first_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {0u = (0u + test_constant - second_test_constant)})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"0u = (0u + test_constant - second_test_constant)"}>(
							std::string{"my_first_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {(0u + test_constant) = (0u + second_test_constant)})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"(0u + test_constant) = (0u + second_test_constant)"}>(
							std::string{"my_first_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {1 < 0 && 2 > 1})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1 < 0 && 2 > 1"}>(
							std::string{"my_first_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {1 < 0 && 2 > 1 || 3 < 8})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1 < 0 && 2 > 1 || 3 < 8"}>(
							std::string{"my_first_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
}
#endif // TEST_1
//#define TEST_2
#ifndef TEST_2
TEST(FunctionDeclarations, PrototypeWithMultipleConstrainedParameter)
{
	bool debug = false;
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {1u > 0u}, my_second_parameter : {8u > 2u})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1u > 0u"}>(
							std::string{"my_first_parameter"}, 
							debug
						)), 
					std::move(single_parameter_from_boolean_expression<FixedString{"8u > 2u"}>(
							std::string{"my_second_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter : {1u > 0u || 342 > 32}, my_second_parameter : {8u > 2u && 34u < 3u})"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1u > 0u || 342 > 32"}>(
							std::string{"my_first_parameter"}, 
							debug
						)), 
					std::move(single_parameter_from_boolean_expression<FixedString{"8u > 2u && 34u < 3u"}>(
							std::string{"my_second_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function("
												"my_first_parameter : {1u > 0u || 342 > 32}, "
												"my_second_parameter : {8u > 0u + test_constant && 34u < 3u}"
											")"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1u > 0u || 342 > 32"}>(
							std::string{"my_first_parameter"}, 
							debug
						)), 
					std::move(single_parameter_from_boolean_expression<FixedString{"8u > 0u + test_constant && 34u < 3u"}>(
							std::string{"my_second_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function("
												"my_first_parameter : {1u > 0u || 342 > 32}, "
												"my_second_parameter : {8u + another_test_constant > 0u + test_constant && 34u < 3u}"
											")"}>(debug) /*Actual*/, 
			std::move(make_alternative_prototype(
					std::string{"my_function"}, 
					std::move(single_parameter_from_boolean_expression<FixedString{"1u > 0u || 342 > 32"}>(
							std::string{"my_first_parameter"}, 
							debug
						)), 
					std::move(single_parameter_from_boolean_expression<FixedString{"8u > 0u + test_constant && 34u < 3u"}>(
							std::string{"my_second_parameter"}, 
							debug
						))
				)), /*Expected*/
			debug
		);
	//check_parse<compare_prototype>(
	//		parse_prototype<FixedString{"let my_function("
	//											"my_first_parameter : {"
	//													"1u + my_constant_5 > 0u + my_constant_6 "
	//															"|| 342u + my_constant_0 > 32u + my_constant_1 "
	//															"|| (0u + my_constant_2) / (0u + my_constant_3) = 0u + my_constant_4"
	//												"}, "
	//											"my_second_parameter : {"
	//													"8u + another_test_constant > 0u + test_constant && 34u < 3u"
	//												"}, "
	//											"my_third_parameter : {"
	//													"(0u + something > 0u || 0u + somthing_else > 0u) "
	//																"|| 0u + constant_0 = 0u + constant_1 "
	//																"&& 0u + constnat_0 < 42u * my_constant_2}"
	//												"}"
	//										")"}>(debug) /*Actual*/, 
	//		
	//		std::move(make_alternative_prototype(
	//				std::string{"my_function"}, 
	//				std::move(single_parameter_from_boolean_expression<FixedString{
	//						"1u + my_constant_5 > 0u + my_constant_6 "
	//								"|| 342u + my_constant_0 > 32u + my_constant_1 "
	//								"|| (0u + my_constant_2) / (0u + my_constant_3) = 0u + my_constant_4"
	//					}>(
	//						std::string{"my_first_parameter"}, 
	//						debug
	//					)), 
	//				std::move(single_parameter_from_boolean_expression<FixedString{
	//						"8u + another_test_constant > 0u + test_constant && 34u < 3u"
	//					}>(
	//						std::string{"my_second_parameter"}, 
	//						debug
	//					)), 
	//				std::move(single_parameter_from_boolean_expression<FixedString{
	//						"(0u + something > 0u || 0u + somthing_else > 0u) "
	//									"|| 0u + constant_0 = 0u + constant_1 "
	//									"&& 0u + constnat_0 < 42u * my_constant_2}"
	//					}>(
	//						std::string{"my_third_parameter"}, 
	//						debug
	//					))
	//			)), /*Expected*/
	//		debug
	//	);
}
#endif // TEST_2

TEST(FunctionDeclarations, SimpleFunctionDeclarations)
{
	bool debug = false;
	auto context = parse_context<
			"let my_function() { 4 }"
		>(debug);
	CHECK(context.has_value() == false);
	CHECK(check_function_call_value<FixedString{"my_function"}>(
			context, 
			NumericValue{4u}
			debug
		));
}

#endif // WARP__TESTING__HEADER__TESTING__TEST__FUNCTION__DECLARATIONS__HPP__DEBUG__ON

