#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__MOVING__VECTOR__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__MOVING__VECTOR__HPP

namespace Warp::Utilities
{
	template<typename ElementParameterType>
	struct MovingVector
	{
		using ElementType = ElementParameterType;
		std::vector<ElementType> data;

		constexpr MovingVector& push_back(const ElementType& new_element) noexcept {
			data.push_back(new_element);
			return *this;
		}

		constexpr size_t size() const noexcept {
			return data.size();
		}

		constexpr MovingVector& append(const MovingVector& other) {
			data.insert(data.end(), other.data.begin(), other.data.end());
			return *this;
		}

		constexpr ElementType& at(size_t index) noexcept {
			return data.at(index);
		}

		constexpr ElementType& operator[](size_t index) noexcept {
			return at(index);
		}

		constexpr const ElementType& operator[](size_t index) const noexcept {
			return at(index);
		}
	};
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__MOVING__VECTOR__HPP

