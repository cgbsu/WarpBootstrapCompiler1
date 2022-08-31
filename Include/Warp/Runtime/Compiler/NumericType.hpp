#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__NUMERIC__TYPE__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__NUMERIC__TYPE__HPP

namespace Warp::Runtime::Compiler
{
	using namespace Warp::Utilities;

	enum class NumericTypeTag
	{
		Whole, 
		Integer, 
		FixedPoint, 
		Character, 
		Bool
	};

	enum class WarpBool : unsigned char {
		True = 1, 
		False = 0 
	};

	constexpr WarpBool to_warp_bool(const bool from) {
		return from == true ? WarpBool::True : WarpBool::False;
	}

	template<NumericTypeTag ParameterTypeConstant, typename NumericParameterType>
	struct NumericType
	{
		using UnderylingType = NumericParameterType;
		constexpr static const NumericTypeTag type = ParameterTypeConstant;
		using ThisType = NumericType<type, UnderylingType>;

		constexpr static const bool fixed_point_type = false;
		constexpr static const size_t default_bits = sizeof(UnderylingType) * CHAR_BIT;

		UnderylingType number; // This would be const, but CTPG requires it this object be writable. //
		size_t bits = default_bits;


		constexpr NumericType() noexcept 
				: number(0), bits(default_bits) {}
		constexpr NumericType(const UnderylingType number, size_t bits = default_bits) noexcept 
				: number(number), bits(bits) {}
		constexpr NumericType(const std::string_view numeric_string_representation, size_t bits = default_bits) noexcept 
				: number(to_integral<UnderylingType>(numeric_string_representation)), bits(bits) {}

		constexpr NumericType(const NumericType& other) noexcept = default;
		constexpr NumericType(NumericType&& other) noexcept = default;
		constexpr NumericType& operator=(const NumericType& other) noexcept = default;
		constexpr NumericType& operator=(NumericType&& other) noexcept = default;

		constexpr static const auto zero() {
			return ThisType{UnderylingType{0}};
		}

		constexpr std::strong_ordering operator<=>(const UnderylingType& other) const noexcept {
			return number <=> other;
		}

		constexpr operator UnderylingType() const noexcept {
			return number;
		}
		constexpr NumericType operator+(const NumericType& other) const noexcept {
			return number + other.number;
		}
		constexpr NumericType operator*(const NumericType& other) const noexcept {
			return number * other.number;
		}
		constexpr NumericType operator-(const NumericType& other) const noexcept {
			return number - other.number;
		}
		constexpr NumericType operator/(const NumericType& other) const noexcept {
			return number / other.number;
		}
		constexpr NumericType operator-() const noexcept {
			//static_assert(std::is_unsigned_v<UnderylingType> == true);
			return -number;
		}
	};

