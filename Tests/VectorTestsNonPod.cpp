#include "../catch.hpp"
#include "../Std/Vector.h"
using namespace FluxStd;

int gConstructions = 0;
int gDesctructions = 0;
int gCopies = 0;
int gMoves = 0;

struct TestType
{
	TestType(const int value = 0)
	{
		int Count = rand() % 5 + 1;
		Value = new int[Count];
		for (int i = 0; i < Count; ++i)
		{
			Value[i] = value;
		}
		++gConstructions;
	}

	TestType(TestType&& other) noexcept
		: Value(other.Value) 
	{ 
		++gMoves;
		other.Value = nullptr;
	}
	~TestType() 
	{ 
		if (Value)
		{
			++gDesctructions;
			delete[] Value;
		}
	}
	TestType(const TestType& other) : Value(new int(*other.Value)) 
	{
		++gCopies;
	}
	TestType& operator=(const TestType& other)
	{
		if (Value)
			delete[] Value;
		Value = new int(*other.Value);
		return *this;
	}
	TestType& operator=(TestType&& other) noexcept
	{
		if (Value)
			delete[] Value;
		Value = other.Value;
		other.Value = nullptr;
		return *this;
	}
	bool operator==(const int value) const { return Value[0] == value; }
	bool operator==(const TestType& other) const { return Value[0] == *other.Value; }
	bool operator!=(const TestType& other) const { return Value[0] != *other.Value; }
	operator int() const { return *Value; }

	int* Value = nullptr;

};

#pragma region Constructors

TEST_CASE("Vector - NonPoD - Constructor", "[Vector]")
{
	SECTION("Empty")
	{
		Vector<TestType> v;
		REQUIRE(v.Size() == 0);
		REQUIRE(v.Capacity() == 0);
		REQUIRE(v.Empty());
		REQUIRE(v.begin() == nullptr);
		REQUIRE(v.end() == nullptr);
	}
	SECTION("TestType")
	{
		const size_t size = 5;
		Vector<TestType> v(size);
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
		Vector<TestType> v(size, 50);
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
		TestType* pData = new TestType[size];
		for (size_t i = 0; i < size; i++)
			pData[i] = 50;
		Vector<TestType> v(pData, size);
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
		Vector<TestType> v = { 0, 1, 2, 3, 4 };
		REQUIRE(v.Size() == 5);
		REQUIRE(v.Capacity() == 5);
		REQUIRE(!v.Empty());
		REQUIRE(v.begin() != nullptr);
		REQUIRE(v.end() != nullptr);
		REQUIRE(v.begin() != v.end());

		for (int i = 0; i < (int)v.Size(); ++i)
		{
			REQUIRE(v.Data()[i] == (TestType)i);
			REQUIRE(v[i] == (TestType)i);
			REQUIRE(v.At(i) == (TestType)i);
		}
	}
	SECTION("Move semantics")
	{
		Vector<TestType> v1 = { 0, 1, 2, 3, 4 };
		Vector<TestType> v2(Move(v1));

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
		Vector<TestType> v1 = { 0, 1, 2, 3, 4 };
		Vector<TestType> v2(v1);
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

TEST_CASE("Vector - NonPoD - Assigment Operator", "[Vector]")
{
	SECTION("Copy - Same Size")
	{
		Vector<TestType> v1 = { 0, 1, 2, 3, 4 };
		Vector<TestType> v2 = { 5, 6, 7, 8, 9 };
		TestType* pData = v1.Data();
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
		Vector<TestType> v1 = { 0, 1, 2, 3, 4 };
		Vector<TestType> v2 = { 5, 6, 7, 8 };
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
		Vector<TestType> v1;
		Vector<TestType> v2 = { 5, 6, 7, 8 };
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
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		Vector<TestType> v2;
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

TEST_CASE("Vector - NonPoD - Comparison", "[Vector]")
{
	SECTION("Same size")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		Vector<TestType> v2 = { 5, 6, 7, 8 };
		REQUIRE(v1 == v2);
		REQUIRE(!(v1 != v2));
	}
	SECTION("Different size")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		Vector<TestType> v2 = { 5, 6, 7 };
		REQUIRE(!(v1 == v2));
		REQUIRE(v1 != v2);
	}
	SECTION("Same size, Different values")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		Vector<TestType> v2 = { 5, 6, 7, 9 };
		REQUIRE(!(v1 == v2));
		REQUIRE(v1 != v2);
	}
}

#pragma region Sizing

TEST_CASE("Vector - NonPoD - Clear", "[Vector]")
{
	SECTION("Empty")
	{
		Vector<TestType> v1;
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
		v1.Clear();
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
	}
	SECTION("Non-empty")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 4);
		REQUIRE(v1.begin() != v1.end());
		v1.Clear();
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 4);
		REQUIRE(v1.begin() == v1.end());
	}
}

TEST_CASE("Vector - NonPoD - Resize", "[Vector]")
{
	SECTION("Increase size and capacity")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		TestType* pData = v1.Data();
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
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		REQUIRE(v1.Size() == 4);
		REQUIRE(v1.Capacity() == 4);
		v1.Resize(2);
		REQUIRE(v1.Size() == 2);
		REQUIRE(v1.Capacity() == 2);
	}
}

