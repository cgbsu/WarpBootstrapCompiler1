#include <Warp/Utilities.hpp>

#ifndef WARP__PARSING__HEADER__TERMS__HPP
#define WARP__PARSING__HEADER__TERMS__HPP

namespace Warp::Parsing
{
	/*using namespace Warp::Utilities;
	template<auto ValueParameterConstant>
	struct Value {
		constexpr static const decltype(ValueParameterConstant) value 
				= ValueParameterConstant;
	};
	
	template<typename TermParameterType>
	concept TermConcept = TermParameterType::term;
	
	template<
			const auto& PatternParameterConstant,
			const auto& NameParameterConstant, 
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
			const auto& StringParameterConstant,
			const auto& NameParameterConstant, 
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

	template<typename ValueParameterType, const auto& NameParameterConstant>
	struct NonTerminalTerm
	{
		using NameType = TemplateStringType<NameParameterConstant>;
		constexpr const static char name[NameType::length] = NameType::string;
		using ValueType = ValueParameterType;
		constexpr const static ctpg::nterm<ValueType> term(NameType::string);
	};
	*/

}
namespace Warp::Parsing
{
	template<auto TagParameterConstant>
	struct Term
	{
		constexpr static auto term(
				const int priority = 0, 
				const ctpg::associativity associativity = ctpg::associativity::no_assoc
			) {}
	};
}
#ifdef WARP__PARSING__HEADER__TERMS__HPP__USE_TERM_DEFINITION_MACROS

#define WARP_STRING_TERM(TAG, STRING) \
	template<> \
	struct Term< TAG > \
	{ \
		constexpr static const char string[] = STRING ; \
		constexpr static auto term( \
				const int priority = 0, \
				const ctpg::associativity associativity = ctpg::associativity::no_assoc \
			) \
		{ \
			return ctpg::string_term(string, priority, associativity); \
		} \
	};

#define WARP_REGEX_TERM(TAG, PATTERN, NAME) \
	template<> \
	struct Term< TAG > \
	{ \
		constexpr static const char name[] = NAME ; \
		constexpr static const char regex[] = PATTERN ; \
		constexpr static auto term( \
				const int priority = 0, \
				const ctpg::associativity associativity = ctpg::associativity::no_assoc \
			) \
		{ \
			return ctpg::regex_term<regex>(name, priority, associativity); \
		} \
	};

#define WARP_CHARACTER_TERM(TAG, CHARACTER) \
	template<> \
	struct Term< TAG > \
	{ \
		constexpr static const char character = CHARACTER ; \
		constexpr static auto term( \
				const int priority = 0, \
				const ctpg::associativity associativity = ctpg::associativity::no_assoc \
			) \
		{ \
			return ctpg::char_term(character, priority, associativity); \
		} \
	};

#define WARP_NON_TERMINAL_TERM(TAG, TYPE, NAME) \
	template<> \
	struct Term< TAG > \
	{ \
		constexpr static const char name[] = NAME ; \
		constexpr static auto term() { \
			return ctpg::nterm< TYPE >(name); \
		} \
	};

#endif
#endif // WARP__PARSING__HEADER__TERMS__HPP


