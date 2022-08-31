#include <Warp/SyntaxAnalysis/MathematicalExpressions.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	extern template struct Node<NodeType::Multiply>;
	extern template struct Node<NodeType::Divide>;
	extern template struct Node<NodeType::Add>;
	extern template struct Node<NodeType::Subtract>;
	extern template struct Node<NodeType::Negation>;
	extern template struct Node<NodeType::Expression>;

	//void auto_variant_delete(Node<NodeType::Multiply>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::Divide>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::Add>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::Subtract>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::Negation>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::Expression>* to_delete) {
	//	delete to_delete;
	//}
	
	#define DELETER_FUNCTION(NODE_TYPE, FUNCTION_SUFFIX) \
		void auto_variant_delete_##FUNCTION_SUFFIX (void* to_delete) { \
			if(to_delete != nullptr) \
				delete static_cast<Node<NodeType :: NODE_TYPE >*>(to_delete); \
		}

	DELETER_FUNCTION(Multiply, multiply)
	DELETER_FUNCTION(Divide, divide)
	DELETER_FUNCTION(Add, add)
	DELETER_FUNCTION(Subtract, subtract)
	DELETER_FUNCTION(Negation, negation)
	DELETER_FUNCTION(Expression, expression)
}

