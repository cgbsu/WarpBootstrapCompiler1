#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__MATHEMATICAL__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__MATHEMATICAL__EXPRESSIONS__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	// Idea have the SyntaxNode type be incomplete inistead of the node type. //
	#define BINARY_NODE(OPERATION) \
			template<> \
			struct Node<NodeType:: OPERATION > : public BaseNode \
			{ \
				constexpr static const auto tag = NodeType:: OPERATION ; \
				SyntaxNode left, right; \
				constexpr Node() noexcept = default; \
				Node(const Node& other) noexcept = default; \
				Node(Node&& other) noexcept = default; \
				constexpr Node& operator=(const Node& other) noexcept = default; \
				Node& operator=(Node&& other) noexcept = default; \
				explicit Node(const SyntaxNode& left, const SyntaxNode& right) noexcept : left(left), right(right) {} \
				constexpr virtual NodeType get_tag() const noexcept final { \
					return tag; \
				} \
				operator SyntaxNode() const noexcept { \
					return SyntaxNode{*this}; \
				} \
			}
	BINARY_NODE(Multiply);
	BINARY_NODE(Divide);
	BINARY_NODE(Add);
	BINARY_NODE(Subtract);

	#undef BINARY_NODE

	template<>
	struct Node<NodeType::Negation> : public BaseNode
	{
		constexpr static const auto tag = NodeType::Negation;
		SyntaxNode negated;
		constexpr Node() noexcept = default; \
		Node(const Node& other) noexcept = default; \
		Node(Node&& other) noexcept = default; \
		Node(const SyntaxNode& negated) noexcept : negated(negated) {} \
		Node& operator=(const Node& other) noexcept = default; \
		Node& operator=(Node&& other) noexcept = default; \
		constexpr virtual NodeType get_tag() const noexcept final {
			return tag;
		}
		operator SyntaxNode() const noexcept {
			return SyntaxNode{*this};
		}
	};

	template<>
	struct Node<NodeType::Expression> : public BaseNode
	{
		constexpr static const auto tag = NodeType::Expression;
		SyntaxNode root;
		constexpr Node() noexcept = default; \
		Node(const Node& other) noexcept = default; \
		Node(Node&& other) noexcept = default; \
		Node(const SyntaxNode& root) noexcept : root(root) {} \
		Node& operator=(const Node& other) noexcept = default; \
		Node& operator=(Node&& other) noexcept = default; \
		constexpr virtual NodeType get_tag() const noexcept final {
			return tag;
		}
		operator SyntaxNode() const noexcept {
			return SyntaxNode{*this};
		}
	};

	extern template struct Node<NodeType::Multiply>;
	extern template struct Node<NodeType::Divide>;
	extern template struct Node<NodeType::Add>;
	extern template struct Node<NodeType::Subtract>;
	extern template struct Node<NodeType::Negation>;
	extern template struct Node<NodeType::Expression>;
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__MATHEMATICAL__EXPRESSIONS__HPP

