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

	

	using MathematicalExpressionTermsType = NumericLiteralTermsType::AddOnePriority<
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
		>::AddOnePriority<
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
		
		//template<auto TermTagParameterConstant>
		//using BaseTermType = BaseTermsType::template TermType<TermTagParameterConstant>;

		//template<auto TagParameterConstant>
		//constexpr static const auto base_term_name = BaseTermType<TagParameterConstant>
		//		::template InjectedTermType<0>::name;

		//constexpr static const auto reduce_string = base_term_name<reduce_to_term_tag>; 
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

		enum class TypeSpecificMathematicalExpressionTermTags {
			Sum, 
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
					>
			>;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto reduce_to = term<reduce_to_term_tag>;
		constexpr static const auto input = term<input_term_tag>;

		constexpr static const auto add = term<MathematicalExpression::Add>;
		constexpr static const auto subtract = term<MathematicalExpression::Subtract>;

		constexpr static const auto sum 
				= term<TypeSpecificMathematicalExpressionTermTags::Sum>;
		constexpr static const auto expression 
				= term<TypeSpecificMathematicalExpressionTermTags::Expression>;

		constexpr static const auto terms = std::tuple_cat(
				BaseType::terms, 
				ctpg::terms(add, subtract)
			);
		constexpr static const auto non_terminal_terms = std::tuple_cat(
				BaseType::non_terminal_terms, 
				ctpg::nterms(
						reduce_to, 
						sum, 
						expression
					)
			);

		constexpr static const auto add_inputs 
				= sum(input, add, input) 
				>= [](auto left, auto plus, auto right) {
					return left + right;
				};
		constexpr static const auto subtract_inputs 
				= sum(input, subtract, input) 
				>= [](auto left, auto minus, auto right) {
					return left - right;
				};
		constexpr static const auto add_input
				= sum(sum, add, input) 
				>= [](auto left, auto plus, auto right) {
					return left + right;
				};
		constexpr static const auto subtract_input
				= sum(sum, subtract, input) 
				>= [](auto left, auto minus, auto right) {
					//std::cout << left.numeric << " : " << right.numeric << "\n";
					return left - right;
				};
		constexpr static const auto sum_to_expression
				= expression(sum)
				>= [](auto sum_) {
					return Expression{sum_.value};
				}; 

		consteval static const auto rules()
		{
			return std::tuple_cat(
					BaseType::rules(), 
					ctpg::rules(
							add_inputs, 
							subtract_inputs, 
							add_input, 
							subtract_input, 
							sum_to_expression
						)
				);
		}

	};

}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

