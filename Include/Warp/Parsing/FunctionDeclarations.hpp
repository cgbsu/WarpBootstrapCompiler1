#define WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON
#include <Warp/Parsing/BooleanExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP
#define WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

namespace Warp::Parsing
{
	using namespace Warp::Runtime::Compiler;

	enum class FunctionDeclaration
	{
		UnconstrainedSingleParameter, 
		IntermediatePrototype, 
		Prototype, 
		Alternative
	};

	using FunctionDeclaritionTermsType_ = BooleanExpressionTermsType 
		::Prepend<
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
				>, 
			TreeTerm<
					Declaration::Colon, 
					CharTerm, 
					':', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm< 
					Brackets::OpenCurleyBracket, 
					CharTerm, 
					'{', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm< 
					Brackets::ClosedCurleyBracket, 
					CharTerm, 
					'}', 
					ctpg::associativity::no_assoc
				>, 
			TypeTreeTerm<
					FunctionDeclaration::IntermediatePrototype, 
					NonTerminalTerm, 
					std::unique_ptr<AlternativePrototypeType>, 
					FixedString{"AlternativeIntermediatePrototype"}
				>, 
			TypeTreeTerm<
					FunctionDeclaration::Prototype, 
					NonTerminalTerm, 
					std::unique_ptr<AlternativePrototypeType>, 
					FixedString{"AlternativePrototype"}
				>, 
			TypeTreeTerm<
					FunctionDeclaration::Alternative, 
					NonTerminalTerm, 
					std::unique_ptr<AlternativeType>, 
					FixedString{"Alternative"}
				>, 
			TreeTerm<
					Call::Comma, 
					CharTerm, 
					',', 
					ctpg::associativity::no_assoc
				>, 
			TypeTreeTerm<
					Call::IntermediateFunction, 
					NonTerminalTerm, 
					std::unique_ptr<Node<NodeType::FunctionCall>>, 
					FixedString{"IntermediateFunctionFunctionCall"}
				>
		>;

	template<>
	struct TemplateInstantiator<TemplateInstantiationTag::FunctionDeclarationTerms> {
		using Type = FunctionDeclaritionTermsType_;
		Type terms;
	};

	#ifdef WARP__PARSING__ENABLE__TEMPLATE__CACHING
		extern template class TemplateInstantiator<TemplateInstantiationTag::FunctionDeclarationTerms>;
	#endif

	using FunctionDeclaritionTermsType = TemplateInstantiator<TemplateInstantiationTag::FunctionDeclarationTerms>::Type;

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

		using BooleanExpressionParserType = BooleanExpressionParser<
				IntermediateTermsType, 
				TypeResolverParameterTemplate
			>;

		using WholeMathematicalParserType = BooleanExpressionParserType::WholeMathematicalParserType;
		#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON
			using IntegerMathematicalParserType = BooleanExpressionParserType::IntegerMathematicalParserType;
			using FixedPointMathematicalParserType = BooleanExpressionParserType::FixedPointMathematicalParserType;
			using CharacterMathematicalParserType = BooleanExpressionParserType::CharacterMathematicalParserType;
		#endif
		using BoolMathematicalParserType = BooleanExpressionParserType::BoolMathematicalParserType;

