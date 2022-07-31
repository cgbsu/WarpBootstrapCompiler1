#include <Warp/Utilities/Decimal.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Utilities;

TEST_GROUP(DecimalNumbers)
{
};

TEST(DecimalNumbers, RawValueComparison)
{
	CHECK((DefaultDecimalType{1} == DefaultDecimalType{1}));
	CHECK((DefaultDecimalType{1, 2} == DefaultDecimalType{1, 2}));
	CHECK((DefaultDecimalType{12, 2} != DefaultDecimalType{13, 2}));
	CHECK((DefaultDecimalType{12, 2} < DefaultDecimalType{13, 2}));
	CHECK((DefaultDecimalType{12, 2} > DefaultDecimalType{13, 2, Polarity::Negative}));
	CHECK((DefaultDecimalType{34324, 10} > DefaultDecimalType{9999999999, 2, Polarity::Negative}));
	CHECK((DefaultDecimalType{12, -2} < DefaultDecimalType{13, 2}));
	CHECK((DefaultDecimalType{12, -2} < DefaultDecimalType{13, 0}));
	CHECK((DefaultDecimalType{10, 1} > DefaultDecimalType{10, 1, Polarity::Positive, 2})); // There are 10 kinds of people in this world... </joke>
};

TEST(DecimalNumbers, Multiplication)
{
	CHECK(((DefaultDecimalType{2} * DefaultDecimalType{2}) == DefaultDecimalType{4}));
	CHECK(((DefaultDecimalType{42, -1} * DefaultDecimalType{2}) == DefaultDecimalType{84, -1}));
	CHECK(((DefaultDecimalType{123456789, -5} * DefaultDecimalType{123456789})
				== DefaultDecimalType{15241578750190521, -5}));
	CHECK(((DefaultDecimalType{123456789, -5} * DefaultDecimalType{3058172, -6})
				== DefaultDecimalType{377552095329708, -11}));
};

//TEST(DecimalNumbers, Division)
//{
//	CHECK(((DefaultDecimalType{2} / DefaultDecimalType{2}) == DefaultDecimalType{1}));
//	CHECK(((DefaultDecimalType{42, -1} / DefaultDecimalType{2}) == DefaultDecimalType{21, -1}));
//	CHECK(((DefaultDecimalType{123456789, -5} / DefaultDecimalType{123456789})
//				== DefaultDecimalType{1, -5}));
//	CHECK(((DefaultDecimalType{123456789, -5} / DefaultDecimalType{3058172, -6})
//				== DefaultDecimalType{4036947202446, -10}));
//	CHECK(((DefaultDecimalType{123, -2} / DefaultDecimalType{456, -6})
//				== DefaultDecimalType{26973684210526315, -13}));
//};
//
TEST(DecimalNumbers, MultipleHighPrecisionOperations)
{
	//const auto result = (DefaultDecimalType{123456789, -5} / DefaultDecimalType{3058172, -6}) 
	//		* DefaultDecimalType{63246745724, -6};
	//std::cout << result << "\n";
	const auto result = (DefaultDecimalType{123456789, -5} 
			/ DefaultDecimalType{3058172, -6}) 
			/ DefaultDecimalType{3058172, -6};
	std::cout << result << "\n";
}


