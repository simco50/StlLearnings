#include "../catch.hpp"
#include "../Std/Vector.h"
#include "../Std/Algorithm.h"

using namespace FluxStd;

TEST_CASE("Algorithm - Swap", "[Algorithm]")
{
	SECTION("Value")
	{
		int a = 4;
		int b = 10;
		Swap(a, b);
		REQUIRE(a == 10);
		REQUIRE(b == 4);
	}
	SECTION("Pointer")
	{
		int a = 4;
		int b = 10;
		int* pA = &a;
		int* pB = &b;
		IteratorSwap(pA, pB);
		REQUIRE(a == 10);
		REQUIRE(b == 4);
	}
	SECTION("Iterator")
	{
		int a = 4;
		int b = 10;
		RandomAccessIterator<int> pA(&a);
		RandomAccessIterator<int> pB(&b);
		IteratorSwap(pA, pB);
		REQUIRE(a == 10);
		REQUIRE(b == 4);
	}
}

TEST_CASE("Algorithm - ForEach", "[Algorithm]")
{
	SECTION("Non-const")
	{
		Vector<int> v1 = { 1,2,3 };
		ForEach(v1.Begin(), v1.End(), [](int& a) { a *= 2; });
		REQUIRE(v1[0] == 2);
		REQUIRE(v1[1] == 4);
		REQUIRE(v1[2] == 6);
	}
	SECTION("Const")
	{
		int nr = 0;
		const Vector<int> v1 = { 1,2,3 };
		ForEach(v1.Begin(), v1.End(), [&nr](const int& a) { a; nr += 1; });
		REQUIRE(nr == 3);
	}
}

TEST_CASE("Algorithm - Generate", "[Algorithm]")
{
	SECTION("Empty")
	{
		Vector<int> v1;
		int nr = 0;
		REQUIRE_NOTHROW(Generate(v1.Begin(), v1.End(), [&nr]() { return nr++; }));
	}
	SECTION("Non-empty")
	{
		Vector<int> v1;
		v1.Resize(10);
		int nr = 0;
		Generate(v1.Begin(), v1.End(), [&nr]() { return nr++; });
		for (size_t i = 0; i < 10; ++i)
		{
			REQUIRE(v1[i] == (int)i);
		}
	}
}

TEST_CASE("Algorithm - Fill", "[Algorithm]")
{
	SECTION("Empty")
	{
		Vector<int> v1;
		REQUIRE_NOTHROW(Fill(v1.Begin(), v1.End(), 20));
	}
	SECTION("Non-empty")
	{
		Vector<int> v1;
		size_t nr = 5;
		v1.Resize(nr);
		Fill(v1.Begin(), v1.End(), (int)nr);
		for (size_t i = 0; i < nr; ++i)
		{
			REQUIRE(v1[i] == (int)nr);
		}
	}
}

TEST_CASE("Algorithm - Find", "[Algorithm]")
{
	SECTION("Find non-const")
	{
		Vector<int> v1 = { 1,2,3 };
		REQUIRE(Find(v1.Begin(), v1.End(), 1) != v1.End());
		REQUIRE(Find(v1.Begin(), v1.End(), 4) == v1.End());
	}
	SECTION("Find Const")
	{
		const Vector<int> v1 = { 1,2,3 };
		REQUIRE(Find(v1.Begin(), v1.End(), 1) != v1.End());
		REQUIRE(Find(v1.Begin(), v1.End(), 4) == v1.End());
	}
	SECTION("FindIf - non-const")
	{
		Vector<int> v1 = { 1,2,3 };
		REQUIRE(FindIf(v1.Begin(), v1.End(), [](int value) { return value == 1; }) != v1.End());
		REQUIRE(FindIf(v1.Begin(), v1.End(), [](int value) { return value == 4; }) == v1.End());
	}
	SECTION("FindIf - Const")
	{
		const Vector<int> v1 = { 1,2,3 };
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
	SECTION("MaxElement - non-const")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MaxElement(v1.Begin(), v1.End());
		REQUIRE(*i == 3);
	}
	SECTION("MaxElement - const")
	{
		const Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::ConstIterator i = MaxElement(v1.Begin(), v1.End());
		REQUIRE(*i == 3);
	}
	SECTION("MaxElement - Predicate - non-const")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MaxElement(v1.Begin(), v1.End(), [](int x, int y) { return x < y; });
		REQUIRE(*i == 3);
	}
	SECTION("MaxElement - Predicate - const")
	{
		const Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::ConstIterator i = MaxElement(v1.Begin(), v1.End(), [](int x, int y) { return x < y; });
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
	SECTION("MinElement - non-const")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MinElement(v1.Begin(), v1.End());
		REQUIRE(*i == 1);
	}
	SECTION("MinElement - const")
	{
		const Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::ConstIterator i = MinElement(v1.Begin(), v1.End());
		REQUIRE(*i == 1);
	}
	SECTION("MinElement - Predicate - non-const")
	{
		Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::Iterator i = MinElement(v1.Begin(), v1.End(), [](int x, int y) { return x < y; });
		REQUIRE(*i == 1);
	}
	SECTION("MinElement - Predicate - const")
	{
		const Vector<int> v1 = { 1,2,2,3 };
		Vector<int>::ConstIterator i = MinElement(v1.Begin(), v1.End(), [](int x, int y) { return x < y; });
		REQUIRE(*i == 1);
	}
}