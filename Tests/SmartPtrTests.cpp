#pragma once
#include "../catch.hpp"
#include "../Std/SharedPtr.h"
#include "../Std/UniquePtr.h"
using namespace FluxStd;

#pragma region UniquePtr

TEST_CASE("UniquePtr - Constructor", "[UniquePtr]")
{
	SECTION("Empty")
	{
		UniquePtr<int> a;
		REQUIRE(!a.IsValid());
	}
	SECTION("Non-empty")
	{
		UniquePtr<int> a(new int(1));
		REQUIRE(a.IsValid());
		REQUIRE(a.Get() != nullptr);
		REQUIRE(*a.Get() == 1);
		REQUIRE(*a == 1);
		REQUIRE(a == true);
	}
	SECTION("Move semantics")
	{
		UniquePtr<int> a(new int(1));
		UniquePtr<int> b(Move(a));
		REQUIRE(!a.IsValid());
		REQUIRE(a.Get() == nullptr);
		REQUIRE(a == false);
		REQUIRE(b.IsValid());
		REQUIRE(b.Get() != nullptr);
		REQUIRE(b);
	}
}

TEST_CASE("UniquePtr - MakeUnique", "[UniquePtr]")
{
	UniquePtr<int> a = MakeUnique<int>(1);
	REQUIRE(a.IsValid());
}

TEST_CASE("UniquePtr - Assignment", "[UniquePtr]")
{
	SECTION("Move semantics")
	{
		UniquePtr<int> a = MakeUnique<int>(1);
		REQUIRE(a.IsValid());
		UniquePtr<int> b;
		REQUIRE(!b.IsValid());
		b = Move(a);
		REQUIRE(!a.IsValid());
		REQUIRE(b.IsValid());
	}
}

TEST_CASE("UniquePtr - Swap", "[UniquePtr]")
{
	UniquePtr<int> a = MakeUnique<int>(1);
	REQUIRE(a.IsValid());
	UniquePtr<int> b;
	REQUIRE(!b.IsValid());
	b.Swap(a);
	REQUIRE(!a.IsValid());
	REQUIRE(b.IsValid());
}

TEST_CASE("UniquePtr - Release", "[UniquePtr]")
{
	UniquePtr<int> a = MakeUnique<int>(1);
	REQUIRE(a.IsValid());
	a.Release();
	a.Release();
	a.Release();
	a.Release();
	REQUIRE(!a.IsValid());
}

TEST_CASE("UniquePtr - Detach", "[UniquePtr]")
{
	UniquePtr<int> a = MakeUnique<int>(1);
	REQUIRE(a.IsValid());
	int* pLooseObject = a.Detach();
	REQUIRE(!a.IsValid());
	REQUIRE(pLooseObject != nullptr);
	delete pLooseObject;
}

TEST_CASE("UniquePtr - Comparison", "[UniquePtr]")
{
	UniquePtr<int> a = MakeUnique<int>(1);
	UniquePtr<int> b = MakeUnique<int>(1);
	REQUIRE(!(a == b));
	REQUIRE(a != b);
}

TEST_CASE("UniquePtr - Array", "[UniquePtr]")
{
	UniquePtr<int>* a = new UniquePtr<int>[10];
	for (int i = 0; i < 10; ++i)
	{
		a[i] = MakeUnique<int>(i);
	}
	for (int i = 0; i < 10; ++i)
	{
		REQUIRE(a[i].IsValid());
	}
	delete[] a;
}

#pragma endregion

#pragma region SharedPtr

TEST_CASE("SharedPtr - Create empty constructor", "[SharedPtr]")
{
	SharedPtr<int> a;
	REQUIRE(!a.IsValid());
	REQUIRE(a.Get() == nullptr);
	REQUIRE(a == false);
	REQUIRE(a.GetRefCount() == 1);
	REQUIRE(a.GetWeakRefCount() == 1);
}

TEST_CASE("SharedPtr - Create constructor", "[SharedPtr]")
{
	SharedPtr<int> a(new int(1));
	REQUIRE(a.IsValid());
	REQUIRE(a.Get() != nullptr);
	REQUIRE(*a.Get() == 1);
	REQUIRE(*a == 1);
	REQUIRE(a == true);
}

TEST_CASE("SharedPtr - MakeShared", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	REQUIRE(a.IsValid());
}

TEST_CASE("SharedPtr - Copy", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	SharedPtr<int> b(a);

	REQUIRE(a.IsValid());
	REQUIRE(b.IsValid());
	REQUIRE(a.Get() == b.Get());
}

TEST_CASE("SharedPtr - Assignment SharedPtr", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	SharedPtr<int> b;
	REQUIRE(a.IsValid());
	REQUIRE(!b.IsValid());
	REQUIRE(a.Get() != b.Get());

	REQUIRE(a.GetRefCount() == 1);
	REQUIRE(b.GetRefCount() == 1);

	b = a;

	REQUIRE(a.GetRefCount() == 2);
	REQUIRE(b.GetRefCount() == 2);
	REQUIRE(a.IsValid());
	REQUIRE(b.IsValid());
	REQUIRE(a.Get() == b.Get());
}

TEST_CASE("SharedPtr - Assignment RawPtr", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	REQUIRE(a.IsValid());
	REQUIRE(*a == 1);
	REQUIRE(a.GetRefCount() == 1);
	a = new int(4);
	REQUIRE(a.IsValid());
	REQUIRE(a.GetRefCount() == 1);
	REQUIRE(*a == 4);
}

