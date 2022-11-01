#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

namespace Warp::Runtime::Compiler
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

	enum class OperationalValueTag {
		InferFromEvaluation
	};

	struct TypeTag
	{
		TypeTag(auto tag) : tag(std::in_place_type_t<CleanType<decltype(tag)>>{}, tag) {}
		bool operator==(auto other)
		{
			using OtherType = CleanType<decltype(other)>;
			if(typeid(OtherType) == tag.type())
				return false;
			return std::any_cast<OtherType>(tag) == other;
		}
		protected:
			std::any tag;
	};

	template<typename ValueParameterType, typename TypeParameterType>
	struct Constant
	{
		using ValueType = ValueParameterType;
		using TypeType = TypeParameterType;
		std::string name;
		TypeType type;
		ValueType value;
	};

	template<typename ConstraintStorageParameterType, typename NameDependancyParameterType = std::string>
	struct Constraint
	{
		using ConstraintStorageType = ConstraintStorageParameterType;
		using NameDependancyType = NameDependancyParameterType;
		ConstraintStorageType constraint;

		constexpr Constraint() noexcept : constraint(nullptr) {}
		constexpr Constraint(const Constraint& other) noexcept = default;
		constexpr Constraint(Constraint&& other) noexcept = default;
		constexpr Constraint& operator=(const Constraint& other) noexcept = default;
		constexpr Constraint& operator=(Constraint&& other) noexcept = default;

		constexpr const std::vector<NameDependancyType>& get_name_dependancies() const noexcept {
			return name_dependancies;
		}
		protected: 
			const std::vector<NameDependancyType> name_dependancies;
	};

	template<
			typename ConstraintParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct SingleParameter
	{
		using ConstraintType = ConstraintParameterType;
		constexpr SingleParameter() noexcept {}
		constexpr SingleParameter(const SingleParameter& other) noexcept = default;
		constexpr SingleParameter(SingleParameter&& other) noexcept = default;
		constexpr SingleParameter& operator=(const SingleParameter& other) noexcept = default;
		constexpr SingleParameter& operator=(SingleParameter&& other) noexcept = default;
		ConstraintType constraint;
	};

	template<
			typename SingleParameterParameterType, 
			typename FunctionStorageParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct Alternative
	{
		using SingleParameterType = SingleParameterParameterType;
		using FunctionStorageType = FunctionStorageParameterType;
		using IdentifierType = IdentifierParameterType;

		explicit constexpr Alternative(const IdentifierType name, const std::vector<SingleParameterType>&& parameters) noexcept 
				: parameter_count(parameters.size()), parameters(std::move(parameters)), name(name) {}
		constexpr Alternative(const Alternative& other) noexcept = default;
		constexpr Alternative(Alternative&& other) noexcept = default;
				//: parameter_count(other.parameter_count), parameters(std::move(other.parameters)), name(other.name) {}
		constexpr Alternative& operator=(const Alternative& other) noexcept = default;
		constexpr Alternative& operator=(Alternative&& other) noexcept = default;

		constexpr const std::vector<SingleParameterType>& get_parameters() const noexcept {
			return parameters;
		}

		constexpr const size_t get_parameter_count() const noexcept {
			return parameter_count;
		}

		constexpr const IdentifierType get_name() const noexcept {
			return name;
		}

		protected: 
			const size_t parameter_count;
			const std::vector<SingleParameterType> parameters;
			const IdentifierType name;
	};

	template<typename AlternativeParameterType, typename IdentifierParameterType = std::string>
	struct Function
	{
		using AlternativeType = AlternativeParameterType;
		using IdentifierType = IdentifierParameterType;
		using AlternativeMatrixType = std::vector<std::vector<AlternativeType>>;

		constexpr Function() noexcept = default;
		constexpr Function(AlternativeType alternative) noexcept : name(alternative.getName()) {
			add_alternative(alternative);
		}
		constexpr Function(const Function& other) noexcept = default;
		constexpr Function(Function&& other) noexcept = default;
		constexpr Function& operator=(const Function& other) noexcept = default;
		constexpr Function& operator=(Function&& other) noexcept = default;

		constexpr const IdentifierType get_name() const noexcept {
			return name;
		}

		constexpr const AlternativeMatrixType& get_alternatives() const noexcept {
			return alternatives;
		}

		const AlternativeMatrixType& add_alternative(AlternativeType alternative)
		{
			const size_t parameter_count = alternative.get_parameter_count();
			if(alternatives.size() <= parameter_count)
				alternatives.reserve(parameter_count);
			alternatives[parameter_count].push_back(std::move(alternative));
			return alternatives;
		}

		protected: 
			const IdentifierType name;
			AlternativeMatrixType alternatives;
	};

	using ConstantType = Constant<SyntaxNode, TypeTag>;
	using ConstraintType = Constraint<SyntaxNode, std::string>;
	using SingleParameterType = SingleParameter<ConstraintType>;
	using AlternativeType = Alternative<SingleParameterType, SyntaxNode, std::string>;
	using FunctionType = Function<AlternativeType, std::string>;
	

	struct Context
	{
		std::unordered_map<std::string, ConstantType> constants;
		std::unordered_map<std::string, FunctionType> functions;
		//Context inject(AlternativeType alternative)
		//{
		//	std::string name = alternative.get_name();
		//	//if(functions.contains(alternative.get_name()) == false)
		//	//	functions.insert({name, std::move(FunctionType{std::move(alternative)}}));
		//	//else
		//		functions.at(name).add_alternative(std::move(alternative));
		//	return std::move(*this);
		//}
		Context inject(ConstantType constant) {
			constants.insert({constant.name, std::move(constant)});
			return std::move(*this);
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

