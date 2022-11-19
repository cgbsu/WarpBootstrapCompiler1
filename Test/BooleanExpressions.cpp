#include <Warp/Testing/Enable.hpp>
#ifdef WARP__TESTING__HEADER__TESTING__TEST__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON

#include <Warp/Parsing/BooleanExpressions.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ParseTestingUtilities.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>
#include <fstream>

using namespace Warp::Utilities;
using namespace Warp::Parsing;
using namespace Warp::Testing;
using namespace Warp::Runtime::Compiler::SimpleExecutor;

using TestParserType = BooleanExpressionParser<
		BooleanExpressionTermsType, 
		NumericTypeResolver
	>;

using UnderlyingBoolType = typename NumericTypeResolver<NumericTypeTag::Bool>::Type::UnderylingType;
using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;

constexpr static const bool from_warp_bool(WarpBool from) {
	return from == WarpBool::True;
}

constexpr static const auto compare_value = [](
		const auto& left, 
		const auto& right, 
		bool debug, 
		std::source_location location
	)
{
	if(debug == true)
		std::cout << "Start Retrieving Value, with Expected: " << from_warp_bool(right) << "\n";
	const auto actual = retrieve_value<
			typename NumericTagResolver<CleanType<decltype(right)>>::NumericType
		>(left.get(), debug).value();
	bool result = (actual == right);
	if(result == false)
	{
		std::cout << location.file_name() << ": " << location.line() 
				<< ": Comparision failure! With Actual: " << result 
				<< " vs. Expected: " << from_warp_bool(right) << "\n";
	}
	return result;
};

template<auto TestParameterConstant>
auto parse_bool(bool debug = false, std::source_location location = std::source_location::current())
{
	if(debug == true)
		std::cout << "(Bool) Parsing: " << TestParameterConstant.string << "\n";
	auto result = runtime_parse<
				TestParserType, 
				TestParameterConstant, 	
				BooleanExpression::Expression
			>(debug);
	if(result.has_value() == false) {
		std::cout << location.file_name() << ": " << location.line() 
				<< ": Failed to parse Whole with: " << TestParameterConstant.string << "\n";
	}
	return result;
}

template<auto TestParameterConstant>
void print_bool(bool debug = false, std::source_location location = std::source_location::current())
{
	const auto result = parse_bool<TestParameterConstant>(debug, location);
	std::cout << "\nBoolResult: " << TestParameterConstant.string 
			<< " = " << to_string(retrieve_value<NumericTypeResolver<NumericTypeTag::Bool>::Type>(
					result.value().get(), debug
				).value()) << "\n\n";
}

template<auto TestParameterConstant>
void bool_test(
		UnderlyingBoolType expected, 
		bool debug = false, 
		std::source_location location = std::source_location::current()
	)
{
	check_parse<compare_value>(
			parse_bool<TestParameterConstant>(debug, location) /*Actual*/, 
			expected, /*Expected*/
			debug, 
			location
		);
}

static bool has_dumped = false;

TEST_GROUP(BooleanExpressionTests)
{
	void setup()
	{
		if(has_dumped == false)
		{
			std::string parser_dump_file_name{"boolean_expression_parser_dump.txt"};
			std::cout << "\n<Dumping Boolean Test Parser Diagnostic to: " << parser_dump_file_name << ">\n";
			std::ofstream parse_dumper;
			parse_dumper.open(parser_dump_file_name);
			TestParserType::parser.write_diag_str(parse_dumper);
			parse_dumper.close();
			std::cout << "\n</Dumping Boolean Test Parser Diagnostic to: " << parser_dump_file_name << ">\n";
			has_dumped = true;
		}
	}
};

TEST(BooleanExpressionTests, Comparisons)
{
	bool debug = false;
	bool_test<FixedString{"2u > 1u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u <= 1u"}>(WarpBool::False, debug);
	bool_test<FixedString{"1u <= 1u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u >= 1u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u >= 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 2u"}>(WarpBool::False, debug);

	bool_test<FixedString{"200u > 5498u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5498u > 200u"}>(WarpBool::True, debug);
	bool_test<FixedString{"1232u < 4895u"}>(WarpBool::True, debug);
	bool_test<FixedString{"424u <= 212u"}>(WarpBool::False, debug);
	bool_test<FixedString{"212u <= 212u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2131u <= 12131u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2131u <= 2131u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5462u >= 5783u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5462u >= 5462u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2765456u >= 221397u"}>(WarpBool::True, debug);
	bool_test<FixedString{"23947u > 23947u"}>(WarpBool::False, debug);
	bool_test<FixedString{"248u < 248u"}>(WarpBool::False, debug);

	bool_test<FixedString{"23809u = 23809u"}>(WarpBool::True, debug);
	bool_test<FixedString{"23809u = 38890u"}>(WarpBool::False, debug);
	bool_test<FixedString{"38890u = 23809u"}>(WarpBool::False, debug);
};

