
	//template<typename... OrPairParameterTypes>
	//struct RuleOr
	//{
	//	std::tuple<OrPairParameterTypes...> terms;
	//	constexpr RuleOr(OrPairParameterTypes... terms) noexcept : terms(std::tuple{terms...}) {}
	//	constexpr RuleOr(std::tuple<OrPairParameterTypes...> terms) noexcept : terms(terms) {}
	//	constexpr RuleOr(const RuleOr& other) noexcept = default;
	//	constexpr RuleOr(RuleOr&& other) noexcept = default;
	//	constexpr RuleOr& operator=(const RuleOr& other) noexcept = default;
	//	constexpr RuleOr& operator=(RuleOr&& other) noexcept = default;

	//	constexpr auto after_first(auto first, auto... remaining) const noexcept {
	//		return RuleOr<CleanType<decltype(remaining)>...>(std::tuple{remaining...});
	//	}
	//	template<size_t... IndexParameterConstants>
	//	constexpr auto remove_first_implementation(std::index_sequence<IndexParameterConstants...>) const noexcept {
	//		return after_first(std::get<IndexParameterConstants>(terms)...);
	//	}
	//	constexpr auto remove_first() const noexcept {
	//		return remove_first_implementation(std::make_index_sequence<std::tuple_size_v<decltype(terms)>>());
	//	}
	//};

	//
	//template<size_t... PrecedingIndexParameterConstants, size_t... ProcedingIndexParameterConstants>
	//constexpr auto make_rule_tuple(
	//		auto reduction, 
	//		auto preceding_terms, 
	//		std::index_sequence<PrecedingIndexParameterConstants...> preceding_indicies, 
	//		auto or_term_term, 
	//		auto proceding_terms, 
	//		std::index_sequence<ProcedingIndexParameterConstants...> proceding_indicies, 
	//		auto rule_generator
	//	)
	//{
	//	return std::tuple{
	//			reduction, 
	//			std::tuple{
	//					std::get<PrecedingIndexParameterConstants>(preceding_terms)..., 
	//					or_term_term, 
	//					std::get<ProcedingIndexParameterConstants>(proceding_terms)...
	//				}, 
	//			rule_generator(or_term_term)
	//		};
	//}
	//		

	//template<
	//		typename ReductionParameterType, 
	//		typename... PrecedingTermParameterTypes, 
	//		typename... OrPairParameterTypes, 
	//		typename... ProcedingTermParameterTypes
	//	>
	//requires(sizeof...(OrPairParameterTypes) > 1)
	//constexpr auto reduce_or_rule(
	//		ReductionParameterType reduction, 
	//		std::tuple<PrecedingTermParameterTypes...> preceding_terms, 
	//		RuleOr<OrPairParameterTypes...> or_terms, 
	//		std::tuple<ProcedingTermParameterTypes...> proceding_terms, 
	//		auto rule_generator
	//	)
	//{
	//	return std::tuple_cat(
	//		ctpg::rules(
	//				make_rule_tuple(
	//						reduction, 
	//						preceding_terms, 
	//						std::make_index_sequence<std::tuple_size_v<decltype(preceding_terms)>>(), 
	//						std::get<0>(or_terms.terms), 
	//						proceding_terms, 
	//						std::make_index_sequence<std::tuple_size_v<decltype(proceding_terms)>>(), 
	//						rule_generator
	//					)
	//			), 
	//		reduce_or_rule(
	//				reduction, 
	//				preceding_terms, 
	//				or_terms.remove_first(), 
	//				proceding_terms, 
	//				rule_generator
	//			)
	//	);
	//}
	//
	//template<
	//		typename ReductionParameterType, 
	//		typename... PrecedingTermParameterTypes, 
	//		typename... OrPairParameterTypes, 
	//		typename... ProcedingTermParameterTypes
	//	>
	//requires(sizeof...(OrPairParameterTypes) == 1)
	//constexpr auto reduce_or_rule(
	//		ReductionParameterType reduction, 
	//		std::tuple<PrecedingTermParameterTypes...> preceding_terms, 
	//		RuleOr<OrPairParameterTypes...> or_terms, 
	//		std::tuple<ProcedingTermParameterTypes...> proceding_terms, 
	//		auto rule_generator
	//	)
	//{
	//	return ctpg::rules(
	//			make_rule_tuple(
	//					reduction, 
	//					preceding_terms, 
	//					std::make_index_sequence<std::tuple_size_v<decltype(preceding_terms)>>(), 
	//					std::get<0>(or_terms.terms), 
	//					proceding_terms, 
	//					std::make_index_sequence<std::tuple_size_v<decltype(proceding_terms)>>(), 
	//					rule_generator
	//				)
	//		);
	//}
	//

	//template<auto ConvertParameterConstant, typename TermParameterType>
	//struct OrTermConstructor
	//{
	//	using TermType = TermParameterType;
	//	constexpr static const auto convert = ConvertParameterConstant;
	//	TermType term;
	//	constexpr OrTermConstructor(TermParameterType term) : term(term) {}
	//	constexpr OrTermConstructor(const OrTermConstructor& other) = default;
	//	constexpr OrTermConstructor(OrTermConstructor&& other) = default;
	//	constexpr OrTermConstructor& operator=(const OrTermConstructor& term) = default;
	//	constexpr OrTermConstructor& operator=(OrTermConstructor&& other) = default;
	//};
