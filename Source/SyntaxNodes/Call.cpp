#include <Warp/SyntaxAnalysis/Call.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	extern template struct Node<NodeType::ConstantCall>;

	#define DELETER_FUNCTION(NODE_TYPE, FUNCTION_SUFFIX) \
		void auto_variant_delete_##FUNCTION_SUFFIX (void* to_delete) { \
			if(to_delete != nullptr) \
				delete static_cast<Node<NodeType :: NODE_TYPE >*>(to_delete); \
		}
	
	DELETER_FUNCTION(ConstantCall, constant_call)

	SyntaxNode constant_call(std::string name) {
		return SyntaxNode(Node<NodeType::ConstantCall>{name});
	}
}

