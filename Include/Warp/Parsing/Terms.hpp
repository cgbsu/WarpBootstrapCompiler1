#include <Warp/Common.hpp>
#include <Warp/Utilities/Conversions.hpp>

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

	template<
			auto TagParameterConstant, 
			template<typename, auto...> typename TermParameterTemplate, 
			typename ParameterType, 
			auto RequiredTermParameterConstants
		>
	struct TypeTreeTerm
	{
		template<int PrecedenceParameterConstant>
		using InjectedTermType = TermParameterTemplate<
				ParameterType, 
				RequiredTermParameterConstants 
				//PrecedenceParameterConstant
			>;
		template<int PrecedenceParameterConstant>
		constexpr static const auto term = InjectedTermType<
				PrecedenceParameterConstant
			>::term;
		constexpr static const auto tag = TagParameterConstant;
	};

	namespace Detail
	{
		struct AssociatedTemplateConstructable
		{
			template<
					template<auto, template<auto...> typename, auto...> 
							typename AssociationParameterType, 
					auto TagParameterConstant, 
					template<auto...> typename AssociatedTemplateParameterTemplate, 
					auto... ParameterConstants 
				>
			AssociatedTemplateConstructable(TypeHolder<AssociationParameterType<
					TagParameterConstant, 
					AssociatedTemplateParameterTemplate, 
					ParameterConstants...>>
				) {}

			template<
					template<auto, template<typename, auto...> typename, typename, auto...> 
							typename AssociationParameterType, 
					auto TagParameterConstant, 
					template<typename, auto...> typename AssociatedTemplateParameterTemplate, 
					typename ParameterType, 
					auto... ParameterConstants 
				>
			AssociatedTemplateConstructable(TypeHolder<AssociationParameterType<
					TagParameterConstant, 
					AssociatedTemplateParameterTemplate, 
					ParameterType, 
					ParameterConstants...>>
				) {}
		};
	}

	// TODO: Would like to avoid the need to call a constructor. //
	template<typename AssociatedTemplateParameterType>
	concept AssociatedTemplateConcept = requires(
			AssociatedTemplateParameterType canidate) {
		Detail::AssociatedTemplateConstructable(TypeHolder<decltype(canidate)>{});
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
		if constexpr(equal_if_comparible<
					TagParameterConstant, 
					CurrentParameterType::tag
				>() == true)
			return TypeHolder<CurrentParameterType>();
		else if constexpr(sizeof...(TermParameterTypes) <= 0)
		{
			static_assert(
					ErrorOnNoMatchParameterConstant, 
					"Term not found in get_term_with_tag"
				);
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

	using TermsNoPreviousType = void;

	template<
			typename PreviousParameterType, 
			auto PrecedenceParameterConstant, 
			AssociatedTemplateConcept... TermParameterTypes
		>
	struct Terms
	{
		using NoPreviousType = TermsNoPreviousType;
		using PreviousType = PreviousParameterType;
		constexpr static const auto precedence = PrecedenceParameterConstant;
		using ThisType = Terms<
				PreviousParameterType, 
				precedence, 
				TermParameterTypes...
			>;
		using PrecedenceType = decltype(precedence);
		constexpr static const auto is_root = std::is_same_v<NoPreviousType, PreviousType>;

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

		template<typename NewPreviousParameterType, 
				auto NewPreviousParameterConstant = precedence>
		using ReplacePrevious = Terms<
				NewPreviousParameterType, 
				NewPreviousParameterConstant, 
				TermParameterTypes...
			>;
		
		template<typename NewPreviousParameterType>
		using ReplacePreviousAddOnePriority = ReplacePrevious<
				NewPreviousParameterType, 
				NewPreviousParameterType::precedence + 1
			>;

		template<AssociatedTemplateConcept... NewTermParameterTypes>
		using ReplaceTerms = Terms<PreviousType, precedence, NewTermParameterTypes...>;


		template<typename NewPreviousParameterType, //A bit of an oximoron </humor>
				auto NewPrecedenceParameterConstant>
		using NewTermsType = TypeHolder<decltype(Terms<
				NewPreviousParameterType, 
				NewPrecedenceParameterConstant, 
				TermParameterTypes...
			>)>;

		template<typename NewPreviousParameterType, 
				auto NewPrecedenceParameterConstant>
		constinit const auto new_terms = NewTermsType<
				NewPreviousParameterType, 
				NewPrecedenceParameterConstant
			>{};
		
		template<
				typename OtherPreviousParameterType, 
				auto OtherPrecedenceParameterConstant, 
				AssociatedTemplateConcept... OtherTermParameterTypes
			>
		requires(OtherPrecedenceParameterConstant::precedence > PreviousType::precedence
				&& is_root == false)
		consteval static const auto merge(Terms<
				OtherTermParameterTypes, 
				OtherPrecedenceParameterConstant, 
				OtherTermParameterTypes...>
			)
		{
			return TypeHolder<ReplacePrevious<
					decltype(ReplacePrevious<OtherPreviouseType>::merge<PreviousType>())::Type
				>>{}
		}

		template<
				typename OtherPreviousParameterType, 
				auto OtherPrecedenceParameterConstant, 
				AssociatedTemplateConcept... OtherTermParameterTypes
			>
		requires(OtherPrecedenceParameterConstant::precedence < PreviousType::precedence
				&& is_root == false)
		consteval static const auto merge(Terms<
				OtherTermParameterTypes, 
				OtherPrecedenceParameterConstant, 
				OtherTermParameterTypes...>
			)
		{
			return TypeHolder<ReplacePrevious<
					decltype(PreviousType::merge<OtherPreviouseType>())::Type
				>>{};
		}

		template<
				typename OtherPreviousParameterType, 
				auto OtherPrecedenceParameterConstant, 
				AssociatedTemplateConcept... OtherTermParameterTypes
			>
		requires(OtherPrecedenceParameterConstant::precedence == PreviousType::precedence
				&& is_root == false)
		consteval static const auto merge(Terms<
				OtherTermParameterTypes, 
				OtherPrecedenceParameterConstant, 
				OtherTermParameterTypes...>
			)
		{
			PreviousType::merge(OtherTermParameterTypes{})
		}

		template<
				typename OtherPreviousParameterType, 
				auto OtherPrecedenceParameterConstant, 
				AssociatedTemplateConcept... OtherTermParameterTypes
			>
		requires(is_root == true)
		consteval static const auto merge(Terms<
				OtherTermParameterTypes, 
				OtherPrecedenceParameterConstant, 
				OtherTermParameterTypes...>
			) {
			return TypeHoder<ReplacePrevious<OtherPreviouseType>>{};
		}









		//template<
		//		typename OtherPreviousParameterType, 
		//		auto OtherPrecedenceParameterConstant, 
		//		AssociatedTemplateConcept... OtherTermParameterTypes
		//	>
		//consteval static const auto merge(Terms<
		//		OtherTermParameterTypes, 
		//		OtherPrecedenceParameterConstant, 
		//		OtherTermParameterTypes...>
		//	)
		//{
		//	using OtherPreviouseType = OtherPreviousParameterType;
		//	constinit const auto other_precedence = OtherPrecedenceParameterConstant;
		//	using OtherPrecedenceType = decltype(other_precedence);
		//	using OtherTermsType = Terms<
		//			OtherPreviouseType, 
		//			other_precedence, 
		//			OtherTermParameterTypes...
		//		>;

		//	constinit bool equally_comparible = is_equally_comparible<
		//			OtherPrecedenceType, 
		//			PrecedenceType
		//		>;
		//	constinit bool comparible = is_comparible<
		//			OtherPrecedenceType, 
		//			PrecedenceType
		//		>;
		//	constinit bool other_has_previous = !std::is_same_v<
		//			OtherPreviouseType, 
		//			OtherTermsType::NoPreviousType
		//		>;
		//	constinit bool have_previous = !std::is_same_v<
		//			PreviousType, 
		//			NoPreviousType
		//		>;
		//	template<typename NewPreviousParameterType, //A bit of an oximoron </humor>
		//			auto NewPrecedenceParameterConstant>
		//	using MergedTermsType = TypeHolder<decltype(Terms<
		//			NewPreviousParameterType, 
		//			NewPrecedenceParameterConstant, 
		//			TermParameterTypes..., 
		//			OtherTermParameterTypes...
		//		>)>;

		//	template<typename NewPreviousParameterType, 
		//			auto NewPrecedenceParameterConstant>
		//	constinit const auto merged_terms = MergedTermsType<
		//			NewPreviousParameterType, 
		//			NewPrecedenceParameterConstant
		//		>{};

		//	if constexpr(equally_comparible == true)
		//	{
		//		if constexpr(other_precedence == precedence)
		//		{
		//			if constexpr(have_previous == true)
		//			{
		//				if constexpr(other_has_previous == true)
		//				{
		//					return MergedTermsType<
		//							PreviousType, 
		//							precedence, 
		//						>::PreviousType::merge(OtherTermsType::PreviousType))>{};
		//				}
		//				return merged_terms<PreviousType, precedence>;
		//			}
		//			else if constexpr(have_previous == false) { /* It does not 
		//														* matter wheather 
		//														* or not other_has_previous 
		//														* at this point. */
		//				return merged_terms<OtherPreviouseType, precedence>;
		//			}
		//				
		//		}
		//		else if constexpr(comparible == true)
		//		{
		//			if constexpr(precedence < other_precedence)
		//			{
		//			}
		//		}
		//	}
		//}

	};

	template<AssociatedTemplateConcept... TermParameterTypes>
	using MakeTerms = Terms<void, 0, TermParameterTypes...>;
}

#endif // WARP__PARSING__HEADER__PARSING__TERMS_HPP

