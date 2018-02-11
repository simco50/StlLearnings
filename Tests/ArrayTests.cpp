#include "../catch.hpp"
#include "../Std/Array.h"
using namespace FluxStd;

TEST_CASE("Array ALL", "[Array]")
{
	Array<int, 4> arr;
	arr[0] = 1;
	REQUIRE(arr[0] == 1);
}