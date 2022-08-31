#include <Warp/SyntaxAnalysis/NumericLiterals.hpp>
#include <Warp/Utilities.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	using namespace Warp::Runtime::Compiler;

	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Whole>::Type value) {
		return to_auto_variant<SyntaxNode>(Node<NodeType::LiteralWhole>{value});
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Integer>::Type value) {
		return to_auto_variant<SyntaxNode>(Node<NodeType::LiteralInteger>{value});
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::FixedPoint>::Type value) {
		return to_auto_variant<SyntaxNode>(Node<NodeType::LiteralFixed>{value});
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Character>::Type value) {
		return to_auto_variant<SyntaxNode>(Node<NodeType::LiteralCharacter>{value});
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Bool>::Type value) {
		return to_auto_variant<SyntaxNode>(Node<NodeType::LiteralBool>{value});
	}
}

