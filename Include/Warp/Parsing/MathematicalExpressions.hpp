#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis.hpp>
#include <Warp/Parsing/GeneralTermTags.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

namespace Warp::Parsing
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	enum class MathematicalExpression
	{
		Add, 
		Subtract, 
		Multiply, 
		Divide, 
		// Modulo
		// Raise
		// Log
		// BaseConvert
		// Solve
		OpenPrioritization, 
		ClosePrioritization, 
		Expression
	};

	struct Expression {
		SyntaxNode node;
	};


	using MathematicalExpressionTermsType = NumericLiteralTermsType
		::Prepend<
			TreeTerm<
					MathematicalExpression::Multiply, 
					CharTerm, 
					'*', 
					ctpg::associativity::ltor
				>, 
			TreeTerm<
					MathematicalExpression::Divide, 
					CharTerm, 
					'/', 
					ctpg::associativity::ltor
				>
		>::Prepend<
			TreeTerm<
					MathematicalExpression::Add, 
					CharTerm, 
					'+', 
					ctpg::associativity::ltor
				>, 
			TreeTerm<
					MathematicalExpression::Subtract, 
					CharTerm, 
					'-', 
					ctpg::associativity::ltor
				>, 
			TypeTreeTerm<
					MathematicalExpression::Expression, 
					NonTerminalTerm, 
					Expression, 
					FixedString{"Expression"}
				>
		>::AddOnePriority<
			TreeTerm<
					Brackets::OpenParenthesis, 
					CharTerm, 
					'(', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					Brackets::CloseParenthesis, 
					CharTerm, 
					')', 
					ctpg::associativity::no_assoc
				>
		>;
		


	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto ReductionTagParameterConstant, 
			auto InputTermTagParameterConstant
		>
	requires
			HasTypeConcept<TypeResolverParameterTemplate<ReductionTagParameterConstant>> 
			&& HasTypeConcept<TypeResolverParameterTemplate<InputTermTagParameterConstant>>
			&& std::is_convertible_v<
					typename TypeResolverParameterTemplate<InputTermTagParameterConstant>::Type, 
					typename TypeResolverParameterTemplate<ReductionTagParameterConstant>::Type
				>
	struct MathematicalExpressionParser : NumericLiteralParser<
			TermsParameterType, 
			TypeResolverParameterTemplate
		>
	{
		using BaseTermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<NonTerminalTypeTagParameterConstant>::Type;

		constexpr static const auto reduce_to_term_tag = ReductionTagParameterConstant;
		constexpr static const auto input_term_tag = ReductionTagParameterConstant;

		using ReduceToType = TypeResolverTemplate<reduce_to_term_tag>;
		using InputType = TypeResolverTemplate<input_term_tag>; // TODO: See if I will need to get non-terminal-terms in here

		using BaseType = NumericLiteralParser<BaseTermsType, TypeResolverParameterTemplate>;

		//enum class AlternativeTag
		//{
		//	Product, 
		//	Sum, 
		//	Term, 
		//	ReduceTo, 
		//	None
		//};

		struct Term
		{
			SyntaxNode node;
			constexpr Term(SyntaxNode&& node) noexcept : node(node) {}
			constexpr Term(SyntaxNode& node) noexcept : node(node) {}
			constexpr Term(InputType value) noexcept : node(literal_node(value)) {}
			constexpr Term() noexcept = default;
			constexpr Term(const Term& other) noexcept = default;
			constexpr Term(Term&& other) noexcept = default;

			constexpr Term& operator=(const Term& other) noexcept = default;
			constexpr Term& operator=(Term&& other) noexcept = default;

			constexpr Term operator*(const InputType& other) const noexcept
			{
				return Term(Node<NodeType::Multiply>{
						SyntaxNode{node}, 
						literal_node(other)
					});
				//return Term{value * absolute_value(other), is_negated(other)};
			}
			constexpr Term operator/(const InputType& other) const noexcept
			{
				return Term{Node<NodeType::Divide>{
						SyntaxNode{node}, 
						literal_node(other)
					}};
				//return Term{value / absolute_value(other), is_negated(other)};
			}
			constexpr Term operator*(const Term& other) const noexcept
			{
				return Term{Node<NodeType::Multiply>{
						node, 
						other.node
					}};
				//return Term{value * other.value, is_negated(other)};
			}
			constexpr Term operator/(const Term& other) const noexcept
			{
				return Term{Node<NodeType::Divide>{
						node, 
						other.node
					}};
				//return Term{value / other.value, is_negated(other)};
			}
			constexpr Term operator-() const noexcept {
				return Term{Node<NodeType::Negation>{node}};
				//return Term{value, !negated};
			}
			constexpr Term as_negated() const noexcept {
				return Term{Node<NodeType::Negation>{node}};
				//return Term{value, !negated};
			}
		};

		template<NodeType>
		struct OperationHolder {};

		struct Sum
		{
			SyntaxNode node;
			constexpr Sum(SyntaxNode node) noexcept : node(node) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(InputType left, OperationHolder<OperateParameterConstant>, InputType right) noexcept 
				: node(Node<OperateParameterConstant>{
						literal_node(left), 
						literal_node(right)
					}) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(InputType left, OperationHolder<OperateParameterConstant>, Term right) noexcept 
				: node(Node<OperateParameterConstant>{
						literal_node(left), 
						right.node
					}) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(Term left, OperationHolder<OperateParameterConstant>, Term right) noexcept 
				: node(Node<OperateParameterConstant>{left.node, right.node}) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(Term left, OperationHolder<OperateParameterConstant>, InputType right) noexcept 
				: node(Node<OperateParameterConstant>{
						left.node, 
						literal_node(right)
					}) {}
			constexpr Sum() noexcept = default;
			constexpr Sum(const Sum& other) noexcept = default;
			constexpr Sum(Sum&& other) noexcept = default;

			constexpr Sum& operator=(const Sum& other) noexcept = default;
			constexpr Sum& operator=(Sum&& other) noexcept = default;

			constexpr Sum operator+(const InputType& other) const noexcept
			{
				return Sum{Node<NodeType::Add>{
						SyntaxNode{node}, 
						literal_node(other)
					}};
			}
			constexpr Sum operator-(const InputType& other) const noexcept
			{
				return Sum{Node<NodeType::Subtract>{
						SyntaxNode{node}, 
						literal_node(other)
					}};
			}
			constexpr Sum operator+(const Term& other) const noexcept
			{
				return Sum{Node<NodeType::Add>{
						SyntaxNode{node}, 
						other.node
					}};
			}
			constexpr Sum operator-(const Term& other) const noexcept
			{
				return Sum{Node<NodeType::Subtract>{
						SyntaxNode{node}, 
						other.node
					}};
			}
		};


		enum class TypeSpecificMathematicalExpressionTermTags
		{
			Sum, 
			Term, 
			//Expression, 
			ParenthesisScope
		};

		using UniqueTermsType = MakeTerms<
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Sum, 
						NonTerminalTerm, 
						Sum, 
						FixedString{"Sum"}
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::ParenthesisScope, 
						NonTerminalTerm, 
						Term, 
						FixedString{"ParenthesisScope"}
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Term, 
						NonTerminalTerm, 
						Term, 
						FixedString{"Term"}
					>
			>;

		using TermsType = decltype(BaseTermsType::template append_terms(UniqueTermsType{}));

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto reduce_to = term<reduce_to_term_tag>;
		constexpr static const auto input = term<input_term_tag>;

		constexpr static const auto add = term<MathematicalExpression::Add>;
		constexpr static const auto subtract = term<MathematicalExpression::Subtract>;

		constexpr static const auto multiply = term<MathematicalExpression::Multiply>;
		constexpr static const auto divide = term<MathematicalExpression::Divide>;

		constexpr static const auto open_parenthesis 
				= term<Brackets::OpenParenthesis>;
		constexpr static const auto close_parenthesis 
				= term<Brackets::CloseParenthesis>;

		constexpr static const auto sum 
				= term<TypeSpecificMathematicalExpressionTermTags::Sum>;
		constexpr static const auto math_term
				= term<TypeSpecificMathematicalExpressionTermTags::Term>;
		constexpr static const auto expression 
				= term<MathematicalExpression::Expression>;
		constexpr static const auto parenthesis_scope
				= term<TypeSpecificMathematicalExpressionTermTags::ParenthesisScope>;

		constexpr static const auto unique_terms = ctpg::terms(
					add, 
					subtract, 
					multiply, 
					divide, 
					open_parenthesis, 
					close_parenthesis
				);

		constexpr static const auto terms = std::tuple_cat(
				BaseType::terms, 
				unique_terms
			);

		constexpr static const auto unique_non_terminal_terms = ctpg::nterms(
				sum, 
				math_term, 
				expression
			);

		constexpr static const auto non_terminal_terms = std::tuple_cat(
				BaseType::non_terminal_terms, 
				unique_non_terminal_terms
				//ctpg::nterms(
				//		//reduce_to, 
				//	)
			);

		template<auto OperateParameterConstant>
		consteval static const auto term_operation_reduction(
				auto operation_term, 
				auto operator_term
			)
		{
			return ctpg::rules(
					operation_term(math_term, operator_term, math_term) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}
				);
		}

		template<
				auto OperateParameterConstant, 
				auto ProductOperateParameterConstant
			>
		consteval static const auto basic_term_operation_rules(
				auto operation_term, 
				auto operator_term
			)
		{
			return concatinate_tuples(
					term_operation_reduction<OperateParameterConstant>(operation_term, operator_term), 
					ctpg::rules(
							operation_term(operation_term, operator_term, math_term) 
							>= [](auto left, auto, auto right) {
								return ProductOperateParameterConstant(left, right);
							}
						)
				);
		}

		template<auto OperateParameterConstant>
		consteval static const auto input_operation_rules(
				auto operation_term, 
				auto operator_term
			)
		{
			return ctpg::rules(
					operation_term(operation_term, operator_term, input)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}
				);
		}

		consteval static const auto from_parenthesis()
		{
			return ctpg::rules(
					math_term(open_parenthesis, math_term, close_parenthesis)
					>= [](auto left, auto term, auto right) {
						return term;
					}, 
					math_term(open_parenthesis, sum, close_parenthesis) 
					>= [](auto left, auto sum, auto right) {
						return Term{sum.node};
					}
				);
		}

		constexpr static const auto input_to_math_term
				= math_term(input)
				>= [](auto input_) {
					return Term{input_};
				}; 

		constexpr static const auto negated_input_to_math_term
				= math_term(subtract, input)
				>= [](auto, auto input_) {
					return Term{input_}.as_negated();//Node<NodeType::Negation>(literal_node(input_))};
				}; 

		constexpr static const auto sum_to_expression
				= expression(sum)
				>= [](auto sum_) {
					return Expression{sum_.node};
				}; 
		
		constexpr static const auto math_term_to_expression
				= expression(math_term)
				>= [](auto math_term_) {
					return Expression{math_term_.node};
				}; 

		consteval static const auto unique_rules()
		{
			return concatinate_tuples(
					basic_term_operation_rules<
							[](auto left, auto right) { return Sum{left, OperationHolder<NodeType::Add>{}, right}; }, 
							[](auto left, auto right) { return left + right; }
						>(sum, add), 
					basic_term_operation_rules<
							[](auto left, auto right) { 
								return Sum{
										left, 
										OperationHolder<NodeType::Subtract>{}, 
										right
									};
								}, 
							[](auto left, auto right) { return left - right; }
						>(sum, subtract), 
					term_operation_reduction<
							[](auto left, auto right) { return left * right; }
						>(math_term, multiply), 
					term_operation_reduction<
							[](auto left, auto right) { return left / right; }
						>(math_term, divide), 
					input_operation_rules<
							[](auto left, auto right) { return left * right; }
						>(math_term, multiply), 
					input_operation_rules<
							[](auto left, auto right) { return left / right; }
						>(math_term, divide), 
					from_parenthesis(), 
					ctpg::rules(
							input_to_math_term, 
							negated_input_to_math_term, 
							sum_to_expression, 
							math_term_to_expression
						)
				);
		}

		consteval static const auto rules()
		{
			return concatinate_tuples(
					BaseType::rules(), 
					unique_rules()
				);
		}

		constexpr static const auto parser = ctpg::parser(
				reduce_to, 
				terms, 
				non_terminal_terms, 
				rules()
			);
	};

	extern template class NumericTypeResolver<NumericTypeTag::Whole>;
	extern template class NumericTypeResolver<NumericTypeTag::Integer>;
	extern template class NumericTypeResolver<NumericTypeTag::FixedPoint>;
	extern template class NumericTypeResolver<NumericTypeTag::Character>;
	extern template class NumericTypeResolver<NumericTypeTag::Bool>;
	
	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Whole, 
			NumericTypeTag::Whole
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Integer, 
			NumericTypeTag::Integer
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::FixedPoint, 
			NumericTypeTag::FixedPoint
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Character, 
			NumericTypeTag::Character
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Bool, 
			NumericTypeTag::Bool
		>;

	template<
			auto TypeTagParameterConstant, 
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate
		>
	using HomogenousMathematicalExpressionParser 
			= MathematicalExpressionParser<
				TermsParameterType, 
				TypeResolverParameterTemplate, 
				TypeTagParameterConstant, 
				TypeTagParameterConstant
			>;
}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

