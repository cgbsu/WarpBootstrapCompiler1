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
		struct AssociatedTemplateConstructable0
		{
			template<
					template<auto, template<auto...> typename, auto...> 
							typename AssociationParameterType, 
					auto TagParameterConstant, 
					template<auto...> typename AssociatedTemplateParameterTemplate, 
					auto... ParameterConstants 
				>
			AssociatedTemplateConstructable0(TypeHolder<AssociationParameterType<
					TagParameterConstant, 
					AssociatedTemplateParameterTemplate, 
					ParameterConstants...>>
				) {}
		};

		struct AssociatedTemplateConstructable1
		{
			template<
					template<auto, template<typename, auto...> typename, typename, auto...> 
							typename AssociationParameterType, 
					auto TagParameterConstant, 
					template<typename, auto...> typename AssociatedTemplateParameterTemplate, 
					typename ParameterType, 
					auto... ParameterConstants 
				>
			AssociatedTemplateConstructable1(TypeHolder<AssociationParameterType<
					TagParameterConstant, 
					AssociatedTemplateParameterTemplate, 
					ParameterType, 
					ParameterConstants...>>
				) {}
		};
	}

	template<typename TreeTermParameterType>
	concept AssociatedTreeTermTemplateConcept = requires(TreeTermParameterType canidate) {
		Detail::AssociatedTemplateConstructable0(TypeHolder<decltype(canidate)>{});
	}; 

	template<typename TypeTreeTermParameterType>
	concept AssociatedTypeTreeTermTemplateConcept = requires(TypeTreeTermParameterType canidate) {
		Detail::AssociatedTemplateConstructable1(TypeHolder<decltype(canidate)>{});
	}; 
	
	template<typename AssociatedTemplateParameterType>
	concept AssociatedTemplateConcept 
		= AssociatedTreeTermTemplateConcept<AssociatedTemplateParameterType>
				|| AssociatedTypeTreeTermTemplateConcept<AssociatedTemplateParameterType>;

	template<typename>
	struct IsTermHelper {
		constexpr static const bool value = false;
	};

	template<AssociatedTreeTermTemplateConcept TermParameterType>
	struct IsTermHelper<TermParameterType> {
		using Type = TermParameterType;
		constexpr static const bool value = true;
	};

	template<typename CanidateParameterType>
	constexpr static const bool is_term = IsTermHelper<CanidateParameterType>::value;

	template<typename>
	struct IsTypeTermHelper {
		constexpr static const bool value = false;
	};

	template<AssociatedTypeTreeTermTemplateConcept TermParameterType>
	struct IsTypeTermHelper<TermParameterType> {
		using Type = TermParameterType;
		constexpr static const bool value = true;
	};

	template<typename CanidateParameterType>
	constexpr static const bool is_type_term = IsTypeTermHelper<CanidateParameterType>::value;

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
		using TermType = decltype(get_term<TagParameterConstant>())::Type;

		template<auto TagParameterConstant>
		constexpr static const auto term  = TermType<TagParameterConstant>
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
		using AppendTerms = Terms<PreviousType, precedence, TermParameterTypes..., NewTermParameterTypes...>;

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

		template<
				bool ReduceToNonTerminalTermsParameterConstant, 
				typename CurrentParameterType, 
				AssociatedTemplateConcept... CanidateTermParameterTypes, 
				typename... CurrentTermsParameterTypes
			>
		consteval static const auto filter_terms_implementation(
				const std::tuple<PlaceHolder, CurrentTermsParameterTypes...> terms
			)
		{
			if constexpr(
					(ReduceToNonTerminalTermsParameterConstant == false 
							&& is_term<CurrentParameterType> == true)
					|| (ReduceToNonTerminalTermsParameterConstant == true 
							&& is_type_term<CurrentParameterType> == true)
				)
			{
				using NextTermsType = decltype(std::tuple_cat(
						terms, 
						std::tuple{std::declval<CurrentParameterType>()}
					));
				if constexpr(sizeof...(CanidateTermParameterTypes) > 0)
				{
					return filter_terms_implementation<
							ReduceToNonTerminalTermsParameterConstant, 
							CanidateTermParameterTypes...
						>(std::declval<NextTermsType>());
				}
				else
					return std::declval<NextTermsType>();
			}
		}

		template<bool ReduceToNonTerminalTermsParameterConstant>
		consteval static const auto filter_terms()
		{
			return filter_terms_implementation<
					ReduceToNonTerminalTermsParameterConstant, 
					TermParameterTypes...
				>(std::tuple{PlaceHolder{}});
		}

		template<bool ReduceToNonTerminalTermsParameterConstant>
		using FilteredTermsType = decltype(filter_terms<
				ReduceToNonTerminalTermsParameterConstant
			>());

		template<bool ReduceToNonTerminalTermsParameterConstant>
		consteval static const auto recursive_filter_to_terms()
		{
			using CurrentType = FilteredTermsType<ReduceToNonTerminalTermsParameterConstant>;
			if constexpr(is_root == false)
			{
				constinit static auto next = PreviousType
						::template recursive_filter_to_terms<
								ReduceToNonTerminalTermsParameterConstant
							>();
				if constexpr(std::tuple_size_v<decltype(next)> > 1)
					return std::tuple_cat(next, std::declval<TupleAfterFirstType<CurrentType>>());
				else
					return std::declval<CurrentType>();
			}
			else 
				return std::declval<CurrentType>();
		}

		template<bool ReduceToNonTerminalTermsParameterConstant>
		using RecursivleyFilteredTermsType = decltype(
				recursive_filter_to_terms<ReduceToNonTerminalTermsParameterConstant>()
			);

		template<bool ReduceToNonTerminalTermsParameterConstant, 
				size_t... TermIndexParameterConstants>
		consteval static const auto create_terms_implementation(
				std::index_sequence<TermIndexParameterConstants...> 
			)
		{
			constinit static const auto tree_terms = recursive_filter_to_terms<
					ReduceToNonTerminalTermsParameterConstant
				>();
			return std::tuple{std::get<TermIndexParameterConstants>(tree_terms)...};
		}
		template<bool ReduceToNonTerminalTermsParameterConstant, 
				size_t... TermIndexParameterConstants>
		consteval static const auto create_terms()
		{
			using RecursivleyFilteredType
					= RecursivleyFilteredTermsType<
							ReduceToNonTerminalTermsParameterConstant
						>;
			if constexpr(std::is_same_v<RecursivleyFilteredType, std::nullopt_t> == true 
					|| std::is_same_v<RecursivleyFilteredType, void>)
				return std::nullopt;
			else
			{
				return create_terms_implementation(
						std::make_index_sequence<std::tuple_size_v<RecursivleyFilteredType
					>>());
			}
		}
	};

	template<AssociatedTemplateConcept... MakeTermParameterTypes>
	using MakeTerms = Terms<void, 0, MakeTermParameterTypes...>;

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
	constexpr const static auto merge_terms( // Merginer Right INTO Left "less than" case
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
		return TypeHolder<Terms<
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

