#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__CALL__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__CALL__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	template<>
	struct Node<NodeType::ConstantCall> : public BaseNode
	{
		constexpr static const auto tag = NodeType::ConstantCall;
		std::string name;
		constexpr Node() noexcept = default;
		constexpr Node(const Node& other) noexcept = default;
		Node(Node&& other) noexcept = default;
		constexpr ~Node() = default;
		constexpr Node& operator=(const Node& other) noexcept = default;
		Node& operator=(Node&& other) noexcept = default;
		constexpr Node(std::string name) noexcept : name(name) {}
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
	struct Node<NodeType::FunctionCall> : public BaseNode
	{
		constexpr static const auto tag = NodeType::FunctionCall;
		std::string name;
		std::vector<SyntaxNode> arguments;
		constexpr Node() noexcept = default;
		constexpr Node(const Node& other) noexcept = default;
		Node(Node&& other) noexcept = default;
		constexpr ~Node() = default;
		constexpr Node& operator=(const Node& other) noexcept = default;
		Node& operator=(Node&& other) noexcept = default;
		constexpr Node(std::string name) noexcept : name(name) {}
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
}

namespace std
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	extern template struct Node<NodeType::ConstantCall>;

	extern template class std::unique_ptr<Node<NodeType::ConstantCall>>;

	extern template std::unique_ptr<Node<NodeType::ConstantCall>> std::make_unique<
			Node<NodeType::ConstantCall>
		>(decltype(std::declval<std::string>()));
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__CALL__HPP

