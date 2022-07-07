#include <Warp/Parsing/Terms.hpp>

#ifndef WARP__UTILITIES__HEADER__PARSING__NUMBERS__HPP
#define WARP__UTILITIES__HEADER__PARSING__NUMBERS__HPP

namespace Warp::Parsing
{
	using DigitsTermType = RegexTerm<"[1-9][0-9]*", "Numerical">;
	using WholeNonTerminalType = NonTerminalTerm<size_t, "Whole">;

	// TODO: Add ability to add dynamic regexes. //
	consteval auto number_rules()
	{

		return ctpg::rules(
				WholeNonTerminalType(DigitsTermType)
						>= [](auto digits) {
							return std::stoi(digits);
						}
			);
	}
}

#endif // WARP__UTILITIES__HEADER__PARSING__NUMBERS__HPP

