#include <Warp/Common.hpp>

#ifndef WARP_BOOTSTRAP_COMPILER_HEADER_UTILITIES_NOT_SO_UNIQUE_POINTER_HPP
#define WARP_BOOTSTRAP_COMPILER_HEADER_UTILITIES_NOT_SO_UNIQUE_POINTER_HPP

namespace Warp::Utilities
{
    /* Before you ask... no, I could not use unique_ptr, in fact I could not manage ****
    ** the memory, for why you cant use unique_ptr see: ********************************
    ** See: https://github.com/cplusplus/papers/issues/961 *****************************
    ** and http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2273r0.pdf *********
    ** https://github.com/riscygeek/constexpr_suff/blob/master/include/unique_ptr.hpp */


    template<typename StorageParameterType>
    struct NotSoUniquePointer
    {
        constexpr NotSoUniquePointer() : pointer(nullptr) {}
        constexpr NotSoUniquePointer(StorageParameterType* pointer) noexcept : pointer(pointer) {}

        template<typename... InitializersParameterTypes>
        constexpr NotSoUniquePointer(
				std::in_place_type_t<StorageParameterType>, 
				InitializersParameterTypes... initializers
			) noexcept
                : pointer(new StorageParameterType(std::forward<InitializersParameterTypes>(initializers)...)) {}

        constexpr NotSoUniquePointer(const NotSoUniquePointer& other) noexcept : pointer(other.pointer) {
            ((NotSoUniquePointer&) other).pointer = nullptr;
        }
        constexpr NotSoUniquePointer(NotSoUniquePointer&& other) noexcept : pointer(other.pointer) {
            other.pointer = nullptr;
        }
        constexpr ~NotSoUniquePointer() noexcept {
            delete pointer; 
        }
        constexpr NotSoUniquePointer& operator=(const NotSoUniquePointer& other) noexcept
        {
            pointer = other.pointer;
            ((NotSoUniquePointer&) other).pointer = nullptr;
            return *this;
        }
        constexpr NotSoUniquePointer& operator=(NotSoUniquePointer&& other) noexcept
        {
            pointer = other.pointer;
            other.pointer = nullptr;
            return *this;
        }
        constexpr StorageParameterType* operator->() const noexcept {
            return pointer;
        }

        constexpr const StorageParameterType* get_pointer() const noexcept {
            return pointer;
        }
        protected: 
            StorageParameterType* pointer;
    };
    
    namespace Detail
    {
        template<typename>
        struct VectorPlaceHolder {};
        template<typename, size_t>
        struct ArrayPlaceHolder {};
    }

    template<typename StorageParameterType>
    constexpr StorageParameterType* copy_append(
            const size_t index, 
            const size_t size, 
            StorageParameterType* from, 
            StorageParameterType* to, 
            const StorageParameterType& to_append
       )
    {
        if(from == nullptr || to == nullptr)
            return nullptr;
        if(index <size) {
            to[index] = from[index];
            return copy_append(index + 1, size, from, to, to_append);
        }
        to[size] = to_append;
        return to;
    }


    template<typename StorageParameterType>
    struct NotSoUniquePointer<Detail::VectorPlaceHolder<StorageParameterType>>
    {
        using ThisType = NotSoUniquePointer<Detail::VectorPlaceHolder<StorageParameterType>>;
        constexpr NotSoUniquePointer() : pointer(nullptr), size(0), end(nullptr) {}

        constexpr NotSoUniquePointer(auto... array) noexcept : 
                pointer(new StorageParameterType[ sizeof...(array) ]{ array... }), 
                size(sizeof...(array)), 
                end(&pointer[ sizeof...(array) - 1 ]) {}

        constexpr NotSoUniquePointer(const ThisType& other, const StorageParameterType& to_append) noexcept : 
                pointer(copy_append(0, other.size, new StorageParameterType[ other.size + 1 ], other.pointer, to_append)), 
                size(other.size + 1), 
                end(&pointer[ other.size ])
        {}

        constexpr NotSoUniquePointer(ThisType&& other, const StorageParameterType& to_append) noexcept : 
                pointer(copy_append(0, other.size, new StorageParameterType[ other.size + 1 ], other.pointer, to_append)), 
                size(other.size + 1), 
                end(&pointer[ other.size ]) 
        {}

        constexpr NotSoUniquePointer(const ThisType& other) noexcept : pointer(other.pointer), size(other.size), end(other.end) {
            ((NotSoUniquePointer&) other).pointer = nullptr;
        }
        constexpr NotSoUniquePointer(ThisType&& other) noexcept : pointer(other.pointer), size(other.size), end(other.end) {
            other.pointer = nullptr;
        }
        constexpr ~NotSoUniquePointer() noexcept {
            delete pointer; 
        }

        constexpr NotSoUniquePointer& operator=(const ThisType& other) noexcept
        {
            pointer = other.pointer;
            ((NotSoUniquePointer&) other).pointer = nullptr;
            size = other.size;
            return *this;
        }
        constexpr NotSoUniquePointer& operator=(ThisType&& other) noexcept
        {
            pointer = other.pointer;
            other.pointer = nullptr;
            size = other.size;
            return *this;
        }
        constexpr StorageParameterType* operator->() const noexcept {
            return pointer;
        }

        constexpr StorageParameterType* get_pointer() const noexcept {
            return pointer;
        }


        constexpr const StorageParameterType& get(size_t index) const noexcept {
            return pointer[ index ];
        }

        constexpr const StorageParameterType& operator[](size_t index) const noexcept {
            return get(index);
        }

        constexpr const StorageParameterType* get_end() {
            return end;
        }

        constexpr const size_t get_size() {
            return size;
        }

        protected: 
            size_t size;
            StorageParameterType* pointer;
            StorageParameterType* end;
    };

    template<typename StorageParameterType>
    using VectorType = NotSoUniquePointer<Detail::VectorPlaceHolder<StorageParameterType>>;

}

#endif // WARP_BOOTSTRAP_COMPILER_HEADER_UTILITIES_NOT_SO_UNIQUE_POINTER_HPP

