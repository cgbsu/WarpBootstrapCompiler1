#include <Warp/Common.hpp>


#ifndef WARP__PARSING__HEADER__PARSING__GENERAL__TERM__TAGS__HPP
#define WARP__PARSING__HEADER__PARSING__GENERAL__TERM__TAGS__HPP
namespace Warp::Parsing
{
	enum class Brackets
	{
		OpenParenthesis, 
		CloseParenthesis, 
		OpenCurleyBracket, 
		ClosedCurleyBracket, 
		OpenSquareBracket, 
		ClosedSquareBracket, 
		OpenAngleBracket, 
		ClosedAngleBracket
	};

	enum class Keyword {
		Let
	};

	enum class MultiPurposeOperator {
		Equal
	};

	enum class Identifier {
		Identifier//, 
		//Meta
	};

	enum class Declaration {
		Constant, 
		SemiColon
	};

	enum class Construct {
		Constant
	};
}
#endif // WARP__PARSING__HEADER__PARSING__GENERAL__TERM__TAGS__HPP

