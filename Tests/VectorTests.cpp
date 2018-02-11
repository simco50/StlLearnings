#include "../catch.hpp"
#include "../Std/Vector.h"
using namespace FluxStd;

TEST_CASE("Vector Create empty constructor", "[Vector]")
{
	Vector<int> v;
	REQUIRE(v.Size() == 0);
	REQUIRE(v.Capacity() == 0);
	REQUIRE(v.Empty());
	REQUIRE(v.begin() == nullptr);
	REQUIRE(v.end() == nullptr);
}

TEST_CASE("Vector Create int constructor", "[Vector]")
{
	const size_t size = 5;
	Vector<int> v(size);
	REQUIRE(v.Size() == size);
	REQUIRE(v.Capacity() == size);
	REQUIRE(!v.Empty());
	REQUIRE(v.begin() != nullptr);
	REQUIRE(v.end() != nullptr);
	REQUIRE(v.begin() != v.end());
	REQUIRE(*v.begin() == v.Front());
	REQUIRE(*(v.end() - 1) == v.Back());

	for (size_t i = 0; i < size; ++i)
	{
		REQUIRE(v.Data()[i] == 0);
		REQUIRE(v[i] == 0);
		REQUIRE(v.At(i) == 0);
	}
}

TEST_CASE("Vector Create int & value constructor", "[Vector]")
{
	const size_t size = 5;
	Vector<int> v(size, 50);
	REQUIRE(v.Size() == size);
	REQUIRE(v.Capacity() == size);
	REQUIRE(!v.Empty());
	REQUIRE(v.begin() != nullptr);
	REQUIRE(v.end() != nullptr);
	REQUIRE(v.begin() != v.end());

	for (size_t i = 0; i < size; ++i)
	{
		REQUIRE(v.Data()[i] == 50);
		REQUIRE(v[i] == 50);
		REQUIRE(v.At(i) == 50);
	}
}

TEST_CASE("Vector Create initialized list constructor", "[Vector]")
{
	Vector<int> v = { 0, 1, 2, 3, 4 };
	REQUIRE(v.Size() == 5);
	REQUIRE(v.Capacity() == 5);
	REQUIRE(!v.Empty());
	REQUIRE(v.begin() != nullptr);
	REQUIRE(v.end() != nullptr);
	REQUIRE(v.begin() != v.end());

	for (size_t i = 0; i < v.Size(); ++i)
	{
		REQUIRE(v.Data()[i] == (int)i);
		REQUIRE(v[i] == (int)i);
		REQUIRE(v.At(i) == (int)i);
	}
}

TEST_CASE("Vector Create move semantics copy constructor", "[Vector]")
{
	Vector<int> v1 = { 0, 1, 2, 3, 4 };
	Vector<int> v2(std::move(v1));
	REQUIRE(v2.Data() != nullptr);
	REQUIRE(v2.Size() == 5);
	REQUIRE(v2.Capacity() == 5);
	REQUIRE(!v2.Empty());
	REQUIRE(v2.begin() != nullptr);
	REQUIRE(v2.end() != nullptr);
	REQUIRE(v2.begin() != v2.end());

	REQUIRE(v1.Data() == nullptr);
	REQUIRE(v1.Size() == 0);
	REQUIRE(v1.Capacity() == 0);
	REQUIRE(v1.Empty());
	REQUIRE(v1.begin() == nullptr);
	REQUIRE(v1.end() == nullptr);
	REQUIRE(v1.begin() == v1.end());
}

TEST_CASE("Vector Create copy constructor", "[Vector]")
{
	Vector<int> v1 = { 0, 1, 2, 3, 4 };
	Vector<int> v2(v1);
	REQUIRE(v2.Data() != nullptr);
	REQUIRE(v2.Size() == 5);
	REQUIRE(v2.Capacity() == 5);
	REQUIRE(!v2.Empty());
	REQUIRE(v2.begin() != nullptr);
	REQUIRE(v2.end() != nullptr);
	REQUIRE(v2.begin() != v2.end());

	REQUIRE(v1.Data() != nullptr);
	REQUIRE(v1.Size() == 5);
	REQUIRE(v1.Capacity() == 5);
	REQUIRE(!v1.Empty());
	REQUIRE(v1.begin() != nullptr);
	REQUIRE(v1.end() != nullptr);
	REQUIRE(v1.begin() != v1.end());

	REQUIRE(v1.begin() != v2.begin());
	REQUIRE(v1.end() != v2.end());
	REQUIRE(v1.Data() != v2.Data());
}

