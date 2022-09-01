#include <Warp/Parsing/MathematicalExpressions.hpp>

namespace Warp::Parsing
{
	template class NumericTypeResolver<NumericTypeTag::Whole>;
	template class NumericTypeResolver<NumericTypeTag::Integer>;
	template class NumericTypeResolver<NumericTypeTag::FixedPoint>;
	template class NumericTypeResolver<NumericTypeTag::Character>;
	template class NumericTypeResolver<NumericTypeTag::Bool>;
	
	template class MathematicalExpressionParser<
					MathematicalExpressionTermsType, 
					NumericTypeResolver, 
					NumericTypeTag::Whole, 
					NumericTypeTag::Whole
			>;
	
	template class MathematicalExpressionParser<
					MathematicalExpressionTermsType, 
					NumericTypeResolver, 
					NumericTypeTag::Integer, 
					NumericTypeTag::Integer
			>;
	
	template class MathematicalExpressionParser<
					MathematicalExpressionTermsType, 
					NumericTypeResolver, 
					NumericTypeTag::FixedPoint, 
					NumericTypeTag::FixedPoint
			>;
	
	template class MathematicalExpressionParser<
					MathematicalExpressionTermsType, 
					NumericTypeResolver, 
					NumericTypeTag::Character, 
					NumericTypeTag::Character
			>;
	
	template class MathematicalExpressionParser<
					MathematicalExpressionTermsType, 
					NumericTypeResolver, 
					NumericTypeTag::Bool, 
					NumericTypeTag::Bool
			>;
}

