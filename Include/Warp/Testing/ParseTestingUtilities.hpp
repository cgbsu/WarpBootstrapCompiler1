#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Testing/TestParser.hpp>

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Important!: Implicit requirement, define: **************************************************
	* WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED *
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifndef WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP

namespace Warp::Testing
{
	using namespace Warp::Utilities;

	#ifndef WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED
		static_assert(false, "Warp::Testing: Please define the macro "
				"WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED"
				"for unit testing."
			);
	#else 
		#define WARP_CHECK(BOOLEAN_EXPRESSION) \
			WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED(BOOLEAN_EXPRESSION)
	#endif 
	
	constexpr static const auto equal 
			= [](const auto& left, const auto& right) { return left == right; };

	static void check(bool value) {
		WARP_CHECK(value);
	}
	
	template<auto CompareParameterConstant = equal, auto CheckParameterConstant = check>
	constexpr void check_parse(const auto& parse_result, auto expected_result)
	{
		CheckParameterConstant(parse_result.has_value() == true);
		WARP_CHECK((parse_result.has_value()));
		WARP_CHECK(((parse_result.has_value() == true)
				? CompareParameterConstant(parse_result.value(), expected_result)
				: false
			));
	}
	
	template<
			typename ResultParameterType, 
			auto CompareParameterConstant = equal, 
			auto CheckParameterConstant = check
		>
	constexpr static void strict_check_parse(
				std::optional<ResultParameterType> parse_result, 
				ResultParameterType expected_result
			) {
		check_parse<CompareParameterConstant, CheckParameterConstant>(parse_result, expected_result);
	}
}

#endif // WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP

