#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP
#define WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

namespace Warp::Parsing
{

	using FunctionDeclaritionTermsType = MathematicalExpressionTermsType
		::Prepend<
			TreeTerm<
					MultiPurposeOperator::Equal, 
					CharTerm, 
					'=', 
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

		using IntermediateTermsType = BaseTermsType;


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
				FixedString{"CharacterTerm"}, 
				FixedString{"CharacterSum"}, 
				FixedString{"CharacterExpression"}
			>;

		using BoolMathematicalParserType = HomogenousMathematicalExpressionParser<
				NumericTypeTag::Bool, 
				IntermediateTermsType, 
				TypeResolverParameterTemplate, 
				FixedString{"BoolTerm"}, 
				FixedString{"BoolSum"}, 
				FixedString{"BoolExpression"}
			>;

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
		constexpr static const auto context = term<Construct::Context>;

		constexpr static const auto unique_terms = ctpg::terms(
				let_keyword, 
				equal, 
				//identifier, 
				//open_parenthesis, 
				//close_parenthesis, 
				semi_colon
			); 

		constexpr static const auto terms = concatinate_tuples(
				WholeMathematicalParserType::terms, // Using this to include NumericLiteral/all previous terms
				IntegerMathematicalParserType::unique_terms, 
				FixedPointMathematicalParserType::unique_terms, 
				CharacterMathematicalParserType::unique_terms, 
				BoolMathematicalParserType::unique_terms, 
				unique_terms
			);

		constexpr static const auto unique_non_terminal_terms = ctpg::nterms(
				constant, 
				context
			);

		constexpr static const auto non_terminal_terms = concatinate_tuples(
				WholeMathematicalParserType::non_terminal_terms, /* Using this to include 
						NumericLiteral/all previous non-terminal-terms */
				IntegerMathematicalParserType::unique_non_terminal_terms, 
				FixedPointMathematicalParserType::unique_non_terminal_terms, 
				CharacterMathematicalParserType::unique_non_terminal_terms, 
				BoolMathematicalParserType::unique_non_terminal_terms, 
				unique_non_terminal_terms
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
			constexpr const auto math_term_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Term>;
			constexpr const auto sum_term 
					= MathematicalExpressionGeneratorParameterType::template term<TagType::Sum>;
			constexpr const auto reduce_to_term
					= MathematicalExpressionGeneratorParameterType::template term<reduction_tag>;
			return ctpg::rules(
					constant(let_keyword, identifier, equal, expression_term)
					>=[](auto let_, auto name, auto equal_, auto expression)
					{
						return ConstantType{
								std::string{name}, 
								reduction_tag, 
								std::move(expression.node)
							};
					}
				);
		}

		constexpr static const auto to_context_rules(auto from_term)
		{
			return ctpg::rules(
					context(from_term, semi_colon)
					>= [](auto new_object, auto semi_colon_) {
						Context new_context;
						return new_context.inject(std::move(new_object));
					}, 
					context(context, from_term, semi_colon)
					>= [](auto context, auto new_object, auto semi_colon_) {
						return context.inject(std::move(new_object));
					}
				);
		}

		constexpr static const auto alias_value 
				= constant(let_keyword, identifier, equal, identifier)
				>= [](auto let_, auto name, auto equal_, auto value)
				{
					return ConstantType{
							std::string{name}, 
							OperationalValueTag::InferFromEvaluation, 
							std::move(constant_call(std::string{std::string_view{value}}))
						};
				};

		consteval static const auto unique_rules()
		{
			return concatinate_tuples(
					constant_from_math_term<WholeMathematicalParserType>(), 
					constant_from_math_term<IntegerMathematicalParserType>(), 
					constant_from_math_term<FixedPointMathematicalParserType>(), 
					constant_from_math_term<CharacterMathematicalParserType>(), 
					constant_from_math_term<BoolMathematicalParserType>(), 
					to_context_rules(constant), 
					ctpg::rules(
							alias_value
					)
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

		constexpr static const auto parser = ctpg::parser(
				context, 
				terms, 
				non_terminal_terms, 
				rules()
			);
					
	};

	extern template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Whole, 
			NumericTypeTag::Whole, 
			FixedString{"WholeTerm"}, 
			FixedString{"WholeSum"}, 
			FixedString{"WholeExpression"}
		>;

	extern template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Integer, 
			NumericTypeTag::Integer, 
			FixedString{"IntegerTerm"}, 
			FixedString{"IntegerSum"}, 
			FixedString{"IntegerExpression"}
		>;

	extern template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::FixedPoint, 
			NumericTypeTag::FixedPoint, 
			FixedString{"FixedPointTerm"}, 
			FixedString{"FixedPointSum"}, 
			FixedString{"FixedPointExpression"}
		>;

	extern template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Character, 
			NumericTypeTag::Character, 
			FixedString{"CharacterTerm"}, 
			FixedString{"CharacterSum"}, 
			FixedString{"CharacterExpression"}
		>;

	extern template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Bool, 
			NumericTypeTag::Bool, 
			FixedString{"BoolTerm"}, 
			FixedString{"BoolSum"}, 
			FixedString{"BoolExpression"}
		>;

	extern template class FunctionDeclarationParser<
			FunctionDeclaritionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag
		>;
	
	using NumericParserType = FunctionDeclarationParser<
			FunctionDeclaritionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag
		>;
}

#endif // WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

