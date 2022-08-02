#include <Warp/Utilities/Conversions.hpp>
#include <CppUTest/TestHarness.h>

using namespace Warp::Utilities;

TEST_GROUP(StringToIntegralConversions) {};

TEST(StringToIntegralConversions, UnderBase11)
{
	CHECK((to_integral<size_t>("123") == 123));
	CHECK((to_integral<size_t>("100000") == 100000));
	CHECK((to_integral<size_t, 2>("000011") == 3));
	CHECK((to_integral<size_t, 2>("0011000") == 24));
	CHECK((to_integral<size_t, 8>("45172") == 19066));
	CHECK((to_integral<size_t, 8>("32467156") == 6975086));
};

TEST(StringToIntegralConversions, Base16)
{
	CHECK((base_16_to_integral<size_t>("7Da31F") == 8233759));
	CHECK((base_16_to_integral<size_t>("83Cc31F") == 138199839));
	CHECK((base_16_to_integral<size_t>("0347824") == 3438628));
};

