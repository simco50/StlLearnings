#include "../catch.hpp"
#include "../Std/Vector.h"
#include "../Std/Sorting.h"

using namespace FluxStd;

TEST_CASE("Sorting - BubbleSort", "[Sorting]")
{
	SECTION("No predicate")
	{
		Vector<int> v1 = { 5,2,6,3,1,4,7 };
		BubbleSort(v1.Begin(), v1.End());
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v1[4] == 5);
		REQUIRE(v1[5] == 6);
		REQUIRE(v1[6] == 7);
	}
	SECTION("Predicate")
	{
		Vector<int> v1 = { 5,2,6,3,1,4,7 };
		BubbleSort(v1.Begin(), v1.End(), [](int a, int b) {return a > b; });
		REQUIRE(v1[6] == 1);
		REQUIRE(v1[5] == 2);
		REQUIRE(v1[4] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v1[2] == 5);
		REQUIRE(v1[1] == 6);
		REQUIRE(v1[0] == 7);
	}
}

TEST_CASE("Sorting - InsertionSort", "[Sorting]")
{
	SECTION("No predicate")
	{
		Vector<int> v1 = { 5,2,6,3,1,4,7 };
		InsertionSort(v1.Begin(), v1.End());
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v1[4] == 5);
		REQUIRE(v1[5] == 6);
		REQUIRE(v1[6] == 7);
	}
	SECTION("Predicate")
	{
		Vector<int> v1 = { 5,2,6,3,1,4,7 };
		InsertionSort(v1.Begin(), v1.End(), [](int a, int b) {return a > b; });
		REQUIRE(v1[6] == 1);
		REQUIRE(v1[5] == 2);
		REQUIRE(v1[4] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v1[2] == 5);
		REQUIRE(v1[1] == 6);
		REQUIRE(v1[0] == 7);
	}
}