#include <Warp/Common.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP
#define WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	using namespace Warp::Runtime::Compiler;
	using namespace Warp::Utilities;

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
		Expression, 
		ConstantCall
	};

	static std::string to_string(NodeType node_type)
	{
		using enum NodeType;
		switch(node_type)
		{
			case Multiply : {
				return "NodeType::Multiply";
			}
			case Divide : {
				return "NodeType::Divide";
			}
			case Add : {
				return "NodeType::Add";
			}
			case Subtract : {
				return "NodeType::Subtract";
			}
			case Negation : {
				return "NodeType::Negation";
			}
			case LiteralWhole : {
				return "NodeType::LiteralWhole";
			}
			case LiteralInteger : {
				return "NodeType::LiteralInteger";
			}
			case LiteralCharacter : {
				return "NodeType::LiteralCharacter";
			}
			case LiteralFixed : {
				return "NodeType::LiteralFixed";
			}
			case LiteralBool : {
				return "NodeType::LiteralBool";
			}
			case Expression : {
				return "NodeType::Expression";
			}
			case ConstantCall : {
				return "NodeType::ConstantCa";
			}
			default : {
				return "ERROR_NO_NODE_WITH_THIS_TAG!!!";
			}
		}
	}
	
	template<NodeType> struct Node {};

	struct BaseNode
	{
		using ViewType = VariantView<
				BaseNode, 
				Node, 
				NodeType::Multiply, 
				NodeType::Divide, 
				NodeType::Add, 
				NodeType::Subtract, 
				NodeType::Negation, 
				NodeType::LiteralWhole, 
				NodeType::LiteralInteger, 
				NodeType::LiteralCharacter, 
				NodeType::LiteralFixed, 
				NodeType::LiteralBool, 
				NodeType::Expression, 
				NodeType::ConstantCall
			>;
		constexpr virtual ViewType to_view() const noexcept = 0;
		constexpr virtual NodeType get_tag() const noexcept = 0;
		virtual ~BaseNode() = default;
	};

	using SyntaxNode = std::unique_ptr<BaseNode>;
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP

