#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP
#define WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

namespace Warp::Parsing
{
	using FunctionDeclaritionTermsType = MathematicalExpressionTermsType::AddOnePriority<
			TreeTerm<
					Identifier::Identifier, 
					RegexTerm, 
					FixedString{"[a-zA-Z_][a-zA-Z0-9_]+"}, 
					FixedString{"Identifier"}
				>
		>::AddOnePriority<
			TreeTerm<
					Keyword::Let, 
					StringTerm, 
					FixedString{"let"}, 
					ctpg::associativity::no_assoc
				>
			>;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto ReductionTagParameterConstant
		>
	struct FunctionDeclarationParser
	{
		using TermsType = TermsParameterType;

		constexpr static const auto reduce_to_tag = ReductionTagParameterConstant;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<
				NonTerminalTypeTagParameterConstant
			>::Type;

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		//constexpr static const auto terms = concatinate_tuples(
	};
}

#endif // WARP__PARSING__HEADER__PARSING__FUNCTION__DECLARATIONS__HPP

