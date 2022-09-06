//#include <Warp/Parsing/MathematicalExpressions.hpp>
//#include <Warp/Parsing/FunctionDeclarations.hpp>
//#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
//#include <CppUTest/TestHarness.h>
//#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
//#include <Warp/Testing/ParseTestingUtilities.hpp>
//
//using namespace Warp::Utilities;
//using namespace Warp::Testing;
//using namespace Warp::Parsing;
//using namespace Warp::Runtime::Compiler::SimpleExecutor;
//
//using WholeType = NumericTypeResolver<NumericTypeTag::Whole>::Type;
//using IntegerType = NumericTypeResolver<NumericTypeTag::Integer>::Type;
//using FixedType = NumericTypeResolver<NumericTypeTag::FixedPoint>::Type;
//using CharType = NumericTypeResolver<NumericTypeTag::Character>::Type;
//using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;
//
//using NumericConstantType = Constant<SyntaxNode, NumericTypeTag>;
//using NumericContexType = std::unordered_map<std::string, NumericConstantType>;
//
//// Test instantiation //
//template struct FunctionDeclarationParser<
//		FunctionDeclaritionTermsType, 
//		NumericTypeResolver, 
//		NumericTypeTag
//	>;
//
//using NumericParserType = FunctionDeclarationParser<
//		FunctionDeclaritionTermsType, 
//		NumericTypeResolver, 
//		NumericTypeTag
//	>;
//
//template<auto TypeTagParameterConstant>
//auto retrieve_constant(const OptionalConcept auto& context, std::string name)
//{
//	if(context.has_value() == false)
//		std::cout << "NO CONTEXT\n";
//	const auto context_ = context.value();
//	std::cout << "Requested constant: " << name << "\n";
//	if(context_.constants.contains(name) == false)
//		std::cout << "Constant not contained!\n";
//	else
//		std::cout << "Name Contained\n";
//	const auto& tree = context_.constants.at(name).value;
//	std::cout << "Retrieveing value\n";
//	const auto value = retrieve_value<
//			typename NumericTypeResolver<TypeTagParameterConstant>::Type
//		>(context_, tree.get());
//		//.value();
//	std::cout << "Has value? " << value.has_value() << "\n";
//	return value.value();
//}
//
//template<auto TypeTagParameterConstant>
//auto retrieve_constant(const OptionalConcept auto& constant)
//{
//	return retrieve_value<
//			typename NumericTypeResolver<TypeTagParameterConstant>::Type
//		>(constant.value().value).value();
//}
//template<auto TypeTagParameterConstant>
//auto print_constant(const OptionalConcept auto& context, std::string name, bool courtesy = true) {
//	std::cout << "{" << name << ":" << to_string(retrieve_constant<TypeTagParameterConstant>(context, name)) << "}"
//			<< ((courtesy == false) ? "" : "\n");
//}
//template<auto TypeTagParameterConstant>
//auto print_constant(const OptionalConcept auto& constant, bool courtesy = true) {
//	std::cout << "{" << constant.name << ":" << to_string(retrieve_constant<TypeTagParameterConstant>(constant)) << "}"
//			<< ((courtesy == false) ? "" : "\n");
//}
//
//bool check(auto left, auto right) {
//	return left == right;
//}
//
//template<
//		auto TestParameterConstant, 
//		auto TypeTagParameterConstant, 
//		//auto CheckParamterConstant = check, 
//		typename ParserParameterType = NumericParserType
//	>
//void check_contex_constant(
//		std::vector<std::string> names, 
//		const auto expected, 
//		bool debug = false, 
//		std::source_location test_location = std::source_location::current()
//	)
//{
//	const auto context = runtime_parse<
//			ParserParameterType, 
//			TestParameterConstant, 
//			NumericParserType::UniqueProductions::Context
//		>(debug);
//	if(const bool parse_success = (context.has_value() == true); parse_success == false)
//	{
//		std::cerr << "\n" << test_location.file_name() << ": " << test_location.line() << ": "
//				<< "Failed to parse or retrieve constants from \"" 
//				<< TestParameterConstant.string << "\"\n";
//		CHECK((parse_success == true));
//	}
//	else
//	{
//		for(size_t ii = 0; ii < names.size(); ++ii)
//		{
//			const auto result = retrieve_constant<TypeTagParameterConstant>(context, names[ii]);
//			if(debug == true)
//				std::cout << "{" << names[ii] << ":" << to_string(result) << "}\n";
//			if(const bool check_result = (result == expected[ii]);
//					check_result == false)
//			{
//				std::cerr << "\n" << test_location.file_name() << ": " << test_location.line() << ": "
//						<< "Check failed with " 
//						<< names[ii] << " = " << to_string(result) << " and " 
//						<< to_string(expected[ii]) << "\n";
//				CHECK((check_result == true));
//			}
//		}
//	}
//}
//
//
//
//TEST_GROUP(FunctionDeclarations) {};
//
//TEST(FunctionDeclarations, DeclareConstantFromLiteral)
//{
//	bool debug = false;
//	//check_constant<
//	//		FixedString{"let TheQuestion = 42u"}, 
//	//		Construct::Constant
//	//	>(debug);
//	//runtime_parse<
//	//		NumericParserType, 
//	//		FixedString{"let TheQuestion = 2u * 21u"}, 
//	//		Construct::Constant
//	//	>(debug);
//	//auto context = runtime_parse<
//	//		NumericParserType, 
//	//		FixedString{"let TheQuestion = 2u * 20u +2u;"}, 
//	//		NumericParserType::UniqueProductions::Context
//	//	>(debug);
//	//print_constant<NumericTypeTag::Whole>(context, std::string{"TheQuestion"});
//	//TestingParser<NumericParserType>::parser<
//	//		NumericParserType::UniqueProductions::Context
//	//	>.write_diag_str(std::cerr);
//	check_contex_constant<
//			FixedString{"let TheAnswer = 42u;"}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer"}, std::vector{42u}, debug);
//	check_contex_constant<
//			FixedString{"let TheAnswer = 2u * 20u + 2u;"}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer"}, std::vector{42u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer"}, std::vector{42u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"PieDay"}, std::vector{314u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"PieDay"}, std::vector{314u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"BakerStreet"}, std::vector{221u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer"}, std::vector{42u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer", "PieDay", "BakerStreet"}, std::vector{42u, 314u, 221u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Character
//		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let LetterOfTheDay = 'A';"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer"}, std::vector{42u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let LetterOfTheDay = 'A';"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer", "PieDay", "BakerStreet"}, std::vector{42u, 314u, 221u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let LetterOfTheDay = 'A';"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Character
//		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let LetterOfTheDay = 'A';"
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Character
//		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Character
//		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let LetterOfTheDay = 'A';"
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//				}, 
//			NumericTypeTag::Whole
//		>({"PieDay"}, std::vector{314u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let BakerStreet = 221;\n"
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Whole
//		>({"PieDay"}, std::vector{314u}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let NextLetter = 'B';"
//					"let BakerStreet = 221;\n"
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Character
//		>({"LetterOfTheDay"}, std::vector{'A'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let NextLetter = 'B';"
//					"let BakerStreet = 221;\n"
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Character
//		>({"NextLetter"}, std::vector{'B'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let NextLetter = 'B';"
//					"let BakerStreet = 221;\n"
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Character
//		>({"LetterOfTheDay", "NextLetter"}, std::vector{'A', 'B'}, debug);
//	check_contex_constant<
//			FixedString{
//					"let TheAnswer = 2u * 20u + 2u;\n"
//					"let PieDay = 314u;\n"
//					"let NextLetter = 'B';"
//					"let BakerStreet = 221;\n"
//					"let LetterOfTheDay = 'A';"
//				}, 
//			NumericTypeTag::Whole
//		>({"TheAnswer", "PieDay", "BakerStreet"}, std::vector{42u, 314u, 221u}, debug);
//
//
//
//	//check_contex_constant<
//	//		FixedString{
//	//				"let TheAnswer = 2u * 20u + 2u;\n"
//	//				//"let Cake = false;"
//	//				"let LetterOfTheDay = 'A';"
//	//			}, 
//	//		NumericTypeTag::Whole
//	//	>({"TheAnswer"}, std::vector{42u}, debug);
//	//check_contex_constant<
//	//		FixedString{
//	//				"let TheAnswer = 2u * 20u + 2u;\n"
//	//				"let LetterOfTheDay = 'A';"
//	//			}, 
//	//		NumericTypeTag::Character
//	//	>({"LetterOfTheDay"}, std::vector{'A'}, debug);
//};
//