TEST_CASE("Vector Deep copy assigment operator - same size", "[Vector]")
{
	Vector<int> v1 = { 0, 1, 2, 3, 4 };
	Vector<int> v2 = { 5, 6, 7, 8, 9 };
	int* pData = v1.Data();
	v1 = v2;
	REQUIRE(v1.Data() == pData);

	REQUIRE(v2.Data() != nullptr);
	REQUIRE(v2.Size() == 5);
	REQUIRE(v2.Capacity() == 5);
	REQUIRE(!v2.Empty());
	REQUIRE(v2.begin() != nullptr);
	REQUIRE(v2.end() != nullptr);
	REQUIRE(v2.begin() != v2.end());

	REQUIRE(v1.Data() != nullptr);
	REQUIRE(v1.Size() == 5);
	REQUIRE(v1.Capacity() == 5);
	REQUIRE(!v1.Empty());
	REQUIRE(v1.begin() != nullptr);
	REQUIRE(v1.end() != nullptr);
	REQUIRE(v1.begin() != v1.end());
}

TEST_CASE("Vector Deep copy assigment operator - different size", "[Vector]")
{
	Vector<int> v1 = { 0, 1, 2, 3, 4 };
	Vector<int> v2 = { 5, 6, 7, 8 };
	v1 = v2;

	REQUIRE(v2.Data() != nullptr);
	REQUIRE(v2.Size() == 4);
	REQUIRE(v2.Capacity() == 4);
	REQUIRE(!v2.Empty());
	REQUIRE(v2.begin() != nullptr);
	REQUIRE(v2.end() != nullptr);
	REQUIRE(v2.begin() != v2.end());

	REQUIRE(v1.Data() != nullptr);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 4);
	REQUIRE(!v1.Empty());
	REQUIRE(v1.begin() != nullptr);
	REQUIRE(v1.end() != nullptr);
	REQUIRE(v1.begin() != v1.end());
}

TEST_CASE("Vector Deep copy assigment operator - From empty", "[Vector]")
{
	Vector<int> v1;
	Vector<int> v2 = { 5, 6, 7, 8 };
	v1 = v2;

	REQUIRE(v2.Data() != nullptr);
	REQUIRE(v2.Size() == 4);
	REQUIRE(v2.Capacity() == 4);
	REQUIRE(!v2.Empty());
	REQUIRE(v2.begin() != nullptr);
	REQUIRE(v2.end() != nullptr);
	REQUIRE(v2.begin() != v2.end());

	REQUIRE(v1.Data() != nullptr);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 4);
	REQUIRE(!v1.Empty());
	REQUIRE(v1.begin() != nullptr);
	REQUIRE(v1.end() != nullptr);
	REQUIRE(v1.begin() != v1.end());
}

TEST_CASE("Vector Comparison same size", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	Vector<int> v2 = { 5, 6, 7, 8 };
	REQUIRE(v1 == v2);
	REQUIRE(!(v1 != v2));
}

TEST_CASE("Vector Comparison different size", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	Vector<int> v2 = { 5, 6, 7 };
	REQUIRE(!(v1 == v2));
	REQUIRE(v1 != v2);
}

TEST_CASE("Vector Comparison same size different values", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	Vector<int> v2 = { 5, 6, 7, 9 };
	REQUIRE(!(v1 == v2));
	REQUIRE(v1 != v2);
}

TEST_CASE("Vector bool conversion", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	Vector<int> v2;
	REQUIRE(v1);
	REQUIRE(!v2);
}

TEST_CASE("Vector Clear", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 4);
	REQUIRE(v1.begin() != v1.end());
	v1.Clear();
	REQUIRE(v1.Size() == 0);
	REQUIRE(v1.Capacity() == 4);
	REQUIRE(v1.begin() == v1.end());
}

