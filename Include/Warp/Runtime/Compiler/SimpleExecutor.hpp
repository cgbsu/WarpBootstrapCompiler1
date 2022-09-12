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

	template<typename ReduceToParameterType, typename CurrentReduceToParameterType>
	struct IntermediateReduction {
		using ReduceToType = ReduceToParameterType;
		using CurrentReduceToType = CurrentReduceToParameterType;
	};

	template<typename NodeParameterType, typename CanidateParameterType>
	concept NoContextConstructibleConcept = requires(CanidateParameterType) {
			CanidateParameterType(std::declval<NodeParameterType>(), false);
	};

	template<typename NodeParameterType, typename CanidateParameterType>
	struct NoContextConstructibleHelper {
		constexpr static const bool value = false;
	};

	template<typename NodeParameterType, NoContextConstructibleConcept<NodeParameterType> CanidateParameterType>
	struct NoContextConstructibleHelper<NodeParameterType, CanidateParameterType> {
		constexpr static const bool value = true;
	};

	template<typename NodeParameterType, typename CanidateParameterType>
	constexpr static const auto no_context_constructible 
			= NoContextConstructibleHelper<NodeParameterType, CanidateParameterType>::value;

	template<typename ReduceToParameterType>
	static auto retrieve_value(const auto* context, const BaseNode* node, bool debug) 
			-> std::optional<ReduceToParameterType>
	{
		return visit<
				ReduceToParameterType, 
				[](auto node, const auto& context, bool debug)
				{ 
					constexpr const auto tag = CleanType<decltype(node)>::tag;
					//if(debug == true)
						std::cout << "Tag: " << to_string(tag) << "\n";
					using ExecutorType = decltype(Executor<
							ReduceToParameterType, 
							tag
						>(context, node, debug));
					using ValueType = decltype(std::declval<ExecutorType>().to_value());
					if constexpr(std::is_convertible_v<std::optional<ReduceToParameterType>, ValueType>)
						return ExecutorType(context, node, debug).to_value(); 
					else
					{
						//if(debug == true)
							std::cerr << "retrieve_value(context): Type not convertable!\n";
						return std::nullopt;
					}
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
					//if(debug == true)
						std::cout << "Tag: " << to_string(tag) << "\n";
					if constexpr(tag != NodeType::ConstantCall)//(no_context_constructible<decltype(node), Executor<ReduceToParameterType, tag>> == true)
					{
						using ExecutorType = decltype(Executor<
								ReduceToParameterType, 
								tag
							>(node, debug));
						using ValueType = decltype(std::declval<ExecutorType>().to_value());
						if constexpr(std::is_convertible_v<std::optional<ReduceToParameterType>, ValueType>)
							return ExecutorType(node, debug).to_value(); 
						else
						{
							//if(debug == true)
								std::cerr << "retrieve_value(no context): Type not convertable!\n";
							return std::nullopt;
						}
					}
					else
					{
						//if(debug == true)
							std::cerr << "retrieve_value(no context): Not no context constructible!\n";
						return std::nullopt;
					}
				}
			>(node->to_view(), debug);
	}

	static std::optional<NumericValue> reduce_to_numeric_value(const BaseNode* node, bool debug) {
		return retrieve_value<NumericValue>(node, debug);
	}
	static std::optional<NumericValue> reduce_to_numeric_value(const auto& context, const BaseNode* node, bool debug) {
		return retrieve_value<NumericValue>(context, node, debug);
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

		#define BINARY_OPERATOR_BODY_(NODE_TYPE, OPERATOR, PARSE, CONVERT) \
			std::string debug_print_value(const auto& value) const { \
					return std::string{"{HasValue?:"} + std::to_string(value.has_value()) + std::string{"}"}; \
			} \
			void debug_output(bool debug) const \
			{ \
				if(debug == true) \
				{ \
					std::cout << "Operator:" << #OPERATOR << "\n" \
							<< "Left: " << debug_print_value(left_value) << "\n" \
							<< "Right: " << debug_print_value(right_value) << "\n" \
							<< "Value: " << debug_print_value(value) << "\n"; \
				} \
			} \
			Executor(const Node<NodeType :: NODE_TYPE >* node, bool debug) \
					: left_value(PARSE(node->left.get(), debug)), \
					right_value(PARSE(node->right.get(), debug)), \
					value(CONVERT( \
							(left_value.has_value() == true && right_value.has_value() == true) \
							? std::optional{left_value.value() OPERATOR right_value.value()} \
							: std::nullopt \
						)) \
			{ \
				debug_output(debug); \
			} \
			Executor(const auto* context, const Node<NodeType :: NODE_TYPE>* node, bool debug) \
					: left_value(PARSE(context, node->left.get(), debug)), \
					right_value(PARSE(context, node->right.get(), debug)), \
					value(CONVERT( \
							(left_value.has_value() == true && right_value.has_value() == true) \
							? std::optional{left_value.value() OPERATOR right_value.value()} \
							: std::nullopt \
						)) \
			{ \
				debug_output(debug); \
			}

		#define BINARY_OPERATOR_BODY(NODE_TYPE, OPERATOR) \
			BINARY_OPERATOR_BODY_(NODE_TYPE, OPERATOR, retrieve_value<ReduceToType>, static_cast<decltype(value)>)

		#define BINARY_OPERATOR(NODE_TYPE, OPERATOR) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: NODE_TYPE > \
			{ \
				using ReduceToType = ReduceToParameterType; \
				std::optional<ReduceToType> left_value, right_value, value; \
				BINARY_OPERATOR_BODY(NODE_TYPE, OPERATOR) \
				std::optional<ReduceToType> to_value() { \
					return value; \
				} \
				operator std::optional<ReduceToType>() { \
					return to_value(); \
				} \
			}

		//template<typename CanidateParameterType>
		//concept LogicalOperatableConcept = requires(CanidateParameterType canidate) {
		//		canidate && canidate;
		//		canidate || canidate;
		//};

		#define BOOLEAN_TO_VALUE \
				std::optional<BoolType> to_value() { \
					return value; \
				} \
				operator std::optional<BoolType>() { \
					return to_value(); \
				}
		
		template<typename ToParameterType>
		static auto optional_to(std::optional<NumericValue> numeric_value)
		{
			return numeric_value.transform([&](const NumericValue& value)
					{
						auto optional = value.to<ToParameterType>();
						return optional.value();
					});
		}
					
		#define COMPARISON_BINARY_OPERATOR(NODE_TYPE, OPERATOR) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: NODE_TYPE > \
			{ \
				using ReduceToType = ReduceToParameterType; \
				std::optional<NumericValue> left_value, right_value; \
				std::optional<BoolType> value; \
				BINARY_OPERATOR_BODY_(NODE_TYPE, OPERATOR, reduce_to_numeric_value, optional_to<WarpBool>) \
				BOOLEAN_TO_VALUE \
			}
//, reduce_to_numeric_value) \
	
		#define LOGICAL_BINARY_OPERATOR(NODE_TYPE, OPERATOR) \
			template<typename ReduceToParameterType> \
			struct Executor<ReduceToParameterType, NodeType :: NODE_TYPE > \
			{ \
				using ReduceToType = WarpBool; \
				std::optional<BoolType> left_value, right_value; \
				std::optional<BoolType> value; \
				BINARY_OPERATOR_BODY_(NODE_TYPE, OPERATOR, retrieve_value<BoolType>, static_cast<decltype(value)>) \
				BOOLEAN_TO_VALUE \
			}

//, reduce_to_numeric_value) \
//
	BINARY_OPERATOR(Multiply, *);
	BINARY_OPERATOR(Divide, /);
	BINARY_OPERATOR(Add, +);
	BINARY_OPERATOR(Subtract, -);

	COMPARISON_BINARY_OPERATOR(GreaterThan, >);
	COMPARISON_BINARY_OPERATOR(LessThan, <);
	COMPARISON_BINARY_OPERATOR(GreaterThanOrEqualTo, >=);
	COMPARISON_BINARY_OPERATOR(LessThankOrEqualTo, <=);
	COMPARISON_BINARY_OPERATOR(Equal, ==);

	LOGICAL_BINARY_OPERATOR(LogicalAnd, &&);
	LOGICAL_BINARY_OPERATOR(LogicalOr, ||);

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

	//template<typename ReduceToParameterType>
	//struct Executor<ReduceToParameterType, NodeType::LogicalExpression>
	//{
	//	using ReduceToType = ReduceToParameterType;
	//	std::optional<ReduceToType> value;
	//	Executor(const Node<NodeType::LogicalExpression>* node, bool debug) 
	//			: value(std::nullopt) {
	//		std::cerr << "LogicalExpression::Error: Attempt input non-bool into LogicalExpression\n";
	//	}
	//	Executor(const auto* context, const Node<NodeType::LogicalExpression>* node, bool debug) 
	//			: value(std::nullopt) {
	//		std::cerr << "LogicalExpression::Error: Attempt input non-bool into LogicalExpression\n";
	//	}
	//	std::optional<ReduceToType> to_value() {
	//		return value;
	//	}
	//	operator std::optional<ReduceToType>() {
	//		return to_value();
	//	}
	//};

	//template<std::convertible_to<typename NumericTypeResolver<NumericTypeTag::Bool>::Type> ReduceToParameterType>
	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::LogicalExpression>
	{
		using ReduceToType = ReduceToParameterType;
		//using ReduceToType = typename NumericTypeResolver<NumericTypeTag::Bool>::Type;
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

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::LogicalNot>//NumericTypeResolver<NumericTypeTag::Bool>::Type, NodeType::LogicalNot>
	{
		using ReduceToType = ReduceToParameterType;//NumericTypeResolver<NumericTypeTag::Bool>::Type;
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

