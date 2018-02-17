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
	SECTION("Empty")
	{
		Vector<int> v1;
		REQUIRE_NOTHROW(BubbleSort(v1.Begin(), v1.End()));
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
	SECTION("Empty")
	{
		Vector<int> v1;
		REQUIRE_NOTHROW(InsertionSort(v1.Begin(), v1.End(), [](int a, int b) {return a > b; }));
	}
}

TEST_CASE("Sorting - QuickSort", "[Sorting]")
{
	SECTION("Non-empty")
	{
		size_t count = 20;
		Vector<int> v1;
		v1.Resize(count);
		for (size_t i = 0; i < count; ++i)
			v1[i] = (int)i;
		std::random_shuffle(v1.begin().pPtr, v1.end().pPtr);
		QuickSort(v1.Begin(), v1.End());
		for (size_t i = 0; i < count; ++i)
			REQUIRE(v1[i] == (int)i);
	}
	SECTION("Empty")
	{
		Vector<int> v1;
		REQUIRE_NOTHROW(QuickSort(v1.Begin(), v1.End()));
	}
}

TEST_CASE("Sorting - IsSorted", "[Sorting]")
{
	SECTION("Non-empty")
	{
		Vector<int> v1 = { 5,2,6,3,1,4,7 };
		InsertionSort(v1.Begin(), v1.End());
		REQUIRE(IsSorted(v1.Begin(), v1.End()));
	}
	SECTION("Non-empty - Predicate")
	{
		Vector<int> v1 = { 5,2,6,3,1,4,7 };
		InsertionSort(v1.Begin(), v1.End(), [](int a, int b) {return a > b; });
		REQUIRE(IsSorted(v1.Begin(), v1.End(), [](int a, int b) {return a > b; }));
	}
	SECTION("Empty")
	{
		Vector<int> v1;
		InsertionSort(v1.Begin(), v1.End());
		REQUIRE(IsSorted(v1.Begin(), v1.End()));
	}
}