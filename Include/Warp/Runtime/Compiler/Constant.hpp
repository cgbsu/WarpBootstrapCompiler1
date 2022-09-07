#include <Warp/Common.hpp>
#include <Warp/Utilities.hpp>
#include <Warp/SyntaxAnalysis/SyntaxTree.hpp>

#ifndef WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP
#define WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

namespace Warp::Runtime::Compiler
{
	using namespace Warp::SyntaxAnalysis::SyntaxTree;

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
		

	using ConstantType = Constant<SyntaxNode, TypeTag>;

	struct Context
	{
		std::unordered_map<std::string, ConstantType> constants;
		Context inject(ConstantType constant) {
			constants.insert({constant.name, std::move(constant)});
			return std::move(*this);
		}
	};
}

#endif // WARP__RUNTIME__COMPILER__HEADER__RUNTIME__COMPILER__CONSTANT__HPP