TEST(BooleanExpressionTests, LogicalOperatorsWithComparisons)
{
	bool debug = false;
	bool_test<FixedString{"5u > 4u && 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u < 4u || 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u || 3u < 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u < 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u && 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u && 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"!5u > 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u && 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u || 3u < 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u < 4u && 3u > 2u || 5u > 8u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u && 3u > 2u || 5u > 8u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u > 4u && 3u < 2u || 5u > 8u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u > 4u && 3u > 2u || 7u = 7u && 8u < 10u"}>(WarpBool::True, debug);
	bool_test<FixedString{"5u > 4u && 3u = 2u || 7u = 7u && 8u > 10u"}>(WarpBool::False, debug);
	bool_test<FixedString{"5u = 4u && 3u > 2u || 7u = 7u && 8u > 10u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 1u && 3u > 4u || 5u > 4u && 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u || 5u > 4u && 3u < 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u || 5u < 4u && 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u || 3u < 4u && 5u > 4u && 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 1u || 3u < 4u && 5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 1u || 3u < 4u && 5u > 4u && 3u < 2u"}>(WarpBool::False, debug);

	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u > 4u || 3u < 2u"}>(WarpBool::True, debug);

	bool_test<FixedString{"2u < 1u && 3u < 4u && 5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && 3u > 4u && 5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u < 4u || 3u > 2u"}>(WarpBool::True, debug);

	bool_test<FixedString{"2u < 1u && 3u < 4u && 5u > 4u || 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u > 1u && 3u > 4u && 5u > 4u || 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u < 4u || 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 1u && 3u < 4u && 5u < 4u || 3u < 2u"}>(WarpBool::False, debug);
};

TEST(BooleanExpressionTests, LogicalOperatorsWithParenthesis)
{
	bool debug = false;
	bool_test<FixedString{"2u > 1u && 3u < 4u && (5u < 4u || 3u > 2u)"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && (5u < 4u || 3u > 2u) && 3u < 4u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && (5u < 4u && 5u > 4u || 3u > 2u) && 3u < 4u"}>(WarpBool::True, debug);
	bool_test<FixedString{"(5u < 4u || 3u > 2u) && 2u > 1u && 3u < 4u"}>(WarpBool::True, debug);

	bool_test<FixedString{"2u > 1u && 3u < 4u && (5u < 4u || 3u < 2u)"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u > 1u && (5u < 4u || 3u < 2u) && 3u < 4u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u > 1u && (5u < 4u && 5u < 4u || 3u < 2u) && 3u < 4u"}>(WarpBool::False, debug);
	bool_test<FixedString{"(5u < 4u || 3u < 2u) && 2u > 1u && 3u < 4u"}>(WarpBool::False, debug);

	bool_test<FixedString{"2u > 1u || 3u < 4u || (5u > 4u && 3u > 2u)"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u || (5u < 4u || 3u > 2u) || 3u < 4u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u || (5u < 4u || 5u < 4u && 3u < 2u) || 3u < 4u"}>(WarpBool::True, debug);
	bool_test<FixedString{"(5u < 4u && 3u < 2u) || 2u > 1u || 3u < 4u"}>(WarpBool::True, debug);

	bool_test<FixedString{"2u < 1u || 3u < 4u || (5u > 4u && 3u < 2u)"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u || (5u < 4u || 3u < 2u) || 3u < 4u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u || (5u < 4u || 5u < 4u && 3u < 2u) || 3u < 4u"}>(WarpBool::True, debug);
	bool_test<FixedString{"(5u < 4u && 3u > 2u) || 2u < 1u || 3u < 4u"}>(WarpBool::True, debug);
};

TEST(BooleanExpressionTests, LogicalOperatorsChainingAnds)
{
	bool debug = false;
	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u > 4u && 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u > 4u && 3u < 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u > 1u && 3u < 4u && 5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u > 1u && 3u > 4u && 5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
	bool_test<FixedString{"2u < 1u && 3u < 4u && 5u < 4u && 3u > 2u"}>(WarpBool::False, debug);
};

TEST(BooleanExpressionTests, LogicalOperatorsChainingOrs)
{
	bool debug = false;
	bool_test<FixedString{"2u > 1u || 3u < 4u || 5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u || 3u < 4u || 5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u || 3u > 4u || 5u > 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u || 3u < 4u || 5u < 4u || 3u > 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u > 1u || 3u < 4u || 5u > 4u || 3u < 2u"}>(WarpBool::True, debug);
	bool_test<FixedString{"2u < 1u || 3u > 4u || 5u < 4u || 3u < 2u"}>(WarpBool::False, debug);
};

TEST(BooleanExpressionTests, BoolArithmaticWithLogicalExpressions)
{
	bool debug = false;
	bool_test<FixedString{"true && true"}>(WarpBool::True, debug);
	bool_test<FixedString{"false && true"}>(WarpBool::False, debug);
	bool_test<FixedString{"true && false"}>(WarpBool::False, debug);
	bool_test<FixedString{"false && false"}>(WarpBool::False, debug);

	bool_test<FixedString{"true || true"}>(WarpBool::True, debug);
	bool_test<FixedString{"false || true"}>(WarpBool::True, debug);
	bool_test<FixedString{"true || false"}>(WarpBool::True, debug);
	bool_test<FixedString{"false || false"}>(WarpBool::False, debug);

	bool_test<FixedString{"!true"}>(WarpBool::False, debug);
	bool_test<FixedString{"!false"}>(WarpBool::True, debug);


	bool_test<FixedString{"!false || false"}>(WarpBool::True, debug);
	bool_test<FixedString{"!false || !false"}>(WarpBool::True, debug);
	bool_test<FixedString{"false || !true"}>(WarpBool::False, debug);
};

#endif // WARP__TESTING__HEADER__TESTING__TEST__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON

