#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__NUMERIC__LITERALS__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__NUMERIC__LITERALS__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	using namespace Warp::Runtime::Compiler;

	#define LITERAL_NODE(NODE_TYPE, SEARCH_TYPE) \
			template<> \
			struct Node<NodeType:: NODE_TYPE > : public BaseNode \
			{ \
				constexpr static const auto tag = NodeType:: NODE_TYPE ; \
				using NumericType =	typename NumericTypeResolver<NumericTypeTag :: SEARCH_TYPE>::Type; \
				NumericType value; \
				constexpr Node() noexcept = default; \
				constexpr Node(const Node& other) noexcept = default; \
				Node(Node&& other) noexcept = default; \
				constexpr Node& operator=(const Node& other) noexcept = default; \
				Node& operator=(Node&& other) noexcept = default; \
				constexpr Node(NumericType value) noexcept : value(value) {} \
				constexpr virtual BaseNode::ViewType to_view() const noexcept final { \
					return BaseNode::ViewType{this}; \
				} \
				constexpr virtual NodeType get_tag() const noexcept { \
					return tag; \
				} \
				operator NumericType() { \
					return value; \
				} \
				operator SyntaxNode() const noexcept { \
					return SyntaxNode{*this}; \
				} \
			}

	LITERAL_NODE(LiteralWhole, Whole);
	LITERAL_NODE(LiteralInteger, Integer);
	LITERAL_NODE(LiteralFixed, FixedPoint);
	LITERAL_NODE(LiteralCharacter, Character);
	LITERAL_NODE(LiteralBool, Bool);
	
	#undef LITERAL_NODE

	extern template struct Node<NodeType::LiteralWhole>;
	extern template struct Node<NodeType::LiteralInteger>;
	extern template struct Node<NodeType::LiteralCharacter>;
	extern template struct Node<NodeType::LiteralFixed>;
	extern template struct Node<NodeType::LiteralBool>;
}

namespace std
{
	using namespace Warp::Runtime::Compiler;
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	extern template class std::unique_ptr<Node<NodeType::LiteralWhole>>;
	extern template class std::unique_ptr<Node<NodeType::LiteralInteger>>;
	extern template class std::unique_ptr<Node<NodeType::LiteralCharacter>>;
	extern template class std::unique_ptr<Node<NodeType::LiteralFixed>>;
	extern template class std::unique_ptr<Node<NodeType::LiteralBool>>;

	extern template std::unique_ptr<Node<NodeType::LiteralWhole>> std::make_unique<
			Node<NodeType::LiteralWhole>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Whole>::Type>()));

	extern template std::unique_ptr<Node<NodeType::LiteralInteger>> std::make_unique<
			Node<NodeType::LiteralInteger>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Integer>::Type>()));

	extern template std::unique_ptr<Node<NodeType::LiteralCharacter>> std::make_unique<
			Node<NodeType::LiteralCharacter>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Character>::Type>()));

	extern template std::unique_ptr<Node<NodeType::LiteralFixed>> std::make_unique<
			Node<NodeType::LiteralFixed>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::FixedPoint>::Type>()));

	extern template std::unique_ptr<Node<NodeType::LiteralBool>> std::make_unique<
			Node<NodeType::LiteralBool>
		>(decltype(std::declval<NumericTypeResolver<NumericTypeTag::Bool>::Type>()));
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__NUMERIC__LITERALS__HPP

