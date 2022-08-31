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

namespace Warp::Utilities
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	void auto_variant_delete(Node<NodeType::Multiply>* to_delete) {
		delete to_delete;
	}
	void auto_variant_delete(Node<NodeType::Divide>* to_delete) {
		delete to_delete;
	}
	void auto_variant_delete(Node<NodeType::Add>* to_delete) {
		delete to_delete;
	}
	void auto_variant_delete(Node<NodeType::Subtract>* to_delete) {
		delete to_delete;
	}
	void auto_variant_delete(Node<NodeType::Negation>* to_delete) {
		delete to_delete;
	}
	void auto_variant_delete(Node<NodeType::Expression>* to_delete) {
		delete to_delete;
	}
}

