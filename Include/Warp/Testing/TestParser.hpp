#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__TESTING__HEADER__TESTING__TEST__PARSER__HPP
#define WARP__TESTING__HEADER__TESTING__TEST__PARSER__HPP

namespace Warp::Testing
{
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
		> consteval auto parse() {
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
	
}

#endif // WARP__TESTING__HEADER__TESTING__TEST__PARSER__HPP

