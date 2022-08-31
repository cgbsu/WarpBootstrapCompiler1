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
				constexpr Node(const Node& other) noexcept = default; \
				constexpr Node(Node&& other) noexcept = default; \
				constexpr Node& operator=(const Node& other) noexcept = default; \
				constexpr Node& operator=(Node&& other) noexcept = default; \
				constexpr Node(const SyntaxNode& left, const SyntaxNode& right) noexcept : left(std::move(left)), right(std::move(right)) {} \
				constexpr virtual NodeType get_tag() const noexcept final { \
					return tag; \
				} \
				constexpr operator SyntaxNode() const noexcept { \
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
		constexpr Node(const Node& other) noexcept = default; \
		constexpr Node(Node&& other) noexcept = default; \
		constexpr Node(const SyntaxNode& negated) noexcept : negated(negated) {} \
		constexpr Node& operator=(const Node& other) noexcept = default; \
		constexpr Node& operator=(Node&& other) noexcept = default; \
		constexpr virtual NodeType get_tag() const noexcept final {
			return tag;
		}
		constexpr operator SyntaxNode() const noexcept {
			return SyntaxNode{*this};
		}
	};

	template<>
	struct Node<NodeType::Expression> : public BaseNode
	{
		constexpr static const auto tag = NodeType::Expression;
		SyntaxNode root;
		constexpr Node() noexcept = default; \
		constexpr Node(const Node& other) noexcept = default; \
		constexpr Node(Node&& other) noexcept = default; \
		constexpr Node(const SyntaxNode& root) noexcept : root(root) {} \
		constexpr Node& operator=(const Node& other) noexcept = default; \
		constexpr Node& operator=(Node&& other) noexcept = default; \
		constexpr virtual NodeType get_tag() const noexcept final {
			return tag;
		}
		constexpr operator SyntaxNode() const noexcept {
			return SyntaxNode{*this};
		}
	};
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__MATHEMATICAL__EXPRESSIONS__HPP

