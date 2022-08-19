#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Utilities;
using namespace Warp::Parsing;

enum class MyTerms
{
	// Test Terms //
	Digits, 
	Dot, 
	Hello, 
	// Secondary Test Terms //
	Plus, 
	Goodbye, 
	Minus, 
	LowercaseLetters, 
	// Terciary Test Terms //
	Muliply, 
	Divide, 
	Ciao, 
	UppercaseLetters, 
	// Quanternary Test Terms //
	Raise, 
	OddTokens, 
	Lower, 
	Hola, 
	// Quinary Test Terms //
	Mumble, 
	Grumble, 
	Fumble, 
	// Senery Test Terms //
	LeftParenthesis, 
	LeftCurleyBracket, 
	LeftSquareBracket, 
	LeftAngleBracket, 
	RightParenthesis, 
	RightCurleyBracket, 
	RightSquareBracket, 
	RightAngleBracket
};

TEST_GROUP(Terms) {};

// Test Terms //

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

// Secondary Test Terms //

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

// Terciary Test Terms //

using MultiplyTestTermType = TreeTerm<MyTerms::Muliply, CharTerm, '*', no_associativity>;

using CiaoTestTermType = TreeTerm<
		MyTerms::Ciao, 
		StringTerm, 
		FixedString("Ciao"), 
		no_associativity
	>;

using DivideTestTermType = TreeTerm<MyTerms::Divide, CharTerm, '/', no_associativity>;

using UppercaseLettersTermType = TreeTerm<
		MyTerms::UppercaseLetters, 
		RegexTerm, 
		FixedString("[A-Z]+"), 
		FixedString("UppercaseLetters"), 
		no_associativity
	>;

// Quanternary Test Terms //

using RaiseTestTermType = TreeTerm<MyTerms::Raise, CharTerm, '^', no_associativity>;

using OddTokenTermType = TreeTerm<
		MyTerms::OddTokens, 
		RegexTerm, 
		FixedString("[!@#$%&*]"), 
		FixedString("GrammarTokens"), 
		no_associativity
	>;

using LowerTestTermType = TreeTerm<MyTerms::Lower, CharTerm, '_', no_associativity>;

using HolaTestTermType = TreeTerm<
		MyTerms::Hola, 
		StringTerm, 
		FixedString("Hola"), 
		no_associativity
	>;
// Quinary Test Terms //

using MumbleTestTermType = TreeTerm<
		MyTerms::Mumble, 
		StringTerm, 
		FixedString("Mumble"), 
		no_associativity
	>;

using GrumbleTestTermType = TreeTerm<
		MyTerms::Grumble, 
		StringTerm, 
		FixedString("Grumble"), 
		no_associativity
	>;

using FumbleTestTermType = TreeTerm<
		MyTerms::Fumble, 
		StringTerm, 
		FixedString("Fumble"), 
		no_associativity
	>;

// Senery Test Terms

using LeftParenthesisTestTermType = TreeTerm<MyTerms::LeftParenthesis, CharTerm, '(', no_associativity>;
using LeftCurleyBracketTestTermType = TreeTerm<MyTerms::LeftCurleyBracket, CharTerm, '{', no_associativity>;
using LeftSquareBracketTestTermType = TreeTerm<MyTerms::LeftSquareBracket, CharTerm, '[', no_associativity>;
using LeftAngleBracketTestTermType = TreeTerm<MyTerms::LeftAngleBracket, CharTerm, '<', no_associativity>;

using RightParenthesisTestTermType = TreeTerm<MyTerms::RightParenthesis, CharTerm, ')', no_associativity>;
using RightCurleyBracketTestTermType = TreeTerm<MyTerms::RightCurleyBracket, CharTerm, '}', no_associativity>;
using RightSquareBracketTestTermType = TreeTerm<MyTerms::RightSquareBracket, CharTerm, ']', no_associativity>;
using RightAngleBracketTestTermType = TreeTerm<MyTerms::RightAngleBracket, CharTerm, '>', no_associativity>;

#define TEST_TERMS DigitsTestTermType 
// DotTestTermType, HelloTestTermType

#define SECONDARY_TEST_TERMS \
		PlusTestTermType 
		//GoodbyeTestTermType, \
		//MinusTestTermType, \
		//LowercaseLettersTermType

#define TERCIARY_TEST_TERMS \
		MultiplyTestTermType 
		//CiaoTestTermType, \
		//DivideTestTermType, \
		//UppercaseLettersTermType

#define QUANTERNARY_TEST_TERMS \
		RaiseTestTermType 
		//OddTokenTermType, \
		//LowerTestTermType, \
		//HolaTestTermType 

#define QUINARY_TEST_TERMS \
	MumbleTestTermType 
	//GrumbleTestTermType, \
	//FumbleTestTermType

#define SENERY_TEST_TERMS \
	LeftAngleBracketTestTermType 
	//LeftSquareBracketTestTermType, \
	//LeftCurleyBracketTestTermType, \
	//LeftParenthesisTestTermType, \
	//RightAngleBracketTestTermType, \
	//RightSquareBracketTestTermType, \
	//RightCurleyBracketTestTermType, \
	//RightParenthesisTestTermType


