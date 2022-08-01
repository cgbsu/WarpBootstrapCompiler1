#include <Warp/Utilities/Math.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Utilities;

TEST_GROUP(Math) {};

TEST(Math, ExtractDigits)
{
	CHECK((extract_digits(1234, 0u) == 4));
	CHECK((extract_digits(1234, 1u) == 3));
	CHECK((extract_digits(1234, 2u) == 2));
	CHECK((extract_digits(1234, 2u, 2u) == 12));
	CHECK((extract_digits(1234, 0u, 2u) == 34));
	CHECK((extract_digits(1234, 1u, 2u) == 23));
};

