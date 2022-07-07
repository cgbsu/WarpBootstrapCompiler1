#include <Warp/Utilities.hpp>

#ifndef WARP__UTILITIES__HEADER__TERMS__HPP
#define WARP__UTILITIES__HEADER__TERMS__HPP

namespace Warp::Parsing
{
	using namespace Warp::Utilities;
	template<auto ValueParameterConstant>
	struct Value {
		constexpr static const decltype(ValueParameterConstant) value 
				= ValueParameterConstant;
	};
	
	template<typename TermParameterType>
	concept TermConcept = TermParameterType::term;
	
	template<
			const char* PatternParameterConstant,
			const char* NameParameterConstant, 
			int PrecedenceParameterConstant = 0, 
			ctpg::associativity AssociativityParameterConstant 
					= ctpg::associativity::no_assoc
		>
	struct RegexTerm
	{
		using PatternType = TemplateStringType<PatternParameterConstant>;
		using NameType = TemplateStringType<NameParameterConstant>;
		using PrecedenceType = Value<PrecedenceParameterConstant>;
		using AssociativityType = Value<AssociativityParameterConstant>;

	    constexpr static const char pattern[PatternType::length] = PatternType::string;
	    constexpr static const char name[NameType::length] = NameType::string;
		constexpr static const int precedence = PrecedenceType::value;
		constexpr static ctpg::associativity associativity = AssociativityType::value;

	    constexpr static ctpg::regex_term<PatternType::string> term(
				NameType::string, 
				PrecedenceType::value, 
				AssociativityType::value
			);
	};
	
	template<
			const char* StringParameterConstant,
			const char* NameParameterConstant, 
			int PrecedenceParameterConstant = 0, 
			ctpg::associativity AssociativityParameterConstant 
					= ctpg::associativity::no_assoc
		>
	struct StringTerm
	{
		using NameType = TemplateStringType<NameParameterConstant>;
		using StringType = TemplateStringType<StringParameterConstant>;
		using PrecedenceType = Value<PrecedenceParameterConstant>;
		using AssociativityType = Value<AssociativityParameterConstant>;

	    constexpr static const char string[StringType::length] = StringType::string;
	    constexpr static const char name[NameType::length] = NameType::string;
		constexpr static int precedence = PrecedenceType::value;
		constexpr static ctpg::associativity associativity = AssociativityType::value;

	    constexpr static ctpg::string_term<StringType::string> term(
				NameType::string, 
				PrecedenceType::value, 
				AssociativityType::value
			);
	};
	
	template<
			char ParameterConstant, 
			int PrecedenceParameterConstant = 0, 
			ctpg::associativity AssociativityParameterConstant 
					= ctpg::associativity::no_assoc
		>
	struct CharTerm
	{
		using CharacterType = Value<ParameterConstant>;
		using PrecedenceType = Value<PrecedenceParameterConstant>;
		using AssociativityType = Value<AssociativityParameterConstant>;

		constexpr static const char character = CharacterType::value;
		constexpr static int precedence = PrecedenceType::value;
		constexpr static ctpg::associativity associativity = AssociativityType::value;

		constexpr static ctpg::char_term term(
				CharacterType::value, 
				PrecedenceType::value, 
				AssociativityType::value
			);
	};

	template<typename ValueParameterType, const char* NameParameterConstant>
	struct NonTerminalTerm
	{
		using NameType = TemplateStringType<NameParameterConstant>;
		constexpr const static char name[NameType::length] = NameType::string;
		using ValueType = ValueParameterType;
		constexpr const static ctpg::nterm<ValueType> term(NameType::string);
	};

}

#endif	// WARP__UTILITIES__HEADER__TERMS__HPP


