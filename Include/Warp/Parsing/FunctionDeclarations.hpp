#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP
#define WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

namespace Warp::Parsing
{
	enum class FunctionTags {
		Constant
	};

	template<typename ValueParameterType, typename TypeParameterType>
	struct Constant
	{
		using ValueType = ValueParameterType;
		using TypeType = TypeParameterType;
		std::string name;
		TypeType type;
		ValueType value;
	};
		

	using FunctionDeclaritionTermsType = MathematicalExpressionTermsType
		::Prepend<
			TreeTerm<
					MultiPurposeOperator::Equal, 
					CharTerm, 
					'=', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					Identifier::Identifier, 
					RegexTerm, 
					FixedString{"[a-zA-Z_][a-zA-Z0-9_]+"}, 
					FixedString{"Identifier"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					Keyword::Let, 
					StringTerm, 
					FixedString{"let"}, 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					Declaration::SemiColon, 
					CharTerm, 
					';', 
					ctpg::associativity::no_assoc
				> 
		>;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			typename TypeTagParameterType
		>
	struct FunctionDeclarationParser
	{
		using TypeType = TypeTagParameterType;

		using BaseTermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<
				NonTerminalTypeTagParameterConstant
			>::Type;

		using ConstantType = Constant<SyntaxNode, TypeType>;

		struct Context
		{
			std::unordered_map<std::string, ConstantType> constants;
			constexpr Context inject(ConstantType constant) {
				constants.insert({constant.name, constant});
				return *this;
			}
		};

		enum class UniqueProductions {
			Context
		};

		using UniqueTermsType = Terms<//BaseTermsType::template AddOnePriority<
				TermsNoPreviousType, 
				BaseTermsType::precedence + 1, 
				TypeTreeTerm<
						Construct::Constant, 
						NonTerminalTerm, 
						ConstantType, 
						FixedString{"Constant"}
					>, 
				TypeTreeTerm< 
						UniqueProductions::Context, 
						NonTerminalTerm, 
						Context, 
						FixedString{"Context"}
					>
			>;

		using IntermediateTermsType = MergeTerms<BaseTermsType, UniqueTermsType>;


		using WholeMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Whole, 
				IntermediateTermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"WholeTerm"}, 
				FixedString{"WholeSum"}, 
				FixedString{"WholeExpression"}
			>;

		using IntegerMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Integer, 
				IntermediateTermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"IntegerTerm"}, 
				FixedString{"IntegerSum"}, 
				FixedString{"IntegerExpression"}
			>;

		using FixedPointMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::FixedPoint, 
				IntermediateTermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"FixedPointTerm"}, 
				FixedString{"FixedPointSum"}, 
				FixedString{"FixedPointExpression"}
			>;

		using CharacterMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Character, 
				IntermediateTermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"CharacterPointTerm"}, 
				FixedString{"CharacterPointSum"}, 
				FixedString{"CharacterPointExpression"}
			>;

		using BoolMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Bool, 
				IntermediateTermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"BoolPointTerm"}, 
				FixedString{"BoolPointSum"}, 
				FixedString{"BoolPointExpression"}
			>;

		//using TermsType = ThisTermsType;
		using TermsType = decltype(
				IntermediateTermsType::append_terms(
						typename WholeMathematicalParserType::UniqueTermsType{}, 
						typename IntegerMathematicalParserType::UniqueTermsType{}, 
						typename FixedPointMathematicalParserType::UniqueTermsType{}, 
						typename CharacterMathematicalParserType::UniqueTermsType{}, 
						typename BoolMathematicalParserType::UniqueTermsType{}
				)
			);

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto let_keyword = term<Keyword::Let>;
		constexpr static const auto equal = term<MultiPurposeOperator::Equal>;
		constexpr static const auto identifier = term<Identifier::Identifier>;
		constexpr static const auto open_parenthesis = term<Brackets::OpenParenthesis>;
		constexpr static const auto close_parenthesis = term<Brackets::CloseParenthesis>;
		constexpr static const auto semi_colon = term<Declaration::SemiColon>;
		constexpr static const auto constant = term<Construct::Constant>;
		constexpr static const auto context = term<UniqueProductions::Context>;

		constexpr static const auto whole_terms = WholeMathematicalParserType::terms;

		constexpr static const auto terms = concatinate_tuples(
				WholeMathematicalParserType::terms, // Using this to include NumericLiteral/all previous terms
				IntegerMathematicalParserType::unique_terms, 
				FixedPointMathematicalParserType::unique_terms, 
				CharacterMathematicalParserType::unique_terms, 
				BoolMathematicalParserType::unique_terms, 
				ctpg::terms(
						let_keyword, 
						equal, 
						identifier, 
						//open_parenthesis, 
						//close_parenthesis, 
						semi_colon
					)
			);

		constexpr static const auto non_terminal_terms = concatinate_tuples(
				WholeMathematicalParserType::non_terminal_terms, /* Using this to include 
						NumericLiteral/all previous non-terminal-terms */
				IntegerMathematicalParserType::unique_non_terminal_terms, 
				FixedPointMathematicalParserType::unique_non_terminal_terms, 
				CharacterMathematicalParserType::unique_non_terminal_terms, 
				BoolMathematicalParserType::unique_non_terminal_terms, 
				ctpg::terms(
						constant, 
						context
					)
			);

		template<typename MathematicalExpressionGeneratorParameterType>
		constexpr static const auto constant_from_math_term()
		{
			using TagType = MathematicalExpressionGeneratorParameterType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto reduction_tag 
					= MathematicalExpressionGeneratorParameterType::reduce_to_term_tag;
			constexpr const auto expression_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Expression>;
			//		= MathematicalExpressionGeneratorParameterType::template term<TagType::Expression>;
			constexpr const auto math_term_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Term>;
			constexpr const auto sum_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Sum>;
			constexpr const auto reduce_to_term
					= MathematicalExpressionGeneratorParameterType::template term<reduction_tag>;
			return ctpg::rules(
					constant(let_keyword, identifier, equal, expression_term)
					>=[](auto let_, auto name, auto equal_, auto expression) {
						return ConstantType{std::string{name}, reduction_tag, expression.node};
					}
				);
		}

		constexpr static const auto to_context_rules(auto from_term)
		{
			return ctpg::rules(
					context(from_term, semi_colon)
					>= [](auto new_object, auto semi_colon_) {
						Context new_context{};
						return new_context.inject(new_object);
					}, 
					context(context, from_term, semi_colon)
					>= [](auto context, auto new_object, auto semi_colon_) {
						return context.inject(new_object);
					}
				);
		}

		consteval static const auto unique_rules()
		{
			return concatinate_tuples(
					constant_from_math_term<WholeMathematicalParserType>(), 
					to_context_rules(constant)
				);
		}

		consteval static const auto rules()
		{
			return concatinate_tuples( 
					WholeMathematicalParserType::rules(), // Including NumericLiteral/all previous rules
					IntegerMathematicalParserType::unique_rules(), 
					FixedPointMathematicalParserType::unique_rules(), 
					CharacterMathematicalParserType::unique_rules(), 
					BoolMathematicalParserType::unique_rules(), 
					unique_rules()
				);
		}
					
	};
}

#endif // WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

