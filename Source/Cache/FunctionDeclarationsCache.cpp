#include <Warp/Parsing/FunctionDeclarations.hpp>

#ifdef WARP__PARSING__ENABLE__TEMPLATE__CACHING
namespace Warp::Parsing
{
	template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Whole, 
			NumericTypeTag::Whole, 
			FixedString{"WholeTerm"}, 
			FixedString{"WholeSum"}, 
			FixedString{"WholeExpression"}
		>;

	template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Integer, 
			NumericTypeTag::Integer, 
			FixedString{"IntegerTerm"}, 
			FixedString{"IntegerSum"}, 
			FixedString{"IntegerExpression"}
		>;

	template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::FixedPoint, 
			NumericTypeTag::FixedPoint, 
			FixedString{"FixedPointTerm"}, 
			FixedString{"FixedPointSum"}, 
			FixedString{"FixedPointExpression"}
		>;

	template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Character, 
			NumericTypeTag::Character, 
			FixedString{"CharacterTerm"}, 
			FixedString{"CharacterSum"}, 
			FixedString{"CharacterExpression"}
		>;

	template class MathematicalExpressionParser<
			FunctionDeclaritionTermsType, 
			Warp::Runtime::Compiler::NumericTypeResolver, 
			NumericTypeTag::Bool, 
			NumericTypeTag::Bool, 
			FixedString{"BoolTerm"}, 
			FixedString{"BoolSum"}, 
			FixedString{"BoolExpression"}
		>;

	template class FunctionDeclarationParser<
			FunctionDeclaritionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag
		>;

}
#endif

