#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/SyntaxAnalysis/LogicalExpression.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ConstantTests.hpp>

namespace Warp::SyntaxAnalysis::SyntaxTree
{
	template struct Node<NodeType::GreaterThan>;
	template struct Node<NodeType::LessThan>;
	template struct Node<NodeType::GreaterThanOrEqualTo>;
	template struct Node<NodeType::LessThankOrEqualTo>;
	template struct Node<NodeType::Equal>;
	template struct Node<NodeType::LogicalAnd>;
	template struct Node<NodeType::LogicalOr>;
	template struct Node<NodeType::LogicalNot>;
	template struct Node<NodeType::LogicalExpression>;
}

namespace std
{
	template class std::unique_ptr<Node<NodeType::GreaterThan>>;
	template class std::unique_ptr<Node<NodeType::LessThan>>;
	template class std::unique_ptr<Node<NodeType::GreaterThanOrEqualTo>>;
	template class std::unique_ptr<Node<NodeType::LessThankOrEqualTo>>;
	template class std::unique_ptr<Node<NodeType::Equal>>;
	template class std::unique_ptr<Node<NodeType::LogicalAnd>>;
	template class std::unique_ptr<Node<NodeType::LogicalOr>>;
	template class std::unique_ptr<Node<NodeType::LogicalNot>>;
	template class std::unique_ptr<Node<NodeType::LogicalExpression>>;

	template std::unique_ptr<Node<NodeType::LogicalExpression>> std::make_unique<
			Node<NodeType::LogicalExpression>
		>(decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::LogicalNot>> std::make_unique<
			Node<NodeType::LogicalNot>
		>(decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::LogicalOr>> std::make_unique<
			Node<NodeType::LogicalOr>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::LogicalAnd>> std::make_unique<
			Node<NodeType::LogicalAnd>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::Equal>> std::make_unique<
			Node<NodeType::Equal>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::LessThankOrEqualTo>> std::make_unique<
			Node<NodeType::LessThankOrEqualTo>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::GreaterThanOrEqualTo>> std::make_unique<
			Node<NodeType::GreaterThanOrEqualTo>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::LessThan>> std::make_unique<
			Node<NodeType::LessThan>
		>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
	template std::unique_ptr<Node<NodeType::GreaterThan>> std::make_unique<
			Node<NodeType::GreaterThan>
	>(decltype(std::move(std::declval<SyntaxNode>())), decltype(std::move(std::declval<SyntaxNode>())));
}

