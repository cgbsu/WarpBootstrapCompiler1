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
		>::AddOnePriority<
			TreeTerm<
					Declaration::SemiColon, 
					CharTerm, 
					';', 
					ctpg::associativity::no_assoc
				>
		>::AddOnePriority<
			TypeTreeTerm<
					Declaration::Constant, 
					NonTerminalTerm, 
					std::string, 
					FixedString{"ConstantDeclaration"}
				>
		>;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto ReductionTagParameterConstant, 
			typename TypeTagParameterType, 
			typename ContextParamterType
		>
	struct FunctionDeclarationParser
	{
		using TypeType = TypeTagParameterType;
		using ContextType = ContextParamterType;

		using BaseTermsType = TermsParameterType;

		constexpr static const auto reduce_to_tag = ReductionTagParameterConstant;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<
				NonTerminalTypeTagParameterConstant
			>::Type;

		using ConstantType = Constant<SyntaxNode, TypeType>;

		using TermsType = BaseTermsType::template AddOnePriority<
				TypeTreeTerm<
						Construct::Constant, 
						NonTerminalTerm, 
						ConstantType, 
						FixedString{"Constant"}
					>
			>;


		using WholeMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Whole, 
				TermsType, 
				TypeResolverParameterTemplate
			>;

		using IntegerMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Integer, 
				TermsType, 
				TypeResolverParameterTemplate
			>;

		using FixedPointMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::FixedPoint, 
				TermsType, 
				TypeResolverParameterTemplate
			>;

		using CharacterMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Character, 
				TermsType, 
				TypeResolverParameterTemplate
			>;

		using BoolMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Bool, 
				TermsType, 
				TypeResolverParameterTemplate
			>;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto let_keyword = term<Keyword::Let>;
		constexpr static const auto equal = term<MultiPurposeOperator::Equal>;
		constexpr static const auto identifier = term<Identifier::Identifier>;
		constexpr static const auto open_parenthesis = term<Brackets::OpenParenthesis>;
		constexpr static const auto close_parenthesis = term<Brackets::CloseParenthesis>;
		constexpr static const auto semi_colon = term<Declaration::SemiColon>;
		constexpr static const auto constant_declaration = term<Declaration::Constant>;
		constexpr static const auto constant = term<Construct::Constant>;

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
				BoolMathematicalParserType::non_terminal_terms, 
				ctpg::terms(
						constant_declaration, 
						constant
					)
			);

		constexpr static const auto constant_declaration_rule 
				= constant_declaration(let_keyword, identifier, equal)
				>= [](auto let, auto name, auto equal) {
					return std::string{name};
				};

		template<typename MathematicalExpressionGeneratorParameterType>
		constexpr static const auto constant_from_math_term()
		{
			using TagType = MathematicalExpressionGeneratorParameterType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto reduction_tag 
					= MathematicalExpressionGeneratorParameterType::reduce_to_term_tag;
			constexpr const auto expression_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Expression>;
			return ctpg::rules(
					constant(constant_declaration, expression_term, semi_colon)
					>>=[](auto& context, auto declaration, auto expression, auto semi_colon)
					{
						const auto name = std::string{declaration};
						const auto constant = ConstantType{name, reduction_tag, expression};
						context[name] = constant;
					}
				);
		}

		consteval static const auto rules()
		{
			return ctpg::rules(
					constant_declaration_rule, 
					constant_from_math_term<WholeMathematicalParserType>()
				);
		}
	};
}

#endif // WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

