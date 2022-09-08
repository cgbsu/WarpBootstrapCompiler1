#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Parsing/NumericLiterals.hpp>
#include <Warp/SyntaxAnalysis.hpp>
#include <Warp/Parsing/GeneralTermTags.hpp>
#include <Warp/Runtime/Compiler/Constant.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

namespace Warp::Parsing
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	enum class MathematicalExpression
	{
		Add, 
		Subtract, 
		Multiply, 
		Divide, 
		// Modulo
		// Raise
		// Log
		// BaseConvert
		// Solve
		OpenPrioritization, 
		ClosePrioritization, 
		//Expression
	};



	using MathematicalExpressionTermsType = NumericLiteralTermsType
		::Prepend<
			TreeTerm<
					MathematicalExpression::Multiply, 
					CharTerm, 
					'*', 
					ctpg::associativity::ltor
				>, 
			TreeTerm<
					MathematicalExpression::Divide, 
					CharTerm, 
					'/', 
					ctpg::associativity::ltor
				>
		>::Prepend<
			TreeTerm<
					MathematicalExpression::Add, 
					CharTerm, 
					'+', 
					ctpg::associativity::ltor
				>, 
			TreeTerm<
					MathematicalExpression::Subtract, 
					CharTerm, 
					'-', 
					ctpg::associativity::ltor
				>
		>::AddOnePriority<
			TreeTerm<
					Identifier::Identifier, 
					RegexTerm, 
					FixedString{
							//"[a-zA-Z_][a-zA-Z_0-9]+"
		//					//"(?!(u|xp|i|c|bl)[0-9]+)
		//					//"[a-zA-Z_][a-zA-Z_0-9]*"
							"([a-zA-Z_]{5}[a-zA-Z_0-9]*)"
									//"|([a-zA-Z_]{2})"
									//"|([a-zA-Z_]{1})"
						}, 
					FixedString{"Identifier"}, 
					ctpg::associativity::no_assoc
				>
		>::AddOnePriority<
			TreeTerm<
					Brackets::OpenParenthesis, 
					CharTerm, 
					'(', 
					ctpg::associativity::no_assoc
				>, 
			TreeTerm<
					Brackets::CloseParenthesis, 
					CharTerm, 
					')', 
					ctpg::associativity::no_assoc
				>
		>::Prepend<
			TypeTreeTerm<
					Construct::Constant, 
					NonTerminalTerm, 
					ConstantType, 
					FixedString{"Constant"}
				>, 
			TypeTreeTerm< 
					Construct::Context, 
					NonTerminalTerm, 
					Context, 
					FixedString{"Context"}
				>, 
			TypeTreeTerm<
					Call::Constant, 
					NonTerminalTerm, 
					std::string, 
					FixedString{"ConstantCall"}
				>
			>;

	template<
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto ReductionTagParameterConstant, 
			auto InputTermTagParameterConstant, 
			auto TermTermNameParameterConstant = FixedString{"Term"},  
			auto SumTermNameParameterConstant = FixedString{"Sum"}, 
			auto ExpressionTermNameParameterConstant = FixedString{"Expression"}
		>
	requires
			HasTypeConcept<TypeResolverParameterTemplate<ReductionTagParameterConstant>> 
			&& HasTypeConcept<TypeResolverParameterTemplate<InputTermTagParameterConstant>>
			&& std::is_convertible_v<
					typename TypeResolverParameterTemplate<InputTermTagParameterConstant>::Type, 
					typename TypeResolverParameterTemplate<ReductionTagParameterConstant>::Type
				>
	struct MathematicalExpressionParser : NumericLiteralParser<
			TermsParameterType, 
			TypeResolverParameterTemplate
		>
	{
		using BaseTermsType = TermsParameterType;

		template<auto NonTerminalTypeTagParameterConstant>
		using TypeResolverTemplate = TypeResolverParameterTemplate<NonTerminalTypeTagParameterConstant>::Type;

		constexpr static const auto reduce_to_term_tag = ReductionTagParameterConstant;
		constexpr static const auto input_term_tag = ReductionTagParameterConstant;

		using ReduceToType = TypeResolverTemplate<reduce_to_term_tag>;
		using InputType = TypeResolverTemplate<input_term_tag>; // TODO: See if I will need to get non-terminal-terms in here

		using BaseType = NumericLiteralParser<BaseTermsType, TypeResolverParameterTemplate>;

		constexpr static const auto term_term_name = TermTermNameParameterConstant; 
		constexpr static const auto sum_term_name = SumTermNameParameterConstant;
		constexpr static const auto expression_term_name = ExpressionTermNameParameterConstant;

		//enum class AlternativeTag
		//{
		//	Product, 
		//	Sum, 
		//	Term, 
		//	ReduceTo, 
		//	None
		//};

		struct Expression {
			SyntaxNode node;
		};

		struct IdentifierWrapper { // Disambiguiator from std::string
			std::string name;
		};

		struct Term
		{
			SyntaxNode node;
			constexpr Term(SyntaxNode node) noexcept : node(std::move(node)) {}
			//template<NodeType NodeTypeParameterConstant>
			//constexpr Term(Node<NodeTypeParameterConstant> node) noexcept 
			//		: node(std::make_unique<Node<NodeTypeParameterConstant>>(node)) {}
			Term(InputType value) noexcept : node(literal_node(value)) {}
			constexpr Term() noexcept = default;
			constexpr Term(Term& other) noexcept = default;
			constexpr Term(Term&& other) noexcept = default;

			constexpr Term& operator=(const Term& other) noexcept = default;
			constexpr Term& operator=(Term&& other) noexcept = default;

			constexpr Term operator*(const InputType& other) noexcept
			{
				return Term(std::make_unique<Node<NodeType::Multiply>>(
						std::move(node), 
						literal_node(other)
					));
			}
			constexpr Term operator/(const InputType& other) noexcept
			{
				return Term(std::make_unique<Node<NodeType::Divide>>(
						std::move(node), 
						literal_node(other)
					));
			}
			constexpr Term operator*(IdentifierWrapper other) noexcept
			{
				return Term(std::make_unique<Node<NodeType::Multiply>>(
						std::move(node), 
						constant_call(other.name)
					));
			}
			constexpr Term operator/(IdentifierWrapper other) noexcept
			{
				return Term(std::make_unique<Node<NodeType::Divide>>(
						std::move(node), 
						constant_call(other.name)
					));
			}
			constexpr Term operator*(Term& other) noexcept
			{
				return Term(std::make_unique<Node<NodeType::Multiply>>(
						std::move(node), 
						std::move(other.node)
					));
			}
			constexpr Term operator/(Term& other) noexcept
			{
				return Term(std::make_unique<Node<NodeType::Divide>>(
						std::move(node), 
						std::move(other.node)
					));
				//return Term{value / other.value, is_negated(other)};
			}
			constexpr Term operator-() noexcept {
				return Term(std::make_unique<Node<NodeType::Negation>>(std::move(node)));
				//return Term{value, !negated};
			}
			constexpr Term as_negated() noexcept {
				return Term(std::make_unique<Node<NodeType::Negation>>(std::move(node)));
				//return Term{value, !negated};
			}
		};

		template<NodeType>
		struct OperationHolder {};

		struct Sum
		{
			SyntaxNode node;
			constexpr Sum(SyntaxNode&& node) noexcept : node(std::move(node)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(InputType left, OperationHolder<OperateParameterConstant>, InputType right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						literal_node(left), 
						literal_node(right)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(InputType left, OperationHolder<OperateParameterConstant>, Term& right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						literal_node(left), 
						std::move(right.node)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(Term& left, OperationHolder<OperateParameterConstant>, Term& right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						std::move(left.node), 
			   			std::move(right.node)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(std::string left, OperationHolder<OperateParameterConstant>, std::string right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						constant_call(left), 
						constant_call(right)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(std::string left, OperationHolder<OperateParameterConstant>, Term& right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						constant_call(left), 
						std::move(right.node)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(Term& left, OperationHolder<OperateParameterConstant>, std::string right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						std::move(left.node), 
						constant_call(right)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(InputType& left, OperationHolder<OperateParameterConstant>, std::string right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						literal_node(left), 
						constant_call(right)
					)) {}
			template<NodeType OperateParameterConstant>
			constexpr Sum(std::string left, OperationHolder<OperateParameterConstant>, InputType right) noexcept 
				: node(std::make_unique<Node<OperateParameterConstant>>(
						constant_call(right), 
						literal_node(right)
					)) {}
			constexpr Sum() noexcept = default;
			constexpr Sum(const Sum& other) noexcept = default;
			constexpr Sum(Sum&& other) noexcept = default;

			constexpr Sum& operator=(const Sum& other) noexcept = default;
			constexpr Sum& operator=(Sum&& other) noexcept = default;

			constexpr Sum operator+(const InputType& other) noexcept
			{
				return Sum(std::make_unique<Node<NodeType::Add>>(
						std::move(node), 
						literal_node(other)
					));
			}
			constexpr Sum operator-(const InputType& other) noexcept
			{
				return Sum(std::make_unique<Node<NodeType::Subtract>>(
						std::move(node), 
						literal_node(other)
					));
			}
			constexpr Sum operator+(std::string other) noexcept
			{
				return Sum(std::make_unique<Node<NodeType::Add>>(
						std::move(node), 
						std::move(std::make_unique<Node<NodeType::ConstantCall>>(other))
					));
			}
			constexpr Sum operator-(std::string other) noexcept
			{
				return Sum(std::make_unique<Node<NodeType::Subtract>>(
						std::move(node), 
						std::move(std::make_unique<Node<NodeType::ConstantCall>>(other))
					));
			}
			constexpr Sum operator+(Term& other) noexcept
			{
				return Sum(std::make_unique<Node<NodeType::Add>>(
						std::move(node), 
						std::move(other.node)
					));
			}
			constexpr Sum operator-(Term& other) noexcept
			{
				return Sum(std::make_unique<Node<NodeType::Subtract>>(
						std::move(node), 
						std::move(other.node)
					));
			}
		};


		enum class TypeSpecificMathematicalExpressionTermTags
		{
			Sum, 
			Term, 
			Expression
		};

		using UniqueTermsType = MakeTerms<
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Sum, 
						NonTerminalTerm, 
						Sum, 
						sum_term_name
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Term, 
						NonTerminalTerm, 
						Term, 
						term_term_name
					>, 
				TypeTreeTerm<
						TypeSpecificMathematicalExpressionTermTags::Expression, 
						NonTerminalTerm, 
						Expression, 
						expression_term_name
					>
			>;

		using TermsType = decltype(BaseTermsType::template append_terms(UniqueTermsType{}));

		template<auto TermTagParameterConstant>
		constexpr static const auto term = TermsType::template term<TermTagParameterConstant>;

		constexpr static const auto reduce_to = term<reduce_to_term_tag>;
		constexpr static const auto input = term<input_term_tag>;

		constexpr static const auto add = term<MathematicalExpression::Add>;
		constexpr static const auto subtract = term<MathematicalExpression::Subtract>;

		constexpr static const auto multiply = term<MathematicalExpression::Multiply>;
		constexpr static const auto divide = term<MathematicalExpression::Divide>;

		constexpr static const auto identifier = term<Identifier::Identifier>;

		constexpr static const auto open_parenthesis 
				= term<Brackets::OpenParenthesis>;
		constexpr static const auto close_parenthesis 
				= term<Brackets::CloseParenthesis>;

		constexpr static const auto sum 
				= term<TypeSpecificMathematicalExpressionTermTags::Sum>;
		constexpr static const auto math_term
				= term<TypeSpecificMathematicalExpressionTermTags::Term>;
		constexpr static const auto expression 
				= term<TypeSpecificMathematicalExpressionTermTags::Expression>;

		constexpr static const auto unique_terms = ctpg::terms(
					add, 
					subtract, 
					multiply, 
					divide
			); 
		constexpr static const auto terms = std::tuple_cat( 
				BaseType::terms, 
				ctpg::terms( 
						open_parenthesis, 
						close_parenthesis, 
						identifier
					), 
				unique_terms
			); 

		constexpr static const auto unique_non_terminal_terms = ctpg::nterms( 
				sum, 
				math_term, 
				expression
			); 

		constexpr static const auto non_terminal_terms = std::tuple_cat(
				BaseType::non_terminal_terms, 
				unique_non_terminal_terms
			); 

		template<auto OperateParameterConstant> 
		consteval static const auto term_operation_reduction(auto operation_term, auto operator_term) {
				return ctpg::rules(
						operation_term(math_term, operator_term, math_term) 
						>= [](auto left, auto, auto right) {
							return OperateParameterConstant(left, right);
						}
					);
			}
		template<auto OperateParameterConstant, auto ProductOperateParameterConstant>
		consteval static const auto basic_term_operation_rules(auto operation_term, auto operator_term)
		{
				return concatinate_tuples(
						term_operation_reduction<OperateParameterConstant>(operation_term, operator_term), 
						ctpg::rules(
								operation_term(operation_term, operator_term, math_term) 
								>= [](auto left, auto, auto right) {
									return ProductOperateParameterConstant(left, right);
								}
							));
		}
		template<auto OperateParameterConstant>
		consteval static const auto input_operation_rules(auto operation_term, auto operator_term)
		{
			return ctpg::rules(
					operation_term(operation_term, operator_term, input)
					>= [](auto left, auto, auto right) {
						return OperateParameterConstant(left, right);
					}
				);
		}

		template<auto OperateParameterConstant>
		consteval static const auto right_identifier_operation_rules(auto to_term, auto from_term, auto operator_term)
		{
			return ctpg::rules(
					to_term(from_term, operator_term, identifier)
					>= [](auto left, auto, auto right)
					{
						auto call = constant_call(std::string{std::string_view{right}});
						auto term = Term{std::move(call)};
						return OperateParameterConstant(
								left, 
								std::move(term)
							);
					}
				);
		}

		template<typename NodeConsumeParameterType, auto OperateParameterConstant>
		consteval static const auto left_identifier_operation_rules(auto operation_term, auto operator_term)
		{
			return ctpg::rules(
					operation_term(identifier, operator_term, input)
					>= [](auto left, auto, auto right)
					{
						auto call = constant_call(std::string{std::string_view{left}});
						auto term = NodeConsumeParameterType{std::move(call)};
						return OperateParameterConstant(
								std::move(term), 
								std::move(right)
							);
					}
				);
		}

		template<auto OperateParameterConstant>
		consteval static const auto left_identifier_term_operation_rules(auto operation_term, auto operator_term)
		{
			return ctpg::rules(
					operation_term(identifier, operator_term, math_term)
					>= [](auto left, auto, auto right)
					{
						auto call = constant_call(std::string{std::string_view{left}});
						auto term = Sum{std::move(call)};
						return OperateParameterConstant(
								term, 
								right
							);
					}
				);
		}

		consteval static const auto from_parenthesis()
		{
			return ctpg::rules(
					math_term(open_parenthesis, math_term, close_parenthesis)
					>= [](auto left, auto term, auto right) { return std::move(term); },
					math_term(open_parenthesis, sum, close_parenthesis)
					>= [](auto left, auto sum, auto right) { return Term{std::move(sum.node)}; }, 
					math_term(open_parenthesis, identifier, close_parenthesis)
					>= [](auto left, auto sum, auto right) { return Term{std::move(constant_call(std::string{right}))}; }
				);
		}

		constexpr static const auto identifier_to_math_term
				= math_term(identifier) >= [](auto identifier) {
					return constant_call(std::string{identifier});
				};

		constexpr static const auto input_to_math_term 
				= math_term(input) >= [](auto input_) { 
					return std::move(Term{input_});
				};
		constexpr static const auto negated_input_to_math_term 
				= math_term(subtract, input) >= [](auto, auto input_) { return Term{input_}.as_negated(); }; 

		constexpr static const auto sum_to_expression
				= expression(sum)
				>= [](auto sum_) {
					return Expression{std::move(sum_.node)};
				}; 
		
		constexpr static const auto math_term_to_expression
				= expression(math_term)
				>= [](auto math_term_) {
					return Expression{std::move(math_term_.node)};
				}; 

		//constexpr static const auto 

		consteval static const auto unique_rules()
		{
			return concatinate_tuples(
					basic_term_operation_rules<
							[](auto& left, auto& right)
							{
								return Sum{
										left, 
										OperationHolder<NodeType::Add>{}, 
										right
									}; 
							}, 
							[](auto& left, auto& right) { return left + right; }
						>(sum, add), 
					basic_term_operation_rules<
							[](auto& left, auto& right)
							{ 
								return Sum{
										left, 
										OperationHolder<NodeType::Subtract>{}, 
										right
									};
							}, 
							[](auto& left, auto& right) { return left - right; }
						>(sum, subtract), 
					right_identifier_operation_rules<
							[](auto& left, auto right) { return left + right; }
						>(sum, sum, add), 
					right_identifier_operation_rules<
							[](auto& left, auto right) { return left - right; }
						>(sum, sum, subtract),
					left_identifier_operation_rules<
							Sum, 
							[](auto left, auto right) { return left + right; }
						>(sum, add), 
					left_identifier_operation_rules<
							Sum, 
							[](auto left, auto right) { return left - right; }
						>(sum, subtract), 
					//left_identifier_term_operation_rules<
					//		[](auto& left, auto right) { return left + right; }
					//	>(sum, add), 
					//left_identifier_term_operation_rules<
					//		[](auto& left, auto right) { return left - right; }
					//	>(sum, subtract), 
				//
					//right_identifier_operation_rules<
					//		[](auto& left, auto& right)
					//		{ 
					//			return Sum{
					//					left, 
					//					OperationHolder<NodeType::Subtract>{}, 
					//					right
					//				};
					//		}
					//	>(sum, math_term, add), 
					//right_identifier_operation_rules<
					//		[](auto& left, auto& right)
					//		{ 
					//			return Sum{
					//					left, 
					//					OperationHolder<NodeType::Subtract>{}, 
					//					right
					//				};
					//		}
					//	>(sum, math_term, subtract), 
					term_operation_reduction<
							[](auto& left, auto& right) { return left * right; }
						>(math_term, multiply), 
					term_operation_reduction<
							[](auto& left, auto& right) { return left / right; }
						>(math_term, divide), 
					input_operation_rules<
							[](auto& left, auto& right) { return left * right; }
						>(math_term, multiply), 
					input_operation_rules<
							[](auto& left, auto& right) { return left / right; }
						>(math_term, divide), 
					left_identifier_operation_rules<
							Term, 
							[](auto left, auto right) { return left * right; }
						>(math_term, multiply), 
					//right_identifier_operation_rules<
					//		[](auto& left, auto right) { return left * right; }
					//	>(math_term, math_term, multiply), 
					left_identifier_operation_rules<
							Term, 
							[](auto left, auto right) { return left / right; }
						>(math_term, divide), 
					//right_identifier_operation_rules<
					//		[](auto& left, auto right) { return left / right; }
					//	>(math_term, math_term, divide), 
					from_parenthesis(), 
					ctpg::rules(
							input_to_math_term, 
							negated_input_to_math_term, 
							sum_to_expression, 
							math_term_to_expression//, 
							//identifier_to_math_term
						)
				);
		}

		consteval static const auto rules()
		{
			return concatinate_tuples(
					BaseType::rules(), 
					unique_rules()
				);
		}

		constexpr static const auto parser = ctpg::parser(
				reduce_to, 
				terms, 
				non_terminal_terms, 
				rules()
			);
	};

	extern template class NumericTypeResolver<NumericTypeTag::Whole>;
	extern template class NumericTypeResolver<NumericTypeTag::Integer>;
	extern template class NumericTypeResolver<NumericTypeTag::FixedPoint>;
	extern template class NumericTypeResolver<NumericTypeTag::Character>;
	extern template class NumericTypeResolver<NumericTypeTag::Bool>;
	
	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Whole, 
			NumericTypeTag::Whole
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Integer, 
			NumericTypeTag::Integer
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::FixedPoint, 
			NumericTypeTag::FixedPoint
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Character, 
			NumericTypeTag::Character
		>;

	extern template class MathematicalExpressionParser<
			MathematicalExpressionTermsType, 
			NumericTypeResolver, 
			NumericTypeTag::Bool, 
			NumericTypeTag::Bool
		>;

	template<
			auto TypeTagParameterConstant, 
			typename TermsParameterType, 
			template<auto> typename TypeResolverParameterTemplate, 
			auto TermTermNameParameterConstant = FixedString{"Term"},  
			auto SumTermNameParameterConstant = FixedString{"Sum"}, 
			auto ExpressionTermNameParameterConstant = FixedString{"Expression"}
		>
	using HomogenousMathematicalExpressionParser 
			= MathematicalExpressionParser<
				TermsParameterType, 
				TypeResolverParameterTemplate, 
				TypeTagParameterConstant, 
				TypeTagParameterConstant, 
				TermTermNameParameterConstant, 
				SumTermNameParameterConstant, 
				ExpressionTermNameParameterConstant
			>;
}

#endif // WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP

