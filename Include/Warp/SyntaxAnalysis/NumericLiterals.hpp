#include <Warp/SyntaxAnalysis/SyntaxNode.hpp>
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
				using NumericType =	typename NumericTypeResolver<NumericTypeTag:: SEARCH_TYPE >::Type; \
				NumericType value; \
				constexpr Node() noexcept = default; \
				constexpr Node(const Node& other) noexcept = default; \
				constexpr Node(Node&& other) noexcept = default; \
				constexpr Node& operator=(const Node& other) noexcept = default; \
				constexpr Node& operator=(Node&& other) noexcept = default; \
				constexpr Node(NumericType value) noexcept : value(value) {} \
				constexpr virtual NodeType get_tag() const noexcept { \
					return tag; \
				} \
				operator NumericType() { \
					return value; \
				} \
			}

	LITERAL_NODE(LiteralWhole, Whole);
	LITERAL_NODE(LiteralInteger, Integer);
	LITERAL_NODE(LiteralFixed, FixedPoint);
	LITERAL_NODE(LiteralCharacter, Character);
	LITERAL_NODE(LiteralBool, Bool);
	
	#undef LITERAL_NODE

	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Whole>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Integer>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::FixedPoint>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Character>::Type value);
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Bool>::Type value);
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__NUMERIC__LITERALS__HPP

