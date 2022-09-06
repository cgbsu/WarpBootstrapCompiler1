#include <Warp/SyntaxAnalysis/Call.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	SyntaxNode constant_call(std::string name) {
		return SyntaxNode(std::make_unique<Node<NodeType::ConstantCall>>(name));
	}
}

