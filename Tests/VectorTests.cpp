#include "../catch.hpp"
#include "../Std/Vector.h"
using namespace FluxStd;

#pragma region Constructors

TEST_CASE("Vector - Constructor", "[Vector]")
{
	SECTION("Empty")
	{
		Vector<int> v;
		REQUIRE(v.Size() == 0);
		REQUIRE(v.Capacity() == 0);
		REQUIRE(v.Empty());
		REQUIRE(v.begin() == nullptr);
		REQUIRE(v.end() == nullptr);
	}
	SECTION("int")
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
	SECTION("Assign")
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
	SECTION("Data")
	{
		const size_t size = 5;
		int* pData = new int[size];
		for (size_t i = 0; i < size ; i++)
			pData[i] = 50;
		Vector<int> v(pData, size);
		delete[] pData;
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
	SECTION("Initializer List")
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
	SECTION("Move semantics")
	{
		Vector<int> v1 = { 0, 1, 2, 3, 4 };
		Vector<int> v2(Move(v1));

		REQUIRE(v1.Data() == nullptr);
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
		REQUIRE(v1.Empty());
		REQUIRE(v1.begin() == nullptr);
		REQUIRE(v1.end() == nullptr);
		REQUIRE(v1.begin() == v1.end());

		REQUIRE(v2.Data() != nullptr);
		REQUIRE(v2.Size() == 5);
		REQUIRE(v2.Capacity() == 5);
		REQUIRE(!v2.Empty());
		REQUIRE(v2.begin() != nullptr);
		REQUIRE(v2.end() != nullptr);
		REQUIRE(v2.begin() != v2.end());
	}
	SECTION("Copy")
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
}

TEST_CASE("Vector - Assigment Operator", "[Vector]")
{
	SECTION("Copy - Same Size")
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
	SECTION("Copy - Different Size")
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
	SECTION("Copy - From Empty")
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
	SECTION("Copy - To Empty")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		Vector<int> v2;
		v1 = v2;

		REQUIRE(v2.Data() == nullptr);
		REQUIRE(v2.Size() == 0);
		REQUIRE(v2.Capacity() == 0);
		REQUIRE(v2.Empty());
		REQUIRE(v2.begin() == nullptr);
		REQUIRE(v2.end() == nullptr);

		REQUIRE(v1.Data() == nullptr);
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
		REQUIRE(v1.Empty());
		REQUIRE(v1.begin() == nullptr);
		REQUIRE(v1.end() == nullptr);
	}
}

TEST_CASE("Vector - Comparison", "[Vector]")
{
	SECTION("Same size")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		Vector<int> v2 = { 5, 6, 7, 8 };
		REQUIRE(v1 == v2);
		REQUIRE(!(v1 != v2));
	}
	SECTION("Different size")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		Vector<int> v2 = { 5, 6, 7 };
		REQUIRE(!(v1 == v2));
		REQUIRE(v1 != v2);
	}
	SECTION("Same size, Different values")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		Vector<int> v2 = { 5, 6, 7, 9 };
		REQUIRE(!(v1 == v2));
		REQUIRE(v1 != v2);
	}
}

#pragma region Sizing

TEST_CASE("Vector - Clear", "[Vector]")
{
	SECTION("Empty")
	{
		Vector<int> v1;
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
		v1.Clear();
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
	}
	SECTION("Non-empty")
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
}

TEST_CASE("Vector - Resize", "[Vector]")
{
	SECTION("Increase size and capacity")
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
	}
	SECTION("Decrease size")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 4);
		v1.Resize(2);
		REQUIRE(v1.Size() == 2);
		REQUIRE(v1.Capacity() == 2);
	}
}

TEST_CASE("Vector - Reserve", "[Vector]")
{
	SECTION("Increase capacity")
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
	SECTION("Lower capacity")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		int* pData = v1.Data();
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 4);
		v1.Reserve(2);
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 4);
		REQUIRE(v1.Data() == pData);
		pData = v1.Data();
		v1.Reserve(10);
		REQUIRE(v1.Data() != pData);
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 10);
	}
}