TEST_CASE("Vector Resize no capacity", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	int* pData = v1.Data();
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 4);
	v1.Resize(10);
	REQUIRE(pData != v1.Data());
	pData = v1.Data();
	REQUIRE(v1.Size() == 10);
	REQUIRE(v1.Capacity() == 10);
	for (size_t i = 4; i < 10; ++i)
	{
		REQUIRE(v1[i] == 0);
	}
	v1.Resize(5);
	REQUIRE(v1.Size() == 5);
	REQUIRE(v1.Capacity() == 5);
}

TEST_CASE("Vector Reserve", "[Vector]")
{
	Vector<int> v1 = { 5, 6, 7, 8 };
	int* pData = v1.Data();
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 4);
	v1.Reserve(10);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 10);
	REQUIRE(v1.Data() != pData);
	pData = v1.Data();
	v1.Reserve(10);
	REQUIRE(v1.Data() == pData);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 10);
}

TEST_CASE("Vector Push", "[Vector]")
{
	Vector<int> v1;
	REQUIRE(v1.Size() == 0);
	v1.Push(1);
	REQUIRE(v1.Size() == 1);
	REQUIRE(v1.Capacity() == 4);
	REQUIRE(v1[0] == 1);
	v1.Push(2);
	v1.Push(3);
	v1.Push(4);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1.Capacity() == 4);
	v1.Push(5);
	REQUIRE(v1.Size() == 5);
	REQUIRE(v1.Capacity() == 8);
	REQUIRE(v1.Back() == 5);
}

TEST_CASE("Vector Pop", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	for (size_t i = v1.Size() - 1; i > 0; --i)
	{
		REQUIRE(v1.Size() == i + 1);
		REQUIRE(v1.Pop() == (int)i + 1);
	}
}

TEST_CASE("Vector Swap", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	Vector<int> v2 = { 6, 7, 8 };
	int* pV1Data = v1.Data();
	int* pV2Data = v2.Data();
	REQUIRE(v1.Size() == 5);
	REQUIRE(v1.Capacity() == 5);
	REQUIRE(v2.Size() == 3);
	REQUIRE(v2.Capacity() == 3);
	v1.Swap(v2);
	REQUIRE(v1.Size() == 3);
	REQUIRE(v1.Capacity() == 3);
	REQUIRE(v2.Size() == 5);
	REQUIRE(v2.Capacity() == 5);
	REQUIRE(v1.Data() == pV2Data);
	REQUIRE(v2.Data() == pV1Data);
}

TEST_CASE("Vector Assign", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	v1.Assign(4, 10);
	REQUIRE(v1.Size() == 9);
	REQUIRE(v1.Capacity() == 9);
	for (size_t i = 5; i < v1.Size(); ++i)
	{
		REQUIRE(v1[i] == 10);
	}
}

TEST_CASE("Vector SwapEraseAt", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	v1.SwapEraseAt(2);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 5);
	REQUIRE(v1[3] == 4);
}

TEST_CASE("Vector EraseAt", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	v1.EraseAt(2);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 4);
	REQUIRE(v1[3] == 5);
	REQUIRE(v1.Size() == 4);
}

TEST_CASE("Vector Insert", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	v1.Insert(2, 10);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 10);
	REQUIRE(v1[3] == 3);
	REQUIRE(v1[4] == 4);
	REQUIRE(v1[5] == 5);
	REQUIRE(v1.Size() == 6);
}

TEST_CASE("Vector Insert at the end", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	v1.Insert(5, 10);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 3);
	REQUIRE(v1[3] == 4);
	REQUIRE(v1[4] == 5);
	REQUIRE(v1[5] == 10);
	REQUIRE(v1.Size() == 6);
}

TEST_CASE("Vector Find", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5, 4 };
	REQUIRE(v1.Find(4) == 3);
	REQUIRE(v1.Find(12) == -1);
}

TEST_CASE("Vector RFind", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5, 4, 10 };
	REQUIRE(v1.RFind(4) == 5);
	REQUIRE(v1.RFind(12) == -1);
}

TEST_CASE("Vector ShrinkToFit", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5, 4, 10 };
	v1.ShrinkToFit();
	REQUIRE(v1.Capacity() == 7);
	v1.Push(10);
	REQUIRE(v1.Capacity() == 11);
	v1.ShrinkToFit();
	REQUIRE(v1.Capacity() == 8);
}