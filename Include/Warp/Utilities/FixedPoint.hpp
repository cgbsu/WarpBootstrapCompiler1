#include <Warp/Utilities/Math.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__FIXED__POINT__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__FIXED__POINT__HPP

namespace Warp::Utilities
{
	template<
			std::unsigned_integral NumberParameterType, 
			std::unsigned_integral auto PrecisionParameterConstant, 
			std::unsigned_integral auto BaseParameterConstant = 10u
		>
	struct FixedPoint
	{
		using NumberType = NumberParameterType;
		constinit const static auto precision = PrecisionParameterConstant;
		constinit const static auto base = BaseParameterConstant;
		using PrecisionType = decltype(precision);
		using BaseType = decltype(base);
		using ThisType = FixedPoint<NumberType, precision, base>;

		static_assert(sizeof(NumberType) > precision);

		constinit static const auto bytes_of_decimal_precision = sizeof(NumberType) - precision;
		constinit static const auto bytes_of_whole_precision 
				= sizeof(NumberType) - bytes_of_decimal_precision;
		struct alignas(NumberType) Number {
				NumberType whole_part : bytes_of_whole_precision;
				NumberType decimal_part : bytes_of_decimal_precision;
		};

		constinit static const auto maximum_whole_digits 
				= base_convert_power<8u, bytes_of_whole_precision, base>();
		constinit static const auto maximum_decimal_digits 
				= base_convert_power<8u, bytes_of_decimal_precision, base>();
		constinit static const auto maximum_whole 
				= raise_constexpr<base, maximum_whole_digits>;
		constinit static const auto maximum_decimal
				= raise_constexpr<base, maximum_decimal_digits>;
		constinit static const auto maximum = Number{maximum_whole, maximum_decimal};

		const Number number;
		const Polarity polarity;

		constexpr FixedPoint(
				const Number number, 
				const Polarity polarity = Polarity::Positive) noexcept 
				: number(number), polarity(polarity) {}

		constexpr const std::unsigned_integral auto count_whole_digits() {
			return log(number.whole_part, base);
		}

		constexpr const std::unsigned_integral auto count_decimal_digits() {
			return log(number.decimal_part, base);
		}

		constexpr const FixedPoint add(const ThisType other)
		{
			return (static_cast<NumberType>(number) 
					+ static_cast<NumberType>(other.number)) 
					% static_cast<NumberType>(maximum);
		}
		constexpr const FixedPoint multiply(const ThisType other)
		{
			return (static_cast<NumberType>(number) 
					* static_cast<NumberType>(other.number)) 
					% static_cast<NumberType>(maximum);
		}
		constexpr const FixedPoint divide(const ThisType other)
		{
			NumberType scalar = 1;
			constexpr const auto whole_digits = count_whole_digits();
			constexpr const auto other_whole_digits = other.count_whole_digits();
			constexpr const auto decimal_digits = count_decimal_digits();
			constexpr const auto other_decimal_digits = other.count_decimal_digits();
			if constexpr(whole_digits < other_whole_digits)
				scalar = raise(base, other_whole_digits - whole_digits);
			else if constexpr(decimal_digits < other_decimal_digits)
				scalar = raise(base, other_decimal_digits - decimal_digits);
			return (((static_cast<NumberType>(number) * scalar)
					* static_cast<NumberType>(other.number)) 
					% static_cast<NumberType>(maximum)) / scalar;
		}
	};
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__FIXED__POINT__HPP

