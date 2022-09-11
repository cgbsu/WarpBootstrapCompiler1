#include <Warp/Parsing/TermWrappers.hpp>
#include <Warp/Parsing/Terms.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/Runtime/Compiler/NumericType.hpp>
#include <Warp/Parsing/GeneralTermTags.hpp>

#ifndef WARP__PARSING__HEADER__PARSING__TYPE__TERMS__HPP
#define WARP__PARSING__HEADER__PARSING__TYPE__TERMS__HPP

namespace Warp::Parsing
{
	
	using namespace Warp::Runtime::Compiler;
	using TypeTerms_ = MakeTerms<
			TypeTreeTerm<
					NumericTypeTag::FixedPoint, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Whole, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Whole>::Type, 
					FixedString{"Whole"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Integer, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Integer>::Type, 
					FixedString{"Integer"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::FixedPoint, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::FixedPoint>::Type, 
					FixedString{"FixedPoint"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Character, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Character>::Type, 
					FixedString{"Character"}
				>, 
			TypeTreeTerm<
					NumericTypeTag::Bool, 
					NonTerminalTerm, 
					NumericTypeResolver<NumericTypeTag::Bool>::Type, 
					FixedString{"Bool"}
				>
		>;

		template<>
		struct TemplateInstantiator<TemplateInstantiationTag::TypeTerms> {
			using Type = TypeTerms_;
			Type terms;
		};

		#ifdef WARP__PARSING__ENABLE__TEMPLATE__CACHING
			extern template class TemplateInstantiator<TemplateInstantiationTag::TypeTerms>;
		#endif
		
		using TypeTerms = TemplateInstantiator<TemplateInstantiationTag::TypeTerms>::Type;
}

#endif // WARP__PARSING__HEADER__PARSING__TYPE__TERMS__HPP

