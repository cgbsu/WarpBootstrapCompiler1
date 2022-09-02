#include <Warp/Parsing/MathematicalExpressions.hpp>
#include <Warp/Parsing/FunctionDeclarations.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>

using namespace Warp::Utilities;
using namespace Warp::Testing;
using namespace Warp::Parsing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;

using WholeType = NumericTypeResolver<NumericTypeTag::Whole>::Type;
using IntegerType = NumericTypeResolver<NumericTypeTag::Integer>::Type;
using FixedType = NumericTypeResolver<NumericTypeTag::FixedPoint>::Type;
using CharType = NumericTypeResolver<NumericTypeTag::Character>::Type;
using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;

using NumericConstantType = Constant<SyntaxNode, NumericTypeTag>;
using NumericContexType = std::unordered_map<std::string, NumericConstantType>;

// Test instantiation //
template struct FunctionDeclarationParser<
		FunctionDeclaritionTermsType, 
		NumericTypeResolver, 
		Construct::Constant, 
		NumericTypeTag, 
		NumericConstantType
	>;

using NumericParserType = FunctionDeclarationParser<
		//FunctionDeclaritionTermsType, 
		MathematicalExpressionTermsType, 
		NumericTypeResolver, 
		//Construct::Constant, 
		MathematicalExpression::Expression, 
		NumericTypeTag, 
		NumericConstantType
	>;

TEST_GROUP(FunctionDeclarations) {};

TEST(FunctionDeclarations, DeclareConstantFromLiteral)
{
	const bool debug = true;
	NumericContexType context;
	runtime_parse<
			NumericParserType, 
			//HomogenousMathematicalExpressionParser<
			//		NumericTypeTag::Whole, 
			//		MathematicalExpressionTermsType, 
			//		NumericTypeResolver
			//	>, 
			//FixedString{"let TheQuestion = 2u * 21u"},
			FixedString{"2u * 21u"},
			//NumericTypeTag::Whole
			MathematicalExpression::Expression
			//NumericParserType::WholeMathematicalParserType::TypeSpecificMathematicalExpressionTermTags::Term
			//Construct::Constant
		>(debug);
	//runtime_parse<
	//		NumericParserType, 
	//		//FixedString{"12"}, 
	//		//FixedString{"let TheQuestion = 1u + 41u"}, 
	//		FixedString{"let TheQuestion = 42u"}, 
	//		//FixedString{"let TheQuestion ="},
	//		//NumericTypeTag::Whole
	//		//NumericParserType::WholeMathematicalParserType::TypeSpecificMathematicalExpressionTermTags::Expression
	//		//Declaration::Constant
	//		Construct::Constant
	//	>(debug);
	////std::cout << "Value: " << retrieve_value<WholeType>(constant.value) << "\n";
	//runtime_parse<
	//		NumericParserType, 
	//		//FixedString{"12"}, 
	//		//FixedString{"let TheQuestion = 1u + 41u"}, 
	//		FixedString{"let TheQuestion = 2u * 21u"}, 
	//		//FixedString{"let TheQuestion ="},
	//		//NumericTypeTag::Whole
	//		//NumericParserType::WholeMathematicalParserType::TypeSpecificMathematicalExpressionTermTags::Expression
	//		//Declaration::Constant
	//		Construct::Constant
	//	>(debug);
};

