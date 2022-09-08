#include <Warp/Parsing/FunctionDeclarations.hpp>

namespace Warp::Parsing
{
	template class FunctionDeclarationParser<
			FunctionDeclaritionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag
		>;
}