TEST_CASE("Vector - ShrinkToFit", "[Vector]")
{
	SECTION("After push")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5, 4, 10 };
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 7);
		v1.Push(10);
		REQUIRE(v1.Capacity() == 7 + Vector<int>::CAPACITY_STEP_SIZE);
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 8);
	}
	SECTION("After resize")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5, 4, 10 };
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 7);
		v1.Resize(10);
		REQUIRE(v1.Capacity() == 10);
		REQUIRE(v1.Size() == 10);
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 10);
		REQUIRE(v1.Size() == 10);
	}
	SECTION("After reserve")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5, 4, 10 };
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 7);
		v1.Reserve(10);
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 7);
		REQUIRE(v1.Size() == 7);
	}
}

#pragma endregion Sizing

#pragma region Addition/Deletion

TEST_CASE("Vector - Push", "[Vector]")
{
	SECTION("Empty vector")
	{
		Vector<int> v1;
		REQUIRE(v1.Size() == 0);
		v1.Push(1);
		REQUIRE(v1.Size() == 1);
		REQUIRE(v1.Capacity() == Vector<int>::CAPACITY_STEP_SIZE);
		REQUIRE(v1[0] == 1);
		for (size_t i = 0; i < Vector<int>::CAPACITY_STEP_SIZE - 1; ++i)
		{
			v1.Push((int)i);
		}
		REQUIRE(v1.Size() == Vector<int>::CAPACITY_STEP_SIZE);
		REQUIRE(v1.Capacity() == Vector<int>::CAPACITY_STEP_SIZE);
		v1.Push(5);
		REQUIRE(v1.Size() == Vector<int>::CAPACITY_STEP_SIZE + 1);
		REQUIRE(v1.Capacity() == 2 * Vector<int>::CAPACITY_STEP_SIZE);
		REQUIRE(v1.Back() == 5);
	}
	SECTION("Non-empty vector")
	{
		Vector<int> v1 = { 1, 2, 3 };
		REQUIRE(v1.Size() == 3);
		int* pData = v1.Data();
		v1.Push(4);
		REQUIRE(pData != v1.Data());
		pData = v1.Data();
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 3 + Vector<int>::CAPACITY_STEP_SIZE);
		for (size_t i = 0; i < Vector<int>::CAPACITY_STEP_SIZE - 1; ++i)
		{
			v1.Push((int)i + 4);
		}
		REQUIRE(pData == v1.Data());
		pData = v1.Data();
		REQUIRE(v1.Capacity() == 3 + Vector<int>::CAPACITY_STEP_SIZE);
		v1.Push(10);
		REQUIRE(v1.Capacity() == 3 + 2 * Vector<int>::CAPACITY_STEP_SIZE);
		REQUIRE(pData != v1.Data());
	}
}

TEST_CASE("Vector - Pop", "[Vector]")
{
	SECTION("Non-empty")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		for (size_t i = v1.Size() - 1; i > 0; --i)
		{
			REQUIRE(v1.Size() == i + 1);
			REQUIRE(v1.Pop() == (int)i + 1);
		}
	}
}

TEST_CASE("Vector - Swap", "[Vector]")
{
	SECTION("Both filled")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		Vector<int> v2 = { 6, 7, 8 };
		int* pV1Data = v1.Data();
		int* pV2Data = v2.Data();
		REQUIRE(v1.Size() == 5);
		REQUIRE(v1.Capacity() == 5);
		REQUIRE(v2.Size() == 3);
		REQUIRE(v2.Capacity() == 3);
		v1.SwapVector(v2);
		REQUIRE(v1.Size() == 3);
		REQUIRE(v1.Capacity() == 3);
		REQUIRE(v2.Size() == 5);
		REQUIRE(v2.Capacity() == 5);
		REQUIRE(v1.Data() == pV2Data);
		REQUIRE(v2.Data() == pV1Data);
	}
	SECTION("One filled")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		Vector<int> v2;
		int* pV1Data = v1.Data();
		int* pV2Data = v2.Data();
		REQUIRE(v1.Size() == 5);
		REQUIRE(v1.Capacity() == 5);
		REQUIRE(v2.Size() == 0);
		REQUIRE(v2.Capacity() == 0);
		v1.SwapVector(v2);
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
		REQUIRE(v2.Size() == 5);
		REQUIRE(v2.Capacity() == 5);
		REQUIRE(v1.Data() == pV2Data);
		REQUIRE(v2.Data() == pV1Data);
	}
}

