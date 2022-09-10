#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

namespace Warp::Runtime::Compiler::SimpleExecutor
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;
	using namespace Warp::Utilities;
	using namespace Warp::Runtime::Compiler;

	template<typename ReduceToParameterType, NodeType>
	struct Executor {};

	template<typename ReduceToParameterType>
	static auto retrieve_value(const auto* context, const BaseNode* node, bool debug) 
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
						>(context, node, debug).to_value(); 
				}
			>(node->to_view(), context, debug);
	}

	template<typename ReduceToParameterType>
	static auto retrieve_value(const BaseNode* node, bool debug) -> std::optional<ReduceToParameterType>
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
							>(node, debug).to_value(); 
					}
					else
						return std::nullopt;
				}
			>(node->to_view(), debug);
	}

	#define LITERAL_NODE(LITERAL_TAG) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: LITERAL_TAG > \
			{ \
				constexpr static const auto tag = NodeType :: LITERAL_TAG ; \
				using ReduceToType = ReduceToParameterType; \
				using NumericType = Node<NodeType:: LITERAL_TAG >::NumericType; \
				NumericType value; \
				Executor(const Node<NodeType :: LITERAL_TAG>* node, bool debug) : value(node->value) { \
					if(debug == true) \
						std::cout << to_string(tag) << ":" << to_string(value) << "\n"; \
				} \
				Executor(const auto* context, const Node<NodeType :: LITERAL_TAG>* node, bool debug) : value(node->value) { \
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

	#define BINARY_OPERATOR_DEFAULT_NO_CONTEXT_CONSTRUCT(NODE_TYPE, OPERATOR) \
				Executor(const Node<NodeType :: NODE_TYPE >* node, bool debug) \
						: left_value(retrieve_value<ReduceToType>(node->left.get(), debug)), \
						right_value(retrieve_value<ReduceToType>(node->right.get(), debug)), \
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
				}
	
	#define BINARY_OPERATOR_DEFAULT_CONTEXT_CONSTRUCT(NODE_TYPE, OPERATOR) \
				Executor(const auto* context, const Node<NodeType :: NODE_TYPE>* node, bool debug) \
						: left_value(retrieve_value<ReduceToType>(context, node->left.get(), debug)), \
						right_value(retrieve_value<ReduceToType>(context, node->right.get(), debug)), \
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
				}

	#define BINARY_OPERATOR_NO_CONTEXT_NULLOPT_CONSTRUCT(NODE_TYPE, OPERATOR) \
				Executor(const Node<NodeType :: NODE_TYPE >* node, bool debug) \
						: left_value(std::nullopt), \
						right_value(std::nullopt), \
						value(std::nullopt) \
				{ \
					std::cerr << #NODE_TYPE << "::Error: cannot use specified type with: '" \
							<< #OPERATOR << "' operator.\n"; \
				}

	#define BINARY_OPERATOR_CONTEXT_NULLOPT_CONSTRUCT(NODE_TYPE, OPERATOR) \
				Executor(const auto* context, const Node<NodeType :: NODE_TYPE >* node, bool debug) \
						: left_value(std::nullopt), \
						right_value(std::nullopt), \
						value(std::nullopt) \
				{ \
					std::cerr << #NODE_TYPE << "::Error: cannot use specified type with: '" \
							<< #OPERATOR << "' operator.\n"; \
				}
	
	
	#define BINARY_OPERATOR_DEFAULT_CONVERSION_FUNCTIONS \
				std::optional<ReduceToType> to_value() { \
					return value; \
				} \
				operator std::optional<ReduceToType>() { \
					return to_value(); \
				}

	#define BEGIN_BINARY_OPERATOR(NODE_TYPE, OPERATOR, TEMPLATE, VALUE_TYPE) \
			TEMPLATE \
			struct Executor<VALUE_TYPE, NodeType :: NODE_TYPE > \
			{ \
				using ReduceToType = VALUE_TYPE; \
				std::optional<ReduceToType> left_value, right_value, value; \
				std::string debug_print_value(std::optional<ReduceToType>& value) { \
						return std::string{"{HasValue?:"} + std::to_string(value.has_value()) + std::string{"}"}; \
				} 

	#define END_BINARY_OPERATOR };

	#define BINARY_OPERATOR(NODE_TYPE, OPERATOR) \
		BEGIN_BINARY_OPERATOR(NODE_TYPE, OPERATOR, template<typename ReduceToParameterType>, ReduceToParameterType) \
			BINARY_OPERATOR_DEFAULT_NO_CONTEXT_CONSTRUCT(NODE_TYPE, OPERATOR) \
			BINARY_OPERATOR_DEFAULT_CONTEXT_CONSTRUCT(NODE_TYPE, OPERATOR) \
			BINARY_OPERATOR_DEFAULT_CONVERSION_FUNCTIONS \
		END_BINARY_OPERATOR

	#define BINARY_OPERATOR_ERROR_ON_TYPE(NODE_TYPE, VALUE_TYPE_TAG, OPERATOR) \
			BEGIN_BINARY_OPERATOR(NODE_TYPE, OPERATOR, template<> , typename NumericTypeResolver<VALUE_TYPE_TAG>::Type) \
				BINARY_OPERATOR_NO_CONTEXT_NULLOPT_CONSTRUCT(NODE_TYPE, OPERATOR) \
				BINARY_OPERATOR_CONTEXT_NULLOPT_CONSTRUCT(NODE_TYPE, OPEARTOR) \
				BINARY_OPERATOR_DEFAULT_CONVERSION_FUNCTIONS \
			END_BINARY_OPERATOR
	
	#define BINARY_OPERATOR_ERROR_BY_DEFAULT_ON_TYPE(NODE_TYPE, OPERATOR) \
			BEGIN_BINARY_OPERATOR(NODE_TYPE, OPERATOR, template<typename ReduceToParameterType>, ReduceToParameterType) \
				BINARY_OPERATOR_NO_CONTEXT_NULLOPT_CONSTRUCT(NODE_TYPE, OPERATOR) \
				BINARY_OPERATOR_CONTEXT_NULLOPT_CONSTRUCT(NODE_TYPE, OPEARTOR) \
				BINARY_OPERATOR_DEFAULT_CONVERSION_FUNCTIONS \
			END_BINARY_OPERATOR

	#define BINARY_OPERATOR_CONSTRUCT_ON_TYPE(NODE_TYPE, OPERATOR) \
		BEGIN_BINARY_OPERATOR(NODE_TYPE, OPERATOR, template<std::convertible_to< \
					typename NumericTypeResolver<NumericTypeTag::Bool>::Type> \
				 ReduceToParameterType>, ReduceToParameterType) \
			BINARY_OPERATOR_DEFAULT_NO_CONTEXT_CONSTRUCT(NODE_TYPE, OPERATOR) \
			BINARY_OPERATOR_DEFAULT_CONTEXT_CONSTRUCT(NODE_TYPE, OPERATOR) \
			BINARY_OPERATOR_DEFAULT_CONVERSION_FUNCTIONS \
		END_BINARY_OPERATOR


	BINARY_OPERATOR(Multiply, *)
	BINARY_OPERATOR(Divide, /)
	BINARY_OPERATOR(Add, +)
	BINARY_OPERATOR(Subtract, -)

	BINARY_OPERATOR(GreaterThan, >);
	BINARY_OPERATOR(LessThan, <);
	BINARY_OPERATOR(GreaterThanOrEqualTo, >=);
	BINARY_OPERATOR(LessThankOrEqualTo, <=);
	BINARY_OPERATOR(Equal, ==);

	#define LOGICAL_BINARY_OPERATOR(NODE_TYPE, OPERATOR) \
		BINARY_OPERATOR_ERROR_BY_DEFAULT_ON_TYPE(NODE_TYPE, OPERATOR) \
		BINARY_OPERATOR_CONSTRUCT_ON_TYPE(NODE_TYPE, OPERATOR)

	LOGICAL_BINARY_OPERATOR(LogicalAnd, &&)
	LOGICAL_BINARY_OPERATOR(LogicalOr, ||)

	#undef BINARY_OPERATOR

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::Negation>
	{
		using ReduceToType = ReduceToParameterType;
		std::optional<ReduceToType> absolute_value, value;
		Executor(const Node<NodeType::Negation>* node, bool debug) 
				: absolute_value(retrieve_value<ReduceToType>(node->negated.get(), debug)), 
				value(
						(absolute_value.has_value() == true)
								? std::optional{-absolute_value.value()}
								: std::nullopt
					) {
				if(debug == true)
					std::cout << "Negation, has value? " << value.has_value() << "\n";
			}
		Executor(const auto* context, const Node<NodeType::Negation>* node, bool debug) 
				: absolute_value(retrieve_value<ReduceToType>(context, node->negated.get(), debug)), 
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
		Executor(const Node<NodeType::Expression>* node, bool debug) 
				: value(retrieve_value<ReduceToType>(node->root.get(), debug)) {
			if(debug == true)
				std::cout << "Expression, has value? " << value.has_value() << "\n";
		}
		Executor(const auto* context, const Node<NodeType::Expression>* node, bool debug) 
				: value(retrieve_value<ReduceToType>(context, node->root.get(), debug)) {
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
	struct Executor<ReduceToParameterType, NodeType::LogicalExpression>
	{
		using ReduceToType = ReduceToParameterType;
		std::optional<ReduceToType> value;
		Executor(const Node<NodeType::LogicalExpression>* node, bool debug) 
				: value(std::nullopt) {
			std::cerr << "LogicalExpression::Error: Attempt input non-bool into LogicalExpression\n";
		}
		Executor(const auto* context, const Node<NodeType::LogicalExpression>* node, bool debug) 
				: value(std::nullopt) {
			std::cerr << "LogicalExpression::Error: Attempt input non-bool into LogicalExpression\n";
		}
		std::optional<ReduceToType> to_value() {
			return value;
		}
		operator std::optional<ReduceToType>() {
			return to_value();
		}
	};

	template<std::convertible_to<typename NumericTypeResolver<NumericTypeTag::Bool>::Type> ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::LogicalExpression>
	{
		using ReduceToType = ReduceToParameterType;
		std::optional<ReduceToType> value;
		Executor(const Node<NodeType::LogicalExpression>* node, bool debug) 
				: value(retrieve_value<ReduceToType>(node->root.get(), debug)) {
			if(debug == true)
				std::cout << "Logical Expression, has value? " << value.has_value() << "\n";
		}
		Executor(const auto* context, const Node<NodeType::LogicalExpression>* node, bool debug) 
				: value(retrieve_value<ReduceToType>(context, node->root.get(), debug)) {
			if(debug == true)
				std::cout << "Logical Expression, has value? " << value.has_value() << "\n";
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
		Executor(const Node<NodeType::ConstantCall>* node, bool debug) = delete;
		Executor(const auto* context, const Node<NodeType::ConstantCall>* node, bool debug)
				: value(retrieve_value<ReduceToType>(
						context, 
						context->constants.at(node->name).value.get(), 
						debug
					)) {
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

	#define LOGICAL_NOT_ERROR_ON_TYPE(VALUE_TYPE_TAG) \
		template<> \
		struct Executor<NumericTypeResolver<VALUE_TYPE_TAG>::Type, NodeType::LogicalNot> \
		{ \
			using ReduceToType = NumericTypeResolver<VALUE_TYPE_TAG>::Type; \
			std::optional<ReduceToType> absolute_value, value; \
			Executor(const Node<NodeType::LogicalNot>* node, bool debug)  \
					: absolute_value(std::nullopt), value(std::nullopt) { \
				std::cerr << "LogicalNot::Error: can not perform LogicalNot on " << #VALUE_TYPE_TAG << "\n"; \
			} \
			Executor(const auto* context, const Node<NodeType::LogicalNot>* node, bool debug)  \
					: absolute_value(std::nullopt), value(std::nullopt) { \
				std::cerr << "LogicalNot::Error: can not perform LogicalNot on " << #VALUE_TYPE_TAG << "\n"; \
			} \
			std::optional<ReduceToType> to_value() { \
				return value; \
			} \
			operator std::optional<ReduceToType>() { \
				return to_value(); \
			} \
		}

	LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::Whole);
	LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::Integer);
	LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::FixedPoint);
	LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::Character);

	template<>
	struct Executor<NumericTypeResolver<NumericTypeTag::Bool>::Type, NodeType::LogicalNot>
	{
		using ReduceToType = NumericTypeResolver<NumericTypeTag::Bool>::Type;
		std::optional<ReduceToType> absolute_value, value;
		Executor(const Node<NodeType::LogicalNot>* node, bool debug) 
				: absolute_value(retrieve_value<ReduceToType>(node->negated.get(), debug)), 
				value(
						(absolute_value.has_value() == true)
								? std::optional{!absolute_value.value()}
								: std::nullopt
					) {
				if(debug == true)
					std::cout << "Negation, has value? " << value.has_value() << "\n";
			}
		Executor(const auto* context, const Node<NodeType::LogicalNot>* node, bool debug) 
				: absolute_value(retrieve_value<ReduceToType>(context, node->negated.get(), debug)), 
				value(
						(absolute_value.has_value() == true)
								? std::optional{!absolute_value.value()}
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
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__SIMPLE__EXECUTOR__HPP