//template<MyTerms TermParameterConstant>
//consteval auto find_term_with_tag_test()
//{
//	return find_term_with_tag_index<
//			0, 
//			TermParameterConstant, 
//			TEST_TERMS
//	>();
//}

//template<MyTerms TermParameterConstant>
//consteval auto get_term_with_tag_concrete()
//{
//	return TreeTermWithTag<
//			true, 
//			TermParameterConstant, 
//			TEST_TERMS
//	>();
//}

//TEST(Terms, TermIndexWithTag)
//{
//	CHECK_EQUAL(0, find_term_with_tag_test<MyTerms::Digits>().value());
//	CHECK_EQUAL(1, find_term_with_tag_test<MyTerms::Dot>().value());
//	CHECK_EQUAL(2, find_term_with_tag_test<MyTerms::Hello>().value());
//};
//
//TEST(Terms, GetTreeTermWithTag)
//{
//	CHECK(MyTerms::Digits == get_term_with_tag_concrete<MyTerms::Digits>().tag);
//	CHECK(MyTerms::Dot == get_term_with_tag_concrete<MyTerms::Dot>().tag);
//	CHECK(MyTerms::Hello == get_term_with_tag_concrete<MyTerms::Hello>().tag);
//};
//
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

//TEST(Terms, TermsObject)
//{
//	using TermsType = Terms<void, 0, TEST_TERMS>;
//	CHECK(compare_regex_terms(
//			DigitsTestTermType::term<0>, 
//			TermsType::term<MyTerms::Digits>
//		));
//	CHECK(DotTestTermType::term<0>.get_data() 
//			== TermsType::term<MyTerms::Dot>.get_data());
//	CHECK(compare_string_terms(
//			HelloTestTermType::term<0>, 
//			TermsType::term<MyTerms::Hello>
//		));
//};
//
//TEST(Terms, MultiLevelTerms)
//{
//	using TermsType = Terms<Terms<void, 1, SECONDARY_TEST_TERMS>, 0, TEST_TERMS>;
//	CHECK(compare_regex_terms(
//			DigitsTestTermType::term<0>, 
//			TermsType::term<MyTerms::Digits>
//		));
//	CHECK(DotTestTermType::term<0>.get_data() 
//			== TermsType::term<MyTerms::Dot>.get_data());
//	CHECK(compare_string_terms(
//			HelloTestTermType::term<0>, 
//			TermsType::term<MyTerms::Hello>
//		));
//	CHECK(PlusTestTermType::term<1>.get_data() 
//			== TermsType::term<MyTerms::Plus>.get_data());
//	CHECK(compare_string_terms(
//			GoodbyeTestTermType::term<1>, 
//			TermsType::term<MyTerms::Goodbye>
//		));
//	CHECK(MinusTestTermType::term<1>.get_data() 
//			== TermsType::term<MyTerms::Minus>.get_data());
//	CHECK(compare_regex_terms(
//			LowercaseLettersTermType::term<1>, 
//			TermsType::term<MyTerms::LowercaseLetters>
//		));
//};
//
////Pretty much the same as last test, may want to "shake it up a little" later on.//
//TEST(Terms, AddOnePriority) 
//{
//	using TermsType = MakeTerms<TEST_TERMS>
//			::AddOnePriority<SECONDARY_TEST_TERMS>;
//	CHECK(compare_regex_terms(
//			DigitsTestTermType::term<0>, 
//			TermsType::term<MyTerms::Digits>
//		));
//	CHECK(DotTestTermType::term<0>.get_data() 
//			== TermsType::term<MyTerms::Dot>.get_data());
//	CHECK(compare_string_terms(
//			HelloTestTermType::term<0>, 
//			TermsType::term<MyTerms::Hello>
//		));
//	CHECK(PlusTestTermType::term<1>.get_data() 
//			== TermsType::term<MyTerms::Plus>.get_data());
//	CHECK(compare_string_terms(
//			GoodbyeTestTermType::term<1>, 
//			TermsType::term<MyTerms::Goodbye>
//		));
//	CHECK(MinusTestTermType::term<1>.get_data() 
//			== TermsType::term<MyTerms::Minus>.get_data());
//	CHECK(compare_regex_terms(
//			LowercaseLettersTermType::term<1>, 
//			TermsType::term<MyTerms::LowercaseLetters>
//		));
//};

TEST(Terms, FlatMerge)
{
	static_assert(std::is_same_v<
			MakeTerms<TEST_TERMS>::FlatMerge<MakeTerms<SECONDARY_TEST_TERMS>>, 
			MakeTerms<TEST_TERMS, SECONDARY_TEST_TERMS>
		>);
	static_assert(std::is_same_v<
			MakeTerms<TEST_TERMS>
					::AddOnePriority<SECONDARY_TEST_TERMS>
					::FlatMerge<Terms<TermsNoPreviousType, 1, TERCIARY_TEST_TERMS>>, 
			MakeTerms<TEST_TERMS>
					::AddOnePriority<
							SECONDARY_TEST_TERMS, 
							TERCIARY_TEST_TERMS
						>
		>);
	static_assert(std::is_same_v<
			Terms<TermsNoPreviousType, 1, TEST_TERMS>
					::FlatMerge<
							MakeTerms<SECONDARY_TEST_TERMS>
									::AddOnePriority<TERCIARY_TEST_TERMS>
						>, 
			MakeTerms<SECONDARY_TEST_TERMS>
					::AddOnePriority<TEST_TERMS, TERCIARY_TEST_TERMS>
		>);
};

