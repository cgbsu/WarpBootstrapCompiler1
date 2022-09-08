#include <Warp/SyntaxAnalysis/MathematicalExpressions.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	template struct Node<NodeType::Multiply>;
	template struct Node<NodeType::Divide>;
	template struct Node<NodeType::Add>;
	template struct Node<NodeType::Subtract>;
	template struct Node<NodeType::Negation>;
	template struct Node<NodeType::Expression>;
}

namespace std
{

	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	template class std::unique_ptr<Node<NodeType::Multiply>>;
	template class std::unique_ptr<Node<NodeType::Divide>>;
	template class std::unique_ptr<Node<NodeType::Add>>;
	template class std::unique_ptr<Node<NodeType::Subtract>>;
	template class std::unique_ptr<Node<NodeType::Negation>>;
	template class std::unique_ptr<Node<NodeType::Expression>>;

	template std::unique_ptr<Node<NodeType::Multiply>> std::make_unique<
			Node<NodeType::Multiply>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::Divide>> std::make_unique<
			Node<NodeType::Divide>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::Add>> std::make_unique<
			Node<NodeType::Add>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::Subtract>> std::make_unique<
			Node<NodeType::Subtract>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::Negation>> std::make_unique<
			Node<NodeType::Negation>
		>(decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::Expression>> std::make_unique<
			Node<NodeType::Expression>
		>(decltype(std::move(std::declval<SyntaxNode>())));
}

