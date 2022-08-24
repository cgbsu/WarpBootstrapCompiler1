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
		template<typename ReduceToParameterType>
		constexpr const static auto typed_parser = ctpg::parser( ParserType::template term<ReduceToParameterType>, ParserType::terms, 
				ParserType::non_terminal_terms, 
				ParserType::rules()
			);
	};


	template<typename ParserParameterType, auto ReduceToTagParameterConstant>
	constexpr static const auto parser 
			= TestingParser<ParserParameterType>::template parser<ReduceToTagParameterConstant>;

	template<typename ParserParameterType, typename ReduceToParameterType>
	constexpr static const auto typed_parser 
			= TestingParser<ParserParameterType>::template typed_parser<ReduceToParameterType>;

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
			typename ReduceToParameterType
		>
	consteval auto typed_parse()
	{
		return parser<ParserParameterType, ReduceToParameterType>.parse(
				ctpg::buffers::cstring_buffer(StringParameterConstant.string), 
				std::cerr
			);
	}

	template<
			typename ParserParameterType, 
			FixedString StringParameterConstant, 
			auto ReduceToTagParameterConstant
		>
	constexpr auto runtime_parse(bool debug = false)
	{
		return parser<ParserParameterType, ReduceToTagParameterConstant>.parse(
				((debug == true) ? ctpg::parse_options{}.set_verbose() : ctpg::parse_options{}), 
				ctpg::buffers::string_buffer(StringParameterConstant.string), 
				std::cerr
			);
	}

	template<
			typename ParserParameterType, 
			FixedString StringParameterConstant, 
			typename ReduceToParameterType
		>
	constexpr auto typed_runtime_parse(bool debug = false)
	{
		return typed_parser<ParserParameterType, ReduceToParameterType>.parse(
				((debug == true) ? ctpg::parse_options{}.set_verbose() : ctpg::parse_options{}), 
				ctpg::buffers::string_buffer(StringParameterConstant.string), 
				std::cerr
			);
	}
	
	constexpr static const auto equal 
			= [](auto left, auto right) { return left == right; };

	static void check(bool value) {
		WARP_CHECK(value);
	}
	
	template<auto CompareParameterConstant = equal, auto CheckParameterConstant = check>
	constexpr void check_parse(auto parse_result, auto expected_result)
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
	constexpr void strict_check_parse(
				std::optional<ResultParameterType> parse_result, 
				ResultParameterType expected_result
			) {
		check_parse<CompareParameterConstant, CheckParameterConstant>(parse_result, expected_result);
	}
}

#endif // WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP

