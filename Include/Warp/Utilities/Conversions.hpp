#include <Warp/Common.hpp>
#include <Warp/Utilities/Math.hpp>
#include <Warp/Utilities/Strings.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP
namespace Warp::Utilities
{
	template<typename ToParameterType>
	struct FromString
	{
		constexpr static ToParameterType from_string(std::string_view from) {
			return from;
		}
	};
	
    template<
			std::integral ParameterType, 
			auto BaseParameterConstant = 10, 
			auto CharacterOffsetParameterConstant = '0'
		>
    constexpr ParameterType to_integral(std::string_view integer_token)
    {
        ParameterType sum = 0;
        for(auto digit : integer_token)
            sum = (sum * BaseParameterConstant) + digit - CharacterOffsetParameterConstant;
        return sum;
    }

	template<NumericConcept NumericParameterType>
	struct FromString<NumericParameterType>
	{
		constexpr static NumericParameterType from_string(std::string_view from) 
		{
			NumericParameterType value;
			std::from_chars(from.begin(), from.end(), value);
			return value;
		}
	};

	template<typename ToParameterType>
	constexpr auto from_string(std::string_view from) -> ToParameterType {
		return FromString<ToParameterType>::from_string(from);
	}


	template<
			FixedString TrueTokenParameterConstant = FixedString("true"), 
			FixedString FalseTokenParameterConstant = FixedString{"false"}
		>
    constexpr std::optional<bool> to_bool(std::string_view boolean_token)
    {
		constexpr static const std::string_view true_token{TrueTokenParameterConstant}; 
		constexpr static const std::string_view false_token{FalseTokenParameterConstant}; 
        return ((boolean_token == true_token) 
				? std::optional{true} 
				: ((boolean_token == false_token) ? std::optional{false} : std::nullopt)
            );
    }


    template<typename ParameterType>
    constexpr std::string to_string(ParameterType to_stringify) {
        return std::string{std::string_view{to_stringify}.data()};
    }

	template<>
	constexpr std::string to_string(std::initializer_list<char> characters) {
		const char characher_array[sizeof...(characters) + 1] = { characters..., '\0' };
		return std::string{&characher_array};
	}

    template<typename ParameterType>
    concept EnumarationConcept = std::is_enum<ParameterType>::value;

    template<EnumarationConcept EnumerationParameterType >
    constexpr std::string to_string(EnumerationParameterType to_stringify) {
        using UnderylingType = std::underlying_type_t<decltype(to_stringify)>;
        return to_string(static_cast<UnderylingType>(to_stringify));
    }

    constexpr char to_char( char from ) {
        return from;
    }

    template< 
            std::integral IntegralParameterType, 
            std::integral auto BaseParameterConstant = 10, 
            char BaseStringParameterConstant = '0', 
            size_t RecursionMaxParameterConstant = 850 
        >
    constexpr std::string integral_to_string_implementation( 
            const IntegralParameterType to_stringify, 
            const std::integral auto number_of_digits, 
            const auto... string_digits 
        ) noexcept 
    {
        if constexpr(RecursionMaxParameterConstant > 0)
        {
            if(to_stringify < BaseParameterConstant)
                return std::string{TemplateString<string_digits..., to_stringify + BaseStringParameterConstant, '\0'>::array};

            const size_t raised = raise_constexpr(BaseParameterConstant, number_of_digits);
            const auto high_number = BaseParameterConstant * static_cast<const IntegralParameterType>( 
                    to_stringify / (BaseParameterConstant * raised) 
                );
            const char digit = static_cast<const IntegralParameterType>(to_stringify / raised) - high_number;

            return integral_to_string_implementation<
                    IntegralParameterType, 
                    BaseParameterConstant, 
                    BaseStringParameterConstant, 
                    RecursionMaxParameterConstant - 1
                >(
                    to_stringify - (high_number * raised), 
                    number_of_digits - 1, 
                    string_digits..., digit + BaseStringParameterConstant
                );

        }
        else {
            return std::string{TemplateString<
					string_digits..., 
					to_stringify + BaseStringParameterConstant, 
					'\0'
				>::array};
		}
    }

    template< 
            std::integral IntegralParameterType, 
            std::integral auto BaseParameterConstant = 10, 
            char BaseStringParameterConstant = '0', 
            size_t RecursionMaxParameterConstant = 90  
        >
    constexpr std::string integral_to_string( IntegralParameterType to_stringify )
    {
        const auto number_of_digits = log<
				IntegralParameterType, 
				BaseParameterConstant, 
				0, 
				RecursionMaxParameterConstant
			>(to_stringify);
        return integral_to_string_implementation< 
                IntegralParameterType, 
                BaseParameterConstant, 
                BaseStringParameterConstant, 
                RecursionMaxParameterConstant 
            >(to_stringify, number_of_digits);
    }


    template<std::integral IntegralParameterType>
    constexpr std::string to_string(IntegralParameterType to_stringify) {
        return integral_to_string(to_stringify);
    }

    // https://cp-algorithms.com/string/string-hashing.html //
    // https://stackoverflow.com/questions/1835976/what-is-a-sensible-prime-for-hashcode-calculation/2816747# //

    using HashedStringType = unsigned long long int;

    template< 
            HashedStringType BaseParameterConstant = 92821, 
            HashedStringType TableSizeOrderParameterConstant = 486187739, 
            size_t OffsetParameterConstant = 'a' + 1 
        >
    constexpr HashedStringType hash_string(std::string_view to_hash)
    {
        HashedStringType power = BaseParameterConstant;
        HashedStringType hash = 0;
        for(size_t ii = 0; ii < to_hash.size(); ++ii)
        {
            hash += (((to_hash[ii] - OffsetParameterConstant) * power) 
                    % TableSizeOrderParameterConstant
                );
            power *= BaseParameterConstant;
        }
        return hash;
    }
}
#endif // WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP

