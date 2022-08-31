#include <Warp/SyntaxAnalysis/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis/MathematicalExpressions.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__NODE__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__NODE__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{

	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Whole>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Integer>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::FixedPoint>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Character>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Bool>::Type value);
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__NODE__HPP