TEST_CASE("SharedPtr - operator->", "[SharedPtr]")
{
	struct Test
	{
		void Do()
		{
			++a;
		}
		int a = 0;
	};

	SharedPtr<Test> a = MakeShared<Test>();
	REQUIRE(a.IsValid());
	REQUIRE(a.GetRefCount() == 1);
	REQUIRE(a->a == 0);
	a->Do();
	REQUIRE(a->a == 1);
}

TEST_CASE("SharedPtr - operator*", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	REQUIRE(a.IsValid());
	REQUIRE(a.GetRefCount() == 1);
	REQUIRE(*a == 1);
	(*a)++;
	REQUIRE(*a == 2);
}

TEST_CASE("SharedPtr - Hash", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	REQUIRE(a.Hash() == (size_t)a.Get() / sizeof(int));
}

TEST_CASE("SharedPtr - Release", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	SharedPtr<int> b = a;
	REQUIRE(a.IsValid());
	REQUIRE(b.IsValid());
	REQUIRE(b.GetRefCount() == 2);
	REQUIRE(a.GetRefCount() == 2);
	a.Release();
	REQUIRE(!a.IsValid());
	REQUIRE(b.IsValid());
	REQUIRE(b.GetRefCount() == 1);
	b.Release();
	REQUIRE(!a.IsValid());
	REQUIRE(!b.IsValid());
}

TEST_CASE("SharedPtr - Comparison", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	SharedPtr<int> b = a;

	REQUIRE(a == b);
	REQUIRE(!(a != b));
}

TEST_CASE("SharedPtr - RefCount", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	SharedPtr<int> b = a;

	REQUIRE(a.GetRefCount() == 2);
	REQUIRE(b.GetRefCount() == 2);
	REQUIRE(b.GetWeakRefCount() == 1);
}

TEST_CASE("SharedPtr - RefCount with scope", "[SharedPtr]")
{
	SharedPtr<int> a = MakeShared<int>(1);
	{
		SharedPtr<int> b = a;
		REQUIRE(a.GetRefCount() == 2);
		REQUIRE(b.GetRefCount() == 2);
		REQUIRE(a.GetWeakRefCount() == 1);
		REQUIRE(b.GetWeakRefCount() == 1);
	}
	REQUIRE(a.GetRefCount() == 1);
	REQUIRE(a.GetWeakRefCount() == 1);
}

#pragma endregion

#pragma region WeakPtr

TEST_CASE("WeakPtr - Create empty constructor", "[WeakPtr]")
{
	WeakPtr<int> a;
	REQUIRE(!a.IsValid());
	REQUIRE(a == false);
	REQUIRE(a.GetRefCount() == 0);
	REQUIRE(a.GetWeakRefCount() == 0);
}

TEST_CASE("WeakPtr - Create constructor", "[WeakPtr]")
{
	SharedPtr<int> shared = MakeShared<int>(1);
	WeakPtr<int> weak(shared);
	REQUIRE(weak.IsValid());
	REQUIRE(weak == true);
	REQUIRE(weak.GetRefCount() == 1);
	REQUIRE(weak.GetWeakRefCount() == 2);
}

TEST_CASE("WeakPtr - Create constructor and leave scope", "[WeakPtr]")
{
	WeakPtr<int> weak;
	REQUIRE(!weak.IsValid());
	{
		SharedPtr<int> shared = MakeShared<int>(1);
		REQUIRE(shared.IsValid());
		weak = shared;
		REQUIRE(weak.GetRefCount() == 1);
		REQUIRE(weak.GetWeakRefCount() == 2);
		REQUIRE(weak.IsValid());
	}
	REQUIRE(!weak.IsValid());
}

TEST_CASE("WeakPtr - Pin", "[WeakPtr]")
{
	WeakPtr<int> weak;
	REQUIRE(!weak.IsValid());
	{
		SharedPtr<int> shared = MakeShared<int>(1);
		REQUIRE(shared.IsValid());
		weak = shared;
		REQUIRE(weak.GetRefCount() == 1);
		REQUIRE(weak.GetWeakRefCount() == 2);
		REQUIRE(weak.IsValid());
		{
			SharedPtr<int> weakPin = weak.Pin();
			REQUIRE(weakPin.IsValid());
			REQUIRE(weakPin.Get() == shared.Get());
			REQUIRE(weak.GetRefCount() == 2);
			REQUIRE(weak.GetWeakRefCount() == 2);
		}
		REQUIRE(weak.GetRefCount() == 1);
		REQUIRE(weak.GetWeakRefCount() == 2);
	}
	REQUIRE(!weak.IsValid());
	REQUIRE(!weak.Pin().IsValid());
}

TEST_CASE("WeakPtr - Release", "[WeakPtr]")
{
	WeakPtr<int> weak;
	REQUIRE(!weak.IsValid());
	{
		SharedPtr<int> shared = MakeShared<int>(1);
		REQUIRE(shared.IsValid());
		weak = shared;
		REQUIRE(shared.GetRefCount() == 1);
		REQUIRE(shared.GetWeakRefCount() == 2);
		REQUIRE(weak.GetRefCount() == 1);
		REQUIRE(weak.GetWeakRefCount() == 2);
		REQUIRE(weak.IsValid());
		weak.Release();
		REQUIRE(shared.GetRefCount() == 1);
		REQUIRE(shared.GetWeakRefCount() == 1);
		REQUIRE(!weak.IsValid());
	}
	REQUIRE(!weak.IsValid());
}

#pragma endregion