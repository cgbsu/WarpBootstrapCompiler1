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
	static auto retrieve_value(const auto& context, const SyntaxNode node) 
			-> std::optional<ReduceToParameterType>
	{
		const auto* pointer = node.data.get_pointer();
		std::cout << "Got pointer, is null?: " << (pointer == nullptr) << "\n";
		return visit<
				[](auto node, const auto& context)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					std::cout << "Tag: " << to_string(tag) << "\n";
					return Executor<
							ReduceToParameterType, 
							tag
						>(context, *node).to_value(); 
				}
			>(*pointer, context);
	}

	template<typename ReduceToParameterType>
	static auto retrieve_value(const SyntaxNode& node) -> std::optional<ReduceToParameterType>
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
			>(*node.data.get_pointer());
	}

	#define LITERAL_NODE(LITERAL_TAG) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: LITERAL_TAG > \
			{ \
				constexpr static const auto tag = NodeType :: LITERAL_TAG ; \
				using ReduceToType = ReduceToParameterType; \
				using NumericType = Node<NodeType:: LITERAL_TAG >::NumericType; \
				NumericType value; \
				Executor(const Node<NodeType:: LITERAL_TAG >& node) : value(node.value) { std::cout << "NO CONTEXT CONSTRUCTOR\n"; } \
				Executor(const auto& context, const Node<NodeType:: LITERAL_TAG >& node) : value(node.value) {std::cout << "Executing constructor.\n";} \
				std::optional<ReduceToType> to_value() \
				{ \
					if constexpr(std::is_same_v<ReduceToType, NumericType> == true) \
					{ \
						std::cout << "Same value confirmed value: "; \
						if constexpr(tag == NodeType::LiteralFixed || tag == NodeType::LiteralBool) \
							std::cout << "FixedOrBool\n"; \
						else { \
							if constexpr(std::is_same_v<CleanType<decltype(value.number)>, WarpBool> == false) \
								std::cout << to_string(value.number) << "\n"; \
						} \
						return value; \
					} \
					else {\
						std::cout << "Not the same confirmed\n"; \
						return std::nullopt; \
					} \
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
				std::optional<ReduceToType> value; \
				Executor(const Node<NodeType:: NODE_TYPE >& node) \
						: value( \
								retrieve_value<ReduceToParameterType>(node.left).value() \
										OPERATOR retrieve_value<ReduceToParameterType>(node.right).value() \
							) {} \
				Executor(const auto& context, const Node<NodeType:: NODE_TYPE >& node) \
				{ \
					std::cout << "Retrieveing left...\n"; \
					const auto left = retrieve_value<ReduceToParameterType>(context, node.left); \
					std::cout << "Left success getting value...\n"; \
					const auto left_ = left.value(); \
					std::cout << "Success getting left value.\n"; \
					std::cout << "Retrieveing right...\n"; \
					const auto right = retrieve_value<ReduceToParameterType>(context, node.right); \
					std::cout << "Right success getting value...\n"; \
					const auto right_ = right.value(); \
					std::cout << "Success getting rigt value.\n"; \
					value = left_ OPERATOR right_; \
					std::cout << "Value computed...\n"; \
				} \
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
		Executor(const Node<NodeType::Negation>& node) 
				: value(-retrieve_value<ReduceToType>(node.negated).value()) {}
		Executor(const auto& context, const Node<NodeType::Negation>& node) 
				: value(-retrieve_value<ReduceToType>(context, node.negated).value()) {}
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
		Executor(const Node<NodeType::Expression>& node) 
				: value(retrieve_value<ReduceToType>(node.root)) {}
		Executor(const auto& context, const Node<NodeType::Expression>& node) 
				: value(retrieve_value<ReduceToType>(context, node.root)) {}
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
		Executor(const Node<NodeType::ConstantCall>& node) = delete;
		Executor(const auto& context, const Node<NodeType::ConstantCall>& node) 
				: value(retrieve_value<ReduceToType>(context, context.constants.at(node.name).value)) {}
		std::optional<ReduceToType> to_value() {
			return value;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

