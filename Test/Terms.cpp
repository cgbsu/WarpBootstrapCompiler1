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

using DigitsTestTermType = TreeTerm<
		MyTerms::Digits, 
		RegexTerm, 
		FixedString("[0-9]+"), 
		FixedString("Digits"), 
		no_associativity
	>;
using DotTestTermType = TreeTerm<MyTerms::Dot, CharTerm, '.', no_associativity>;

using HelloTestTermType = TreeTerm<
		MyTerms::Hello, 
		StringTerm, 
		FixedString("Hello"), 
		no_associativity
	>;

#define TEST_TERMS DigitsTestTermType, DotTestTermType, HelloTestTermType

template<MyTerms TermParameterConstant>
consteval auto find_term_with_tag_test()
{
	return find_term_with_tag_index<
			0, 
			TermParameterConstant, 
			TEST_TERMS
	>();
}

template<MyTerms TermParameterConstant>
consteval auto get_term_with_tag()
{
	return TreeTermWithTag<
			TermParameterConstant, 
			TEST_TERMS
	>();
}

TEST(Terms, TermIndexWithTag)
{
	CHECK_EQUAL(0, find_term_with_tag_test<MyTerms::Digits>().value());
	CHECK_EQUAL(1, find_term_with_tag_test<MyTerms::Dot>().value());
	CHECK_EQUAL(2, find_term_with_tag_test<MyTerms::Hello>().value());
};

TEST(Terms, GetTreeTermWithTag)
{
	CHECK(MyTerms::Digits == get_term_with_tag<MyTerms::Digits>().tag);
	CHECK(MyTerms::Dot == get_term_with_tag<MyTerms::Dot>().tag);
	CHECK(MyTerms::Hello == get_term_with_tag<MyTerms::Hello>().tag);
};

std::string_view get_term_pattern(const auto& term) {
	return std::string_view(term.get_data().pattern);
}

std::string_view get_term_string(const auto& term) {
	return std::string_view(term.get_data());
}

bool compare_regex_terms(const auto& first, const auto& second) {
	return get_term_pattern(first) == get_term_pattern(second);
}

bool compare_string_terms(const auto& first, const auto& second) {
	return get_term_string(first) == get_term_string(second);
}

TEST(Terms, TermsObject)
{
	using TermsType = Terms<int, 0, TEST_TERMS>;
	CHECK(compare_regex_terms(DigitsTestTermType::term<0>, TermsType::term<MyTerms::Digits>));
	CHECK(DotTestTermType::term<0>.get_data() == TermsType::term<MyTerms::Dot>.get_data());
	CHECK(compare_string_terms(HelloTestTermType::term<0>, TermsType::term<MyTerms::Hello>));
};

