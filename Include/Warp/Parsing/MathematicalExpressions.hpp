#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis/Expression.hpp>

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
		ClosePrioritization
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
		

		struct Term
		{
			ReduceToType value;
			constexpr Term operator*(const InputType& other) {
				return Term{value * other};
			}
			constexpr Term operator/(const InputType& other) {
				return Term{value / other};
			}
			constexpr Term operator*(const Term& other) {
				return Term{value * other.value};
			}
			constexpr Term operator/(const Term& other) {
				return Term{value / other.value};
			}
		};

		struct Sum
		{
			ReduceToType value;
			constexpr Sum operator+(const InputType& other) {
				return Sum{value + other};
			}
			constexpr Sum operator-(const InputType& other) {
				return Sum{value - other};
			}
			constexpr Sum operator+(const Term& other) {
				return Sum{value + other.value};
			}
			constexpr Sum operator-(const Term& other) {
				return Sum{value - other.value};
			}
		};

		using ExpressionNodeType = std::variant<Sum, Term>;

		struct Expression {
			ReduceToType value;
		};

		enum class TypeSpecificMathematicalExpressionTermTags {
			Sum, 
			Term, 
			NegatedTerm, 
			Expression
		};

		using TermsType = BaseTermsType::template AppendTerms<
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Sum, 
						NonTerminalTerm, 
						Sum, 
						FixedString{"Sum"}
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Expression, 
						NonTerminalTerm, 
						Expression, 
						FixedString{"Expression"}
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Term, 
						NonTerminalTerm, 
						Term, 
						FixedString{"Term"}
					>//, 
				//TypeTreeTerm<
				//		TypeSpecificMathematicalExpressionTermTags::NegatedTerm, 
				//		NonTerminalTerm, 
				//		Term, 
				//		FixedString{"NegatedTerm"}
				//	>
			>;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto reduce_to = term<reduce_to_term_tag>;
		constexpr static const auto input = term<input_term_tag>;

		constexpr static const auto add = term<MathematicalExpression::Add>;
		constexpr static const auto subtract = term<MathematicalExpression::Subtract>;

		constexpr static const auto multiply = term<MathematicalExpression::Multiply>;
		constexpr static const auto divide = term<MathematicalExpression::Divide>;

		constexpr static const auto sum 
				= term<TypeSpecificMathematicalExpressionTermTags::Sum>;
		constexpr static const auto math_term
				= term<TypeSpecificMathematicalExpressionTermTags::Term>;
		constexpr static const auto expression 
				= term<TypeSpecificMathematicalExpressionTermTags::Expression>;

		constexpr static const auto terms = std::tuple_cat(
				BaseType::terms, 
				ctpg::terms(
						add, 
						subtract, 
						multiply, 
						divide
					)
			);

		constexpr static const auto non_terminal_terms = std::tuple_cat(
				BaseType::non_terminal_terms, 
				ctpg::nterms(
						reduce_to, 
						sum, 
						math_term, 
						expression
					)
			);

		template<
				auto TermOperateParameterConstant, 
				auto ProductOperateParameterConstant
			>
		consteval static const auto basic_operation_rules(
				auto operation_term, 
				auto operator_term
			)
		{
			return ctpg::rules(
					operation_term(math_term, operator_term, math_term) 
					>= [](auto left, auto, auto right) {
						return TermOperateParameterConstant(left, right);
					}, 
					operation_term(operation_term, operator_term, math_term) 
					>= [](auto left, auto, auto right) {
						return ProductOperateParameterConstant(left, right);
					}
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

		constexpr static const auto input_to_math_term
				= math_term(input)
				>= [](auto input_) {
					return Term{input_};
				}; 

		constexpr static const auto sum_to_expression
				= expression(sum)
				>= [](auto sum_) {
					return Expression{sum_.value};
				}; 

		constexpr static const auto sum_to_term
				= math_term(sum)
				>= [](auto sum_) {
					return Term{sum_.value};
				}; 
		
		constexpr static const auto math_term_to_expression
				= expression(math_term)
				>= [](auto math_term_) {
					return Expression{math_term_.value};
				}; 

		consteval static const auto rules()
		{
			constexpr auto base_rules = concatinate_tuples(
					BaseType::rules(), 
					basic_operation_rules<
							[](auto left, auto right) { return Sum{left.value + right.value}; }, 
							[](auto left, auto right) { return left + right; }
						>(sum, add), 
					basic_operation_rules<
							[](auto left, auto right) { return Sum{left.value - right.value}; }, 
							[](auto left, auto right) { return left - right; }
						>(sum, subtract), 
					//input_operation_rules<
					//		[](auto left, auto right) { return left + right; }
					//	>(sum, add), 
					//input_operation_rules<
					//		[](auto left, auto right) { return left - right; }
					//	>(sum, subtract), 
					basic_operation_rules<
							[](auto left, auto right) { return left * right; }, 
							[](auto left, auto right) { return left * right; }
						>(math_term, multiply), 
					basic_operation_rules<
							[](auto left, auto right) { return left / right; }, 
							[](auto left, auto right) { return left / right; }
						>(math_term, divide), 
					input_operation_rules<
							[](auto left, auto right) { return left * right; }
						>(math_term, multiply), 
					input_operation_rules<
							[](auto left, auto right) { return left / right; }
						>(math_term, divide), 
					ctpg::rules(
							input_to_math_term, 
							sum_to_expression, 
							sum_to_term, 
							math_term_to_expression
						)
				);
			return base_rules;
		}

	};

}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

