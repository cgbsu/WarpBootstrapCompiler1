#include <Warp/Common.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__TERMS_HPP
#define WARP__PARSING__HEADER__PARSING__TERMS_HPP

namespace Warp::Parsing
{
	template<
			auto TagParameterConstant, 
			template<auto...> typename TermParameterType, 
			auto... RequiredTermParameterConstants
		>
	struct TreeTerm
	{
		using InjectedTermType = TermParameterType<
				RequiredTermParameterConstants... 
			>;
		template<
				int PrecedenceParameterConstant, 
				ctpg::associativity AssociativityParameterConstant 
			>
		constexpr static const auto term = InjectedTermType::term;
		constexpr static const auto tag = TagParameterConstant;
	};

	template<
			size_t IndexParamterConstant, 
			auto TagParameterConstant, 
			typename CurrentParameterType, 
			typename... TermParameterTypes
		>
	consteval static const std::optional<size_t> find_term_with_tag_index()
	{
		if constexpr(TagParameterConstant == CurrentParameterType::tag)
			return IndexParamterConstant;
		if constexpr(sizeof...(TermParameterTypes) <= 0)
			return std::nullopt;
		else
		{
			return find_term_with_tag_index<
					IndexParamterConstant + 1, 
					TagParameterConstant, 
					TermParameterTypes...
				>();
		}
	};

	template<
			typename PreviousParameterType, 
			auto PrecedenceParameterConstant
		>
	struct Terms
	{
		using PreviousType = PreviousParameterType;
		constexpr static const auto predence = PrecedenceParameterConstant;

		
	};
}

#endif // WARP__PARSING__HEADER__PARSING__TERMS_HPP
