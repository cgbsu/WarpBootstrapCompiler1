#include <Warp/SyntaxAnalysis/Call.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	SyntaxNode constant_call(std::string name) {
		return SyntaxNode(std::make_unique<Node<NodeType::ConstantCall>>(name));
	}
}

namespace std
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	template class std::unique_ptr<Node<NodeType::ConstantCall>>;

	template std::unique_ptr<Node<NodeType::ConstantCall>> std::make_unique<
			Node<NodeType::ConstantCall>
		>(decltype(std::declval<std::string>()));

}

