//#include <Warp/SyntaxAnalysis/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis/SyntaxNode.hpp>
#include <Warp/Utilities.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	using namespace Warp::Runtime::Compiler;

	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Whole>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralWhole>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Integer>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralInteger>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::FixedPoint>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralFixed>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Character>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralCharacter>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Bool>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralBool>>(value));
	}

	template struct Node<NodeType::LiteralWhole>;
	template struct Node<NodeType::LiteralInteger>;
	template struct Node<NodeType::LiteralCharacter>;
	template struct Node<NodeType::LiteralFixed>;
	template struct Node<NodeType::LiteralBool>;
}

