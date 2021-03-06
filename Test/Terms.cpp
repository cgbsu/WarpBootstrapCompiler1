#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Utilities;
using namespace Warp::Parsing;

enum class MyTerms
{
	Digits, 
	Dot, 
	Hello, 
	Plus, 
	Goodbye, 
	Minus, 
	LowercaseLetters
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

using PlusTestTermType = TreeTerm<MyTerms::Plus, CharTerm, '+', no_associativity>;

using GoodbyeTestTermType = TreeTerm<
		MyTerms::Goodbye, 
		StringTerm, 
		FixedString("Goodbye"), 
		no_associativity
	>;

using MinusTestTermType = TreeTerm<MyTerms::Minus, CharTerm, '-', no_associativity>;

using LowercaseLettersTermType = TreeTerm<
		MyTerms::LowercaseLetters, 
		RegexTerm, 
		FixedString("[a-z]+"), 
		FixedString("LowercaseLetters"), 
		no_associativity
	>;

#define TEST_TERMS DigitsTestTermType, DotTestTermType, HelloTestTermType

#define SECONDARY_TEST_TERMS \
		PlusTestTermType, \
		GoodbyeTestTermType, \
		MinusTestTermType, \
		LowercaseLettersTermType

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
			true, 
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
	using TermsType = Terms<void, 0, TEST_TERMS>;
	CHECK(compare_regex_terms(
			DigitsTestTermType::term<0>, 
			TermsType::term<MyTerms::Digits>
		));
	CHECK(DotTestTermType::term<0>.get_data() 
			== TermsType::term<MyTerms::Dot>.get_data());
	CHECK(compare_string_terms(
			HelloTestTermType::term<0>, 
			TermsType::term<MyTerms::Hello>
		));
};

TEST(Terms, MultiLevelTerms)
{
	using TermsType = Terms<Terms<void, 1, SECONDARY_TEST_TERMS>, 0, TEST_TERMS>;
	CHECK(compare_regex_terms(
			DigitsTestTermType::term<0>, 
			TermsType::term<MyTerms::Digits>
		));
	CHECK(DotTestTermType::term<0>.get_data() 
			== TermsType::term<MyTerms::Dot>.get_data());
	CHECK(compare_string_terms(
			HelloTestTermType::term<0>, 
			TermsType::term<MyTerms::Hello>
		));
	CHECK(PlusTestTermType::term<1>.get_data() 
			== TermsType::term<MyTerms::Plus>.get_data());
	CHECK(compare_string_terms(
			GoodbyeTestTermType::term<1>, 
			TermsType::term<MyTerms::Goodbye>
		));
	CHECK(MinusTestTermType::term<1>.get_data() 
			== TermsType::term<MyTerms::Minus>.get_data());
	CHECK(compare_regex_terms(
			LowercaseLettersTermType::term<1>, 
			TermsType::term<MyTerms::LowercaseLetters>
		));
};

//Pretty much the same as last test, may want to "shake it up a little" later on.//
TEST(Terms, AddOnePriority) 
{
	using TermsType = MakeTerms<TEST_TERMS>
			::AddOnePriority<SECONDARY_TEST_TERMS>;
	CHECK(compare_regex_terms(
			DigitsTestTermType::term<0>, 
			TermsType::term<MyTerms::Digits>
		));
	CHECK(DotTestTermType::term<0>.get_data() 
			== TermsType::term<MyTerms::Dot>.get_data());
	CHECK(compare_string_terms(
			HelloTestTermType::term<0>, 
			TermsType::term<MyTerms::Hello>
		));
	CHECK(PlusTestTermType::term<1>.get_data() 
			== TermsType::term<MyTerms::Plus>.get_data());
	CHECK(compare_string_terms(
			GoodbyeTestTermType::term<1>, 
			TermsType::term<MyTerms::Goodbye>
		));
	CHECK(MinusTestTermType::term<1>.get_data() 
			== TermsType::term<MyTerms::Minus>.get_data());
	CHECK(compare_regex_terms(
			LowercaseLettersTermType::term<1>, 
			TermsType::term<MyTerms::LowercaseLetters>
		));
};

