#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP
#define WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

namespace Warp::Parsing
{
	enum class FunctionTags {
		Constant
	};

	using FunctionDeclaritionTermsType = MathematicalExpressionTermsType
		::AddOnePriority<
			TreeTerm<
					MultiPurposeOperator::Equal, 
					CharTerm, 
					'=', 
					ctpg::associativity::no_assoc
				>
		>::AddOnePriority<
			TreeTerm<
					Identifier::Identifier, 
					RegexTerm, 
					FixedString{"[a-zA-Z_][a-zA-Z0-9_]+"}, 
					FixedString{"Identifier"}, 
					ctpg::associativity::no_assoc
				>
		>::AddOnePriority<
			TreeTerm<
					Keyword::Let, 
					StringTerm, 
					FixedString{"let"}, 
					ctpg::associativity::no_assoc
				>
			//TypeTreeTerm<
			//		FunctionTags::Constant, 
			//		NonTerminalTerm, 
					
		>;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto ReductionTagParameterConstant
		>
	struct FunctionDeclarationParser
	{
		using BaseTermsType = TermsParameterType;

		constexpr static const auto reduce_to_tag = ReductionTagParameterConstant;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<
				NonTerminalTypeTagParameterConstant
			>::Type;


		using WholeMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Whole, 
				BaseTermsType, 
				TypeResolverParameterTemplate
			>;

		using IntegerMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Integer, 
				BaseTermsType, 
				TypeResolverParameterTemplate
			>;

		using FixedPointMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::FixedPoint, 
				BaseTermsType, 
				TypeResolverParameterTemplate
			>;

		using CharacterMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Character, 
				BaseTermsType, 
				TypeResolverParameterTemplate
			>;

		using BoolMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Bool, 
				BaseTermsType, 
				TypeResolverParameterTemplate
			>;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = BaseTermsType::template term<TermTagParameterConstant>;

		constexpr static const auto let_keyword = term<Keyword::Let>;
		constexpr static const auto equal = term<MultiPurposeOperator::Equal>;
		constexpr static const auto identifier = term<Identifier::Identifier>;
		constexpr static const auto open_parenthesis = term<Brackets::OpenParenthesis>;
		constexpr static const auto close_parenthesis = term<Brackets::CloseParenthesis>;

		constexpr static const auto reduce_to = term<reduce_to_tag>;

		constexpr static const auto terms = concatinate_tuples(
				WholeMathematicalParserType::terms, 
				IntegerMathematicalParserType::terms, 
				FixedPointMathematicalParserType::terms, 
				CharacterMathematicalParserType::terms, 
				BoolMathematicalParserType::terms, 
				ctpg::terms(
						let_keyword, 
						equal, 
						open_parenthesis, 
						close_parenthesis
					)
			);

		constexpr static const auto non_terminal_terms = concatinate_tuples(
				WholeMathematicalParserType::non_terminal_terms, 
				IntegerMathematicalParserType::non_terminal_terms, 
				FixedPointMathematicalParserType::non_terminal_terms, 
				CharacterMathematicalParserType::non_terminal_terms, 
				BoolMathematicalParserType::non_terminal_terms
			);

	};
}

#endif // WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

