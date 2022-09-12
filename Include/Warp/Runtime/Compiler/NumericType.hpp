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

	static WarpBool operator&&(const WarpBool& left, const WarpBool& right) {
		return (left == WarpBool::True && right == WarpBool::True)
				? WarpBool::True
				: WarpBool::False;
	}

	static WarpBool operator||(const WarpBool& left, const WarpBool& right) {
		return (left == WarpBool::True || right == WarpBool::True)
				? WarpBool::True
				: WarpBool::False;
	}

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
				: number(zero().number), bits(default_bits) {}
		constexpr NumericType(const UnderylingType number, size_t bits = default_bits) noexcept 
				: number(number), bits(bits) {}
		constexpr NumericType(const std::string_view numeric_string_representation, size_t bits = default_bits) noexcept 
				: number(to_integral<UnderylingType>(numeric_string_representation)), bits(bits) {}

		constexpr NumericType(const NumericType& other) noexcept = default;
		constexpr NumericType(NumericType&& other) noexcept = default;
		constexpr NumericType& operator=(const NumericType& other) noexcept = default;
		constexpr NumericType& operator=(NumericType&& other) noexcept = default;

		constexpr static const ThisType zero() {
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
		constexpr NumericType operator||(const NumericType& other) const noexcept {
			return (*this + other);
		}
		constexpr NumericType operator&&(const NumericType& other) const noexcept {
			return (*this * other);
		}
		constexpr NumericType operator!() const noexcept {
			return ThisType{-number};
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

		constexpr static const ThisType zero() {
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
		constexpr NumericType operator||(const NumericType& other) const noexcept {
			return (*this + other);
		}
		constexpr NumericType operator&&(const NumericType& other) const noexcept {
			return (*this * other);
		}
		constexpr NumericType operator!() const noexcept {
			return ThisType{-number};
		}
	};

	template<>
	struct NumericType<NumericTypeTag::Bool, WarpBool>
	{
		using UnderylingType = WarpBool;
		constexpr static const NumericTypeTag type = NumericTypeTag::Bool;
		using ThisType = NumericType<type, UnderylingType>;

		constexpr static const bool fixed_point_type = false;
		constexpr static const size_t default_bits = sizeof(UnderylingType) * CHAR_BIT;

		UnderylingType number; // This would be const, but CTPG requires it this object be writable. //
		size_t bits = default_bits;


		constexpr NumericType() noexcept 
				: number(zero().number), bits(default_bits) {}
		constexpr NumericType(const bool number, size_t bits = default_bits) noexcept 
				: number(to_warp_bool(number)), bits(bits) {}
		constexpr NumericType(WarpBool number) noexcept 
				: number(number), bits(default_bits) {}
		constexpr NumericType(const std::string_view numeric_string_representation, size_t bits = default_bits) noexcept 
				: number(to_warp_bool(to_bool(numeric_string_representation).value())), bits(bits) {}

		constexpr NumericType(const NumericType& other) noexcept = default;
		constexpr NumericType(NumericType&& other) noexcept = default;
		constexpr NumericType& operator=(const NumericType& other) noexcept = default;
		constexpr NumericType& operator=(NumericType&& other) noexcept = default;

		constexpr static const ThisType zero() {
			return ThisType{WarpBool::False};
		}

		constexpr std::strong_ordering operator<=>(
				const UnderylingType& other) const noexcept {
			return number <=> other;
		}

		constexpr operator UnderylingType() const noexcept {
			return number;
		}
		constexpr static NumericType or_operation(
				const NumericType& left, 
				const NumericType& right
			) noexcept
		{
			return ((left.number == WarpBool::True) || (right.number == WarpBool::True))
					? WarpBool::True
					: WarpBool::False;
		}

		constexpr static NumericType and_operation(
				const NumericType& left, 
				const NumericType& right
			) noexcept
		{
			return ((left.number == WarpBool::True) && (right.number == WarpBool::True))
					? WarpBool::True
					: WarpBool::False;
		}

		constexpr static NumericType not_operation(
				const NumericType& other) noexcept
		{
			return (other.number == WarpBool::True)
					? WarpBool::False
					: WarpBool::True;
		}

		constexpr NumericType operator||(const NumericType& other) const noexcept {
			return or_operation(*this, other);
		}
		constexpr NumericType operator&&(const NumericType& other) const noexcept {
			return and_operation(*this, other);
		}
		constexpr NumericType operator+(const NumericType& other) const noexcept {
			return or_operation(*this, other);
		}
		constexpr NumericType operator*(const NumericType& other) const noexcept {
			return and_operation(*this, other);
		}
		constexpr NumericType operator-(const NumericType& other) const noexcept {
			return or_operation(*this, not_operation(other));
		}
		constexpr NumericType operator/(const NumericType& other) const noexcept {
			return and_operation(*this, not_operation(other));
		}
		constexpr NumericType operator-() const noexcept {
			return not_operation(*this);
		}
		constexpr NumericType operator!() const noexcept {
			return not_operation(*this);
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

	using WholeType = NumericTypeResolver<NumericTypeTag::Whole>::Type;
	using IntegerType = NumericTypeResolver<NumericTypeTag::Integer>::Type;
	using CharacterType = NumericTypeResolver<NumericTypeTag::Character>::Type;
	using FixedPointType = NumericTypeResolver<NumericTypeTag::FixedPoint>::Type;
	using BoolType = NumericTypeResolver<NumericTypeTag::Bool>::Type;

	using NumericVariantType = std::variant<
			WholeType, 
			IntegerType, 
			CharacterType, 
			FixedPointType, 
			BoolType
		>;

	struct NumericValue
	{
		std::optional<NumericVariantType> number;

		NumericValue() noexcept : number(std::nullopt) {}
		NumericValue(NumericVariantType number) noexcept : number(std::optional{number}) {}
		NumericValue(std::optional<NumericVariantType> number) noexcept : number(number) {}

		//template<NumericTypeTag NumericTypeParameterConstant>
		//constexpr NumericValue(NumericTypeResolver<NumericTypeParameterConstant>::Type number) noexcept : number(std::optional{NumericVariantType{number}}) {}

		NumericValue(WholeType number) noexcept : number(std::optional{NumericVariantType{number}}) {}
		NumericValue(IntegerType number) noexcept : number(std::optional{NumericVariantType{number}}) {}
		NumericValue(CharacterType number) noexcept : number(std::optional{NumericVariantType{number}}) {}
		NumericValue(FixedPointType number) noexcept : number(std::optional{NumericVariantType{number}}) {}
		NumericValue(BoolType number) noexcept : number(std::optional{NumericVariantType{number}}) {}

		NumericValue(const NumericValue& other) noexcept = default;
		NumericValue(NumericValue&& other) noexcept = default;
		NumericValue& operator=(const NumericValue& other) noexcept = default;
		NumericValue& operator=(NumericValue&& other) noexcept = default;
		NumericValue& operator=(NumericVariantType other) noexcept {
			number = std::optional{other};
			return *this;
		}
		NumericValue& operator=(std::optional<NumericVariantType> other) noexcept {
			number = other;
			return *this;
		}
		NumericValue& operator=(WholeType other) noexcept {
			number = std::optional{other};
			return *this;
		}
		NumericValue& operator=(IntegerType other) noexcept {
			number = std::optional{other};
			return *this;
		}
		NumericValue& operator=(CharacterType other) noexcept {
			number = std::optional{other};
			return *this;
		}
		NumericValue& operator=(FixedPointType other) noexcept {
			number = std::optional{other};
			return *this;
		}
		NumericValue& operator=(BoolType other) noexcept {
			number = std::optional{other};
			return *this;
		}

		template<typename ReduceToParameterType>
		auto to() const -> std::optional<ReduceToParameterType>
		{
			if(number.has_value() == true)
			{
				return std::visit([&](const auto& number_concrete)
				{
					using ConcreteType = typename NumericTypeResolver<CleanType<decltype(number_concrete)>::type>::Type;
					using ToType = CleanType<ReduceToParameterType>;
					if constexpr(std::is_convertible_v<ToType, ConcreteType> == true)
						return std::optional<ToType>{static_cast<ToType>(number_concrete)};
					else
						return std::optional<ToType>{std::nullopt};
				}, number.value());
			}
			return std::nullopt;
		}
		
		template<auto OperationParameterConstant, bool BooleanParameterConstant = false>
		NumericValue operate(const NumericValue& other) const
		{
			if(number.has_value() == true && other.number.has_value() == true)
			{
				return std::visit([&](const auto& number_concrete)
				{
					return std::visit([&](const auto& other_concrete)
					{
						constexpr const auto number_tag = CleanType<decltype(number_concrete)>::type;
						constexpr const auto other_tag = CleanType<decltype(other_concrete)>::type;
						using ConcreteType = typename NumericTypeResolver<number_tag>::Type;
						using OtherConcreteType = typename NumericTypeResolver<other_tag>::Type;
						if constexpr(BooleanParameterConstant == false)
						{
							if constexpr(std::is_convertible_v<ConcreteType, OtherConcreteType> == true)
							{
								const auto& other_concrete_converted = static_cast<ConcreteType>(other_concrete);
								return NumericValue{NumericVariantType{ConcreteType(
										OperationParameterConstant(number_concrete, other_concrete_converted)
									 )}};
							}
							else
								return NumericValue{std::nullopt};
						}
						else if constexpr(std::is_same_v<ConcreteType, OtherConcreteType> == true)
						{
							using BoolType = typename NumericTypeResolver<NumericTypeTag::Bool>::Type;
							return NumericValue{NumericVariantType{BoolType(
									OperationParameterConstant(number_concrete, other_concrete)
							  )}};
						}
						else
						   return NumericValue{std::nullopt};
					}, other.number.value());
				}, number.value());
			}
			return NumericValue{std::nullopt};
		}

		template<auto OperationParameterConstant, bool NegatingOperationConstant = false>
		NumericValue operate() const
		{
			if(number.has_value() == true)
			{
				return std::visit([&](const auto& number_concrete)
				{
					using ConcreteType = typename NumericTypeResolver<CleanType<decltype(number_concrete)>::type>::Type;
					if constexpr(NegatingOperationConstant == true && 
							  (std::is_unsigned_v<typename ConcreteType::UnderylingType> == true
									|| std::is_same_v<typename ConcreteType::UnderylingType, WarpBool> == false))
						return NumericValue{std::nullopt};
					else
					{
						return NumericValue{std::optional{NumericVariantType{ConcreteType(
								OperationParameterConstant(number_concrete)
						  )}}};
					}
				}, number.value());
			}
			return *this;
		}

		NumericValue operator*(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return left * right; }>(other);
		}
		NumericValue operator/(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return left / right; }>(other);
		}
		NumericValue operator+(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return left + right; }>(other);
		}
		NumericValue operator-(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return left - right; }>(other);
		}
		NumericValue operator>(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return to_warp_bool(left > right); }, true>(other);
		}
		NumericValue operator<(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return to_warp_bool(left < right); }, true>(other);
		}
		NumericValue operator<=(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return to_warp_bool(left <= right); }, true>(other);
		}
		NumericValue operator>=(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return to_warp_bool(left >= right); }, true>(other);
		}
		NumericValue operator==(const NumericValue& other) const {
			return operate<[](const auto& left, const auto& right) { return to_warp_bool(left == right); }, true>(other);
		}
		NumericValue operator-() const {
			return operate<[](const auto& value) { return -value; }, true>();
		}
		NumericValue operator!() const {
			return operate<[](const auto& value) { return !value; }, true>();
		}
		operator std::optional<WholeType>() const {
			return to<WholeType>();
		}
		operator std::optional<IntegerType>() const {
			return to<IntegerType>();
		}
		operator std::optional<FixedPointType>() const {
			return to<FixedPointType>();
		}
		operator std::optional<CharacterType>() const {
			return to<CharacterType>();
		}
		operator std::optional<BoolType>() const {
			return to<BoolType>();
		}
	};

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

    constexpr std::string to_string(Warp::Runtime::Compiler::WarpBool to_stringify)
	{
		if(to_stringify == WarpBool::True)
			return std::string{"True"};
		else
			return std::string{"False"};
	}

	template<
			NumericTypeTag ParameterTypeConstant, 
			size_t WholePartBitsParameterConstant, 
			size_t DecimalPartBitsParameterConstant
		>
	constexpr std::string to_string(numeric::fixed<
					WholePartBitsParameterConstant, 
					DecimalPartBitsParameterConstant
				> fixed) {
		return std::to_string(fixed.to_double());
	}

    template<typename ParameterType>
    constexpr std::string to_string(NumericType<NumericTypeTag::FixedPoint, ParameterType> to_stringify) {
		return std::to_string(to_stringify.number.to_double());
	}
    template<NumericTypeTag TermTagParameterConstant, typename ParameterType>
    constexpr std::string to_string(NumericType<TermTagParameterConstant, ParameterType> to_stringify) {
			return to_string(to_stringify.number);
	}
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__NUMERIC__TYPE__HPP

