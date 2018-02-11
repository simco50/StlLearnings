#include "../catch.hpp"
#include "../Std/String.h"
using namespace FluxStd;

TEST_CASE("String StrLen char", "[String]")
{
	char pData[] = "Hello world";
	REQUIRE(StrLen(pData) == 11);
}

TEST_CASE("String StrLen wchar_t", "[String]")
{
	wchar_t pData[] = L"Hello world";
	REQUIRE(StrLen(pData) == 11);
}