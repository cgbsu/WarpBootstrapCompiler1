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
	static auto retrieve_value(const SyntaxNode& node) -> ReduceToParameterType
	{
		return visit<
				[](auto node)
				{ 
					return Executor<
							ReduceToParameterType, 
							CleanType<decltype(node)>::tag
						>(*node).to_value(); 
				}
			>(*node.data.get_pointer());
	}

	#define LITERAL_NODE(LITERAL_TAG) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType:: LITERAL_TAG > \
			{ \
				using ReduceToType = ReduceToParameterType; \
				using NumericType = Node<NodeType:: LITERAL_TAG >::NumericType; \
				NumericType value; \
				Executor(const Node<NodeType:: LITERAL_TAG >& node) : value(node.value) {} \
				ReduceToType to_value() \
				{ \
					if constexpr(std::is_same_v<ReduceToType, NumericType> == true) \
						return value; \
					else \
						return decltype(Zero{std::declval<ReduceToType>()})::zero; \
				} \
				operator ReduceToType() { \
					return to_value(); \
				} \
			}
	
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
				ReduceToType value; \
				Executor(const Node<NodeType:: NODE_TYPE >& node) \
						: value( \
								retrieve_value<ReduceToParameterType>(node.left) \
										OPERATOR retrieve_value<ReduceToParameterType>(node.right) \
								) {} \
				ReduceToType to_value() { \
					return value; \
				} \
				operator ReduceToType() { \
					return to_value(); \
				} \
			}

	BINARY_OPERATOR(Multiply, *);
	BINARY_OPERATOR(Divide, /);
	BINARY_OPERATOR(Add, +);
	BINARY_OPERATOR(Subtract, -);

	#undef BINARY_OPERATOR

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::Negation>
	{
		using ReduceToType = ReduceToParameterType;
		ReduceToType value;
		Executor(const Node<NodeType::Negation>& node) : value(-retrieve_value<ReduceToType>(node)) {}
		ReduceToType to_value() {
			return value;
		}
		operator ReduceToType() {
			return to_value();
		}
	};

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::Expression>
	{
		using ReduceToType = ReduceToParameterType;
		ReduceToType value;
		Executor(const Node<NodeType::Expression>& node) : value(retrieve_value<ReduceToType>(node)) {}
		ReduceToType to_value() {
			return value;
		}
		operator ReduceToType() {
			return to_value();
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

