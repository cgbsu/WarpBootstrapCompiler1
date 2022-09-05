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
		constexpr Node& operator=(const Node& other) noexcept = default;
		Node& operator=(Node&& other) noexcept = default;
		constexpr Node(std::string name) noexcept : name(name) {}
		constexpr virtual NodeType get_tag() const noexcept final {
			return tag;
		}
		operator SyntaxNode() const noexcept {
			return SyntaxNode{*this};
		}
	};

	extern template struct Node<NodeType::ConstantCall>;
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__CALL__HPP

