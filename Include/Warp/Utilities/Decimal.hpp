#include <Warp/Common.hpp>
#include <Warp/Utilities/Math.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__DECIMAL__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__DECIMAL__HPP

namespace Warp::Utilities
{
	enum class Polarity : bool {
		Positive = true, 
		Negative = false
	};

	template<std::integral NumberParameterType = unsigned long int, 
			std::integral PowerOffsetParameterType = int>
	requires(std::is_unsigned<NumberParameterType>::value == true 
			&& std::is_signed<PowerOffsetParameterType>::value == true)
	struct Decimal
	{
		using NumberType = NumberParameterType;
		using PowerOffsetType = PowerOffsetParameterType;
		using ThisType = Decimal<NumberType, PowerOffsetType>;
		constexpr static const NumberType default_base = 10;
		constexpr static const PowerOffsetType default_power_offset = 0;
		Polarity polarity;
		NumberType coefficent, base;
		PowerOffsetType power_offset;

		Decimal(
				NumberType coefficent, 
				PowerOffsetType power_offset = default_power_offset, 
				Polarity polarity = Polarity::Positive, 
				NumberType base = default_base
			) : coefficent(coefficent), 
				power_offset(power_offset), 
				polarity(polarity), 
				base(base) {}

		constexpr static NumberType calculate_max_power(const Decimal& decimal)
		{
			const NumberType digit_power = log(decimal.coefficent, decimal.base);
			const auto max_power = log(
					std::numeric_limits<NumberType>::max() / decimal.base, 
					decimal.base
				) - digit_power; // MAXIMUM POWER!!!!!!!! //
			return max_power;
		}

		//3*10^5 * 8*10^12 = S?
		//--> 3*10^5 * 8*10^12 = S?
		template<bool SkipConversionParameterConstant = false>
		constexpr static void multiply(Decimal& left, Decimal right)
		{
			if constexpr(SkipConversionParameterConstant == false)
				base_convert(right, left.base);
			left.coefficent *= right.coefficent;
			left.power_offset += right.power_offset;
			left.polarity = static_cast<Polarity>(left.polarity == right.polarity);
		}
		constexpr static void decay(Decimal& left, const Decimal& right)
		{
			//absolute_value(calculate_max_power(numerator).power_offset)
		}
		//(3*10^5)/(8*10^12) = S?
		//= (3/8)(10^(-7))= 
		//
		//Y/X = S
		//= Y*X^(-1) = S
		//
		//X^(-1)... ?
		//
		// (S*q) % (S * p) = 0
		// --> ((b^n)*q) % ((b^n) * p) = 0
		template<bool SkipConversionParameterConstant = false>
		constexpr static void divide(Decimal& numerator, Decimal denomonator)
		{
			if constexpr(SkipConversionParameterConstant == false)
				base_convert(denomonator, numerator.base);
			NumberType scalar_power = PowerOffsetType{0};
			NumberType scalar = NumberType{1};
			const auto max_power = calculate_max_power(numerator);
			std::cout << "Max Power: " << max_power << "\n";
			const auto remainder_check = [&]() { 
				return (numerator.coefficent * scalar)
							% denomonator.coefficent == 0;
			};
			while(remainder_check() == false)
			{
				scalar = raise(numerator.base, ++scalar_power);
				if(scalar_power >= max_power)
					break;
			}
			numerator.coefficent 
					= (static_cast<NumberType>(numerator.coefficent * scalar) 
							/ static_cast<NumberType>(denomonator.coefficent))
							/ ((scalar_power >= max_power) ? NumberType{1} : scalar);
			numerator.power_offset -= static_cast<PowerOffsetType>(scalar_power);
			numerator.power_offset -= denomonator.power_offset;
		}
		//3*10^5 + 8*10^12 = S?
		//--> A + B = S
		//--> 1 + B/A = S/A
		//--> 1 + (8*10^12)/(3*10^5) = S/A
		//--> 1 + (8/3)*10^7 = S/A
		//--> (3*10^5)(1+(8/3)*10^7) = S
		//--> A(1+(B/A)) = S
		template<bool SkipConversionParameterConstant = false>
		constexpr static void add_or_subtract(Decimal& left, Decimal right)
		{
			if constexpr(SkipConversionParameterConstant == false)
				base_convert(right, left.base);
			const Decimal numerator = right.copy();
			auto ratio = divide(numerator, left);
			const auto one_factor = raise(
					left.right, 
					static_cast<NumberType>(absolute_value(right.power_offset))
				);
			ratio.coefficent += (right.power_offset < 0) 
					? static_cast<NumberType>(1 / one_factor) 
					: static_cast<NumberType>(1 * one_factor);
			multiply(ratio, left);
			left = ratio;
			left.polarity = (left.polarity == right.polarity);
		}
		//x^q = y^p
		//--> log(y, x^q) = p
		//-------------------
		//C * x^q = D * y^p
		//--> (C * x^q)/(y^p) = D
		constexpr static void base_convert(Decimal& to_convert, NumberType base)
		{
			if(base != to_convert.base)
			{
				const auto new_power_offset /*p*/ = ((to_convert.power_offset < 0) ? -1 : 1) 
						* static_cast<PowerOffsetType>(log(
							raise(
									to_convert.base, 
									static_cast<NumberType>(absolute_value(to_convert.power_offset))
								), //x^q
							base //y
					));
				auto numerator = to_convert.copy();
				divide(numerator, 
						Decimal{1, new_power_offset, Polarity::Positive, base}
					);
				to_convert.coefficent = numerator.coefficent;
				to_convert.power_offset = new_power_offset;
			}
		}

		constexpr ThisType copy() const {
			return ThisType{coefficent, power_offset, polarity, base};
		}

		constexpr ThisType operator*(const ThisType& other) const
		{
			auto to_multiply = copy();
			multiply(to_multiply, other);
			return to_multiply;
		}

		constexpr ThisType operator/(const ThisType& other) const
		{
			auto numerator = copy();
			divide(numerator, other);
			return numerator;
		}

		constexpr ThisType operator+(const ThisType& other) const
		{
			auto to_add_to = copy();
			add_or_subtract(to_add_to, other);
			return to_add_to;
		}

		constexpr ThisType operator-() const
		{
			auto to_negate = copy();
			to_negate.polarity 
					= static_cast<Polarity>(static_cast<bool>(to_negate.polarity));
			return to_negate;
		}

		constexpr ThisType operator-(const ThisType& other) const
		{
			auto to_subtract_from = copy();
			add_or_subtract(to_subtract_from, -other);
			return to_subtract_from;
		}


		std::strong_ordering operator<=>(const ThisType& other) const = default;

		//friend std::ostream& operator<<(std::ostream&, const ThisType&);
	};
	template<std::unsigned_integral NumberParameterType, std::integral PowerParameterConstant>
	std::ostream& operator<<(std::ostream& output_stream, 
			const Decimal<NumberParameterType, PowerParameterConstant>& to_output)
	{
		output_stream << "Decimal {\n"
				<< "\tcoefficent: " << to_output.coefficent << "\n"
				<< "\tpower_offset: " << to_output.power_offset << "\n"
				<< "\tbase: " << to_output.base << "\n"
				<<"}\n";
		return output_stream;
	}

	using DefaultDecimalType = Decimal<unsigned long long int, int>;
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__DECIMAL__HPP

