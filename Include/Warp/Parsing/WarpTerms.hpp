#include <Warp/Parsing/TermWrappers.hpp>
//#include <Warp/Parsing/Terms.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__WARP__TERMS__HPP
#define WARP__PARSING__HEADER__PARSING__WARP__TERMS__HPP
namespace Warp::Parsing
{
	//#define WARP__BOOTSTRAP__COMPILER__SPECIALIZE__TERM(TAG, ...)
	//	template<>
	//	struct WarpTerms
	//TreeTerm<__VA_ARGS__>;
	//TreeTerm<
	//		NumericLiteral::Minus, 
	//		CharTerm, 
	//		'-', 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::Dot, 
	//		CharTerm, 
	//		'.', 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::IntegerMark, 
	//		CharTerm, 
	//		'i', 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::UnsignedMark, 
	//		CharTerm, 
	//		'u', 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::CharacterMark, 
	//		CharTerm, 
	//		'c', 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::FixedMark, 
	//		StringTerm, 
	//		FixedString{"xp"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::BoolMark, 
	//		StringTerm, 
	//		FixedString{"bl"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TypeTreeTerm<
	//		NumericLiteral::Digits, 
	//		NonTerminalTerm, 
	//		std::string, 
	//		FixedString{"Digits"}
	//	>, 
	//TypeTreeTerm<
	//		NumericLiteral::AnyDecimalDigitsReduction, 
	//		NonTerminalTerm, 
	//		std::string, 
	//		FixedString{"AnyDecimalDigitsReduction"}
	//	>, 
	//TreeTerm<
	//		NumericLiteral::Base10Digits, 
	//		RegexTerm, 
	//		FixedString{"([0-9]+)|(0d[0-9]+)"}, 
	//		FixedString{"Base10Digits"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::Base2Digits, 
	//		RegexTerm, 
	//		FixedString{"0b[0-1]+"}, 
	//		FixedString{"Base2Digits"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::Base16Digits, 
	//		RegexTerm, 
	//		FixedString{"0x[0-9A-Fa-f]+"}, 
	//		FixedString{"Base16Digits"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::Base8Digits, 
	//		RegexTerm, 
	//		FixedString{"0o[0-7]+"}, 
	//		FixedString{"Base8Digits"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::CharacterLiteral, 
	//		RegexTerm, 
	//		FixedString{"('[a-zA-Z~`!@#$%^&*()-=_+<>,\\.\"/?;:|{}]')|(' ')"}, //(' ') is a quick fix for now.
	//		FixedString{"CharacterLiteral"}, 
	//		ctpg::associativity::no_assoc
	//	>,
	//TreeTerm<
	//		NumericLiteral::BooleanLiteral, 
	//		RegexTerm, 
	//		FixedString{"(true)|(false)"}, 
	//		FixedString{"BooleanLiteral"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::EscapeCharacterLiteral, 
	//		RegexTerm, 
	//		FixedString{"'\\\\[0nt'\\\\]'"}, 
	//		FixedString{"CharacterLiteral"}, 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::NumericalDelinator, 
	//		CharTerm, 
	//		'_', 
	//		ctpg::associativity::no_assoc
	//	>, 
	//TreeTerm<
	//		NumericLiteral::AnyDecimalDigits, 
	//		RegexTerm, 
	//		FixedString{"\\.[0-9A-Fa-f]+"}, 
	//		FixedString{"AnyDecimalDigits"}, 
	//		ctpg::associativity::no_assoc
	//	>
	//}
}
#endif // WARP__PARSING__HEADER__PARSING__WARP__TERMS__HPP

