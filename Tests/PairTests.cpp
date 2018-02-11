#pragma once
#include "../catch.hpp"
#include "../Std/Pair.h"
using namespace FluxStd;

TEST_CASE("Pair - Constructor - Empty", "[Pair]")
{
	Pair<int, double> p;
	REQUIRE(p.First == 0);
	REQUIRE(p.Second == 0.0);
	REQUIRE(*reinterpret_cast<int*>(&p) == 0);
}

TEST_CASE("Pair - Constructor - By reference", "[Pair]")
{
	Pair<int, double> p(1, 5.0);
	REQUIRE(p.First == 1);
	REQUIRE(p.Second == 5.0);
	REQUIRE(*reinterpret_cast<int*>(&p) == 1);
}

TEST_CASE("Pair - Copy constructor - From non-empty", "[Pair]")
{
	Pair<int, double> p1(1, 5.0);
	Pair<int, double> p2(p1);
	REQUIRE(p1.First == 1);
	REQUIRE(p1.Second == 5.0);
	REQUIRE(p2.First == 1);
	REQUIRE(p2.Second == 5.0);
}

TEST_CASE("Pair - Copy constructor - From empty", "[Pair]")
{
	Pair<int, double> p1;
	Pair<int, double> p2(p1);
	REQUIRE(p1.First == 0);
	REQUIRE(p1.Second == 0.0);
	REQUIRE(p2.First == 0);
	REQUIRE(p2.Second == 0.0);
}

TEST_CASE("Pair - Assignment operator - From non-empty", "[Pair]")
{
	Pair<int, double> p1(1, 5.0);
	Pair<int, double> p2;
	p2 = p1;
	REQUIRE(p1.First == 1);
	REQUIRE(p1.Second == 5.0);
	REQUIRE(p2.First == 1);
	REQUIRE(p2.Second == 5.0);
}

TEST_CASE("Pair - Assignment operator - From empty", "[Pair]")
{
	Pair<int, double> p1;
	Pair<int, double> p2(1, 5.0);
	p2 = p1;
	REQUIRE(p1.First == 0);
	REQUIRE(p1.Second == 0.0);
	REQUIRE(p2.First == 0);
	REQUIRE(p2.Second == 0.0);
}

TEST_CASE("Pair - Comparison - Not equal", "[Pair]")
{
	Pair<int, double> p1;
	Pair<int, double> p2(1, 5.0);
	REQUIRE(!(p1 == p2));
	REQUIRE(p1 != p2);
}

TEST_CASE("Pair - Comparison - Equal", "[Pair]")
{
	Pair<int, double> p1(1, 5.0);
	Pair<int, double> p2(1, 5.0);
	REQUIRE(p1 == p2);
	REQUIRE(!(p1 != p2));
}