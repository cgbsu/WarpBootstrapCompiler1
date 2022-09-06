//#include <Warp/SyntaxAnalysis/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis/SyntaxNode.hpp>
#include <Warp/Utilities.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	using namespace Warp::Runtime::Compiler;

	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Whole>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralWhole>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Integer>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralInteger>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::FixedPoint>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralFixed>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Character>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralCharacter>>(value));
	}
	SyntaxNode literal_node(NumericTypeResolver<NumericTypeTag::Bool>::Type value) {
		return SyntaxNode(std::make_unique<Node<NodeType::LiteralBool>>(value));
	}

	template struct Node<NodeType::LiteralWhole>;
	template struct Node<NodeType::LiteralInteger>;
	template struct Node<NodeType::LiteralCharacter>;
	template struct Node<NodeType::LiteralFixed>;
	template struct Node<NodeType::LiteralBool>;

	//void auto_variant_delete(Node<NodeType::LiteralWhole>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::LiteralInteger>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::LiteralCharacter>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::LiteralFixed>* to_delete) {
	//	delete to_delete;
	//}
	//void auto_variant_delete(Node<NodeType::LiteralBool>* to_delete) {
	//	delete to_delete;
	//}

	#define DELETER_FUNCTION(NODE_TYPE, FUNCTION_SUFFIX) \
		void auto_variant_delete_##FUNCTION_SUFFIX (void* to_delete) { \
			if(to_delete != nullptr) \
				delete static_cast<Node<NodeType :: NODE_TYPE >*>(to_delete); \
		}

	DELETER_FUNCTION(LiteralWhole, literal_whole)
	DELETER_FUNCTION(LiteralInteger, literal_integer)
	DELETER_FUNCTION(LiteralCharacter, literal_character)
	DELETER_FUNCTION(LiteralFixed, literal_fixed)
	DELETER_FUNCTION(LiteralBool, literal_bool)

	#undef DELETER_FUNCTION

}

