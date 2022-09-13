#include <Warp/SyntaxAnalysis/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis/MathematicalExpressions.hpp>
#include <Warp/SyntaxAnalysis/LogicalExpression.hpp>
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

	SyntaxNode constant_call(std::string name);

	template<NodeType NodeTypeParameterConstant>
	SyntaxNode binary_node(SyntaxNode left, SyntaxNode right)
	{
		return std::make_unique<
				Node<NodeTypeParameterConstant>
			>(std::move(left), std::move(right));
	}

	template<NodeType NodeTypeParameterConstant>
	SyntaxNode unary_node(SyntaxNode child)
	{
		return std::make_unique<
				Node<NodeTypeParameterConstant>
			>(std::move(child));
	}
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__NODE__HPP

