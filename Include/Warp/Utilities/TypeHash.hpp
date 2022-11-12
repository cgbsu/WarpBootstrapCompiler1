#include <Warp/Common.hpp>

#ifndef WARP__UTILITIES__HEADER__UTILITIES__TYPE_HASH__HPP
#define WARP__UTILITIES__HEADER__UTILITIES__TYPE_HASH__HPP

/****************************************************************************************
* For a detailed explination, see here: *************************************************
* https://stackoverflow.com/questions/56292104/hashing-types-at-compile-time-in-c17-c2a *
* I think the original solution is absolutly briliant, I wish I thought of it! **********
* <brief> *******************************************************************************
********* This does require some explination as it relies on some not-so-obvious C++ ****
***** mechanichs. The goal is to obtain a number at compile time that uniquely **********
***** represents a certain type, basically a "hash" for that type. We can do this *******
***** within a translation unit by creating a static constexpr variable within **********
***** a class template, with the template parameter being the type we wish to hash. *****
***** For each new type inputted into the template, a new type is created for that ******
***** combination of class - template and type is created, for example: *****************
************ template<typename T>                         *******************************
************ struct A { //Not a type a (class-)"template" *******************************
************     constexpr static const size_t value = 0; *******************************
************ };                                           *******************************
************ // ...                                       *******************************
************ A<int> intHolder; // New type                *******************************
************ A<float> floatHolder; // Another new type    *******************************
******** Static members have a 1 to 1 correspondence with their types (except when it ***
**** comes to inheritance). Since this is the case each time a new type is created ******
**** using a class-template (like A<T>), if that class-template declares static data ****
**** members, it creates new instances of those static data members for that type *******
**** such that `&A<int>::value == &A<float>::value` evaluates to `false`. So a **********
**** variable with a unique address is created for each type, which, though we **********
**** cant nessisiarily predict the address of the variable, can be interpreted as *******
**** a hash for the type. Great! We can std::bit_cast the addresss of this value to an **
**** int and get a "nice clean" integral value for the hash at compile time and even ****
**** potentiall use it in templates. Nice! However up until now if you have understood **
**** most of this codebase, I assume you are likely familiar with this mechanich. The ***
**** tricky part comes when we introduce translation units. For though we can get a *****
**** unique integral value for a type at compile time, anything declared with static ****
**** linkage (using the `static` keyword) will be reinstantiated in different ***********
**** translation units. That means if you use this technique to generate hashes for the *
**** same type in two different translation units you will get different values *********
**** and thus if you store the values and attempt to compare them later, they will not **
**** be the same, thus is required a subtle aspect of the `inline` keyword which ********
**** gaurntees that anyting declared using it will have as single instance across all ***
**** translation units, and luckilly, we can create `constexpr inline static` values in *
**** C++. So the `inline` keyword here is absolutly essential to making this work *******
**** (accross translation units). And that is most of what needed to be explained, to ***
**** modify the example from earlier: ***************************************************
************ template<typename T>                                ************************
************ struct A { // Not a type a (class-)"template"       ************************
************     inline constexpr static const size_t value = 0; ************************
************ };                                                  ************************
************ // ...                                              ************************
************ // Translation Unit A //                            ************************
************ size_t* ia = &A<int>::value;                         ***********************
************ size_t* fa = &A<float>::value;                       ***********************
************ // ...                                              ************************
************ // Translation Unit B //                            ************************
************ size_t* ib = &A<int>::value;                         ***********************
************ size_t* fb = &A<float>::value;                       ***********************
************ // ...                                              ************************
************ // Any Translation Unit! //                         ************************
************ ia == ib // True                                    ************************
************ fa == fb // True                                    ************************
* </brief> ******************************************************************************
* NOTE: Not made specifically for this project. *****************************************
****************************************************************************************/

namespace Warp::Utilities
{
	template<typename ToHashParameterType>
	struct TypeHasher
	{
		using Type = ToHashParameterType;
		constexpr inline static const uintptr_t get_hash() noexcept { /*
				ACCORDING TOO C++ STANDARD INLINE GUARANTEES 
				ONE COPY ACROSS ALL TRANSLATION UNITS */
			return std::bit_cast<uintptr_t>(&hash_object);
		}
		constexpr inline static const size_t hash_object = 0;
	};

	template<typename ToHashParameterType>
	constexpr inline static const uintptr_t type_hash 
			= TypeHasher<ToHashParameterType>::get_hash();
	template<typename ToHashParameterType>
	constexpr inline static const size_t* type_pointer_hash 
			= &TypeHasher<ToHashParameterType>::hash_object;
}

#endif // WARP__UTILITIES__HEADER__UTILITIES__TYPE_HASH__HPP

