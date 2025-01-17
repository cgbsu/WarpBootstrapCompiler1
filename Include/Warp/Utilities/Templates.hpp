#include <Warp/Common.hpp>
/* This work is under the Copyright Christopher A. Greeley (2024) and it is distributed
* under the No Kill Do No Harm License, a legally non-binding sumemry is as follows: 
* 
* # No Kill Do No Harm Licence – Summary
* 
* Based on version 0.3, July 2022 of the Do No Harm License
* 
* https://github.com/raisely/NoHarm
* 
* LEGALLY NON-BINDING SUMMARY OF THE TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
* 
* ## Licence Grants
* 
* You're allowed
* 
* - to distribute the licensed work,
* - to create, publish, sublicense and patent derivative works and
* - to put your modifications or your derivative work under a seperate licence,
* 
* free of charge. Though, filing patent litigation leads to the loss of the patent licence. Also, the licence grants don't include the right to use the licensor's trademarks.
* 
* ## Unethical Behaviour
* 
* You may not use the licensed work if you engage in:
* 
* - human rights violations,
* - environmental destruction,
* - warfare,
* - addictive/destructive products or services or
* - actions that frustrate:
*   * peace,
*   * access to human rights,
*   * peaceful assembly and association,
*   * a sustainable environment or
*   * democratic processes
*   * abortion
*   * euthanasia
*   * human embryonic stem cell research (if human organisms are killed in the process)
* - except for actions that may be contrary to "human rights" (or interpretations thereof), do not kill and that frustrate 
*   * abortion
*   * euthanasia
*   * killing
* and; the software must never be used to kill, including: abortion, euthanasia, human stem cell research, in war, or law enforcement or as a part of any lethal weapon
* 
* ## Contributions
* 
* Contributions to the licensed work must be licensed under the exact same licence.
* 
* ## Licence Notice
* 
* When distributing the licensed work or your derivative work, you must
* 
* - include a copy of this licence,
* - retain attribution notices,
* - state changes that you made and
* - not use the names of the author and the contributors to promote your derivative work.
* 
* If the licensed work includes a "NOTICE" text file with attribution notices, you must copy those notices to:
* 
* - a "NOTICE" file within your derivative work,
* - a place within the source code or the documentation or
* - a place within a display generated by your derivative work.
* 
* ## No Warranty or Liability
* 
* The licensed work is offered on an as-is basis without any warranty or liability. You may choose to offer warranty or liability for your derivative work, but only fully on your own responsibility. */



#ifndef WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

namespace Warp::Utilities
{
	struct PlaceHolder {};

	/* Useful for when you need an instance of something,    * 
	 * say for a constexpr/consteval function, but dont want * 
	 * to instantiate it (its just for retrieving the type)  */
	template<typename ParameterType>
	struct TypeHolder {
		using Type = ParameterType;
	};

	template<typename DoNotConvertToParameterType, typename ConvertToParameterType, typename CanidateParameterType>
	concept ConvertableToExceptConcept =
			(!std::convertible_to<CleanType<DoNotConvertToParameterType>, CleanType<CanidateParameterType>> 
					&& std::convertible_to<CleanType<ConvertToParameterType>, CleanType<CanidateParameterType>>)
			|| (!std::same_as<CleanType<DoNotConvertToParameterType>, CleanType<CanidateParameterType>> 
					&& std::convertible_to<CleanType<ConvertToParameterType>, CleanType<CanidateParameterType>>);

	template<typename, typename>
	struct TotallyOrderedWith {
		constexpr static const bool value = false;
	};

	template<typename LeftParameterType, typename RightParameterType>
	requires std::totally_ordered_with<LeftParameterType, RightParameterType>
	struct TotallyOrderedWith<LeftParameterType, RightParameterType> {
		constexpr static const bool value = true;
	};

	template<typename, typename>
	struct EquallyComparibleWith {
		constexpr static const bool value = false;
	};

	template<typename LeftParameterType, typename RightParameterType>
	requires std::equality_comparable_with<LeftParameterType, RightParameterType>
	struct EquallyComparibleWith<LeftParameterType, RightParameterType> {
		constexpr static const bool value = true;
	};

	template<typename LeftParameterType, typename RightParameterType>
	constexpr static const bool is_comparible
			= TotallyOrderedWith<LeftParameterType, RightParameterType>::value;

	template<typename LeftParameterType, typename RightParameterType>
	constexpr static const bool is_equally_comparible 
			= EquallyComparibleWith<LeftParameterType, RightParameterType>::value;

	template<typename CanidateParameterType>
	concept HasTypeConcept = requires(CanidateParameterType) { typename CanidateParameterType::Type; };

	template<typename ParserParameterType>
	concept ParserConcept = requires(ParserParameterType) {
			ParserParameterType::rules();
			ParserParameterType::terms;
			ParserParameterType::non_terminal_terms;
		};

	template<size_t ToIgnoreParmeterConstant, size_t... IndexParameterConstants>
	constexpr static const auto take_after_implentation(auto from, std::index_sequence<IndexParameterConstants...>) {
		return std::tuple{std::get<IndexParameterConstants + ToIgnoreParmeterConstant>(from)...};
	}

	template<size_t ToIgnoreParmeterConstant>
	constexpr static const auto take_after(auto from) {
		return take_implentation(from, (std::tuple_size_v<decltype(from)> - ToIgnoreParmeterConstant));
	}

	template<typename TupleParameterType, size_t ToIgnoreParmeterConstant>
	using TupleAfterType = decltype(take_after<ToIgnoreParmeterConstant>(std::declval<TupleParameterType>()));

	template<typename TupleParameterType>
	using TupleAfterFirstType = TupleAfterType<TupleParameterType, 1>;

	constexpr const auto concatinate_tuples(auto current_tuple, auto last_tuple) {
		return std::tuple_cat(current_tuple, last_tuple);
	}

	constexpr const auto concatinate_tuples(auto current_tuple, auto... tuples)
			requires(sizeof...(tuples) > 0) {
		return std::tuple_cat(current_tuple, concatinate_tuples(tuples...));
	}
	// Shameless yoink from cppreference.com/reference/en/cpp/utility/variant/visit.html
	template<class... Ts> struct OverloadedVisit : Ts... { using Ts::operator()...; };
	template<class... Ts> OverloadedVisit(Ts...) -> OverloadedVisit<Ts...>;

	template<typename ParameterType>
	struct IsOptionalHelper {
		constexpr static const bool value = false;
		constexpr IsOptionalHelper(ParameterType) {}
	};
	
	template<typename ParameterType>
	struct IsOptionalHelper<std::optional<ParameterType>> {
		constexpr static const bool value = true;
		constexpr IsOptionalHelper(std::optional<ParameterType>) {}
	};
	
	template<typename ParameterType>
	constexpr static const auto is_optional = IsOptionalHelper<ParameterType>::value;
	
	template<typename ParameterType>
	concept OptionalConcept = is_optional<ParameterType>;
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__TEMPLATES__HPP