TEST_CASE("Vector - NonPoD - Reserve", "[Vector]")
{
	SECTION("Increase capacity")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		TestType* pData = v1.Data();
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
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		TestType* pData = v1.Data();
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

TEST_CASE("Vector - NonPoD - ShrinkToFit", "[Vector]")
{
	SECTION("After push")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5, 4, 10 };
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 7);
		v1.Push(10);
		v1.ShrinkToFit();
		REQUIRE(v1.Capacity() == 8);
	}
	SECTION("After resize")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5, 4, 10 };
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
		Vector<TestType> v1 = { 1, 2, 3, 4, 5, 4, 10 };
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

TEST_CASE("Vector - NonPoD - Push", "[Vector]")
{
	SECTION("Empty vector")
	{
		Vector<TestType> v1;
		REQUIRE(v1.Size() == 0);
		v1.Push(1);
		REQUIRE(v1.Size() == 1);
	}
	SECTION("Non-empty vector")
	{
		Vector<TestType> v1 = { 1, 2, 3 };
		REQUIRE(v1.Size() == 3);
		TestType* pData = v1.Data();
		v1.Push(4);
		REQUIRE(pData != v1.Data());
		pData = v1.Data();
		REQUIRE(v1.Size() == 4);
	}
}

TEST_CASE("Vector - NonPoD - Pop", "[Vector]")
{
	SECTION("Non-empty")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		for (int i = (int)v1.Size() - 1; i > 0; --i)
		{
			REQUIRE(v1.Size() == i + 1);
			REQUIRE(v1.Pop() == (TestType)(i + 1));
		}
	}
}

TEST_CASE("Vector - NonPoD - Swap", "[Vector]")
{
	SECTION("Both filled")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		Vector<TestType> v2 = { 6, 7, 8 };
		TestType* pV1Data = v1.Data();
		TestType* pV2Data = v2.Data();
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
	SECTION("One filled")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		Vector<TestType> v2;
		TestType* pV1Data = v1.Data();
		TestType* pV2Data = v2.Data();
		REQUIRE(v1.Size() == 5);
		REQUIRE(v1.Capacity() == 5);
		REQUIRE(v2.Size() == 0);
		REQUIRE(v2.Capacity() == 0);
		v1.Swap(v2);
		REQUIRE(v1.Size() == 0);
		REQUIRE(v1.Capacity() == 0);
		REQUIRE(v2.Size() == 5);
		REQUIRE(v2.Capacity() == 5);
		REQUIRE(v1.Data() == pV2Data);
		REQUIRE(v2.Data() == pV1Data);
	}
}

TEST_CASE("Vector - NonPoD - Assign multiple", "[Vector]")
{
	SECTION("No capacity")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		REQUIRE(v1.Size() == 5);
		TestType* pData = v1.Data();
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
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		REQUIRE(v1.Size() == 5);
		v1.Reserve(100);
		REQUIRE(v1.Capacity() == 100);
		REQUIRE(v1.Size() == 5);
		TestType* pData = v1.Data();
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

TEST_CASE("Vector - NonPoD - SwapEraseAt", "[Vector]")
{
	Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
	v1.RemoveSwapAt(2);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 5);
	REQUIRE(v1[3] == 4);
}

TEST_CASE("Vector - NonPoD - EraseAt", "[Vector]")
{
	Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
	v1.RemoveAt(2);
	REQUIRE(v1.Size() == 4);
	REQUIRE(v1[0] == 1);
	REQUIRE(v1[1] == 2);
	REQUIRE(v1[2] == 4);
	REQUIRE(v1[3] == 5);
	REQUIRE(v1.Size() == 4);
}

TEST_CASE("Vector - NonPoD - Insert", "[Vector]")
{
	SECTION("No capacity - In the middle")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		TestType* pData = v1.Data();
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
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		v1.Reserve(100);
		REQUIRE(v1.Capacity() == 100);
		TestType* pData = v1.Data();
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
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
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
		Vector<TestType> v1 = { 1, 2, 3, 4, 5 };
		v1.Reserve(100);
		REQUIRE(v1.Capacity() == 100);
		TestType* pData = v1.Data();
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

TEST_CASE("Vector - NonPoD - Find", "[Vector]")
{
	SECTION("Non-Empty")
	{
		Vector<TestType> v1 = { 1, 2, 3, 4, 5, 4 };
		REQUIRE(v1.Find(4) != nullptr);
		REQUIRE(v1.Find(12) == nullptr);
	}
	SECTION("Non-Empty")
	{
		Vector<TestType> v1;
		REQUIRE(v1.Find(12) == nullptr);
	}
}

#pragma endregion Search

#pragma region Misc

TEST_CASE("Vector - NonPoD - Conversion", "[Vector]")
{
	SECTION("bool")
	{
		Vector<TestType> v1 = { 5, 6, 7, 8 };
		Vector<TestType> v2;
		REQUIRE(v1);
		REQUIRE(!v2);
	}
}

#pragma endregion Misc