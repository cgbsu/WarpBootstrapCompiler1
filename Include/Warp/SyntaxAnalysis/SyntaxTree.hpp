#include <Warp/Common.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>
#include <Warp/Utilities/AutoVariant.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{

	using namespace Warp::Runtime::Compiler;

	enum class NodeType
	{
		Multiply, 
		Divide, 
		Add, 
		Subtract, 
		Negation, 
		LiteralWhole, 
		LiteralInteger, 
		LiteralCharacter, 
		LiteralFixed, 
		LiteralBool, 
		Expression
	};

	template<NodeType> struct Node {};

	struct BaseNode {
		constexpr virtual NodeType get_tag() const noexcept = 0;
	};

	template<> struct Node<NodeType::Multiply>;
	template<> struct Node<NodeType::Divide>;
	template<> struct Node<NodeType::Add>;
	template<> struct Node<NodeType::Subtract>;
	template<> struct Node<NodeType::Negation>;

	template<> struct Node<NodeType::LiteralWhole>;
	template<> struct Node<NodeType::LiteralInteger>;
	template<> struct Node<NodeType::LiteralCharacter>;
	template<> struct Node<NodeType::LiteralFixed>;
	template<> struct Node<NodeType::LiteralBool>;

	template<> struct Node<NodeType::Expression>;

	//using SyntaxNode = std::variant<
	//using SyntaxNode = AutoVariant<
	//		Node<NodeType::Multiply>, 
	//		Node<NodeType::Divide>, 
	//		Node<NodeType::Add>, 
	//		Node<NodeType::Subtract>, 
	//		Node<NodeType::Negation>, 
	//		Node<NodeType::Expression>, 
	//		Node<NodeType::LiteralWhole>, 
	//		Node<NodeType::LiteralInteger>, 
	//		Node<NodeType::LiteralCharacter>, 
	//		Node<NodeType::LiteralFixed>, 
	//		Node<NodeType::LiteralBool>
	//	>;
	//struct SyntaxNode;
	using SyntaxNodeVariant = AutoVariant<
			Node<NodeType::Multiply>, 
			Node<NodeType::Divide>, 
			Node<NodeType::Add>, 
			Node<NodeType::Subtract>, 
			Node<NodeType::Negation>, 
			Node<NodeType::Expression>, 
			Node<NodeType::LiteralWhole>, 
			Node<NodeType::LiteralInteger>, 
			Node<NodeType::LiteralCharacter>, 
			Node<NodeType::LiteralFixed>, 
			Node<NodeType::LiteralBool>
		>;

	struct SyntaxNode
	{
		SyntaxNodeVariant data;
		constexpr SyntaxNode(SyntaxNodeVariant data) noexcept : data(data) {}
		constexpr SyntaxNode() noexcept = default;
		constexpr SyntaxNode(const SyntaxNode& other) noexcept = default;
		constexpr SyntaxNode(SyntaxNode&& other) noexcept = default;

		constexpr SyntaxNode& operator=(const SyntaxNode& other) noexcept = default;
		constexpr SyntaxNode& operator=(SyntaxNode&& other) noexcept = default;

		constexpr operator const SyntaxNodeVariant&() const {
			return data;
		}
		constexpr operator SyntaxNodeVariant&() {
			return data;
		}
	};
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP

