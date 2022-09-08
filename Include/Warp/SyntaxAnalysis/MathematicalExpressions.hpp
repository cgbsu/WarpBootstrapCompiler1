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
				explicit Node(SyntaxNode left, SyntaxNode right) noexcept : left(std::move(left)), right(std::move(right)) {} \
				explicit Node(BaseNode* left, BaseNode* right) noexcept : left(left), right(right) {} \
				constexpr virtual BaseNode::ViewType to_view() const noexcept final { \
					return BaseNode::ViewType(this); \
				} \
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
		Node(SyntaxNode negated) noexcept : negated(std::move(negated)) {} \
		Node& operator=(const Node& other) noexcept = default; \
		Node& operator=(Node&& other) noexcept = default; \
		constexpr virtual BaseNode::ViewType to_view() const noexcept final { \
			return BaseNode::ViewType{this}; \
		} \
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
		Node(SyntaxNode root) noexcept : root(std::move(root)) {} \
		Node& operator=(const Node& other) noexcept = default; \
		Node& operator=(Node&& other) noexcept = default; \
		constexpr virtual BaseNode::ViewType to_view() const noexcept final {
			return BaseNode::ViewType{this};
		}
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
namespace std
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	extern template class std::unique_ptr<Node<NodeType::Multiply>>;
	extern template class std::unique_ptr<Node<NodeType::Divide>>;
	extern template class std::unique_ptr<Node<NodeType::Add>>;
	extern template class std::unique_ptr<Node<NodeType::Subtract>>;
	extern template class std::unique_ptr<Node<NodeType::Negation>>;
	extern template class std::unique_ptr<Node<NodeType::Expression>>;

	extern template std::unique_ptr<Node<NodeType::Multiply>> std::make_unique<
			Node<NodeType::Multiply>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::Divide>> std::make_unique<
			Node<NodeType::Divide>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::Add>> std::make_unique<
			Node<NodeType::Add>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::Subtract>> std::make_unique<
			Node<NodeType::Subtract>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::Negation>> std::make_unique<
			Node<NodeType::Negation>
		>(decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::Expression>> std::make_unique<
			Node<NodeType::Expression>
		>(decltype(std::move(std::declval<SyntaxNode>())));
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__MATHEMATICAL__EXPRESSIONS__HPP

