#include <Warp/Common.hpp>


#ifndef WARP__PARSING__HEADER__PARSING__GENERAL__TERM__TAGS__HPP
#define WARP__PARSING__HEADER__PARSING__GENERAL__TERM__TAGS__HPP
namespace Warp::Parsing
{
	enum class Brackets
	{
		OpenParenthesis, 
		CloseParenthesis, 
		OpenCurleyBracket, 
		ClosedCurleyBracket, 
		OpenSquareBracket, 
		ClosedSquareBracket, 
		OpenAngleBracket, 
		ClosedAngleBracket
	};

	enum class Keyword {
		Let
	};

	enum class MultiPurposeOperator {
		Equal
	};

	enum class Identifier {
		Identifier//, 
		//Meta
	};

	enum class Declaration {
		SemiColon, 
		Colon
	};

	enum class Construct {
		Constant, 
		Context
	};

	enum class Call
	{
		Constant, 
		Comma, 
		Function, 
		IntermediateFunction
	};

	enum class TemplateInstantiationTag
	{
		TypeTerms, 
		NumericTerms, 
		MathematicalExpressionTerms, 
		FunctionDeclarationTerms,	
		BooleanTerms
	};

	template<TemplateInstantiationTag TagParameterConstant>
	struct TemplateInstantiator {
		constexpr static const auto tag = TagParameterConstant;
		constexpr static const bool has_value = false;
	};

	//extern template class TemplateInstantiator<TemplateInstantiationTag::TypeTerms>;
	//extern template class TemplateInstantiator<TemplateInstantiationTag::NumericTerms>;
	//extern template class TemplateInstantiator<TemplateInstantiationTag::MathematicalExpressionTerms>;
	//extern template class TemplateInstantiator<TemplateInstantiationTag::FunctionDeclarationTerms>;
	//extern template class TemplateInstantiator<TemplateInstantiationTag::BooleanTerms>;
}
#endif // WARP__PARSING__HEADER__PARSING__GENERAL__TERM__TAGS__HPP

