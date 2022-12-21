#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTRAINT__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTRAINT__HPP

namespace Warp::Runtime::Compiler
{
	// This is a small file now but I think it may grow later. 2022/11/11 //

	enum class OperationalValueTag {
		InferFromEvaluation
	};

	template<
			typename ConstraintStorageParameterType, 
			typename NameDependancyParameterType = std::string
		>
	struct Constraint
	{
		using ConstraintStorageType = ConstraintStorageParameterType;
		using NameDependancyType = NameDependancyParameterType;
		ConstraintStorageType constraint;

		//constexpr Constraint() noexcept : constraint(nullptr) {}
		//constexpr Constraint(const Constraint& other) noexcept = default;
		//constexpr Constraint(Constraint&& other) noexcept = default;
		//constexpr Constraint(ConstraintStorageParameterType&& constraint) noexcept : constraint(constraint) {}
		//constexpr Constraint& operator=(const Constraint& other) noexcept = default;
		//constexpr Constraint& operator=(Constraint&& other) noexcept = default;

		// TODO: Traverse tree and get name dependancies for easy lookup. //
		//constexpr const std::vector<NameDependancyType>& get_name_dependancies() const noexcept {
		//	return name_dependancies;
		//}
		//protected: 
		//	std::vector<NameDependancyType> name_dependancies;
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTRAINT__HPP

