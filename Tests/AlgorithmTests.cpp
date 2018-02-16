#include "../catch.hpp"
#include "../Std/Vector.h"
#include "../Std/Algorithm.h"

using namespace FluxStd;

TEST_CASE("Algorithm - ForEach", "[Algorithm]")
{
	Vector<int> v1 = { 1,2,3 };
	ForEach(v1.Begin(), v1.End(), [](int& a) { a *= 2; });
	REQUIRE(v1[0] == 2);
	REQUIRE(v1[1] == 4);
	REQUIRE(v1[2] == 6);
}

TEST_CASE("Algorithm - Find", "[Algorithm]")
{
	SECTION("Find")
	{
		Vector<int> v1 = { 1,2,3 };
		REQUIRE(Find(v1.Begin(), v1.End(), 1) != v1.End());
		REQUIRE(Find(v1.Begin(), v1.End(), 4) == v1.End());
	}
	SECTION("FindIf")
	{
		Vector<int> v1 = { 1,2,3 };
		REQUIRE(FindIf(v1.Begin(), v1.End(), [](int value) { return value == 1; }) != v1.End());
		REQUIRE(FindIf(v1.Begin(), v1.End(), [](int value) { return value == 4; }) == v1.End());
	}
}

TEST_CASE("Algorithm - Reverse", "[Algorithm]")
{
	Vector<int> v1 = { 1,2,3 };
	Reverse(v1.Begin(), v1.End());
	REQUIRE(v1[0] == 3);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 1);
}

TEST_CASE("Algorithm - Replace", "[Algorithm]")
{
	SECTION("Replace")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Replace(v1.Begin(), v1.End(), 2, 5);
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 5);
		REQUIRE(v1[2] == 5);
		REQUIRE(v1[3] == 3);
	}
	SECTION("ReplaceIf")
	{
		Vector<int> v1 = { 1,2,2,5 };
		ReplaceIf(v1.Begin(), v1.End(), [](int value) { return value < 4; }, 3);
		REQUIRE(v1[0] == 3);
		REQUIRE(v1[1] == 3);
		REQUIRE(v1[2] == 3);
		REQUIRE(v1[3] == 5);
	}
}

TEST_CASE("Algorithm - Max", "[Algorithm]")
{
	SECTION("Max")
	{
		int a = 1, b = 4;
		REQUIRE(Max(a, b) == b);
	}
	SECTION("Max - Predicate")
	{
		int a = 1, b = 4;
		REQUIRE(Max(a, b, [](int x, int y) { return x < y; }) == b);
	}
	SECTION("MaxElement")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MaxElement(v1.Begin(), v1.End());
		REQUIRE(*i == 3);
	}
	SECTION("MaxElement - Predicate")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MaxElement(v1.Begin(), v1.End(), [](int x, int y) { return x < y; });
		REQUIRE(*i == 3);
	}
}

TEST_CASE("Algorithm - Min", "[Algorithm]")
{
	SECTION("Min")
	{
		int a = 1, b = 4;
		REQUIRE(Min(a, b) == a);
	}
	SECTION("Min - Predicate")
	{
		int a = 1, b = 4;
		REQUIRE(Min(a, b, [](int x, int y) { return x < y; }) == a);
	}
	SECTION("MinElement")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MinElement(v1.Begin(), v1.End());
		REQUIRE(*i == 1);
	}
	SECTION("MinElement - Predicate")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MinElement(v1.Begin(), v1.End(), [](int x, int y) { return x < y; });
		REQUIRE(*i == 1);
	}
}