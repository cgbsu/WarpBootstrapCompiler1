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
			size_t ParameterCountParameterConstant, 
			typename SingleParameterParameterType, 
			typename FunctionTreeStorageParameterType, 
			typename IdentifierParameterType
		>
	struct CountedParameterAlternative;

	template<
			typename SingleParameterParameterType, 
			typename FunctionTreeStorageParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct Alternative
	{
		using SingleParameterType = SingleParameterParameterType;
		using FunctionTreeStorageType = FunctionTreeStorageParameterType;
		using IdentifierType = IdentifierParameterType;
		using ParameterView = ArrayView<SingleParameterType>;
		using AlternativeType = std::unique_ptr<Alternative>;

		constexpr Alternative(const IdentifierType name) noexcept : name(name) {}

		constexpr virtual const ParameterView get_parameters() const noexcept = 0;
		constexpr virtual const size_t parameter_count() const noexcept = 0;
		constexpr virtual const FunctionTreeStorageType& function_tree() const noexcept = 0;
		constexpr virtual const AlternativeType add_parameter(SingleParameterType parameter) const noexcept = 0;

		constexpr const IdentifierType get_name() const noexcept {
			return name;
		}

		protected: 
			const IdentifierType name;
	};

	//template<
	//		typename SingleParameterParameterType, 
	//		typename FunctionTreeStorageParameterType, 
	//		typename IdentifierParameterType = std::string
	//	>
	//using AlternativeType = std::unique_ptr<Alternative<
	//		SingleParameterParameterType, 
	//		FunctionTreeStorageParameterType, 
	//		IdentifierParameterType
	//	>>;

	template<
			size_t ParameterCountParameterConstant, 
			typename SingleParameterParameterType, 
			typename FunctionTreeStorageParameterType, 
			typename IdentifierParameterType = std::string
		>
	struct CountedParameterAlternative : public Alternative<
			SingleParameterParameterType, 
			FunctionTreeStorageParameterType, 
			IdentifierParameterType
		>
	{
		constexpr static const size_t parameter_count_ = ParameterCountParameterConstant; 

		using SingleParameterType = SingleParameterParameterType;
		using FunctionTreeStorageType = FunctionTreeStorageParameterType;
		using IdentifierType = IdentifierParameterType;
		using ParameterView = ArrayView<SingleParameterType>;

		using BaseType = Alternative<
				SingleParameterType, 
				FunctionTreeStorageType, 
				IdentifierType
			>;

		using ThisType = CountedParameterAlternative<
				parameter_count_, 
				SingleParameterType, 
				FunctionTreeStorageType, 
				IdentifierType
			>;

		using NextType = CountedParameterAlternative<
				parameter_count_ + 1, 
				SingleParameterType, 
				FunctionTreeStorageType,
				IdentifierType
			>;
		using AlternativeType = std::unique_ptr<Alternative<
				SingleParameterType, 
				FunctionTreeStorageType, 
				IdentifierType
			>>;

		constexpr CountedParameterAlternative(
				IdentifierType name, 
				FunctionTreeStorageType function_tree_, 
				auto... parameters_
			) noexcept : 
					BaseType(name), 
					function_tree_(std::move(function_tree_)), 
					parameters{std::move(parameters_)...} {}

		template<size_t... IndexParameterConstants>
		constexpr CountedParameterAlternative(
				ThisType from, 
				SingleParameterType parameter, 
				std::index_sequence<IndexParameterConstants...> = std::make_index_sequence<parameter_count>()
			) noexcept : 
					BaseType(from.name), 
					function_tree_(std::move(from.function_tree_)), 
					parameters{std::move(from.get_parameters()[IndexParameterConstants])..., std::move(parameter)} {}

		constexpr virtual const ParameterView get_parameters() const noexcept final {
			return ParameterView(parameters);
		}

		constexpr virtual const size_t parameter_count() const noexcept final {
			return parameter_count_;
		}

		constexpr virtual const FunctionTreeStorageType& function_tree() const noexcept final {
			return function_tree_;
		}

		constexpr virtual const AlternativeType add_parameter(SingleParameterType parameter) const noexcept final {
			return std::make_unique<NextType>(new CountedParameterAlternative(std::move(*this), std::move(parameter)));
		}

		protected: 
			std::array<SingleParameterType, parameter_count_> parameters;
			FunctionTreeStorageType function_tree_;
	};

	template<typename AlternativeParameterType, typename IdentifierParameterType = std::string>
	struct Function
	{
		using AlternativeType = AlternativeParameterType;
		using IdentifierType = IdentifierParameterType;
		using AlternativeMatrixType = std::vector<std::vector<std::unique_ptr<AlternativeType>>>;

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
		std::unordered_map<std::string, std::unique_ptr<FunctionType>> functions;
		Context inject(std::unique_ptr<AlternativeType> alternative)
		{
			if(functions.contains(alternative->get_name()) == false)
			{
				auto new_pair = std::make_pair(
						alternative->get_name(), 
						std::move(std::make_unique<FunctionType>(std::move(alternative)))
					);
				functions.insert(std::move(new_pair));
			}
			else
				functions.at(alternative->get_name())->add_alternative(std::move(alternative));
			return std::move(*this);
		}
		Context inject(ConstantType constant) {
			constants.insert({constant.name, std::move(constant)});
			return std::move(*this);
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

