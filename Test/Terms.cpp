#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Utilities;
using namespace Warp::Parsing;

enum class MyTerms
{
	Digits, 
	Dot, 
	Hello
};

TEST_GROUP(Terms)
{
};

TEST(Terms, TermIndexWithTag)
{
	constexpr const auto result = find_term_with_tag_index<
			0, MyTerms::Digits, 
			TreeTerm<MyTerms::Digits, RegexTerm, FixedString("[0-9]+"), FixedString("Digits")>
		>();
	CHECK_EQUAL(0, result.value());
};

