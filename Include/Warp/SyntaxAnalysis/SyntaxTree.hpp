#include <Warp/Common.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>
#include <Warp/Utilities/AutoVariant.hpp>

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
			case Multiply: {
				return "Multiply";
			}
			case Divide: {
				return "Divide";
			}
			case Add: {
				return "Add";
			}
			case Subtract: {
				return "Subtract";
			}
			case Negation: {
				return "Negation";
			}
			case LiteralWhole: {
				return "LiteralWhole";
			}
			case LiteralInteger: {
				return "LiteralInteger";
			}
			case LiteralCharacter: {
				return "LiteralCharacter";
			}
			case LiteralFixed: {
				return "LiteralFixed";
			}
			case LiteralBool: {
				return "LiteralBool";
			}
			case Expression: {
				return "Expression";
			}
			case ConstantCall: {
				return "ConstantCa";
			}
			default: 
				return "ERROR_NO_NODE_WITH_THIS_TAG!!!";
		}
	}
	
	template<NodeType> struct Node {};

	struct BaseNode {
		constexpr virtual NodeType get_tag() const noexcept = 0;
	};

	using SyntaxNode = std::unique_ptr<BaseNode>;
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP

