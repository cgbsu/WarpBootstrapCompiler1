#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/Parsing/MathematicalExpressions.hpp>
#include <Warp/Utilities.hpp>
#include <ThirdParty/fpm/ios.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Testing/TestParser.hpp>
#include <Warp/Runtime/Compiler/SimpleExecutor.hpp>

using namespace Warp::Testing;
using namespace Warp::Parsing;
using namespace Warp::Utilities;

extern template class NumericTypeResolver<NumericTypeTag::Whole>;
extern template class NumericTypeResolver<NumericTypeTag::Integer>;
extern template class NumericTypeResolver<NumericTypeTag::Character>;
extern template class NumericTypeResolver<NumericTypeTag::Bool>;

extern template class MathematicalExpressionParser<MathematicalExpressionTermsType, NumericTypeResolver, NumericTypeTag::Whole, NumericTypeTag::Whole>;
extern template class MathematicalExpressionParser<MathematicalExpressionTermsType, NumericTypeResolver, NumericTypeTag::Integer, NumericTypeTag::Integer>;
extern template class MathematicalExpressionParser<MathematicalExpressionTermsType, NumericTypeResolver, NumericTypeTag::FixedPoint, NumericTypeTag::FixedPoint>;

