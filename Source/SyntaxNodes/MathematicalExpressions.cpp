#include <Warp/SyntaxAnalysis/MathematicalExpressions.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	extern template struct Node<NodeType::Multiply>;
	extern template struct Node<NodeType::Divide>;
	extern template struct Node<NodeType::Add>;
	extern template struct Node<NodeType::Subtract>;
	extern template struct Node<NodeType::Negation>;
	extern template struct Node<NodeType::Expression>;
}

