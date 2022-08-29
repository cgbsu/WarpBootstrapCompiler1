#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__CONSTANT__VECTOR__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__CONSTANT__VECTOR__HPP

namespace Warp::Utilities
{
	template<typename ElementParameterType>
	struct ConstantVector
	{
		using ElementType = ElementParameterType;

		virtual constexpr size_t size() const noexcept = 0;
		virtual constexpr ElementType& at(size_t index) noexcept = 0;
		constexpr ElementType& operator[](size_t index) noexcept {
			return at(index);
		}
		constexpr const ElementType& operator[](size_t index) const noexcept {
			return at(index);
		}

		virtual constexpr std::unique_ptr<ConstantVector> clone() const noexcept = 0;
		virtual constexpr std::unique_ptr<ConstantVector> push_back(const ElementType& new_element) const noexcept = 0;
		virtual constexpr std::unique_ptr<ConstantVector> append(const std::unique_ptr<ConstantVector>& to_append) const noexcept = 0;
	};

	
	template<typename ElementParameterType, size_t LengthParameterConstant>
	struct ConstantVectorArray final : public ConstantVector<ElementParameterType>
	{
		using ElementType = ElementParameterType;
		using BaseType = ConstantVector<ElementType>;

		constexpr static const auto length = LengthParameterConstant;

		using ThisType = ConstantVectorArray<ElementType, length>;

		constexpr ConstantVectorArray() noexcept = default;
		constexpr ConstantVectorArray(const ConstantVectorArray& other) noexcept = default;
		constexpr ConstantVectorArray(ConstantVectorArray&& other) noexcept = default;

		constexpr ConstantVectorArray& operator=(const ConstantVectorArray& other) noexcept = default;
		constexpr ConstantVectorArray& operator=(ConstantVectorArray&& other) noexcept = default;

		constexpr ConstantVectorArray(std::initializer_list<ElementType> elements) noexcept 
				: underlying_array(to_array<length>(elements)) {}

		virtual constexpr size_t size() const noexcept final {
			return length;
		}

		virtual constexpr ElementType& at(size_t index) noexcept final {
			return underlying_array[index];
		}

		virtual constexpr std::unique_ptr<BaseType> clone() const noexcept final {
			return std::make_unique<ConstantVectorArray<ElementType, length>>(*this);
		}

		virtual constexpr std::unique_ptr<BaseType> push_back(const ElementType& new_element) const noexcept final {
			return push_back_implementation(std::make_index_sequence<length + 1>(), new_element);
		}
		virtual constexpr std::unique_ptr<BaseType> append(const std::unique_ptr<BaseType>& to_append) const noexcept final
		{
			std::unique_ptr<BaseType> next = clone();
			for(size_t ii = 0; ii < to_append->size(); ++ii)
				next = next->push_back(to_append->at(ii));
			return next;
		}

		template<size_t... IndexParameterConstants>
		constexpr std::unique_ptr<BaseType> push_back_implementation(
				std::index_sequence<IndexParameterConstants...>, 
				auto... new_elements
			) const noexcept
		{
			return std::make_unique<ConstantVectorArray<ElementType, length + 1>>(std::initializer_list<ElementType>{
					underlying_array[IndexParameterConstants]..., 
					new_elements...
				});
		}

		std::array<ElementType, length> underlying_array;
	};

	template<typename ElementParameterType>
	using ConstantVectorType = std::unique_ptr<ConstantVector<ElementParameterType>>;

	template<typename ElementParameterType>
	constexpr static ConstantVectorType<ElementParameterType> make_constant_vector(auto... initial_elements) {
		return std::make_unique<ConstantVectorArray<ElementParameterType, sizeof...(initial_elements)>>(initial_elements...);
	}
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__CONSTANT__VECTOR__HPP

