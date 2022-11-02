#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__ARRAY__VIEW__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__ARRAY__VIEW__HPP

namespace Warp::Utilities
{
	template<typename ElementParameterType>
	struct IteratorBase
	{
		using IteratorType = std::unique_ptr<IteratorBase<ElementParameterType>>;
		using ElementType = ElementParameterType;
		constexpr const ElementType& operator++() const noexcept {
			return increment();
		}
		constexpr const ElementType& operator--() const noexcept {
			return decrement();
		}
		constexpr const ElementType& operator*() const noexcept {
			return dereference();
		}
		constexpr const ElementType& operator->() const noexcept {
			return dereference();
		}
		constexpr const ElementType& operator+(size_t offset) const noexcept {
			return add(offset);
		}
		constexpr const ElementType& operator+(const IteratorType& offset) const noexcept {
			return add(offset);
		}
		constexpr const ElementType& operator-(size_t offset) const noexcept {
			return subtract(offset);
		}
		constexpr const ElementType& operator-(const IteratorType& offset) const noexcept {
			return subtract(offset);
		}
		constexpr virtual const ElementType& increment() const noexcept = 0;
		constexpr virtual const ElementType& decrement() const noexcept = 0;
		constexpr virtual const ElementType& dereference() const noexcept = 0;
		constexpr virtual const IteratorType add(size_t index) const noexcept = 0;
		constexpr virtual const IteratorType subtract(size_t index) const noexcept = 0;
		constexpr virtual const IteratorType add(const IteratorType& ofset) const noexcept = 0;
		constexpr virtual const IteratorType subtract(const IteratorType& offset) const noexcept = 0;
	};
	
	template<typename ElementParameterType, typename IteratorParameterType>
	struct Iterator : public IteratorBase<ElementParameterType>
	{
		using IteratorType = IteratorParameterType;
		using ElementType = ElementParameterType;
		using ThisType = Iterator<ElementType, IteratorType>;

		constexpr Iterator(const IteratorParameterType iterator) : iterator(iterator) {}

		constexpr virtual const ElementType& increment() const noexcept {
			return ++iterator;
		}
		constexpr virtual const ElementType& decrement() const noexcept {
			return *iterator;
		}
		constexpr virtual const ElementType& dereference() const noexcept {
			return --iterator;
		}
		constexpr virtual const IteratorType add(size_t index) const noexcept {
			return iterator + index;
		}
		constexpr virtual const IteratorType subtract(size_t index) const noexcept {
			return iterator - index;
		}
		constexpr virtual const IteratorType add(const IteratorType& offset) const noexcept {
			return iterator + dynamic_cast<const ThisType&>(offset);
		}
		constexpr virtual const IteratorType subtract(const IteratorType& offset) const noexcept {
			return iterator - dynamic_cast<const ThisType&>(offset);
		}
		
		protected:
			const IteratorType iterator;
	};
	
	template<typename ElementParameterType>
	using IteratorType = std::unique_ptr<IteratorBase<ElementParameterType>>;
	
	template<typename ElementParameterType, typename IteratorParameterType>
	IteratorType<ElementParameterType> iterator_from(const IteratorParameterType& iterator)
	{
		return static_cast<IteratorType<ElementParameterType>>(
				std::make_unique<Iterator<
						ElementParameterType, 
						IteratorParameterType
					>>(iterator).get()
			);
	}
	
	template<typename ElementParameterType>
	struct ArrayView
	{
		using ElementType = ElementParameterType;
	
		template<
				auto LengthParameterConstant, 
				template<typename, auto> typename ArrayParameterTemplate
			>
		explicit constexpr ArrayView(const ArrayParameterTemplate<
				ElementParameterType, 
				LengthParameterConstant
			>& array) noexcept
				: size_(LengthParameterConstant), 
					begin_(iterator_from(array.begin())), 
					end_(iterator_from(array.end())) {}
	
		template<template<typename> typename ArrayParameterTemplate>
		explicit constexpr ArrayView(const ArrayParameterTemplate<ElementParameterType>& array) noexcept
				: size_(array.size()), 
					begin_(iterator_from(array.begin())), 
					end_(iterator_from(array.end())) {}
		
		constexpr const size_t size() const noexcept {
			return size_;
		}

		constexpr const IteratorType<ElementType>& begin() const noexcept {
			return begin_;
		}

		constexpr const IteratorType<ElementType>& end() const noexcept {
			return end_;
		}

		constexpr const ElementType& operator[](size_t index) const noexcept {
			return begin_;
		}
	
		protected: 
			const size_t size_;
			const IteratorType<ElementType> begin_;
			const IteratorType<ElementType> end_;
	};
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__ARRAY__VIEW__HPP


