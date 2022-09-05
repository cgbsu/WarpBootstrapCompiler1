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

	template<> struct Node<NodeType::ConstantCall>;
}
namespace Warp::Utilities
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	extern template struct Warp::Utilities::AutoVariant<
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
			Node<NodeType::LiteralBool>, 
			Node<NodeType::ConstantCall>
		>;
}

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	using namespace Warp::Runtime::Compiler;
	using namespace Warp::Utilities;

	using InternalSyntaxNodeVariant = AutoVariant<
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
			Node<NodeType::LiteralBool>, 
			Node<NodeType::ConstantCall>
		>;

	using SyntaxNodeVariant = NotSoUniquePointer<InternalSyntaxNodeVariant>;

	extern void auto_variant_delete_literal_whole(void* to_delete);
	extern void auto_variant_delete_literal_integer(void* to_delete);
	extern void auto_variant_delete_literal_character(void* to_delete);
	extern void auto_variant_delete_literal_fixed(void* to_delete);
	extern void auto_variant_delete_literal_bool(void* to_delete);

	extern void auto_variant_delete_multiply(void* to_delete);
	extern void auto_variant_delete_divide(void* to_delete);
	extern void auto_variant_delete_add(void* to_delete);
	extern void auto_variant_delete_subtract(void* to_delete);
	extern void auto_variant_delete_negation(void* to_delete);
	extern void auto_variant_delete_expression(void* to_delete);

	extern void auto_variant_delete_constant_call(void* to_delete);

	template<NodeType NodeTypeParameterConstant>
	struct FindDeleter {};
	
	#define DEFINE_FIND_DELETER(NODE_TYPE, FUNCTION_SUFFIX) \
			template<> \
			struct FindDeleter<NodeType :: NODE_TYPE > { \
				constexpr static const auto tag = NodeType :: NODE_TYPE ; \
				constexpr static const auto deleter = auto_variant_delete_##FUNCTION_SUFFIX ; \
			}

	DEFINE_FIND_DELETER(LiteralWhole, literal_whole);
	DEFINE_FIND_DELETER(LiteralInteger, literal_integer);
	DEFINE_FIND_DELETER(LiteralCharacter, literal_character);
	DEFINE_FIND_DELETER(LiteralFixed, literal_fixed);
	DEFINE_FIND_DELETER(LiteralBool, literal_bool);

	DEFINE_FIND_DELETER(Multiply, multiply);
	DEFINE_FIND_DELETER(Divide, divide);
	DEFINE_FIND_DELETER(Add, add);
	DEFINE_FIND_DELETER(Subtract, subtract);
	DEFINE_FIND_DELETER(Negation, negation);
	DEFINE_FIND_DELETER(Expression, expression);

	DEFINE_FIND_DELETER(ConstantCall, constant_call);

	#undef DEFINE_FIND_DELETER

	template<NodeType NodeTypeParameterConstant>
	SyntaxNodeVariant allocate_node(auto... initializers)
	{
		using AlternativeParameterType = Node<NodeTypeParameterConstant>;
		return NotSoUniquePointer{
				std::in_place_type_t<InternalSyntaxNodeVariant>{}, 
				//FindDeleter<NodeTypeParameterConstant>::deleter, 
				std::in_place_type_t<AlternativeParameterType>{}, 
				initializers...
			};
	}

	struct SyntaxNode
	{
		SyntaxNodeVariant data;
		template<NodeType NodeTypeParameterConstant>
		constexpr SyntaxNode(Node<NodeTypeParameterConstant> data) noexcept 
				: data(allocate_node<decltype(data)::tag>(data)) {}
		SyntaxNode(SyntaxNodeVariant data) noexcept : data(data) {}
		constexpr SyntaxNode() noexcept = default;
		SyntaxNode(const SyntaxNode& other) noexcept = default;
		SyntaxNode(SyntaxNode&& other) noexcept = default;

		constexpr SyntaxNode& operator=(const SyntaxNode& other) noexcept = default;
		SyntaxNode& operator=(SyntaxNode&& other) noexcept = default;

		operator const InternalSyntaxNodeVariant&() const {
			return *data.get_pointer();
		}
	};
}

#endif // WARP__PARSING__HEADER__SYNTAX__ANALYSIS__SYNTAX__TREE__HPP

