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

		template<typename NewPreviousParameterType, auto NewPreviousParameterConstant>
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

		template<AssociatedTemplateConcept... NewTermParameterTypes>
		using MergeTerms = Terms<PreviousType, precedence, TermParameterTypes..., NewTermParameterTypes...>;

		template<
				typename OtherPreviousParameterType, 
				auto OtherPrecedenceParameterConstant
			>
		constinit static const bool can_flat_merge = (
						std::is_same_v<OtherPreviousParameterType, NoPreviousType> 
						|| is_root == true
						|| std::is_same_v<OtherPreviousParameterType, PreviousType>
					)
				&& OtherPrecedenceParameterConstant == precedence;

		template<
				typename OtherPreviousParameterType, 
				auto OtherPrecedenceParameterConstant, 
				typename... OtherTermParameterTypes
			>
		constexpr const static auto flat_merge(Terms<
				OtherPreviousParameterType, 
				OtherPrecedenceParameterConstant, 
				OtherTermParameterTypes...
			> other)
		requires(ThisType::template can_flat_merge<
					OtherPreviousParameterType, 
					OtherPrecedenceParameterConstant
				> == true) 
		{
			if constexpr(is_root == false)
			{
				return TypeHolder<Terms<
						PreviousType, 
						precedence, 
						TermParameterTypes..., 
						OtherTermParameterTypes...
					>>{};
			}
			else 
			{
				return TypeHolder<Terms<
						OtherPreviousParameterType, 
						precedence, 
						TermParameterTypes..., 
						OtherTermParameterTypes...
					>>{};
			}
		}

		template<typename OtherTermParameterType>
		using FlatMerge = decltype(flat_merge(std::declval<OtherTermParameterType>()))::Type;

	};

	template<AssociatedTemplateConcept... TermParameterTypes>
	using MakeTerms = Terms<void, 0, TermParameterTypes...>;

	template<
			typename LeftPreviousParameterType, 
			auto LeftPrecedentParameterConstant, 
			typename... LeftTermParameterTypes, 
			typename RightPreviousParameterType, 
			auto RightPrecedentParameterConstant, 
			typename... RightTermParameterTypes
		>
	constexpr const static auto merge_terms( 
			Terms<
				LeftPreviousParameterType, 
				LeftPrecedentParameterConstant, 
				LeftTermParameterTypes...
			> left, 
			Terms<
				RightPreviousParameterType, 
				RightPrecedentParameterConstant, 
				RightTermParameterTypes...
			> right
		)
	requires(decltype(left)::template can_flat_merge<
					RightPreviousParameterType, 
					RightPrecedentParameterConstant
			>
		) // Merging Right INTO Left "equal" case
	{
		return decltype(left)::flat_merge(right);
	}

	template<
			typename LeftPreviousParameterType, 
			auto LeftPrecedentParameterConstant, 
			typename... LeftTermParameterTypes, 
			typename RightPreviousParameterType, 
			auto RightPrecedentParameterConstant, 
			typename... RightTermParameterTypes
		>
	requires(
			RightPrecedentParameterConstant < LeftPrecedentParameterConstant 
					&& (RightPrecedentParameterConstant > LeftPreviousParameterType::precedence
							|| std::same_as<LeftPreviousParameterType, TermsNoPreviousType>)
		)
	constexpr const static auto merge_terms( // Merginer Right INTO Left "in-between" case
			Terms<
				LeftPreviousParameterType, 
				LeftPrecedentParameterConstant, 
				LeftTermParameterTypes...
			> left, 
			Terms<
				RightPreviousParameterType, 
				RightPrecedentParameterConstant, 
				RightTermParameterTypes...
			> right
		)
	{
		using LeftTermsType = decltype(left);
		using RightTermsType = decltype(right);
		using LeftPreviousType = LeftPreviousParameterType;
		using RightPreviousType = RightPreviousParameterType;
		if constexpr(LeftTermsType::is_root == true)
		{
			return TypeHolder<Terms<
					RightTermsType, 
					LeftPrecedentParameterConstant, 
					LeftTermParameterTypes...
				>>{};
		}
		else if constexpr(RightTermsType::is_root == true)
		{
			return TypeHolder<Terms<
					Terms<
							LeftPreviousType, 
							RightPrecedentParameterConstant, 
							RightTermParameterTypes...
						>, 
					LeftPrecedentParameterConstant, 
					LeftTermParameterTypes...
				>>{};
		}
		else
		{
			using MergedPreviousType = decltype(merge_terms(
					std::declval<LeftPreviousType>(), 
					std::declval<RightPreviousType>()
				))::Type;
			using NewRightType = Terms<
					MergedPreviousType, 
					RightPrecedentParameterConstant, 
					RightTermParameterTypes...
				>;
			return TypeHolder<Terms<
					NewRightType, 
					LeftPrecedentParameterConstant, 
					LeftTermParameterTypes...
				>>{};
		}
	}

	template<
			typename LeftPreviousParameterType, 
			auto LeftPrecedentParameterConstant, 
			typename... LeftTermParameterTypes, 
			typename RightPreviousParameterType, 
			auto RightPrecedentParameterConstant, 
			typename... RightTermParameterTypes
		>
	requires(RightPrecedentParameterConstant > LeftPrecedentParameterConstant)
	constexpr const static auto merge_terms( // Merginer Right INTO Left "partial sorted/greater than" case
			Terms<
				LeftPreviousParameterType, 
				LeftPrecedentParameterConstant, 
				LeftTermParameterTypes...
			> left, 
			Terms<
				RightPreviousParameterType, 
				RightPrecedentParameterConstant, 
				RightTermParameterTypes...
			> right
		)
	{
		using LeftTermsType = decltype(left);
		using RightTermsType = decltype(right);
		using LeftPreviousType = LeftPreviousParameterType;
		using RightPreviousType = RightPreviousParameterType;
		if constexpr(RightTermsType::is_root == true)
		{
			return TypeHolder<Terms<
					LeftTermsType, 
					RightPrecedentParameterConstant, 
					RightTermParameterTypes...
				>>{};
		}
		else
		{
			using MergedPreviousType = decltype(merge_terms(
					std::declval<LeftTermsType>(), 
					std::declval<RightPreviousType>()
				))::Type;
			return TypeHolder<Terms<
					MergedPreviousType, 
					RightPrecedentParameterConstant, 
					RightTermParameterTypes...
				>>{};
		}
	}

	template<
			typename LeftPreviousParameterType, 
			auto LeftPrecedentParameterConstant, 
			typename... LeftTermParameterTypes, 
			typename RightPreviousParameterType, 
			auto RightPrecedentParameterConstant, 
			typename... RightTermParameterTypes
		>
	requires(RightPrecedentParameterConstant < LeftPrecedentParameterConstant 
			&& RightPrecedentParameterConstant < LeftPreviousParameterType::precedence)
	constexpr const static auto merge_terms( // Merginer Right INTO Left "in-between" case
			Terms<
				LeftPreviousParameterType, 
				LeftPrecedentParameterConstant, 
				LeftTermParameterTypes...
			> left, 
			Terms<
				RightPreviousParameterType, 
				RightPrecedentParameterConstant, 
				RightTermParameterTypes...
			> right
		)
	{
		using LeftTermsType = decltype(left);
		using RightTermsType = decltype(right);
		using LeftPreviousType = LeftPreviousParameterType;
		using RightPreviousType = RightPreviousParameterType;
		using MergedPreviousType = decltype(merge_terms(
				std::declval<LeftPreviousType>(), 
				std::declval<RightTermsType>()
			))::Type;
	//	using MergedPreviousType = decltype(merge_terms(
	//			std::declval<LeftPreviousType>(), 
	//			std::declval<RightPreviousType>()
	//		))::Type;
		//using NewRightType = Terms<
		//		MergedPreviousType, 
		//		RightPrecedentParameterConstant, 
		//		RightTermParameterTypes...
		//	>;
		return TypeHolder<Terms<
				//NewRightType, 
				MergedPreviousType, 
				LeftPrecedentParameterConstant, 
				LeftTermParameterTypes...
			>>{};
	}
	
	template<typename LeftTermParameterType, typename RightTermParameterType>
	using MergeTerms = decltype(merge_terms(
			std::declval<LeftTermParameterType>(), 
			std::declval<RightTermParameterType>()
		))::Type;
}

#endif // WARP__PARSING__HEADER__PARSING__TERMS_HPP