		using TermsType = decltype(
				IntermediateTermsType::append_terms(
						typename WholeMathematicalParserType::UniqueTermsType{}, 
						#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON
							typename IntegerMathematicalParserType::UniqueTermsType{}, 
							typename FixedPointMathematicalParserType::UniqueTermsType{}, 
							typename CharacterMathematicalParserType::UniqueTermsType{}, 
						#endif
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
		constexpr static const auto open_curley_bracket = term<Brackets::OpenCurleyBracket>;
		constexpr static const auto close_curley_bracket = term<Brackets::ClosedCurleyBracket>;
		constexpr static const auto semi_colon = term<Declaration::SemiColon>;
		constexpr static const auto colon = term<Declaration::Colon>;
		constexpr static const auto comma = term<Call::Comma>;
		constexpr static const auto constant = term<Construct::Constant>;
		constexpr static const auto intermediate_prototype = term<FunctionDeclaration::IntermediatePrototype>;
		constexpr static const auto prototype = term<FunctionDeclaration::Prototype>;
		constexpr static const auto alternative = term<FunctionDeclaration::Alternative>;
		constexpr static const auto context = term<Construct::Context>;
		constexpr static const auto intermediate_function_call = term<Call::IntermediateFunction>;
		constexpr static const auto function_call = term<Call::Function>;

		constexpr static const auto unique_terms = ctpg::terms(
				let_keyword, 
				equal, 
				semi_colon, 
				colon, 
				comma, 
				open_curley_bracket, 
				close_curley_bracket
			); 

		constexpr static const auto terms = concatinate_tuples(
				BooleanExpressionParserType::terms, // Using this to include NumericLiteral/all previous terms
				unique_terms
			);

		constexpr static const auto unique_non_terminal_terms = ctpg::nterms(
				constant, 
				intermediate_prototype, 
				prototype, 
				alternative, 
				context, 
				intermediate_function_call, 
				function_call
			);

		constexpr static const auto non_terminal_terms = concatinate_tuples(
				BooleanExpressionParserType::non_terminal_terms, /* Using this to include 
						NumericLiteral/all previous non-terminal-terms */
				unique_non_terminal_terms
			);

		constexpr static const auto make_function_call()
		{
			return ctpg::rules(
					intermediate_function_call(identifier, open_parenthesis)
					>=[](auto name, auto open_parenthesis_) {
						return std::make_unique<Node<NodeType::FunctionCall>>(std::move(Node<NodeType::FunctionCall>()));
					}, 
					function_call(intermediate_function_call, close_parenthesis)
					>=[](auto&& call, auto clos_parenthesis_) {
						return std::move(call);
					}
				);
		}

		template<typename MathematicalExpressionGeneratorParameterType>
		constexpr static const auto call_from_math_term()
		{
			using TagType = typename MathematicalExpressionGeneratorParameterType
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
					intermediate_function_call(intermediate_function_call, expression_term, comma)
					>=[](auto&& function_call_, auto&& expression, auto comma_) {
						function_call_->arguments.push_back(std::move(expression.node));
						return std::move(function_call_);
					}, 
					function_call(intermediate_function_call, expression_term, close_parenthesis)
					>=[](auto&& function_call_, auto&& expression, auto close_parenthesis_) {
						function_call_->arguments.push_back(std::move(expression.node));
						return std::move(function_call_);
					}
				);
		}

		template<typename MathematicalExpressionGeneratorParameterType>
		constexpr static const auto function_from_math_term()
		{
			using TagType = typename MathematicalExpressionGeneratorParameterType
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
						auto type_tag = make_clean_any<
								decltype(reduction_tag)
							>(reduction_tag);
						return std::move(ConstantType{
								std::string{name}, 
								std::move(type_tag), 
								std::move(expression.node)
							});
					}, 
					alternative(prototype, open_curley_bracket, expression_term, close_curley_bracket)
					>=[](auto&& function_prototype, auto open_curley_bracket_, auto expression, auto close_curley_bracket_) 
					{
						return std::make_unique<AlternativeType>(std::move(AlternativeType(
								std::move(*function_prototype.get()), 
								std::move(expression.node)
							)));
					}
				);
		}

		constexpr static const auto to_context_rules(auto from_term)
		{
			return ctpg::rules(
					context(from_term, semi_colon)
					>= [](auto new_object, auto semi_colon_) {
						ContextType new_context;
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
							to_tag(OperationalValueTag::InferFromEvaluation), 
							std::move(ConstantValueType{std::move(constant_call(std::string{std::string_view{value}}))})
						};
				};

		consteval static const auto parse_parameter()
		{
			constexpr const auto expression_term 
					= BooleanExpressionParserType::template term<BooleanExpression::Expression>;
			return ctpg::rules(
					intermediate_prototype(intermediate_prototype, identifier, colon, open_curley_bracket, expression_term, close_curley_bracket, comma)
					>= [](auto&& prototype_, auto name, auto colon_, auto open_, auto expression, auto close_, auto comma_)
					{
						prototype_->parameters.push_back(SingleParameterType{
								std::string{name}, 
								ConstraintType{std::move(expression)}
							}); 
						return prototype_;
					}, 
					prototype(intermediate_prototype, identifier, colon, open_curley_bracket, expression_term, close_curley_bracket, close_parenthesis)
					>= [](auto&& prototype_, auto name, auto colon_, auto open_, auto expression, auto close_, auto close_parenthesis_)
					{
						prototype_->parameters.push_back(SingleParameterType{
								std::string{name}, 
								ConstraintType{std::move(expression)}
							}); 
						return prototype_;
					}, 
					intermediate_prototype(intermediate_prototype, identifier, comma)
					>= [](auto&& prototype_, auto name, auto comma_)
					{
						prototype_->parameters.push_back(SingleParameterType{
								std::string{name}, 
								ConstraintType{nullptr}
							}); 
						return prototype_;
					}, 
					prototype(intermediate_prototype, identifier, close_parenthesis)
					>= [](auto&& prototype_, auto name, auto parenthesis_)
					{
						prototype_->parameters.push_back(SingleParameterType{
								std::string{name}, 
								ConstraintType{nullptr}
							});
						return prototype_;
					}
				);
		}

		consteval static const auto declare_function()
		{
			return ctpg::rules(
					intermediate_prototype(let_keyword, identifier, open_parenthesis)
					>= [](auto let_, auto name, auto open_parenthesis_)
					{
						return make_alternative_prototype_with_no_parameters<
								SingleParameterType
							>(std::string{name});
					}, 
					prototype(intermediate_prototype, close_parenthesis)
					>= [](auto prototype_, auto close_parenthesis_) {
						return std::move(prototype_);
					}
				);
		}

		consteval static const auto unique_rules()
		{
			return concatinate_tuples(
					function_from_math_term<WholeMathematicalParserType>(), 
					call_from_math_term<WholeMathematicalParserType>(), 
					#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP__DEBUG__ON
						function_from_math_term<IntegerMathematicalParserType>(), 
						call_from_math_term<IntegerMathematicalParserType>(), 
						function_from_math_term<FixedPointMathematicalParserType>(), 
						call_from_math_term<FixedPointMathematicalParserType>(), 
						function_from_math_term<CharacterMathematicalParserType>(), 
						call_from_math_term<CharacterMathematicalParserType>(), 
					#endif
					function_from_math_term<BoolMathematicalParserType>(), 
					call_from_math_term<BoolMathematicalParserType>(), 
					to_context_rules(constant), 
					to_context_rules(alternative), 
					declare_function(), 
					parse_parameter(), 
					make_function_call(), 
					ctpg::rules(
							alias_value
					)
				);
		}

		consteval static const auto rules()
		{
			return concatinate_tuples( 
					BooleanExpressionParserType::rules(), // Including NumericLiteral/all previous rules
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

	#ifdef WARP__PARSING__ENABLE__TEMPLATE__CACHING
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
	#endif
	
	using NumericParserType = FunctionDeclarationParser<
			FunctionDeclaritionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag
		>;
}


#endif // WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

