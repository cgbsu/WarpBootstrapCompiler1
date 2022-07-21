#include <Warp/Common.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__TERMS_HPP
#define WARP__PARSING__HEADER__PARSING__TERMS_HPP

namespace Warp::Parsing
{
	template<
			auto TagParameterConstant, 
			template<auto...> typename TermParameterTemplate, 
			auto... RequiredTermParameterConstants
		>
	struct TreeTerm
	{
		template<int PrecedenceParameterConstant>
		using InjectedTermType = TermParameterTemplate<
				RequiredTermParameterConstants..., 
				PrecedenceParameterConstant
			>;
		template<int PrecedenceParameterConstant>
		constexpr static const auto term = InjectedTermType<
				PrecedenceParameterConstant
			>::term;
		constexpr static const auto tag = TagParameterConstant;
	};

	namespace Detail
	{
		struct AssociatedTemplateCase0
		{
			template<
					template<auto, template<auto...> typename, auto...> typename AssociationParameterType, 
					auto TagParameterConstant, 
					template<auto...> typename AssociatedTemplateParameterTemplate, 
					auto... ParameterConstants 
				>
			AssociatedTemplateCase0(TypeHolder<AssociationParameterType<
					TagParameterConstant, 
					AssociatedTemplateParameterTemplate, 
					ParameterConstants...>>
				) {}
		};
	}

	// TODO: Would like to avoid the need to call a constructor. //
	template<typename AssociatedTemplateParameterType>
	concept AssociatedTemplateConcept = requires(AssociatedTemplateParameterType canidate) {
		Detail::AssociatedTemplateCase0(TypeHolder<decltype(canidate)>{});
	};

	// This does not work... Im not sure why...
	template<typename CanidateParameterType>
	concept TermInjectableConcept = CanidateParameterType::template InjectedTermType<0>;

	template<
			size_t IndexParamterConstant, 
			auto TagParameterConstant, 
			AssociatedTemplateConcept CurrentParameterType, 
			AssociatedTemplateConcept... TermParameterTypes
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
			bool ErrorOnNoMatchParameterConstant, 
			auto TagParameterConstant, 
			AssociatedTemplateConcept CurrentParameterType, 
			AssociatedTemplateConcept... TermParameterTypes
		>
	consteval auto get_term_with_tag()
	{
		if constexpr(TagParameterConstant == CurrentParameterType::tag)
			return TypeHolder<CurrentParameterType>();
		else if constexpr(sizeof...(TermParameterTypes) <= 0) {
			static_assert(ErrorOnNoMatchParameterConstant, "Term not found in get_term_with_tag");
			return std::nullopt;
		}
		else
		{
			return get_term_with_tag<
					ErrorOnNoMatchParameterConstant, 
					TagParameterConstant, 
					TermParameterTypes...
				>();
		}
	}

	template<
			bool ErrorOnNoMatchParameterConstant, 
			auto TagParameterConstant, 
			AssociatedTemplateConcept... TermParameterTypes
		>
	using TreeTermWithTag = decltype(get_term_with_tag<
			ErrorOnNoMatchParameterConstant, 
			TagParameterConstant, 
			TermParameterTypes...
		>())::Type;

	enum class TermPrecedence : int {
		NoPriority = 0
	};

	template<
			typename PreviousParameterType, 
			auto PrecedenceParameterConstant, 
			AssociatedTemplateConcept... TermParameterTypes
		>
	struct Terms
	{
		using PreviousType = PreviousParameterType;
		constexpr static const auto precedence = PrecedenceParameterConstant;
		using ThisType = Terms<PreviousParameterType, precedence, TermParameterTypes...>;

		template<auto TagParameterConstant>
		consteval static auto get_term()
		{
			if constexpr(auto result = get_term_with_tag<
						std::is_void_v<PreviousType> == false, 
						TagParameterConstant, 
						TermParameterTypes...
					>(); std::is_same<decltype(result), std::nullopt_t>::value == false)
				return result;
			else
				return PreviousType::template get_term<TagParameterConstant>();
		}

		template<auto TagParameterConstant>
		constexpr static const auto term 
				= decltype(get_term<TagParameterConstant>())
					::Type
					::template term<static_cast<int>(precedence)>;

		template<AssociatedTemplateConcept... NewTermParameterTypes>
		using AddOnePriority = Terms<
				ThisType, 
				static_cast<int>(precedence) + 1, 
				NewTermParameterTypes...
			>;
	};

	template<AssociatedTemplateConcept... TermParameterTypes>
	using MakeTerms = Terms<void, 0, TermParameterTypes...>;
}

#endif // WARP__PARSING__HEADER__PARSING__TERMS_HPP

