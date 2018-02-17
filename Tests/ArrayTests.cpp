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

TEST_CASE("Array - Fill", "[Array]")
{
	SECTION("Value")
	{
		Array<int, 4> arr(123);
		arr.Fill(20);
		for (size_t i = 0; i < arr.Size(); ++i)
		{
			REQUIRE(arr[i] == 20);
		}
	}
}