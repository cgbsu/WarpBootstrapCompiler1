#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ConstantTests.hpp>

using namespace Warp::Utilities;
using namespace Warp::Parsing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;
using namespace Warp::Testing;

TEST_GROUP(FunctionDeclarations) {};

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

