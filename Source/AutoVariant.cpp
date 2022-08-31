#include <Warp/SyntaxAnalysis/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis/MathematicalExpressions.hpp>
namespace Warp::Utilities
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	template struct Warp::Utilities::AutoVariant<
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
}

