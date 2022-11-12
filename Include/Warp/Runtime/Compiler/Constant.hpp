#include <Warp/Common.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

namespace Warp::Runtime::Compiler
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	template<
			typename ValueParameterType, 
			typename TypeParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct Constant
	{
		using ValueType = ValueParameterType;
		using TypeType = TypeParameterType;
		using IdentifierType = IdentifierParameterType;
		IdentifierType name;
		TypeType type;
		ValueType value;
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

