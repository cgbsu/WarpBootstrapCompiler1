#include <Warp/Common.hpp>
#include <Warp/Utilities/Conversions.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__ARRAY__VIEW__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__ARRAY__VIEW__HPP

namespace Warp::Utilities
{
	template<typename ElementParameterType>
	struct IteratorBase
	{
		using ElementType = ElementParameterType;
		using ThisType = IteratorBase<ElementType>;
		using IteratorType = std::unique_ptr<ThisType>;

		constexpr const IteratorType operator++() const noexcept {
			return increment();
		}
		constexpr const IteratorType operator--() const noexcept {
			return decrement();
		}
		constexpr const ElementType& operator*() const noexcept {
			return dereference();
		}
		constexpr const ElementType& operator->() const noexcept {
			return dereference();
		}
		constexpr const IteratorType operator+(size_t offset) const noexcept {
			return add(offset);
		}
		constexpr const IteratorType operator+(const IteratorType& offset) const noexcept {
			return add(offset);
		}
		constexpr const IteratorType operator-(size_t offset) const noexcept {
			return subtract(offset);
		}
		constexpr const IteratorType operator-(const IteratorType& offset) const noexcept {
			return subtract(offset);
		}
		constexpr virtual const IteratorType increment() const noexcept = 0;
		constexpr virtual const IteratorType decrement() const noexcept = 0;
		constexpr virtual const ElementType& dereference() const noexcept = 0;
		constexpr virtual const ElementType& at(size_t index) const noexcept = 0;
		constexpr virtual const IteratorType add(size_t index) const noexcept = 0;
		constexpr virtual const IteratorType subtract(size_t index) const noexcept = 0;
		constexpr virtual const IteratorType add(const IteratorType& ofset) const noexcept = 0;
		constexpr virtual const IteratorType subtract(const IteratorType& offset) const noexcept = 0;
		constexpr virtual IteratorType copy() const noexcept = 0;
	};

	template<typename ElementParameterType>
	using IteratorType = std::unique_ptr<IteratorBase<ElementParameterType>>;

	template<typename ElementParameterType, typename IteratorParameterType>
	struct Iterator;
	
	template<typename ElementParameterType, typename IteratorParameterType>
	constexpr auto iterator_from(const IteratorParameterType iterator)
			-> IteratorType<ElementParameterType>
	{
		return make_derived_unique<
				IteratorBase<ElementParameterType>, 
				Iterator<ElementParameterType, IteratorParameterType>
			>(iterator);
	}
	
	template<typename ElementParameterType, typename IteratorParameterType>
	struct Iterator : public IteratorBase<ElementParameterType>
	{
		using ElementType = ElementParameterType;
		using IteratorType = IteratorParameterType;
		using BaseType = IteratorBase<ElementParameterType>;
		using ThisType = Iterator<ElementType, IteratorType>;
		using ThisPointerType = std::unique_ptr<ThisType>;
		using IteratorPointerType = BaseType::IteratorType;

		constexpr Iterator(const IteratorParameterType iterator) : iterator(iterator) {}

		constexpr virtual const IteratorPointerType increment() const noexcept final {
			return iterator_from<ElementType>(iterator + 1);
		}
		constexpr virtual const IteratorPointerType decrement() const noexcept final {
			return iterator_from<ElementType>(iterator - 1);
		}
		constexpr virtual const ElementType& dereference() const noexcept final {
			return *iterator;
		}
		constexpr virtual const ElementType& at(size_t index) const noexcept final {
			return *(iterator + index);
		}
		constexpr virtual const IteratorPointerType add(size_t index) const noexcept final {
			return iterator_from<ElementType>(iterator + index);
		}
		constexpr virtual const IteratorPointerType subtract(size_t index) const noexcept final {
			return iterator_from<ElementType>(iterator - index);
		}
		constexpr virtual const IteratorPointerType add(
				const IteratorPointerType& offset) const noexcept final
		{
			const IteratorType offset_ 
					= cast_unique_object<ThisType>(offset).get_iterator();
			const auto distance = std::distance(iterator, offset_);
			return add(distance);
		}
		constexpr virtual const IteratorPointerType subtract(
				const IteratorPointerType& offset) const noexcept final
		{
			const IteratorType offset_ 
					= cast_unique_object<ThisType>(offset).get_iterator();
			const auto distance = std::distance(iterator, offset_);
			return add(distance);
		}
		constexpr virtual IteratorPointerType copy() const noexcept final {
			return make_derived_unique<BaseType, ThisType>(iterator);
		}
		constexpr const IteratorType get_iterator() const noexcept {
			return iterator;
		}
		
		protected:
			const IteratorType iterator;
	};
	
	
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
					begin_(iterator_from<ElementParameterType>(array.begin())), 
					end_(iterator_from<ElementParameterType>(array.end())) {}
	
		template<template<typename> typename ArrayParameterTemplate>
		explicit constexpr ArrayView(const ArrayParameterTemplate<ElementParameterType>& array) noexcept
				: size_(array.size()), 
					begin_(iterator_from<ElementParameterType>(array.begin())), 
					end_(iterator_from<ElementParameterType>(array.end())) {}
		
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
			return begin_->at(index);
		}
	
		protected: 
			const size_t size_;
			const IteratorType<ElementType> begin_;
			const IteratorType<ElementType> end_;
	};
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__ARRAY__VIEW__HPP


