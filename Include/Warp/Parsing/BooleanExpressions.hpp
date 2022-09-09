#define WARP__PARSING__HEADER__PARSING__MATHEMATICAL__EXPRESSIONS__HPP__DONT__UNDEF
#include <Warp/Parsing/MathematicalExpressions.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP
#define WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP

namespace Warp::Parsing
{
	enum class BooleanExpression {
		And, 
		Or, 
		Not, 
		Expression
	}

	using BooleanExpressionTermsType_ = MathematicalExpressionTermsType::AddOnePriority< 
					TreeTerm< 
							BooleanExpression::FixedMark,  
							StringTerm,  
							FixedString{"&&"},  
							ctpg::associativity::no_assoc 
						>,  
					TreeTerm< 
							BooleanExpression::FixedMark,  
							StringTerm,  
							FixedString{"||"},  
							ctpg::associativity::no_assoc 
						>,  
					TreeTerm< 
							BooleanExpression::Not,  
							CharTerm,  
							'!',  
							ctpg::associativity::no_assoc 
						>,  
					TypeTreeTerm< 
							BooleanExpression::Expression 
							NonTerminalTerm,  
							SyntaxNode,  
							FixedString{"BooleanLogicExpression"} 
						> 
				>;

	template<>
	struct TemplateInstantiator<TemplateInstantiationTag::BooleanTerms> {
		using Type = BooleanExpressionTermsType_; 
		Type terms;
	};

	extern template class TemplateInstantiator<TemplateInstantiationTag::BooleanTerms>;

}

#endif // WARP__PARSING__HEADER__PARSING__BOOLEAN__EXPRESSIONS__HPP

