#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP

namespace Warp::Parsing
{
	enum class BooleanExpression
	{
		LogicalAndOperator, 
		LogicalOrOperator, 
		LogicalNotOperator, 
		LogicalTerm, 
		LogicalOr, 
		LogicalNot, 
		GreaterThan, 
		LessThan, 
		GreaterThanOrEqualTo, 
		LessThankOrEqualTo, 
		Comparison, 
		LogicalOperation, 
		Expression
	};

	using BooleanExpressionTermsType_ = MathematicalExpressionTermsType::AddOnePriority< 
				TreeTerm<
						MultiPurposeOperator::Equal, 
						CharTerm, 
						'=', 
						ctpg::associativity::no_assoc
					>
			>::AddOnePriority<
				TreeTerm< 
						BooleanExpression::LogicalOrOperator, 
						StringTerm,  
						FixedString{"||"},  
						ctpg::associativity::no_assoc 
					>
			>::AddOnePriority<
					TreeTerm< 
							BooleanExpression::LogicalAndOperator, 
							StringTerm,  
							FixedString{"&&"},  
							ctpg::associativity::no_assoc 
						>
			>::AddOnePriority<
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
							BooleanExpression, 
							FixedString{"GreaterThanOrEqualTo"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::LessThankOrEqualTo, 
							NonTerminalTerm,  
							BooleanExpression, 
							FixedString{"LessThankOrEqualTo"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::Comparison, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"Comparison"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::LogicalOperation, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"LogicalOperation"} 
						>, 
					TypeTreeTerm< 
							BooleanExpression::LogicalTerm, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"LogicalTerm"} 
						>,  
					TypeTreeTerm< 
							BooleanExpression::LogicalOr, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"LogicalOr"} 
						>,  
					TypeTreeTerm< 
							BooleanExpression::LogicalNot, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"LogicalNot"} 
						>,  
					TypeTreeTerm< 
							BooleanExpression::Expression, 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"BooleanLogicExpression"} 
						> 
				>::AddOnePriority<
					TreeTerm< 
							BooleanExpression::LogicalNotOperator, 
							CharTerm,  
							'!',  
							ctpg::associativity::no_assoc 
						>
				>;


		template<>
		struct TemplateInstantiator<TemplateInstantiationTag::BooleanTerms> {
			using Type = BooleanExpressionTermsType_; 
			Type terms;
		};

		#ifdef WARP__PARSING__ENABLE__TEMPLATE__CACHING
			extern template class TemplateInstantiator<TemplateInstantiationTag::BooleanTerms>;
		#endif

		using BooleanExpressionTermsType = TemplateInstantiator<TemplateInstantiationTag::BooleanTerms>::Type;

	//template<typename... OrPairParameterTypes>
	//struct RuleOr
	//{
	//	std::tuple<OrPairParameterTypes...> terms;
	//	constexpr RuleOr(OrPairParameterTypes... terms) noexcept : terms(std::tuple{terms...}) {}
	//	constexpr RuleOr(std::tuple<OrPairParameterTypes...> terms) noexcept : terms(terms) {}
	//	constexpr RuleOr(const RuleOr& other) noexcept = default;
	//	constexpr RuleOr(RuleOr&& other) noexcept = default;
	//	constexpr RuleOr& operator=(const RuleOr& other) noexcept = default;
	//	constexpr RuleOr& operator=(RuleOr&& other) noexcept = default;

	//	constexpr auto after_first(auto first, auto... remaining) const noexcept {
	//		return RuleOr<CleanType<decltype(remaining)>...>(std::tuple{remaining...});
	//	}
	//	template<size_t... IndexParameterConstants>
	//	constexpr auto remove_first_implementation(std::index_sequence<IndexParameterConstants...>) const noexcept {
	//		return after_first(std::get<IndexParameterConstants>(terms)...);
	//	}
	//	constexpr auto remove_first() const noexcept {
	//		return remove_first_implementation(std::make_index_sequence<std::tuple_size_v<decltype(terms)>>());
	//	}
	//};

	//
	//template<size_t... PrecedingIndexParameterConstants, size_t... ProcedingIndexParameterConstants>
	//constexpr auto make_rule_tuple(
	//		auto reduction, 
	//		auto preceding_terms, 
	//		std::index_sequence<PrecedingIndexParameterConstants...> preceding_indicies, 
	//		auto or_term_term, 
	//		auto proceding_terms, 
	//		std::index_sequence<ProcedingIndexParameterConstants...> proceding_indicies, 
	//		auto rule_generator
	//	)
	//{
	//	return std::tuple{
	//			reduction, 
	//			std::tuple{
	//					std::get<PrecedingIndexParameterConstants>(preceding_terms)..., 
	//					or_term_term, 
	//					std::get<ProcedingIndexParameterConstants>(proceding_terms)...
	//				}, 
	//			rule_generator(or_term_term)
	//		};
	//}
	//		

	//template<
	//		typename ReductionParameterType, 
	//		typename... PrecedingTermParameterTypes, 
	//		typename... OrPairParameterTypes, 
	//		typename... ProcedingTermParameterTypes
	//	>
	//requires(sizeof...(OrPairParameterTypes) > 1)
	//constexpr auto reduce_or_rule(
	//		ReductionParameterType reduction, 
	//		std::tuple<PrecedingTermParameterTypes...> preceding_terms, 
	//		RuleOr<OrPairParameterTypes...> or_terms, 
	//		std::tuple<ProcedingTermParameterTypes...> proceding_terms, 
	//		auto rule_generator
	//	)
	//{
	//	return std::tuple_cat(
	//		ctpg::rules(
	//				make_rule_tuple(
	//						reduction, 
	//						preceding_terms, 
	//						std::make_index_sequence<std::tuple_size_v<decltype(preceding_terms)>>(), 
	//						std::get<0>(or_terms.terms), 
	//						proceding_terms, 
	//						std::make_index_sequence<std::tuple_size_v<decltype(proceding_terms)>>(), 
	//						rule_generator
	//					)
	//			), 
	//		reduce_or_rule(
	//				reduction, 
	//				preceding_terms, 
	//				or_terms.remove_first(), 
	//				proceding_terms, 
	//				rule_generator
	//			)
	//	);
	//}
	//
	//template<
	//		typename ReductionParameterType, 
	//		typename... PrecedingTermParameterTypes, 
	//		typename... OrPairParameterTypes, 
	//		typename... ProcedingTermParameterTypes
	//	>
	//requires(sizeof...(OrPairParameterTypes) == 1)
	//constexpr auto reduce_or_rule(
	//		ReductionParameterType reduction, 
	//		std::tuple<PrecedingTermParameterTypes...> preceding_terms, 
	//		RuleOr<OrPairParameterTypes...> or_terms, 
	//		std::tuple<ProcedingTermParameterTypes...> proceding_terms, 
	//		auto rule_generator
	//	)
	//{
	//	return ctpg::rules(
	//			make_rule_tuple(
	//					reduction, 
	//					preceding_terms, 
	//					std::make_index_sequence<std::tuple_size_v<decltype(preceding_terms)>>(), 
	//					std::get<0>(or_terms.terms), 
	//					proceding_terms, 
	//					std::make_index_sequence<std::tuple_size_v<decltype(proceding_terms)>>(), 
	//					rule_generator
	//				)
	//		);
	//}
	//

	//template<auto ConvertParameterConstant, typename TermParameterType>
	//struct OrTermConstructor
	//{
	//	using TermType = TermParameterType;
	//	constexpr static const auto convert = ConvertParameterConstant;
	//	TermType term;
	//	constexpr OrTermConstructor(TermParameterType term) : term(term) {}
	//	constexpr OrTermConstructor(const OrTermConstructor& other) = default;
	//	constexpr OrTermConstructor(OrTermConstructor&& other) = default;
	//	constexpr OrTermConstructor& operator=(const OrTermConstructor& term) = default;
	//	constexpr OrTermConstructor& operator=(OrTermConstructor&& other) = default;
	//};

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

		constexpr static const auto logical_and_operator = term<BooleanExpression::LogicalAndOperator>;
		constexpr static const auto logical_or_operator = term<BooleanExpression::LogicalOrOperator>;
		constexpr static const auto logical_not_operator = term<BooleanExpression::LogicalNotOperator>;
		constexpr static const auto logical_term = term<BooleanExpression::LogicalTerm>;
		constexpr static const auto logical_or = term<BooleanExpression::LogicalOr>;
		constexpr static const auto logical_not = term<BooleanExpression::LogicalNot>;
		constexpr static const auto greater_than = term<BooleanExpression::GreaterThan>;
		constexpr static const auto less_than = term<BooleanExpression::LessThan>;
		constexpr static const auto greater_than_or_equal_to = term<BooleanExpression::GreaterThanOrEqualTo>;
		constexpr static const auto less_than_or_equal_to = term<BooleanExpression::LessThankOrEqualTo>;
		constexpr static const auto comparison = term<BooleanExpression::Comparison>;
		constexpr static const auto logical_operation = term<BooleanExpression::LogicalOperation>;
		constexpr static const auto logical_expression = term<BooleanExpression::Expression>;
		constexpr static const auto equal = term<MultiPurposeOperator::Equal>;

		constexpr static const auto open_parenthesis = term<Brackets::OpenParenthesis>;
		constexpr static const auto close_parenthesis = term<Brackets::CloseParenthesis>;

		constexpr static const auto unique_terms = ctpg::terms(
				logical_and_operator, 
				logical_or_operator, 
				logical_not_operator,
				less_than, 
				greater_than, 
				equal
			); 

		
		constexpr static const auto terms = concatinate_tuples(
				WholeMathematicalParserType::terms, // Using this to include NumericLiteral/all previous terms
				//IntegerMathematicalParserType::unique_terms, 
				//FixedPointMathematicalParserType::unique_terms, 
				//CharacterMathematicalParserType::unique_terms, 
				BoolMathematicalParserType::unique_terms, 
				unique_terms
			);

		constexpr static const auto unique_non_terminal_terms = ctpg::nterms(
				less_than_or_equal_to, 
				greater_than_or_equal_to, 
				comparison, 
				logical_term, 
				logical_or, 
				logical_not, 
				logical_operation, 
				logical_expression
			);



		constexpr static const auto non_terminal_terms = concatinate_tuples(
				WholeMathematicalParserType::non_terminal_terms, /* Using this to include 
						NumericLiteral/all previous non-terminal-terms */
				//IntegerMathematicalParserType::unique_non_terminal_terms, 
				//FixedPointMathematicalParserType::unique_non_terminal_terms, 
				//CharacterMathematicalParserType::unique_non_terminal_terms, 
				BoolMathematicalParserType::unique_non_terminal_terms, 
				unique_non_terminal_terms
			);

		constexpr static const auto to_logical_expression(auto from)
		{
			return ctpg::rules(
					logical_expression(from)
					>=[](auto from) {
						return std::move(from);
					}
				);
		}

		template<NodeType LogicalOperatorParameterConstant>
		constexpr static const auto make_logical_term(auto logical_reduction, auto logical_operator)
		{
			using TagType = BoolMathematicalParserType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto expression_term 
					= BoolMathematicalParserType::template term<TagType::Expression>;

			//std::tuple term_constructors{
			//		OrTermConstructor<[](auto&& term) { return std::move(term); }, decltype(comparison)>{comparison}, 
			//		OrTermConstructor<[](auto&& term) { return std::move(term.node); }, decltype(expression_term)>{expression_term}, 
			//		OrTermConstructor<[](auto&& term) { return std::move(term); }, decltype(logical_not)>{logical_not}
			//	};
			//std::tuple<> empty_terms{};
			//RuleOr rulor{term_constructors};
			//auto rules = reduce_or_rule(
			//		logical_reduction, 
			//		empty_terms, 
			//		rulor, 
			//		empty_terms, 
			//		[&](auto term_constructor) { 
			//				return logical_reduction(term_constructor.term) 
			//				>=[&term_constructor](auto&& from) {
			//					return term_constructor.convert(from);
			//				};
			//		}
			//	);
			return ctpg::rules(
					logical_reduction(comparison, logical_operator, comparison)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								LogicalOperatorParameterConstant
							>(std::move(left), std::move(right));
					}, 
					logical_reduction(expression_term, logical_operator, comparison)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								LogicalOperatorParameterConstant
							>(std::move(left.node), std::move(right));
					}, 
					logical_reduction(expression_term, logical_operator, expression_term)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								LogicalOperatorParameterConstant
							>(std::move(left.node), std::move(right.node));
					}, 
					logical_reduction(comparison, logical_operator, expression_term)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								LogicalOperatorParameterConstant
							>(std::move(left), std::move(right.node));
					}, 
				
				
				
					//reduce_or_rule(std::tuple<>{}, RuleOr{std::tuple{comparison, expression_term 
					logical_reduction(comparison)
					>=[](auto&& term) {
						return std::move(term);
					}, 
					logical_reduction(expression_term)
					>=[](auto&& expression) {
						return std::move(expression.node);
					}, 
					logical_reduction(logical_not)
					>=[](auto&& logical_not_) {
						return std::move(logical_not_);
					}

					//std::get<2>(std::get<0>(rules)), 
					//std::get<2>(std::get<1>(rules)), 
					//std::get<2>(std::get<2>(rules))
											

						//, 
					//logical_reduction(logical_operation)
					//>=[](auto&& operation) {
					//	return std::move(operation);
					//}
					//, 
					//logical_operation(logical_reduction)
					//>=[](auto&& operation) {
					//	return operation;
					//}
				);
		}

		constexpr static const auto and_rules()
		{
			using TagType = BoolMathematicalParserType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto expression_term 
					= BoolMathematicalParserType::template term<TagType::Expression>;
			return concatinate_tuples(
					make_logical_term<NodeType::LogicalAnd>(logical_term, logical_and_operator), 
					ctpg::rules(
							logical_term(logical_term, logical_and_operator, comparison)
							>=[](auto left, auto operator_, auto right)
							{
								return binary_node<
										NodeType::LogicalAnd
									>(std::move(left), std::move(right));
							}, 
							logical_term(logical_term, logical_and_operator, expression_term)
							>=[](auto left, auto operator_, auto right)
							{
								return binary_node<
										NodeType::LogicalAnd
									>(std::move(left), std::move(right.node));
							}
						)
				);
		}

		constexpr static const auto or_rules()
		{
			using TagType = BoolMathematicalParserType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto expression_term 
					= BoolMathematicalParserType::template term<TagType::Expression>;
			return ctpg::rules(
					logical_or(logical_term, logical_or_operator, logical_term)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								NodeType::LogicalOr
							>(std::move(left), std::move(right));
					}, 
					logical_or(logical_or, logical_or_operator, logical_term)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								NodeType::LogicalOr
							>(std::move(left), std::move(right));
					}
				);
		}

		constexpr static const auto negate_rules()
		{
			using TagType = BoolMathematicalParserType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto expression_term 
					= BoolMathematicalParserType::template term<TagType::Expression>;
			return ctpg::rules(
					//logical_not(logical_not_operator, logical_not_operator, logical_term)
					//>=[](auto first_operator_, auto second_operator_, auto&& operand) {
					//	return operand;
					//}, 
					logical_not(logical_not_operator, comparison)
					>=[](auto operator_, auto operand)
					{
						return unary_node<
								NodeType::LogicalNot
							>(std::move(operand));
					}, 
					logical_not(logical_not_operator, logical_term)
					>=[](auto operator_, auto operand)
					{
						return unary_node<
								NodeType::LogicalNot
							>(std::move(operand));
					}, 
					//logical_not(logical_not_operator, logical_or)
					//>=[](auto operator_, auto operand)
					//{
					//	return unary_node<
					//			NodeType::LogicalNot
					//		>(std::move(operand));
					//}, 
					logical_not(logical_not_operator, expression_term)
					>=[](auto operator_, auto operand)
					{
						return unary_node<
								NodeType::LogicalNot
							>(std::move(operand.node));
					}
				);
		}

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
			return comparison(expression_term, comparison_operator, expression_term)
					>=[](auto left, auto operator_, auto right)
					{
						return binary_node<
								ComparisonParameterConstant
							>(std::move(left.node), std::move(right.node));
					};
		}

		template<BooleanExpression ComparisonParameterConstant>
		constexpr static const auto or_equal_to_operator(auto comparison_operator, auto resulting_operator)
		{
			return resulting_operator(comparison_operator, equal)
					>=[](auto comparison, auto equal_) {
							return ComparisonParameterConstant; // It has to return something...
					};
		}

		template<typename MathematicalExpressionGeneratorParameterType>
		constexpr static const auto mathematical_parser_unique_rules()
		{
			return ctpg::rules(
					subsume_algebraic_expression_to_comparison<
								NodeType::GreaterThan, 
								MathematicalExpressionGeneratorParameterType
						>(greater_than), 
					subsume_algebraic_expression_to_comparison<
								NodeType::LessThan, 
								MathematicalExpressionGeneratorParameterType
						>(less_than), 
					subsume_algebraic_expression_to_comparison<
								NodeType::GreaterThanOrEqualTo, 
								MathematicalExpressionGeneratorParameterType
						>(greater_than_or_equal_to), 
					subsume_algebraic_expression_to_comparison<
								NodeType::LessThankOrEqualTo, 
								MathematicalExpressionGeneratorParameterType
						>(less_than_or_equal_to), 
					subsume_algebraic_expression_to_comparison<
								NodeType::Equal, 
								MathematicalExpressionGeneratorParameterType
						>(equal)
				);
		}

		constexpr static const auto or_equal_to_operators()
		{
			return ctpg::rules(
					or_equal_to_operator<BooleanExpression::LessThankOrEqualTo>(less_than, less_than_or_equal_to), 
					or_equal_to_operator<BooleanExpression::GreaterThanOrEqualTo>(greater_than, greater_than_or_equal_to)
				);
		}

		constexpr static const auto from_parenthesis(auto from_term)
		{
			using TagType = BoolMathematicalParserType
					::TypeSpecificMathematicalExpressionTermTags;
			constexpr const auto expression_term 
					= BoolMathematicalParserType::template term<TagType::Expression>;
			return ctpg::rules(
					expression_term(open_parenthesis, from_term, close_parenthesis)
					>=[](auto open_, auto&& from, auto close_) {
						std::cout << "from: " << friendly_type_name<CleanType<decltype(from)>>() << "\n";
						return std::move(from);
					}
				);
		}

		constexpr static const auto unique_rules()
		{
			return concatinate_tuples(
					or_equal_to_operators(), 
					mathematical_parser_unique_rules<WholeMathematicalParserType>(), 
					//mathematical_parser_unique_rules<IntegerMathematicalParserType>(), 
					mathematical_parser_unique_rules<BoolMathematicalParserType>(), 
					and_rules(), 
					or_rules(), 
					negate_rules(), 
					to_logical_expression(comparison), 
					to_logical_expression(logical_term), 
					to_logical_expression(logical_or), 
					to_logical_expression(logical_operation), 
					//from_parenthesis(comparison), 
					from_parenthesis(logical_term), 
					from_parenthesis(logical_or)
					//from_parenthesis(logical_operation)
				);
		}

		consteval static const auto rules()
		{
			return concatinate_tuples(
					WholeMathematicalParserType::rules(), 
					//IntegerMathematicalParserType::unique_rules(), 
					//FixedPointMathematicalParserType::unique_rules(), 
					//CharacterMathematicalParserType::unique_rules(), 
					BoolMathematicalParserType::unique_rules(), 
					unique_rules()
				);
		}

		constexpr static const auto parser = ctpg::parser(
				logical_expression, 
				terms, 
				non_terminal_terms, 
				rules()
			);
	};

}

#endif // WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP

