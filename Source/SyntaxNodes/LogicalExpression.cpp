#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>
#include <Warp/SyntaxAnalysis/LogicalExpression.hpp>
#include <CppUTest/TestHarness.h>
#define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK
#include <Warp/Testing/ConstantTests.hpp>
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

