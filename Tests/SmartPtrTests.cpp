#pragma once
#include "../catch.hpp"
#include "../Std/SharedPtr.h"
#include "../Std/WeakPtr.h"
#include "../Std/UniquePtr.h"
using namespace FluxStd;

TEST_CASE("SharedPtr - Constructor", "[SharedPtr]")
{
	SharedPtr<int> p;
	SharedPtr<int> q = MakeShared<int>(4);
	WeakPtr<int> f;
	WeakPtr<int> h(q);
	UniquePtr<int> s;
	UniquePtr<int> t = MakeUnique<int>(20);
}