#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP

namespace Warp::Parsing
{
	enum class BooleanExpression
	{
		LogicalAnd, 
		LogicalOr, 
		LogicalNot, 
		GreaterThan, 
		LessThan, 
		GreaterThanOrEqualTo, 
		LessThankOrEqualTo, 
		Comparison, 
		Expression
	};

	using BooleanExpressionTermsType_ = MathematicalExpressionTermsType::AddOnePriority< 
			TreeTerm< 
					BooleanExpression::LogicalOr,  
					StringTerm,  
					FixedString{"||"},  
					ctpg::associativity::no_assoc 
				>
			>::AddOnePriority<
					TreeTerm< 
							BooleanExpression::LogicalAnd,  
							StringTerm,  
							FixedString{"&&"},  
							ctpg::associativity::no_assoc 
						>
			>::AddOnePriority<
					TreeTerm< 
							BooleanExpression::LogicalNot,  
							CharTerm,  
							'!',  
							ctpg::associativity::no_assoc 
						>,  
					TreeTerm< 
							BooleanExpression::GreaterThan,  
							CharTerm,  
							'>',  
							ctpg::associativity::no_assoc 
						>,  
					TreeTerm< 
							BooleanExpression::LessThan,  
							CharTerm,  
							'<',  
							ctpg::associativity::no_assoc 
						>,  
					TypeTreeTerm< 
							BooleanExpression::GreaterThanOrEqualTo, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"GreaterThanOrEqualTo"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::LessThankOrEqualTo, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"LessThankOrEqualTo"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::Comparison, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"Comparison"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::Expression, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"BooleanLogicExpression"} 
						> 
				>;

	template<>
	struct TemplateInstantiator<TemplateInstantiationTag::BooleanTerms> {
		using Type = BooleanExpressionTermsType_; 
		Type terms;
	};

	extern template class TemplateInstantiator<TemplateInstantiationTag::BooleanTerms>;

	using BooleanExpressionTermsType = TemplateInstantiator<TemplateInstantiationTag::BooleanTerms>::Type;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate
		>
	struct BooleanExpressionParser
	{
		using TermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<
				NonTerminalTypeTagParameterConstant
			>::Type;

		using WholeMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Whole, 
				TermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"WholeTerm"}, 
				FixedString{"WholeSum"}, 
				FixedString{"WholeExpression"}
			>;

		using IntegerMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Integer, 
				TermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"IntegerTerm"}, 
				FixedString{"IntegerSum"}, 
				FixedString{"IntegerExpression"}
			>;

		using FixedPointMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::FixedPoint, 
				TermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"FixedPointTerm"}, 
				FixedString{"FixedPointSum"}, 
				FixedString{"FixedPointExpression"}
			>;

		using CharacterMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Character, 
				TermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"CharacterTerm"}, 
				FixedString{"CharacterSum"}, 
				FixedString{"CharacterExpression"}
			>;

		using BoolMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Bool, 
				TermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"BoolTerm"}, 
				FixedString{"BoolSum"}, 
				FixedString{"BoolExpression"}
			>;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto logical_and = term<BooleanExpression::LogicalAnd>;
		constexpr static const auto logical_or = term<BooleanExpression::LogicalOr>;
		constexpr static const auto logical_not = term<BooleanExpression::LogicalNot>;
		constexpr static const auto greater_than = term<BooleanExpression::GreaterThan>;
		constexpr static const auto less_than = term<BooleanExpression::LessThan>;
		constexpr static const auto greater_than_or_equal_to = term<BooleanExpression::GreaterThanOrEqualTo>;
		constexpr static const auto less_than_or_equal_to = term<BooleanExpression::LessThankOrEqualTo>;
		constexpr static const auto boolean_expression = term<BooleanExpression::Expression>;
		constexpr static const auto equal = term<MultiPurposeOperator::Equal>;

		constexpr static const auto unique_terms = ctpg::terms(
				logical_and, 
				logical_or, 
				logical_not,
				less_than, 
				greater_than
			); 

		constexpr static const auto terms = concatinate_tuples(
				WholeMathematicalParserType::terms, // Using this to include NumericLiteral/all previous terms
				IntegerMathematicalParserType::unique_terms, 
				//FixedPointMathematicalParserType::unique_terms, 
				//CharacterMathematicalParserType::unique_terms, 
				//BoolMathematicalParserType::unique_terms, 
				unique_terms
			);

		constexpr static const auto unique_non_terminal_terms = ctpg::nterms(
				less_than_or_equal_to, 
				greater_than_or_equal_to, 
				boolean_expression
			);

		constexpr static const auto non_terminal_terms = concatinate_tuples(
				WholeMathematicalParserType::non_terminal_terms, /* Using this to include 
						NumericLiteral/all previous non-terminal-terms */
				IntegerMathematicalParserType::unique_non_terminal_terms, 
				//FixedPointMathematicalParserType::unique_non_terminal_terms, 
				//CharacterMathematicalParserType::unique_non_terminal_terms, 
				//BoolMathematicalParserType::unique_non_terminal_terms, 
				unique_non_terminal_terms
			);

		template<NodeType ComparisonParameterConstant, typename MathematicalExpressionGeneratorParameterType>
		constexpr static const auto subsume_algebraic_expression_to_comparison(auto comparison_operator)
		{
			using TagType = MathematicalExpressionGeneratorParameterType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto reduction_tag 
					= MathematicalExpressionGeneratorParameterType::reduce_to_term_tag;
			constexpr const auto expression_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Expression>;
			constexpr const auto math_term_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Term>;
			constexpr const auto sum_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Sum>;
			constexpr const auto reduce_to_term
					= MathematicalExpressionGeneratorParameterType::template term<reduction_tag>;
			return ctpg::rules(
					boolean_expression(expression_term, comparison_operator, expression_term)
					>=[](auto left, auto operator_, auto right) {
						return binary_node(std::move(left), std::move(right));
					}
				);
		}

		constexpr static const auto unique_rules()
		{
			return ctpg::rules(
					subsume_algebraic_expression_to_comparison<
							NodeType::GreaterThan
						>(greater_than)
				);
		}

		constexpr static const auto rules()
		{
			return concatinate_tuples(
					WholeMathematicalParserType::rules(), 
					IntegerMathematicalParserType::rules(), 
					//FixedPointMathematicalParserType::rules(), 
					//CharacterMathematicalParserType::rules(), 
					//BoolMathematicalParserType::rules(), 
					unique_rules()
				);
		}
	};

}

#endif // WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP

