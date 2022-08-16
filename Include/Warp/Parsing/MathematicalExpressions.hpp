#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

namespace Warp::Parsing
{
	enum class MathematicalExpression
	{
		Add, 
		Subtract, 
		Multiply, 
		Divide
		// Modulo
		// Raise
		// Log
		// BaseConvert
		// Solve
		OpenPrioritization, 
		ClosePrioritization
	};

	using MathematicalExpressionTermsType = MakeTerms<
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
		>;

	template<
			typename TermsParameterTemplate, 
			template<auto> typename TypeResolverParameterTemplate>, 
			auto ReductionTagParameterConstant
		>
	struct MathematicalExpressionParser
	{
		using TermsType = TermsParameterType;
		constexpr static const auto reduce_to_tag = ReductionTagParameterConstant;
		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;
	};

}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

