#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/NumericLiterals.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

namespace Warp::Parsing
{
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

	using MathematicalExpressionTermsType = NumericLiteralTermsType::FlatMerge<MakeTerms<
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
		>>;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto ReductionTagParameterConstant, 
			auto InputTermTagParameterConstant
		>
	requires
			HasTypeConcept<TypeResolverParameterTemplate<ReductionTagParameterConstant>> 
			&& HasTypeConcept<TypeResolverParameterTemplate<InputTermTagParameterConstant>>
			//&& std::is_convertible_v<
			//		typename TypeResolverParameterTemplate<InputTermTagParameterConstant>::Type, 
			//		typename TypeResolverParameterTemplate<ReductionTagParameterConstant>::Type
			//	>
	struct MathematicalExpressionParser : NumericLiteralParser<
			TermsParameterType, 
			TypeResolverParameterTemplate
		>
	{
		using TermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<NonTerminalTypeTagParameterConstant>::Type;

		using BaseType = NumericLiteralParser<TermsType, TypeResolverParameterTemplate>;

		constexpr static const auto reduce_to_term_tag = ReductionTagParameterConstant;
		constexpr static const auto input_term_tag = ReductionTagParameterConstant;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		using ReduceToType = TypeResolverTemplate<reduce_to_term_tag>;
		using InputType = TypeResolverTemplate<input_term_tag>; // TODO: See if I will need to get non-terminal-terms in here

		constexpr static const auto reduce_to = term<reduce_to_term_tag>;
		constexpr static const auto input = term<input_term_tag>;

		constexpr static const auto add = term<MathematicalExpression::Add>;
		constexpr static const auto subtract = term<MathematicalExpression::Subtract>;

		constexpr static const auto terms = std::tuple_cat(
				BaseType::terms, 
				ctpg::terms(add, subtract)
			);
		constexpr static const auto non_terminal_terms = std::tuple_cat(
				BaseType::non_terminal_terms, 
				ctpg::nterms(reduce_to)
			);

		constexpr static const auto add_inputs 
				= reduce_to(input, add, input) 
				>= [](auto left, auto plus, auto right) {
					return left + right;
				};

		constexpr static const auto subtract_inputs 
				= reduce_to(input, subtract, input) 
				>= [](auto left, auto minus, auto right) {
					return left - right;
				};

		consteval static const auto rules()
		{
			return std::tuple_cat(BaseType::rules(), ctpg::rules(
					add_inputs, 
					subtract_inputs
				));
		}

	};

}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

