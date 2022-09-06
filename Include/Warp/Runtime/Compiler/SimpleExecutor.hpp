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
	static auto retrieve_value(const auto& context, const BaseNode* node, bool debug) 
			-> std::optional<ReduceToParameterType>
	{
		return visit<
				ReduceToParameterType, 
				[](auto node, const auto& context, bool debug)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					if(debug == true)
						std::cout << "Tag: " << to_string(tag) << "\n";
					return Executor<
							ReduceToParameterType, 
							tag
						>(debug, context, node).to_value(); 
				}
			>(node->to_view(), context, debug);
	}

	//template<typename ReduceToParameterType>
	//static auto retrieve_value(const auto& context, const SyntaxNode& node) 
	//		-> std::optional<ReduceToParameterType> {
	//	return retrieve_value(context, node.get());
	//}

	template<typename ReduceToParameterType>
	static auto retrieve_value(bool debug, const BaseNode* node) -> std::optional<ReduceToParameterType>
	{
		return visit<
				ReduceToParameterType, 
				[](auto node, bool debug)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					if constexpr(tag != NodeType::ConstantCall)
					{
						if(debug == true)
							std::cout << "Tag: " << to_string(tag) << "\n";
						return Executor<
								ReduceToParameterType, 
								tag
							>(debug, static_cast<std::remove_reference_t<decltype(node)>>(debug, node)).to_value(); 
					}
					else
						return std::nullopt;
				}
			>(node->to_view(), debug);
	}

	//template<typename ReduceToParameterType>
	//static auto retrieve_value(const SyntaxNode& node) -> std::optional<ReduceToParameterType> {
	//	return retrieve_value<ReduceToParameterType>(debug, node.get());
	//}

	#define LITERAL_NODE(LITERAL_TAG) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: LITERAL_TAG > \
			{ \
				constexpr static const auto tag = NodeType :: LITERAL_TAG ; \
				using ReduceToType = ReduceToParameterType; \
				using NumericType = Node<NodeType:: LITERAL_TAG >::NumericType; \
				NumericType value; \
				Executor(bool debug, const Node<NodeType :: LITERAL_TAG>* node) : value(node->value) { \
					if(debug == true) \
						std::cout << to_string(tag) << ":" << to_string(value) << "\n"; \
				} \
				Executor(bool debug, const auto& context, const Node<NodeType :: LITERAL_TAG>* node) : value(node->value) { \
					if(debug == true) \
						std::cout << to_string(tag) << ":" << to_string(value) << "\n"; \
				} \
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
				std::string debug_print_value(std::optional<ReduceToType>& value) { \
						return std::string{"{HasValue?:"} + std::to_string(value.has_value()) + std::string{"}"}; \
				} \
				Executor(bool debug, const Node<NodeType:: NODE_TYPE >* node) \
						: left_value(retrieve_value<ReduceToParameterType>(debug, node->left.get())), \
						right_value(retrieve_value<ReduceToParameterType>(debug, node->right.get())), \
						value( \
								(left_value.has_value() == true && right_value.has_value() == true) \
								? std::optional{left_value.value() OPERATOR right_value.value()} \
								: std::nullopt \
							) \
				{ \
					if(debug == true) \
					{ \
						std::cout << "Operator:" << #OPERATOR << "\n" \
								<< "Left: " << debug_print_value(left_value) << "\n" \
								<< "Right: " << debug_print_value(right_value) << "\n" \
								<< "Value: " << debug_print_value(value) << "\n"; \
					} \
				} \
				Executor(bool debug, const auto& context, const Node<NodeType:: NODE_TYPE>* node) \
						: left_value(retrieve_value<ReduceToParameterType>(debug, context, node->left.get())), \
						right_value(retrieve_value<ReduceToParameterType>(debug, context, node->right.get())), \
						value( \
								(left_value.has_value() == true && right_value.has_value() == true) \
								? std::optional{left_value.value() OPERATOR right_value.value()} \
								: std::nullopt \
							) \
				{ \
					if(debug == true) \
					{ \
						std::cout << "Operator:" << #OPERATOR << "\n" \
								<< "Left: " << debug_print_value(std::cout, left_value) << "\n" \
								<< "Right: " << debug_print_value(std::cout, right_value) << "\n" \
								<< "Value: " << debug_print_value(std::cout, value) << "\n"; \
					} \
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
				//				retrieve_value<ReduceToParameterType>(debug, context, &node.left).value() \
				//						OPERATOR retrieve_value<ReduceToParameterType>(debug, context, &node.right).value() \
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
		std::optional<ReduceToType> absolute_value, value;
		Executor(bool debug, const Node<NodeType::Negation>* node) 
				: absolute_value(retrieve_value<ReduceToType>(debug, node->negated.get())), 
				value(
						(absolute_value.has_value() == true)
								? std::optional{-absolute_value.value()}
								: std::nullopt
					) {
				if(debug == true)
					std::cout << "Negation, has value? " << value.has_value() << "\n";
			}
		Executor(bool debug, const auto& context, const Node<NodeType::Negation>* node) 
				: absolute_value(retrieve_value<ReduceToType>(debug, context, node->negated.get())), 
				value(
						(absolute_value.has_value() == true)
								? std::optional{-absolute_value.value()}
								: std::nullopt
					) {
				if(debug == true)
					std::cout << "Negation, has value? " << absolute_value.has_value() << "\n";
			}
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
		Executor(bool debug, const Node<NodeType::Expression>* node) 
				: value(retrieve_value<ReduceToType>(debug, node->root.get())) {
			if(debug == true)
				std::cout << "Expression, has value? " << value.has_value() << "\n";
		}
		Executor(bool debug, const auto& context, const Node<NodeType::Expression>* node) 
				: value(retrieve_value<ReduceToType>(debug, context, node->root.get())) {
			if(debug == true)
				std::cout << "Expression, has value? " << value.has_value() << "\n";
		}
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
		Executor(bool debug, const Node<NodeType::ConstantCall>* node) = delete;
		Executor(bool debug, const auto& context, const Node<NodeType::ConstantCall>* node)
				: value(retrieve_value<ReduceToType>(debug, context, context.constants.at(node->name).value.get())) {
			if(debug == true)
				std::cout << "Constant Call for " << node->name << " , has value? " << value.has_value() << "\n";
		}
		std::optional<ReduceToType> to_value() {
			return value;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

