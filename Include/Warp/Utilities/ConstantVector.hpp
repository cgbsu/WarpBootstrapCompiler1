#include <Warp/Common.hpp>
/* This work is under the Copyright Christopher A. Greeley (2024) and it is distributed
* under the No Kill Do No Harm License, a legally non-binding sumemry is as follows: 
* 
* # No Kill Do No Harm Licence – Summary
* 
* Based on version 0.3, July 2022 of the Do No Harm License
* 
* https://github.com/raisely/NoHarm
* 
* LEGALLY NON-BINDING SUMMARY OF THE TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
* 
* ## Licence Grants
* 
* You're allowed
* 
* - to distribute the licensed work,
* - to create, publish, sublicense and patent derivative works and
* - to put your modifications or your derivative work under a seperate licence,
* 
* free of charge. Though, filing patent litigation leads to the loss of the patent licence. Also, the licence grants don't include the right to use the licensor's trademarks.
* 
* ## Unethical Behaviour
* 
* You may not use the licensed work if you engage in:
* 
* - human rights violations,
* - environmental destruction,
* - warfare,
* - addictive/destructive products or services or
* - actions that frustrate:
*   * peace,
*   * access to human rights,
*   * peaceful assembly and association,
*   * a sustainable environment or
*   * democratic processes
*   * abortion
*   * euthanasia
*   * human embryonic stem cell research (if human organisms are killed in the process)
* - except for actions that may be contrary to "human rights" (or interpretations thereof), do not kill and that frustrate 
*   * abortion
*   * euthanasia
*   * killing
* and; the software must never be used to kill, including: abortion, euthanasia, human stem cell research, in war, or law enforcement or as a part of any lethal weapon
* 
* ## Contributions
* 
* Contributions to the licensed work must be licensed under the exact same licence.
* 
* ## Licence Notice
* 
* When distributing the licensed work or your derivative work, you must
* 
* - include a copy of this licence,
* - retain attribution notices,
* - state changes that you made and
* - not use the names of the author and the contributors to promote your derivative work.
* 
* If the licensed work includes a "NOTICE" text file with attribution notices, you must copy those notices to:
* 
* - a "NOTICE" file within your derivative work,
* - a place within the source code or the documentation or
* - a place within a display generated by your derivative work.
* 
* ## No Warranty or Liability
* 
* The licensed work is offered on an as-is basis without any warranty or liability. You may choose to offer warranty or liability for your derivative work, but only fully on your own responsibility. */



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

