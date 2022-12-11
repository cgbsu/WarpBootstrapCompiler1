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
	if(left->parameter_count() == right->parameter_count())
	{
		const auto left_parameters = left->get_parameters();
		const auto right_parameters = right->get_parameters();
		bool same = true;
		for(size_t ii = 0; ii < left->parameter_count(); ++ii)
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
constexpr static const auto parse_prototype(bool debug = false)
{
	return runtime_parse<
			FunctionDeclarationParserType, 
			TestParameterConstant, 
			FunctionDeclaration::Prototype
		>(debug);
}

TEST(FunctionDeclarations, Prototype)
{
	bool debug = false;
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function()"}>() /*Actual*/, 
			make_alternative_prototype_with_no_parameters<SingleParameterType>("my_function"), /*Expected*/
			debug
		);
};

TEST(FunctionDeclarations, PrototypeWithUnconstrainedParameters)
{
	bool debug = false;
	check_parse<compare_prototype>(
			parse_prototype<FixedString{"let my_function(my_first_parameter)"}>() /*Actual*/, 
			make_alternative_prototype(
					std::string{"my_function"}, 
					SingleParameterType{std::string{"my_first_parameter"}, ConstraintType()}
				), /*Expected*/
			debug
		);
}

#endif // WARP__TESTING__HEADER__TESTING__TEST__FUNCTION__DECLARATIONS__HPP__DEBUG__ON

