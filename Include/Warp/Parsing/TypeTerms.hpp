#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__TYPE__TERMS__HPP
#define WARP__PARSING__HEADER__PARSING__TYPE__TERMS__HPP

namespace Warp::Parsing
{
	using TypeTerms = MakeTerms<
			TypeTreeTerm<
					NumericTypeTag::FixedPoint, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Whole, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Whole>::Type, 
					FixedString{"Whole"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Integer, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Integer>::Type, 
					FixedString{"Integer"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::FixedPoint, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Character, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Character>::Type, 
					FixedString{"Character"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Bool, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Bool>::Type, 
					FixedString{"Bool"}
				>
		>;
}

#endif // WARP__PARSING__HEADER__PARSING__TYPE__TERMS__HPP

