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
					MathematicalExpression::Add, 
					CharTerm, 
					'+', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					MathematicalExpression::Subtract, 
					CharTerm, 
					'-', 
					ctpg::associativity::no_assoc
				>
		>::Prepend<
				TreeTerm<
						MathematicalExpression::Multiply, 
						CharTerm, 
						'*', 
						ctpg::associativity::no_assoc
					>, 
				TreeTerm<
						MathematicalExpression::Divide, 
						CharTerm, 
						'/', 
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
		
		struct Expression {
			ReduceToType value;
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
		};

		struct Product
		{
			ReduceToType value;
			constexpr Product operator*(const InputType& other) {
				return Product{value * other};
			}
			constexpr Product operator/(const InputType& other) {
				return Product{value / other};
			}
		};

		enum class TypeSpecificMathematicalExpressionTermTags {
			Sum, 
			Product, 
			NegatedProduct, 
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
						TypeSpecificMathematicalExpressionTermTags::Product, 
						NonTerminalTerm, 
						Product, 
						FixedString{"Product"}
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::NegatedProduct, 
						NonTerminalTerm, 
						Product, 
						FixedString{"NegatedProduct"}
					>
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
		constexpr static const auto product
				= term<TypeSpecificMathematicalExpressionTermTags::Product>;
		constexpr static const auto negated_product
				= term<TypeSpecificMathematicalExpressionTermTags::Product>;
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
						product, 
						negated_product, 
						expression
					)
			);

		template<auto OperateParameterConstant>
		consteval static const auto basic_operation_rules(
				auto operation_term, 
				auto operator_term
			)
		{
			return ctpg::rules(
					operation_term(input, operator_term, input) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}, 
					operation_term(operation_term, operator_term, input) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}
				);
		}

		template<auto OperateParameterConstant>
		consteval static const auto sum_product_operation_rules(auto operator_term)
		{
			return ctpg::rules(
					sum(sum, operator_term, product) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right.value);
					}, 
					sum(product, operator_term, sum) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left.value, right.value);
					}, 
					sum(input, operator_term, product)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right.value);
					}, 
					sum(product, operator_term, input)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left.value, right);
					}
				);
		}

		template<auto OperateParameterConstant>
		consteval static const auto sum_negative_product_operation_rules(auto operator_term)
		{
			return ctpg::rules(
					sum(negated_product, operator_term, sum) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left.value, right.value);
					}, 
					sum(input, operator_term, negated_product)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right.value);
					}, 
					sum(negated_product, operator_term, input)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left.value, right);
					}
				);
		}

		consteval static const auto product_sum_operation_rules()
		{
			return ctpg::rules(
					sum(sum, negated_product) 
					>= [](auto left, auto right) {
						return left - right.value;
					}, 
					sum(product, negated_product)
					>= [](auto left, auto right) {
						return left.value - right.value;
					}
				);
		}

		//consteval static const auto signed_rules()
		//{
		//}


		template<auto OperateParameterConstant>
		consteval static const auto sum_product_basic_rule(
				auto product_term, 
				auto operator_term
			)
		{
			return ctpg::rules(
					sum(product_term, operator_term, product_term) 
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left.value, right.value);
					}
				);
		}

		constexpr static const auto negate_product
				= negated_product(subtract, product)
				>= [](auto negative, auto operhand) {
					return Product{operhand.value};
				};

		constexpr static const auto negate_negated_product
				= product(subtract, negated_product)
				>= [](auto negative, auto operhand) {
					return Product{operhand.value};
				};

		template<auto OperateParameterConstant>
		consteval static const auto negated_product_input_rules(auto operator_term)
		{
			return ctpg::rules(
					negated_product(input, operator_term, subtract, input)
					>= [](auto left, auto, auto, auto right) {
						return OperateParameterConstant(left, right);
					}, 
					product(subtract, input, operator_term, subtract, input)
					>= [](auto, auto left, auto, auto, auto right) {
						return OperateParameterConstant(left, right);
					}, 
					negated_product(product, operator_term, subtract, input)
					>= [](auto left, auto, auto, auto right) {
						return OperateParameterConstant(left.value, right);
					}, 
					product(negated_product, operator_term, subtract, input)
					>= [](auto left, auto, auto, auto right) {
						return OperateParameterConstant(left.value, right);
					}
				);
		}


		consteval const static auto product_negated_product_rules(auto operator_term)
		{
			return ctpg::rules(
					negated_product(product, operator_term, negated_product)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}, 
					product(negated_product, operator_term, negated_product)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}, 
					negated_product(negated_product, operator_term, product)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}
				);
		}

		constexpr static const auto sum_to_expression
				= expression(sum)
				>= [](auto sum_) { return Expression{sum_.value};
				}; 
		
		constexpr static const auto product_to_expression
				= expression(product)
				>= [](auto product_) {
					return Expression{product_.value};
				}; 

		consteval static const auto rules()
		{
			constexpr auto base_rules = concatinate_tuples(
					BaseType::rules(), 
					basic_operation_rules<
							[](auto left, auto right) { return left + right; }
						>(sum, add), 
					basic_operation_rules<
							[](auto left, auto right) { return left - right; }
						>(sum, subtract), 
					basic_operation_rules<
							[](auto left, auto right) { return left * right; }
						>(product, multiply), 
					basic_operation_rules<
							[](auto left, auto right) { return left / right; }
						>(product, divide), 
					sum_product_operation_rules<
							[](auto left, auto right) { return left + right; }
						>(add), 
					sum_product_basic_rule<
							[](auto left, auto right) { return left + right; }
						>(product, add), 
					sum_product_basic_rule<
							[](auto left, auto right) { return left + right; }
						>(negated_product, add), 
					product_sum_operation_rules(), 
					sum_negative_product_operation_rules<
							[](auto left, auto right) { return left + right; }
						>(add),
					negated_product_input_rules<
							[](auto left, auto right) { return left * right; }
						>(multiply), 
					negated_product_input_rules<
							[](auto left, auto right) { return left / right; }
						>(divide), 
					//sum_negative_product_operation_rules<
					//		[](auto left, auto right) { return left / right; }
					//	>(divide), 
					//
					ctpg::rules(
							negate_product, 
							negate_negated_product, 
							sum_to_expression, 
							product_to_expression
						)
				);
			return base_rules;
			//if constexpr(std::is_unsigned_v<typename ReduceToType::UnderylingType> 
			//		== false)
			//{
			//	return concatinate_tuples(
			//		base_rules, 
			//		ctpg::rules(
			//				sum(negated_product, negated_product)
			//				>= [](auto left, auto right) {
			//					return -left.value - right.value;
			//				}, 
			//				expression(negated_product) 
			//				>= [](auto product) {
			//					return product;
			//				}
			//			)
			//		);
			//}
			//else
			//	return base_rules;
		}

	};

}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