TEST(Terms, Merge)
{
	//<Basically test forwarding to flat_merge>//
	static_assert(std::is_same_v<
			MergeTerms<
					MakeTerms<TEST_TERMS>, 
					MakeTerms<SECONDARY_TEST_TERMS>
				>, 
			MakeTerms<TEST_TERMS, SECONDARY_TEST_TERMS>
		>);
	static_assert(std::is_same_v<
			MergeTerms<
					MakeTerms<TEST_TERMS>::AddOnePriority<SECONDARY_TEST_TERMS>, 
					Terms<TermsNoPreviousType, 1, TERCIARY_TEST_TERMS>
				>, 
			MakeTerms<TEST_TERMS>::AddOnePriority<
					SECONDARY_TEST_TERMS, 
					TERCIARY_TEST_TERMS
				>
		>);
	static_assert(std::is_same_v<
			MergeTerms<
					Terms<TermsNoPreviousType, 1, TEST_TERMS>, 
					MakeTerms<SECONDARY_TEST_TERMS>
							::AddOnePriority<TERCIARY_TEST_TERMS>
				>, 
			MakeTerms<SECONDARY_TEST_TERMS>
					::AddOnePriority<TEST_TERMS, TERCIARY_TEST_TERMS>
		>);
	//</Basically test forwarding to flat_merge>//
	static_assert(std::is_same_v<
			MergeTerms<
					Terms<
							Terms<TermsNoPreviousType, 1, TEST_TERMS>, 
							3, 
							SECONDARY_TEST_TERMS
						>, 
					Terms<
							Terms<TermsNoPreviousType, 0, TERCIARY_TEST_TERMS>, 
							2, 
							QUANTERNARY_TEST_TERMS
						>
				>,
			MakeTerms<TERCIARY_TEST_TERMS>
					::AddOnePriority<TEST_TERMS>
					::AddOnePriority<QUANTERNARY_TEST_TERMS>
					::AddOnePriority<SECONDARY_TEST_TERMS>
		>);
	static_assert(std::is_same_v<
			MergeTerms<
					Terms<
							TermsNoPreviousType, 
							4, 
							SECONDARY_TEST_TERMS
						>::AddOnePriority<QUINARY_TEST_TERMS>, 
					Terms<
							TermsNoPreviousType, 
							2, 
							QUANTERNARY_TEST_TERMS
						>::AddOnePriority<SENERY_TEST_TERMS>
				>, 
			Terms<TermsNoPreviousType, 2, QUANTERNARY_TEST_TERMS>
					::AddOnePriority<SENERY_TEST_TERMS>
					::AddOnePriority<SECONDARY_TEST_TERMS>
					::AddOnePriority<QUINARY_TEST_TERMS>
		>);
	static_assert(std::is_same_v<
			MergeTerms<
					Terms<
							Terms<TermsNoPreviousType, 1, TEST_TERMS>, 
							4, 
							SECONDARY_TEST_TERMS
						>::AddOnePriority<QUINARY_TEST_TERMS>, 
					Terms<
							//Terms<TermsNoPreviousType, 0, TERCIARY_TEST_TERMS>, 
							TermsNoPreviousType, 
							2, 
							QUANTERNARY_TEST_TERMS
						>::AddOnePriority<SENERY_TEST_TERMS>
				>, 
			Terms<TermsNoPreviousType, 1, TEST_TERMS>
					::AddOnePriority<QUANTERNARY_TEST_TERMS>
					::AddOnePriority<SENERY_TEST_TERMS>
					::AddOnePriority<SECONDARY_TEST_TERMS>
					::AddOnePriority<QUINARY_TEST_TERMS>
		>);
	using T = 
			MergeTerms<
					Terms<
							Terms<TermsNoPreviousType, 1, TEST_TERMS>, 
							4, 
							SECONDARY_TEST_TERMS
						>::AddOnePriority<QUINARY_TEST_TERMS>, 
					Terms<
							Terms<TermsNoPreviousType, 0, TERCIARY_TEST_TERMS>, 
							2, 
							QUANTERNARY_TEST_TERMS
						>::AddOnePriority<SENERY_TEST_TERMS>
				>;
	//T::nothing;
	static_assert(std::is_same_v<
			T, 
			MakeTerms<TERCIARY_TEST_TERMS>
					::AddOnePriority<TEST_TERMS>
					::AddOnePriority<QUANTERNARY_TEST_TERMS>
					::AddOnePriority<SENERY_TEST_TERMS>
					::AddOnePriority<SECONDARY_TEST_TERMS>
					::AddOnePriority<QUINARY_TEST_TERMS>
		>);
};

