#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Important!: Implicit requirement, define: **************************************************
	* WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED *
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifndef WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP

namespace Warp::Testing
{
	#ifndef WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED
		static_assert(false, "Warp::Testing: Please define the macro "
				"WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED"
				"for unit testing."
			);
	#else 
		#define WARP_CHECK(BOOLEAN_EXPRESSION) \
			WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED(BOOLEAN_EXPRESSION)
	#endif 
	using namespace Warp::Utilities;

	template<typename ParserParameterType>
	struct TestingParser
	{
		using ParserType = ParserParameterType;
		template<auto ReduceToTagParameterConstant>
		constexpr const static auto parser = ctpg::parser(
				ParserType::template term<ReduceToTagParameterConstant>, 
				ParserType::terms, 
				ParserType::non_terminal_terms, 
				ParserType::rules()
			);
	};


	template<typename ParserParameterType, auto ReduceToTagParameterConstant>
	constexpr static const auto parser 
			= TestingParser<ParserParameterType>::template parser<ReduceToTagParameterConstant>;

	template<
			typename ParserParameterType, 
			FixedString StringParameterConstant, 
			auto ReduceToTagParameterConstant
		>
	consteval auto parse()
	{
		return parser<ParserParameterType, ReduceToTagParameterConstant>.parse(
				ctpg::buffers::cstring_buffer(StringParameterConstant.string), 
				std::cerr
			);
	}

	template<
			typename ParserParameterType, 
			FixedString StringParameterConstant, 
			auto ReduceToTagParameterConstant
		>
	constexpr auto runtime_parse()
	{
		return parser<ParserParameterType, ReduceToTagParameterConstant>.parse(
				ctpg::buffers::string_buffer(StringParameterConstant.string), 
				std::cerr
			);
	}
	
	constexpr static const auto equal 
			= [](auto left, auto right) { return left == right; };
	
	template<auto CompareParameterConstant = equal>
	constexpr void check_parse(auto parse_result, auto expected_result)
	{
		WARP_CHECK(parse_result.has_value() == true);
		WARP_CHECK((parse_result.has_value() == true)
				? CompareParameterConstant(parse_result.value(), expected_result)
				: false
			);
	}
	
	template<typename ResultParameterType, auto CompareParameterConstant = equal>
	constexpr void strict_check_parse(
			std::optional<ResultParameterType> parse_result, 
			ResultParameterType expected_result) {
		check_parse(parse_result, expected_result);
	}
}

#endif // WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP

