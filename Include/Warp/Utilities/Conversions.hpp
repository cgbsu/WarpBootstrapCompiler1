#include <Warp/Common.hpp>
#include <Warp/Utilities/Math.hpp>
#include <Warp/Utilities/Strings.hpp>
#include <Warp/Utilities/TemplateUtilities.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP
namespace Warp::Utilities
{
	std::string_view to_string_view(auto from) {
		return std::string_view{from};
	}

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
    constexpr ParameterType to_integral(
			std::convertible_to<std::string_view> auto integer_token
		)
    {
		auto integer_token_as_string_view = std::string_view{integer_token};
        ParameterType sum = ParameterType{0};
        for(auto digit : integer_token_as_string_view)
            sum = (sum * BaseParameterConstant) + digit - CharacterOffsetParameterConstant;
        return sum;
    }
	
	struct CharacterRange {
		const char begin, end;
		const size_t offset;
	};

	template<
			std::integral ParameterType, 
			auto BaseParameterConstant, 
			auto... CharacterOffsetAndRangesParameterConstants 
		>
	constexpr ParameterType above_base_10_to_integral(std::string_view integer_token)
	{
		const std::array character_ranges{CharacterOffsetAndRangesParameterConstants...};
		ParameterType sum = ParameterType{0};
		for(auto digit : integer_token)
		{
			for(auto& range : character_ranges)
			{
				if(digit >= range.begin && digit <= range.end)
				{
					sum = (sum * BaseParameterConstant) 
							+ (digit - range.begin + range.offset);
					break;
				}
			}
		}
		return sum;
	}
	template<std::integral ParameterType>
	constexpr ParameterType base_16_to_integral(std::string_view integer_token)
	{
		return above_base_10_to_integral<
					ParameterType, 
					16, 
					CharacterRange{'0', '9', 0}, 
					CharacterRange{'a', 'f', 10}, 
					CharacterRange{'A', 'F', 10}
			>(integer_token);
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
			FixedString TrueTokenParameterConstant = FixedString{"true"}, 
			FixedString FalseTokenParameterConstant = FixedString{"false"}
		>
    constexpr std::optional<bool> to_bool(std::string_view boolean_token)
    {
		constexpr const std::string_view true_token{TrueTokenParameterConstant.string}; 
		constexpr const std::string_view false_token{FalseTokenParameterConstant.string}; 
        return ((boolean_token == true_token) 
				? std::optional{true} 
				: ((boolean_token == false_token) ? std::optional{false} : std::nullopt)
            );
    }

    template<typename ParameterType>
    constexpr std::string to_string(ParameterType to_stringify) {
        return std::string{std::string_view{to_stringify}};
    }

    constexpr std::string to_string(char to_stringify) {
		char string[2] = {to_stringify, '\0'};
        return std::string{std::string_view{string}};
    }

	struct Characters {
		const std::string string;
		constexpr Characters(std::initializer_list<char> characters) noexcept : string(std::data(characters)) {}
	};

	template<>
	constexpr std::string to_string<Characters>(Characters characters) {
		return characters.string;
	}

    template<typename ParameterType>
    concept EnumarationConcept = std::is_enum<ParameterType>::value;

    template<EnumarationConcept EnumerationParameterType >
    constexpr std::string to_string(EnumerationParameterType to_stringify) {
        using UnderylingType = std::underlying_type_t<decltype(to_stringify)>;
        return to_string(static_cast<UnderylingType>(to_stringify));
    }

    constexpr char to_char(char from) {
        return from;
    }

    template< 
            std::integral auto BaseParameterConstant = 10u, 
            char BaseStringParameterConstant = '0', 
            std::unsigned_integral auto RecursionMaxParameterConstant = 850u
        >
    constexpr std::string integral_to_string_implementation( 
            const std::integral auto to_stringify, 
            const std::integral auto number_of_digits, 
            const auto... string_digits 
        ) noexcept 
    {
        if constexpr(RecursionMaxParameterConstant > 0)
        {
			using IntegralType = decltype(to_stringify);
            if(to_stringify < BaseParameterConstant)
			{
				return to_string(Characters{
						static_cast<char>(string_digits)..., 
						'\0'
					});
			}

            const auto raised = raise(BaseParameterConstant, number_of_digits+1);
            const auto high_number = BaseParameterConstant * static_cast<IntegralType>( 
                    to_stringify / (BaseParameterConstant * raised) 
                );
            const char digit = static_cast<IntegralType>(to_stringify / raised) - high_number;

            return integral_to_string_implementation<
                    BaseParameterConstant, 
                    BaseStringParameterConstant, 
                    RecursionMaxParameterConstant - 1
                >(
                    to_stringify - (high_number * raised), 
                    number_of_digits - 1, 
                    string_digits..., digit + BaseStringParameterConstant
                );

        }
        else
		{
			return to_string(Characters{
					static_cast<char>(string_digits)..., 
					static_cast<char>(to_stringify + BaseStringParameterConstant), 
					'\0'
				});
		}
    }

    template< 
            std::integral auto BaseParameterConstant = 10u, 
            char BaseStringParameterConstant = '0', 
            std::unsigned_integral auto RecursionMaxParameterConstant = 90u
        >
    constexpr std::string integral_to_string(const std::integral auto to_stringify)
    {
		if(to_stringify < BaseParameterConstant)
			return to_string(Characters{static_cast<char>(to_stringify + BaseStringParameterConstant), '\0'});
        const auto number_of_digits = log<
				BaseParameterConstant, 
				0u, 
				RecursionMaxParameterConstant
			>(to_stringify);
        return integral_to_string_implementation< 
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

	template<
			std::unsigned_integral WholeParameterType, 
			typename FixedPointParameterType, 
			std::unsigned_integral auto BaseParameterConstant = 10u
		>
	constexpr auto to_fixed_point_integral(
			const std::unsigned_integral auto major_value, 
			const std::unsigned_integral auto minor_value
		)
	{
		using FixedPointType = FixedPointParameterType;
		using WholeType = WholeParameterType;
		FixedPointType major_fixed{major_value};
		const auto minor_string = integral_to_string(minor_value);
		const size_t minor_value_length = minor_string.size();
		FixedPointType denomonator{raise(BaseParameterConstant, minor_value_length)};
		return FixedPointType{major_fixed + (FixedPointType{minor_value} / denomonator)};
	}

	//template<typename ParameterType>
	//using CleanType = std::remove_pointer_t<std::decay_t<ParameterType>>;

	template<typename LeftParameterType, typename RightParameterType>
	concept ComparibleConcept = std::is_same_v<LeftParameterType, RightParameterType>
			|| requires(LeftParameterType left, RightParameterType right) { left == right; }
			|| requires(LeftParameterType left) { { left } -> std::convertible_to<RightParameterType>; }
			|| requires(RightParameterType right) { { right } -> std::convertible_to<LeftParameterType>; };

	template<auto LeftParamterConstant, auto RightParameterConstant>
	consteval bool equal_if_comparible()
	{
		if constexpr(ComparibleConcept<
				CleanType<decltype(LeftParamterConstant)>, 
				CleanType<decltype(RightParameterConstant)>
			>)
			return (LeftParamterConstant == RightParameterConstant);
		return false;
	}

	template<size_t LengthParameterConstant, typename ElementParameterType, size_t... IndexParameterConstants>
	std::array<ElementParameterType, LengthParameterConstant> to_array(
			std::index_sequence<IndexParameterConstants...>, 
			const std::initializer_list<ElementParameterType>& initializers
		)
	{
		std::array<ElementParameterType, LengthParameterConstant> new_array{
				((*(initializers.begin() + IndexParameterConstants)), ...)
			};
		return new_array;
	}

	template<size_t LengthParameterConstant, typename ElementParameterType>
	std::array<ElementParameterType, LengthParameterConstant> to_array(
			const std::initializer_list<ElementParameterType>& initializers)
	{
		return to_array<LengthParameterConstant, ElementParameterType>(std::make_index_sequence<LengthParameterConstant>(), initializers);
		//struct Builder
		//{
		//	std::array<ElementParameterType, LengthParameterConstant> new_array;
		//	constexpr Builder(const std::initializer_list<ElementParameterType>& initializers) noexcept
		//			: new_array{} {
		//		std::copy(initializers.begin(), initializers.end(), std::begin(new_array));
		//	}
		//};
		////std::array<ElementParameterType, LengthParameterConstant> new_array{};
		//return Builder{initializers}.new_array;
	}

	template<typename ParameterType>
	struct Zero
	{
		using Type = ParameterType;
		constexpr Zero(Type) noexcept {}
		constexpr static const auto zero = 0;
	};

	template<
			size_t WholePartBitsParameterConstant, 
			size_t DecimalPartBitsParameterConstant
		>
	struct Zero<
			numeric::fixed<
					WholePartBitsParameterConstant, 
					DecimalPartBitsParameterConstant
				>
		>
	{
		using Type = numeric::fixed<
				WholePartBitsParameterConstant, 
				DecimalPartBitsParameterConstant
			>;
		constexpr Zero(Type) noexcept {}
		constexpr static const auto zero = Type{0};
	};

	template<typename ParameterType>
	constexpr static const auto zero = Zero<ParameterType>::zero;


	template<typename ToParameterType, typename FromParameterType>
	requires(std::derived_from<ToParameterType, FromParameterType> == true)
	constexpr static auto dynamic_cast_unique(
			std::unique_ptr<FromParameterType> from
		) -> std::unique_ptr<ToParameterType>
	{
		return std::unique_ptr<ToParameterType>(
				dynamic_cast<ToParameterType*>(from.release())
			);
	}

	template<typename ToParameterType, typename FromParameterType>
	requires(std::derived_from<ToParameterType, FromParameterType> == true)
	constexpr static auto cast_unique_object(
				const std::unique_ptr<FromParameterType>& from
			) -> const ToParameterType& {
		return dynamic_cast<const ToParameterType&>(*from.get());
	}

	template<typename ToParameterType, typename FromParameterType>
	requires(std::derived_from<FromParameterType, ToParameterType> == true)
	constexpr static auto base_cast(
			std::unique_ptr<FromParameterType> from
		) -> std::unique_ptr<ToParameterType>
	{
		return std::unique_ptr<ToParameterType>(
				static_cast<ToParameterType*>(from.release())
			);
	}

	template<typename ToParameterType, typename FromParameterType>
	requires(std::derived_from<FromParameterType, ToParameterType> == true)
	constexpr static auto make_derived_unique(
			auto... initializors
		) -> std::unique_ptr<ToParameterType>
	{
		return std::move(base_cast<ToParameterType, FromParameterType>(
				std::make_unique<FromParameterType>(initializors...)
			));
	}
}
#endif // WARP__UTILITIES__HEADER__UTILITIES__CONVERSIONS__HPP

