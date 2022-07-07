#include <Warp/Parsing/Terms.hpp>

#ifndef WARP__UTILITIES__HEADER__PARSING__NUMBERS__HPP
#define WARP__UTILITIES__HEADER__PARSING__NUMBERS__HPP

namespace Warp::Parsing
{
	/*constexpr auto digits_term() noexcept {
		return RegexTerm<"[1-9][0-9]*", "Numerical">::term;
	}

	constexpr auto whole_non_terminal_term() {
		return NonTerminalTerm<size_t, "Whole">::term;
	}

	// TODO: Add ability to add dynamic regexes. //
	consteval auto number_rules()
	{

		return ctpg::rules(
				whole_non_terminal_term()(digits_term())
						>= [](auto digits) {
							return std::stoi(digits);
						}
			);
	}*/
}

#endif // WARP__UTILITIES__HEADER__PARSING__NUMBERS__HPP