	template<
			NumericTypeTag ParameterTypeConstant, 
			size_t WholePartBitsParameterConstant, 
			size_t DecimalPartBitsParameterConstant
		>
	struct NumericType<
			ParameterTypeConstant, 
			numeric::fixed<
					WholePartBitsParameterConstant, 
					DecimalPartBitsParameterConstant
				>
		>
	{
		constexpr static const NumericTypeTag type = ParameterTypeConstant;
		constexpr const static auto whole_part_bits = WholePartBitsParameterConstant;
		constexpr const static auto decimal_part_bits = DecimalPartBitsParameterConstant;
		using UnderylingType = numeric::fixed<whole_part_bits, decimal_part_bits>;
		using ThisType = NumericType<type, UnderylingType>;

		constexpr static const bool fixed_point_type = true;
		constexpr static size_t const bits = whole_part_bits + decimal_part_bits; // Constant for now. //

		UnderylingType number; // This would be const, but CTPG requires it this object be writable. //

		constexpr NumericType(const UnderylingType number = UnderylingType{0}) noexcept 
				: number(number) {}
		constexpr NumericType(const std::string_view whole_part, const std::string_view decimal_part) noexcept
				: number(to_fixed_point_integral<size_t, UnderylingType>(
							to_integral<size_t>(whole_part), 
							to_integral<size_t>(decimal_part)
						)) {}
		constexpr NumericType(const std::string_view whole_part) noexcept
				: number(to_fixed_point_integral<size_t, UnderylingType>(to_integral<size_t>(whole_part), 0u)) {}
		constexpr NumericType(const size_t whole_part, const size_t decimal_part) noexcept
				: number(to_fixed_point_integral<size_t, UnderylingType>(whole_part, decimal_part)) {}

		constexpr NumericType(const NumericType& other) noexcept = default;
		constexpr NumericType(NumericType&& other) noexcept = default;
		constexpr NumericType& operator=(const NumericType& other) noexcept = default;
		constexpr NumericType& operator=(NumericType&& other) noexcept = default;

		constexpr static const auto zero() {
			return ThisType{0, 0};
		}

		constexpr std::strong_ordering spaceship_underlying(const UnderylingType& other) const noexcept
		{
			return ((number == other)
					? std::strong_ordering::equal 
					: (number < other) ? std::strong_ordering::less : std::strong_ordering::greater);
		}
		constexpr bool operator==(const NumericType& other) const noexcept {
			return number.to_uint() == other.number.to_uint();
		}
		constexpr bool operator<(const NumericType& other) const noexcept {
			return number.to_uint() < other.number.to_uint();
		}
		constexpr bool operator>(const NumericType& other) const noexcept {
			return number.to_uint() > other.number.to_uint();
		}
		constexpr bool operator<=(const NumericType& other) const noexcept {
			return number.to_uint() <= other.number.to_uint();
		}
		constexpr bool operator>=(const NumericType& other) const noexcept {
			return number.to_uint() >= other.number.to_uint();
		}
		//////////////////////////////////////////////////////////////////////
		constexpr bool operator==(const UnderylingType& other) const noexcept {
			return number.to_uint() == other.to_uint();
		}
		constexpr bool operator<(const UnderylingType& other) const noexcept {
			return number.to_uint() < other.to_uint();
		}
		constexpr bool operator>(const UnderylingType& other) const noexcept {
			return number.to_uint() > other.to_uint();
		}
		constexpr bool operator>=(const UnderylingType& other) const noexcept {
			return number.to_uint() >= other.to_uint();
		}
		constexpr bool operator<=(const UnderylingType& other) const noexcept {
			return number.to_uint() <= other.to_uint();
		}
		constexpr operator UnderylingType() const noexcept {
			return number;
		}
		constexpr NumericType operator+(const NumericType& other) const noexcept {
			return ThisType{number + other.number};
		}
		constexpr NumericType operator*(const NumericType& other) const noexcept {
			return ThisType{number * other.number};
		}
		constexpr NumericType operator-(const NumericType& other) const noexcept {
			return ThisType{number - other.number};
		}
		constexpr NumericType operator/(const NumericType& other) const noexcept {
			return ThisType{number / other.number};
		}
		constexpr NumericType operator-() const noexcept {
			return ThisType{-number};
		}
	};

	template<auto NumericalTypeTag>
	struct NumericTypeResolver {};
	
	template<>
	struct NumericTypeResolver<NumericTypeTag::Whole> {
		using Type = NumericType<NumericTypeTag::Whole, size_t>;
	};

	template<>
	struct NumericTypeResolver<NumericTypeTag::Integer> {
		using Type = NumericType<NumericTypeTag::Integer, long long signed int>;
	};

	template<>
	struct NumericTypeResolver<NumericTypeTag::FixedPoint> {
		using Type = NumericType<NumericTypeTag::FixedPoint, numeric::fixed<16, 16>>;
	};

	template<>
	struct NumericTypeResolver<NumericTypeTag::Character> {
		using Type = NumericType<NumericTypeTag::Character, char>;
	};

	template<>
	struct NumericTypeResolver<NumericTypeTag::Bool> {
		using Type = NumericType<NumericTypeTag::Bool, WarpBool>;
	};

	template<typename>
	struct NumericTagResolver {};

	#define TAG_RESOLVER(TAG) \
			template<> \
			struct NumericTagResolver<NumericTypeResolver<NumericTypeTag:: TAG >::Type::UnderylingType> \
			{ \
				using Type = NumericTypeResolver<NumericTypeTag:: TAG >::Type::UnderylingType; \
				using NumericType = NumericTypeResolver<NumericTypeTag:: TAG >::Type; \
				constexpr static const auto tag = NumericTypeTag:: TAG ; \
				constexpr NumericTagResolver(Type) noexcept {} \
			}

	TAG_RESOLVER(Whole);
	TAG_RESOLVER(Integer);
	TAG_RESOLVER(FixedPoint);
	TAG_RESOLVER(Character);
	TAG_RESOLVER(Bool);

	#undef TAG_RESOLVER

}

namespace Warp::Utilities
{
	using namespace Warp::Runtime::Compiler;


	template<NumericTypeTag ParameterTypeConstant, typename NumericParameterType>
	struct Zero<NumericType<ParameterTypeConstant, NumericParameterType>>
	{
		using Type = NumericType<ParameterTypeConstant, NumericParameterType>;
		constexpr static const auto zero = Type::zero();
		constexpr Zero(Type) noexcept {}
	};

}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__NUMERIC__TYPE__HPP

