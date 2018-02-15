#include "../catch.hpp"
#include "../Std/Array.h"
using namespace FluxStd;

TEST_CASE("Array - Constructor - Empty", "[Array]")
{
	SECTION("Empty")
	{
		Array<int, 4> arr;
		REQUIRE(arr.Size() == 4);
	}
	SECTION("Value")
	{
		Array<int, 4> arr(123);
		REQUIRE(arr.Size() == 4);
		for (size_t i = 0; i < arr.Size(); ++i)
		{
			REQUIRE(arr[i] == 123);
		}
	}
}