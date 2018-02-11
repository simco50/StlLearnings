#include "../catch.hpp"
#include "../Std/String.h"
using namespace FluxStd;

TEST_CASE("String - StrLen - Non-empty char", "[String]")
{
	char pData[] = "Hello world";
	REQUIRE(StrLen(pData) == 11);
}

TEST_CASE("String - StrLen - Empty char", "[String]")
{
	char pData[] = "";
	REQUIRE(StrLen(pData) == 0);
}

TEST_CASE("String - StrLen - Non-empty wchar_t", "[String]")
{
	wchar_t pData[] = L"Hello world";
	REQUIRE(StrLen(pData) == 11);
}

TEST_CASE("String - StrLen - Eempty wchar_t", "[String]")
{
	wchar_t pData[] = L"";
	REQUIRE(StrLen(pData) == 0);
}