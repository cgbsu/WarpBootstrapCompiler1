#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__VARIANT__VIEW__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__VARIANT__VIEW__HPP

namespace Warp::Utilities
{
	template<
	        typename BaseParameterType, 
	        template<auto> typename ChildParameterTemplate, 
	        auto... AlternativeTagParameterConstants
	    >
	struct VariantView
	{
		using BaseType = BaseParameterType;
	    template<auto Tag>
	    using ChildType = ChildParameterTemplate<Tag>;
	    template<auto Tag>
	    explicit VariantView(ChildParameterTemplate<Tag>* alternative) : alternative(alternative) {}
	    std::any get_alternative_tag() const {
	        return alternative_tag;
	    }
	    size_t get_alternative_index() const {
	        return alternative_index;
	    }
	    const BaseType* const get_alternative() const {
	        return alternative;
	    }
	    protected: 
	        std::any alternative_tag;
	        size_t alternative_index;
	        BaseType* alternative;
	};
	
	template<
	        typename ReturnParameterType, 
	        auto VisitorParameterConstant, 
	        typename BaseParameterType, 
	        template<auto> typename ChildParameterTemplate, 
	        auto CurrentAlternativeParameterConstant, 
	        auto... AlternativeTagParameterConstants, 
	        auto... AllAlternativeTagParameterConstants
	    >
	auto visit_implementation(
	        size_t current_alternative_index, 
	        const VariantView<
					BaseParameterType, 
					ChildParameterTemplate, 
					AllAlternativeTagParameterConstants...
				>& view, 
	        auto... additional_arguments
	    ) -> std::optional<ReturnParameterType>
	{
	    using VariantType = VariantView<
				BaseParameterType, 
				ChildParameterTemplate, 
				AllAlternativeTagParameterConstants...
			>;

	    if(current_alternative_index == view.get_alternative_index())
	    {
	        return VisitorParameterConstant(
	                static_cast<
	                        typename VariantType::ChildType<
									CurrentAlternativeParameterConstant
								> const*
	                    >(view.get_alternative()),
	                additional_arguments...
	            );
	    }
	    else
	    {
	        if constexpr(sizeof...(AlternativeTagParameterConstants) > 0)
	        {
	            return visit_implementation<
	                    ReturnParameterType, 
	                    VisitorParameterConstant, 
	                    BaseParameterType, 
	                    ChildParameterTemplate, 
	                    AlternativeTagParameterConstants...
	                >(
	                        current_alternative_index + 1, 
	                        view, 
	                        additional_arguments...
	                    );
	        }
	        else
	            return std::nullopt;
	    }
	}
	
	
	template<
	        typename ReturnParameterType, 
	        auto VisitorParameterConstant, 
	        typename BaseParameterType, 
	        template<auto> typename ChildParameterTemplate, 
	        auto... AlternativeTagParameterConstants
	    >
	auto visit(
	        const VariantView<
					BaseParameterType, 
					ChildParameterTemplate, 
					AlternativeTagParameterConstants...
				>& view, 
	        auto... additional_arguments
	    ) -> std::optional<ReturnParameterType>
	{
	    return visit_implementation< 
	                ReturnParameterType, 
	                VisitorParameterConstant, 
	                BaseParameterType, 
	                ChildParameterTemplate, 
	                AlternativeTagParameterConstants...
	        >(0, view, additional_arguments...);
	}
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__VARIANT__VIEW__HPP

