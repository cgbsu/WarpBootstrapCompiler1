#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

namespace Warp::Runtime::Compiler::SimpleExecutor
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;

	template<typename ReduceToParameterType, NodeType>
	struct Executor {};

	template<typename ReduceToParameterType>
	static auto retrieve_value(const auto& context, const BaseNode* node) 
			-> std::optional<ReduceToParameterType>
	{
		return visit<
				ReduceToParameterType, 
				[](auto node, const auto& context)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					std::cout << "Tag: " << to_string(tag) << "\n";
					return Executor<
							ReduceToParameterType, 
							tag
						>(context, *node).to_value(); 
				}
			>(node->to_view(), context);
	}

	template<typename ReduceToParameterType>
	static auto retrieve_value(const BaseNode* node) -> std::optional<ReduceToParameterType>
	{
		return visit<
				[](auto node)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					if constexpr(tag != NodeType::ConstantCall)
					{
						return Executor<
								ReduceToParameterType, 
								tag
							>(*node).to_value(); 
					}
					else
						return std::nullopt;
				}
			>(node->to_view);
	}

	#define LITERAL_NODE(LITERAL_TAG) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: LITERAL_TAG > \
			{ \
				constexpr static const auto tag = NodeType :: LITERAL_TAG ; \
				using ReduceToType = ReduceToParameterType; \
				using NumericType = Node<NodeType:: LITERAL_TAG >::NumericType; \
				NumericType value; \
				Executor(const Node<NodeType :: LITERAL_TAG>* node) : value(node.value) { \
				Executor(const auto& context, const Node<NodeType :: LITERAL_TAG>* node) : value(node->value) { \
				std::optional<ReduceToType> to_value() \
				{ \
					if constexpr(std::is_same_v<ReduceToType, NumericType> == true) \
						return value; \
					else \
						return std::nullopt; \
				} \
				operator std::optional<ReduceToType>() { \
					return to_value(); \
				} \
			}
	
						//return decltype(Zero{std::declval<ReduceToType>()})::zero;
	LITERAL_NODE(LiteralWhole);
	LITERAL_NODE(LiteralInteger);
	LITERAL_NODE(LiteralCharacter);
	LITERAL_NODE(LiteralFixed);
	LITERAL_NODE(LiteralBool);

	#undef LITERAL_NODE

	#define BINARY_OPERATOR(NODE_TYPE, OPERATOR) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType:: NODE_TYPE > \
			{ \
				using ReduceToType = ReduceToParameterType; \
				std::optional<ReduceToType> left_value, right_value, value; \
				Executor(const Node<NodeType:: NODE_TYPE >& node) \
						: left_value(retrieve_value<ReduceToParameterType>(node.left)), \
						right_value(retrieve_value<ReduceToParameterType>(node.right)), \
						value( \
								(left_value.has_value() == true && right_value.has_value() == true) \
								? left_value.value() * right_value.value() \
								: std::nullopt \
							) {} \
				Executor(const auto& context, const Node<NodeType:: NODE_TYPE >& node) \
						: left_value(retrieve_value<ReduceToParameterType>(context, node.left.get())), \
						right_value(retrieve_value<ReduceToParameterType>(context, node.right.get())), \
						value( \
								(left_value.has_value() == true && right_value.has_value() == true) \
								? left_value.value() * right_value.value() \
								: std::nullopt \
							) {} \
				std::optional<ReduceToType> to_value() { \
					return value; \
				} \
				operator std::optional<ReduceToType>() { \
					return to_value(); \
				} \
			}

				//Executor(const auto& context, const Node<NodeType:: NODE_TYPE >& node) \
				//		: value( \
				//				retrieve_value<ReduceToParameterType>(context, &node.left).value() \
				//						OPERATOR retrieve_value<ReduceToParameterType>(context, &node.right).value() \
				//			) {} \
				//			
	BINARY_OPERATOR(Multiply, *);
	BINARY_OPERATOR(Divide, /);
	BINARY_OPERATOR(Add, +);
	BINARY_OPERATOR(Subtract, -);

	#undef BINARY_OPERATOR

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::Negation>
	{
		using ReduceToType = ReduceToParameterType;
		std::optional<ReduceToType> value;
		Executor(const Node<NodeType::Negation>* node) 
				: value(-retrieve_value<ReduceToType>(node.negated.get()).to_value()) {}
		Executor(const auto& context, const Node<NodeType::Negation>* node) 
				: value(-retrieve_value<ReduceToType>(context, node.negated.get()).to_value()) {}
		std::optional<ReduceToType> to_value() {
			return value;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::Expression>
	{
		using ReduceToType = ReduceToParameterType;
		std::optional<ReduceToType> value;
		Executor(const Node<NodeType::Expression>* node) 
				: value(retrieve_value<ReduceToType>(node.root.get())) {}
		Executor(const auto& context, const Node<NodeType::Expression>* node) 
				: value(retrieve_value<ReduceToType>(context, node.root.get())) {}
		std::optional<ReduceToType> to_value() {
			return value;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::ConstantCall>
	{
		using ReduceToType = ReduceToParameterType;
		std::optional<ReduceToType> value;
		Executor(const Node<NodeType::ConstantCall>* node) = delete;
		Executor(const auto& context, const Node<NodeType::ConstantCall>* node) 
				: value(retrieve_value<ReduceToType>(context, context.constants.at(node.name).value.get())) {}
		std::optional<ReduceToType> to_value() {
			return value;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

