#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>
/* This work is under the Copyright Christopher A. Greeley (2024) and it is distributed
* under the No Kill Do No Harm License, a legally non-binding sumemry is as follows: 
* 
* # No Kill Do No Harm Licence – Summary
* 
* Based on version 0.3, July 2022 of the Do No Harm License
* 
* https://github.com/raisely/NoHarm
* 
* LEGALLY NON-BINDING SUMMARY OF THE TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
* 
* ## Licence Grants
* 
* You're allowed
* 
* - to distribute the licensed work,
* - to create, publish, sublicense and patent derivative works and
* - to put your modifications or your derivative work under a seperate licence,
* 
* free of charge. Though, filing patent litigation leads to the loss of the patent licence. Also, the licence grants don't include the right to use the licensor's trademarks.
* 
* ## Unethical Behaviour
* 
* You may not use the licensed work if you engage in:
* 
* - human rights violations,
* - environmental destruction,
* - warfare,
* - addictive/destructive products or services or
* - actions that frustrate:
*   * peace,
*   * access to human rights,
*   * peaceful assembly and association,
*   * a sustainable environment or
*   * democratic processes
*   * abortion
*   * euthanasia
*   * human embryonic stem cell research (if human organisms are killed in the process)
* - except for actions that may be contrary to "human rights" (or interpretations thereof), do not kill and that frustrate 
*   * abortion
*   * euthanasia
*   * killing
* and; the software must never be used to kill, including: abortion, euthanasia, human stem cell research, in war, or law enforcement or as a part of any lethal weapon
* 
* ## Contributions
* 
* Contributions to the licensed work must be licensed under the exact same licence.
* 
* ## Licence Notice
* 
* When distributing the licensed work or your derivative work, you must
* 
* - include a copy of this licence,
* - retain attribution notices,
* - state changes that you made and
* - not use the names of the author and the contributors to promote your derivative work.
* 
* If the licensed work includes a "NOTICE" text file with attribution notices, you must copy those notices to:
* 
* - a "NOTICE" file within your derivative work,
* - a place within the source code or the documentation or
* - a place within a display generated by your derivative work.
* 
* ## No Warranty or Liability
* 
* The licensed work is offered on an as-is basis without any warranty or liability. You may choose to offer warranty or liability for your derivative work, but only fully on your own responsibility. */



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
					if(debug == true)
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
						if(debug == true)
						{
							std::cerr << "retrieve_value(context): Type not convertable: " 
									<< friendly_type_name<ReduceToParameterType>() << " from " 
									<< friendly_type_name<ValueType>() << "\n";
						}
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
					if(debug == true)
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
							if(debug == true)
							{
								std::cerr << "retrieve_value(no context): Type not convertable: "
										<< friendly_type_name<ReduceToParameterType>() << " from " 
										<< friendly_type_name<ValueType>() << "\n";
							}
							return std::nullopt;
						}
					}
					else
					{
						if(debug == true)
							std::cerr << "retrieve_value(no context): Not no context constructible!\n";
						return std::nullopt;
					}
				}
			>(node->to_view(), debug);
	}

	static std::optional<NumericValue> reduce_to_numeric_value(const BaseNode* node, bool debug)
	{
		if(debug == true)
			std::cout << "Reduce Value\n";
		return retrieve_value<NumericValue>(node, debug);
	}
	static std::optional<NumericValue> reduce_to_numeric_value(const auto& context, const BaseNode* node, bool debug)
	{
		if(debug == true)
			std::cout << "Reduce Value (context)\n";
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
					if constexpr(std::is_same_v<ReduceToType, NumericValue> == true) \
						return value; \
					else \
						return std::nullopt; \
				} \
				operator std::optional<ReduceToType>() { \
					return to_value(); \
				} \
			}
	
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

	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::LogicalExpression>
	{
		using ReduceToType = BoolType;
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

	//#define LOGICAL_NOT_ERROR_ON_TYPE(VALUE_TYPE_TAG) \
	//	template<> \
	//	struct Executor<NumericTypeResolver<VALUE_TYPE_TAG>::Type, NodeType::LogicalNot> \
	//	{ \
	//		using ReduceToType = NumericTypeResolver<VALUE_TYPE_TAG>::Type; \
	//		std::optional<ReduceToType> absolute_value, value; \
	//		Executor(const Node<NodeType::LogicalNot>* node, bool debug)  \
	//				: absolute_value(std::nullopt), value(std::nullopt) { \
	//			std::cerr << "LogicalNot::Error: can not perform LogicalNot on " << #VALUE_TYPE_TAG << "\n"; \
	//		} \
	//		Executor(const auto* context, const Node<NodeType::LogicalNot>* node, bool debug)  \
	//				: absolute_value(std::nullopt), value(std::nullopt) { \
	//			std::cerr << "LogicalNot::Error: can not perform LogicalNot on " << #VALUE_TYPE_TAG << "\n"; \
	//		} \
	//		std::optional<ReduceToType> to_value() { \
	//			return value; \
	//		} \
	//		operator std::optional<ReduceToType>() { \
	//			return to_value(); \
	//		} \
	//	}

	//LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::Whole);
	//LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::Integer);
	//LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::FixedPoint);
	//LOGICAL_NOT_ERROR_ON_TYPE(NumericTypeTag::Character);

	//template<typename ReduceToParameterType>
	//struct Executor<ReduceToParameterType, NodeType::LogicalNot>
	//{
	//	using ReduceToType = ReduceToParameterType;
	//	std::optional<ReduceToType> absolute_value, value;
	//	Executor(const Node<NodeType::LogicalNot>* node, bool debug) 
	//			: absolute_value(retrieve_value<ReduceToType>(node->negated.get(), debug)), 
	//			value(
	//					(absolute_value.has_value() == true)
	//							? std::optional{!absolute_value.value()}
	//							: std::nullopt
	//				) {
	//			if(debug == true)
	//				std::cout << "Negation, has value? " << value.has_value() << "\n";
	//		}
	//	Executor(const auto* context, const Node<NodeType::LogicalNot>* node, bool debug) 
	//			: absolute_value(retrieve_value<ReduceToType>(context, node->negated.get(), debug)), 
	//			value(
	//					(absolute_value.has_value() == true)
	//							? std::optional{!absolute_value.value()}
	//							: std::nullopt
	//				) {
	//			if(debug == true)
	//				std::cout << "Negation, has value? " << absolute_value.has_value() << "\n";
	//		}
	//	std::optional<ReduceToType> to_value() {
	//		return value;
	//	}
	//	operator std::optional<ReduceToType>() {
	//		return to_value();
	//	}
	//};
	template<typename ReduceToParameterType>
	struct Executor<ReduceToParameterType, NodeType::LogicalNot>
	{
		using ReduceToType = BoolType;
		std::optional<BoolType> absolute_value, value;
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

