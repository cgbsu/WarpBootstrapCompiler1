#include <Warp/Utilities/ArrayView.hpp>
#include <CppUTest/TestHarness.h>
// #define WARP__TESTING__HEADER__TESTING__PARSE__TESTING__UTILITIES__HPP__CHECK__MACRO__REQUIRED CHECK

using namespace Warp::Utilities;

TEST_GROUP(ArrayViewUtility) {};

template<
		size_t CountParameterConstant, 
		size_t PreviousParamterConstant, 
		size_t SecondPreviousParameterConstant, 
		size_t... SequenceParameterConstants
	>
constexpr const auto fill_fibonacci_implementation()
{
	if constexpr(CountParameterConstant <= 0)
	{
		return std::array{
				PreviousParamterConstant, 
				SecondPreviousParameterConstant, 
				SequenceParameterConstants...
			};
	}
	else
	{
		return fill_fibonacci_implementation<
				CountParameterConstant - 1, 
				PreviousParamterConstant + SecondPreviousParameterConstant, 
				PreviousParamterConstant, 
				SecondPreviousParameterConstant, 
				SequenceParameterConstants...
			>();
	}
}

template<size_t CountParameterConstant>
constexpr const auto fill_fibonacci()
{
	return fill_fibonacci_implementation<
			CountParameterConstant, 
			0, 
			1
		>();
}

TEST(ArrayViewUtility, CheckValues)
{
	{
		const auto test = std::array<size_t, 3>{394, 576, 329};
		const auto view = ArrayView<size_t>{test};
		CHECK(view[0] == 394);
		CHECK(view[1] == 576);
		CHECK(view[2] == 329);
	}
	{
		const auto test = std::array<size_t, 4>{5489, 4357898, 347898, 56739};
		const auto view = ArrayView{test};
		CHECK(view[3] == 56739);
		CHECK(view[2] == 347898);
		CHECK(view[1] == 4357898);
		CHECK(view[0] == 5489);
	}
	{
		const auto test = fill_fibonacci<10>();
		const auto view = ArrayView{test};
		CHECK(view.size() == test.size());
		for(size_t ii = 0; ii < test.size(); ++ii)
		{
			CHECK(view[ii] == test[ii]);
			CHECK(**(*view.begin() + ii) == *(test.begin() + ii));
			CHECK(**(*view.end() - ii) == *(test.end() - ii));
		}
	}
};

