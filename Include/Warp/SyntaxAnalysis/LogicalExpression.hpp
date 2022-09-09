#include <Warp/Common.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__COMPARISON__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__COMPARISON__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	#define COMPARISON_OPERATOR_NODE(OPERATION) \
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

		COMPARISON_OPERATOR_NODE(GreaterThan);
		COMPARISON_OPERATOR_NODE(LessThan);
		COMPARISON_OPERATOR_NODE(GreaterThanOrEqualTo);
		COMPARISON_OPERATOR_NODE(LessThankOrEqualTo);
		COMPARISON_OPERATOR_NODE(Equal);
		COMPARISON_OPERATOR_NODE(LogicalOr);
		COMPARISON_OPERATOR_NODE(LogicalAnd);

		#undef COMPARISON_OPERATOR_NODE

		template<>
		struct Node<NodeType::LogicalNot> : public BaseNode
		{
			constexpr static const auto tag = NodeType::LogicalNot;
			SyntaxNode negated;
			constexpr Node() noexcept = default; \
			Node(const Node& other) noexcept = default; \
			Node(Node&& other) noexcept = default; \
			Node(SyntaxNode negated) noexcept : negated(std::move(negated)) {} \
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

		template<>
		struct Node<NodeType::LogicalExpression> : public BaseNode
		{
			constexpr static const auto tag = NodeType::LogicalExpression;
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

	extern template struct Node<NodeType::GreaterThan>;
	extern template struct Node<NodeType::LessThan>;
	extern template struct Node<NodeType::GreaterThanOrEqualTo>;
	extern template struct Node<NodeType::LessThankOrEqualTo>;
	extern template struct Node<NodeType::Equal>;
	extern template struct Node<NodeType::LogicalAnd>;
	extern template struct Node<NodeType::LogicalOr>;
	extern template struct Node<NodeType::LogicalNot>;
	extern template struct Node<NodeType::LogicalExpression>;
}

namespace std
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	extern template class std::unique_ptr<Node<NodeType::GreaterThan>>;
	extern template class std::unique_ptr<Node<NodeType::LessThan>>;
	extern template class std::unique_ptr<Node<NodeType::GreaterThanOrEqualTo>>;
	extern template class std::unique_ptr<Node<NodeType::LessThankOrEqualTo>>;
	extern template class std::unique_ptr<Node<NodeType::Equal>>;
	extern template class std::unique_ptr<Node<NodeType::LogicalAnd>>;
	extern template class std::unique_ptr<Node<NodeType::LogicalOr>>;
	extern template class std::unique_ptr<Node<NodeType::LogicalNot>>;
	extern template class std::unique_ptr<Node<NodeType::LogicalExpression>>;

	extern template std::unique_ptr<Node<NodeType::LogicalExpression>> std::make_unique<
			Node<NodeType::LogicalExpression>
		>(decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::LogicalNot>> std::make_unique<
			Node<NodeType::LogicalNot>
		>(decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::LogicalOr>> std::make_unique<
			Node<NodeType::LogicalOr>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::LogicalAnd>> std::make_unique<
			Node<NodeType::LogicalAnd>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::Equal>> std::make_unique<
			Node<NodeType::Equal>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::LessThankOrEqualTo>> std::make_unique<
			Node<NodeType::LessThankOrEqualTo>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::GreaterThanOrEqualTo>> std::make_unique<
			Node<NodeType::GreaterThanOrEqualTo>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::LessThan>> std::make_unique<
			Node<NodeType::LessThan>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	extern template std::unique_ptr<Node<NodeType::GreaterThan>> std::make_unique<
			Node<NodeType::GreaterThan>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__COMPARISON__HPP

