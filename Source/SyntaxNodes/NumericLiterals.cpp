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
}

namespace std
{
	using namespace Warp::Runtime::Compiler;
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	template struct Node<NodeType::LiteralWhole>;
	template struct Node<NodeType::LiteralInteger>;
	template struct Node<NodeType::LiteralCharacter>;
	template struct Node<NodeType::LiteralFixed>;
	template struct Node<NodeType::LiteralBool>;

	template class std::unique_ptr<Node<NodeType::LiteralWhole>>;
	template class std::unique_ptr<Node<NodeType::LiteralInteger>>;
	template class std::unique_ptr<Node<NodeType::LiteralCharacter>>;
	template class std::unique_ptr<Node<NodeType::LiteralFixed>>;
	template class std::unique_ptr<Node<NodeType::LiteralBool>>;

	template std::unique_ptr<Node<NodeType::LiteralWhole>> std::make_unique<
			Node<NodeType::LiteralWhole>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Whole>::Type>()));
	
	template std::unique_ptr<Node<NodeType::LiteralInteger>> std::make_unique<
			Node<NodeType::LiteralInteger>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Integer>::Type>()));

	template std::unique_ptr<Node<NodeType::LiteralCharacter>> std::make_unique<
			Node<NodeType::LiteralCharacter>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Character>::Type>()));

	template std::unique_ptr<Node<NodeType::LiteralFixed>> std::make_unique<
			Node<NodeType::LiteralFixed>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::FixedPoint>::Type>()));

	template std::unique_ptr<Node<NodeType::LiteralBool>> std::make_unique<
			Node<NodeType::LiteralBool>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Bool>::Type>()));
}

