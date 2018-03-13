#include "../catch.hpp"
#include "../Std/Array.h"
using namespace FluxStd;

TEST_CASE("Array - Constructor", "[Array]")
{
	SECTION("Empty")
	{
		Array<int, 4> arr;
		REQUIRE(arr.Size() == 4);
	}
}