TEST_CASE("Vector - Assign multiple", "[Vector]")
{
	SECTION("No capacity")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		REQUIRE(v1.Size() == 5);
		int* pData = v1.Data();
		v1.Assign(4, 10);
		REQUIRE(pData != v1.Data());
		REQUIRE(v1.Size() == 9);
		REQUIRE(v1.Capacity() == 9);
		for (size_t i = 5; i < v1.Size(); ++i)
		{
			REQUIRE(v1[i] == 10);
		}
	}
	SECTION("Has capacity")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		REQUIRE(v1.Size() == 5);
		v1.Reserve(100);
		REQUIRE(v1.Capacity() == 100);
		REQUIRE(v1.Size() == 5);
		int* pData = v1.Data();
		v1.Assign(4, 10);
		REQUIRE(pData == v1.Data());
		REQUIRE(v1.Size() == 9);
		REQUIRE(v1.Capacity() == 100);
		for (size_t i = 5; i < v1.Size(); ++i)
		{
			REQUIRE(v1[i] == 10);
		}
	}
}

TEST_CASE("Vector - SwapEraseAt", "[Vector]")
{
	Vector<int> v1 = { 1, 2, 3, 4, 5 };
	v1.SwapEraseAt(2);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 5);
	REQUIRE(v1[3] == 4);
}

TEST_CASE("Vector - EraseAt", "[Vector]")
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

TEST_CASE("Vector - Insert", "[Vector]")
{
	SECTION("No capacity - In the middle")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		int* pData = v1.Data();
		v1.Insert(2, 10);
		REQUIRE(pData != v1.Data());
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 10);
		REQUIRE(v1[3] == 3);
		REQUIRE(v1[4] == 4);
		REQUIRE(v1[5] == 5);
		REQUIRE(v1.Size() == 6);
	}
	SECTION("Has capacity - In the middle")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		v1.Reserve(100);
		REQUIRE(v1.Capacity() == 100);
		int* pData = v1.Data();
		v1.Insert(2, 10);
		REQUIRE(pData == v1.Data());
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 10);
		REQUIRE(v1[3] == 3);
		REQUIRE(v1[4] == 4);
		REQUIRE(v1[5] == 5);
		REQUIRE(v1.Size() == 6);
		REQUIRE(v1.Capacity() == 100);
	}
	SECTION("No capacity - At the end")
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
	SECTION("Has capacity - At the end")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5 };
		v1.Reserve(100);
		REQUIRE(v1.Capacity() == 100);
		int* pData = v1.Data();
		v1.Insert(5, 10);
		REQUIRE(pData == v1.Data());
		REQUIRE(v1[0] == 1);
		REQUIRE(v1[1] == 2);
		REQUIRE(v1[2] == 3);
		REQUIRE(v1[3] == 4);
		REQUIRE(v1[4] == 5);
		REQUIRE(v1[5] == 10);
		REQUIRE(v1.Size() == 6);
		REQUIRE(v1.Capacity() == 100);
	}
}

#pragma endregion Addition/Deletion

#pragma region Search

TEST_CASE("Vector - Find", "[Vector]")
{
	SECTION("Non-Empty")
	{
		Vector<int> v1 = { 1, 2, 3, 4, 5, 4 };
		REQUIRE(v1.Find(4) != v1.end());
		REQUIRE(v1.Find(12) == v1.end());
	}
	SECTION("Non-Empty")
	{
		Vector<int> v1;
		REQUIRE(v1.Find(12) == v1.end());
	}
}

#pragma endregion Search

#pragma region Misc

TEST_CASE("Vector - Conversion", "[Vector]")
{
	SECTION("bool")
	{
		Vector<int> v1 = { 5, 6, 7, 8 };
		Vector<int> v2;
		REQUIRE(v1);
		REQUIRE(!v2);
	}
}

#pragma endregion Misc