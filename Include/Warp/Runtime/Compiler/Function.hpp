#include <Warp/Runtime/Compiler/Alternative.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__FUNCTION__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__FUNCTION__HPP

namespace Warp::Runtime::Compiler
{
	template<
			typename AlternativeParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct Function
	{
		using AlternativeType = AlternativeParameterType;
		using IdentifierType = IdentifierParameterType;
		using AlternativesOfUniformArityRowType = std::vector<std::unique_ptr<AlternativeType>>;
		using AlternativeMatrixType = std::vector<AlternativesOfUniformArityRowType>;

		constexpr Function() noexcept = default;
		constexpr Function(std::unique_ptr<AlternativeType> alternative) noexcept : name(alternative->get_name()) {
			add_alternative(std::move(alternative));
		}
		constexpr Function(const Function& other) noexcept = default;
		constexpr Function(Function&& other) noexcept = default;
		constexpr Function& operator=(const Function& other) noexcept = default;
		constexpr Function& operator=(Function&& other) noexcept = default;
		constexpr ~Function() = default;

		constexpr const IdentifierType get_name() const noexcept {
			return name;
		}

		constexpr const AlternativeMatrixType& get_alternatives() const noexcept {
			return alternatives;
		}

		const AlternativeMatrixType& add_alternative(std::unique_ptr<AlternativeType> alternative)
		{
			const size_t parameter_count = alternative->parameter_count();
			while(alternatives.size() <= (parameter_count + 1))
				alternatives.push_back(AlternativesOfUniformArityRowType{});
			alternatives[parameter_count].push_back(std::move(alternative));
			return alternatives;
		}

		protected: 
			const IdentifierType name;
			AlternativeMatrixType alternatives;
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__FUNCTION__HPP

