#pragma once
#include "../catch.hpp"
#include "../Std/Pair.h"
using namespace FluxStd;

TEST_CASE("Pair ALL", "[Pair]")
{
	Pair<int, double> p(1, 5.0);
	Pair<int, double> p2(p);
	REQUIRE(p == p2);
}