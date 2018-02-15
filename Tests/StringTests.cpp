#include "../catch.hpp"
#include "../Std/String.h"
using namespace FluxStd;

#pragma region StrLen

TEST_CASE("String - StrLen", "[String]")
{
	SECTION("Non-empty char")
	{
		char pData[] = "Hello world";
		REQUIRE(StrLen(pData) == 11);
	}
	SECTION("Empty char")
	{
		char pData[] = "";
		REQUIRE(StrLen(pData) == 0);
	}
	SECTION("Non-empty wchar_t")
	{
		wchar_t pData[] = L"Hello world";
		REQUIRE(StrLen(pData) == 11);
	}
	SECTION("Empty wchar_t")
	{
		wchar_t pData[] = L"";
		REQUIRE(StrLen(pData) == 0);
	}
}

#pragma endregion StrLen

#pragma region Constructors

TEST_CASE("String - Constructor", "[String]")
{
	SECTION("Empty")
	{
		String s;
		REQUIRE(s.Empty());
		REQUIRE(s == "");
		REQUIRE(s.Size() == 0);
		REQUIRE(s.Capacity() == 0);
		REQUIRE(s.begin() == nullptr);
		REQUIRE(s.end() == nullptr);
		REQUIRE(s.Data() == nullptr);
		REQUIRE(s.C_Str() == nullptr);
	}
	SECTION("int")
	{
		String s(10);
		REQUIRE(!s.Empty());
		REQUIRE(s == "");
		REQUIRE(s.Size() == 10);
		REQUIRE(s.Capacity() == 10);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.Data() != nullptr);
		REQUIRE(s.C_Str() != nullptr);
	}
	SECTION("char array")
	{
		String s("Hello world");
		REQUIRE(!s.Empty());
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.Data() != nullptr);
		REQUIRE(s.C_Str() != nullptr);
	}
	SECTION("wchar_t array")
	{
		String s(L"Hello world");
		REQUIRE(!s.Empty());
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.Data() != nullptr);
		REQUIRE(s.C_Str() != nullptr);
	}
	SECTION("begin/end")
	{
		char test[] = "Hello world";
		String s(&test[0], &test[0] + 11);
		REQUIRE(!s.Empty());
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.Data() != nullptr);
		REQUIRE(s.C_Str() != nullptr);
	}
	SECTION("Move semantics")
	{
		String s1(L"Hello world");
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);
		REQUIRE(s1.Data() != nullptr);
		REQUIRE(s1.C_Str() != nullptr);

		String s2(std::move(s1));
		REQUIRE(s1.Empty());
		REQUIRE(s1 == "");
		REQUIRE(s1.Size() == 0);
		REQUIRE(s1.Capacity() == 0);
		REQUIRE(s1.begin() == nullptr);
		REQUIRE(s1.end() == nullptr);
		REQUIRE(s1.Data() == nullptr);
		REQUIRE(s1.C_Str() == nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello world");
		REQUIRE(s2.Size() == 11);
		REQUIRE(s2.Capacity() == 11);
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);
		REQUIRE(s2.Data() != nullptr);
		REQUIRE(s2.C_Str() != nullptr);
	}
	SECTION("Deep copy")
	{
		String s1(L"Hello world");
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);
		REQUIRE(s1.Data() != nullptr);
		REQUIRE(s1.C_Str() != nullptr);

		String s2(s1);
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);
		REQUIRE(s1.Data() != nullptr);
		REQUIRE(s1.C_Str() != nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello world");
		REQUIRE(s2.Size() == 11);
		REQUIRE(s2.Capacity() == 11);
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);
		REQUIRE(s2.Data() != nullptr);
		REQUIRE(s2.C_Str() != nullptr);

		REQUIRE(s1.Data() != s2.Data());
	}
}

#pragma endregion Constructors

#pragma region Assignment

TEST_CASE("String - Assignment", "[String]")
{
	SECTION("Deep copy")
	{
		String s1(L"Hello world");
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);
		REQUIRE(s1.Data() != nullptr);
		REQUIRE(s1.C_Str() != nullptr);

		String s2;
		s2 = s1;

		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);
		REQUIRE(s1.Data() != nullptr);
		REQUIRE(s1.C_Str() != nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello world");
		REQUIRE(s2.Size() == 11);
		REQUIRE(s2.Capacity() == 11);
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);
		REQUIRE(s2.Data() != nullptr);
		REQUIRE(s2.C_Str() != nullptr);

		REQUIRE(s1.Data() != s2.Data());
	}
	SECTION("Deep copy empty")
	{
		String s1;
		String s2 = "Hello World";
		s2 = s1;

		REQUIRE(s1.Empty());
		REQUIRE(s1 == "");
		REQUIRE(s1.Size() == 0);
		REQUIRE(s1.Capacity() == 0);
		REQUIRE(s1.begin() == nullptr);
		REQUIRE(s1.end() == nullptr);
		REQUIRE(s1.Data() == nullptr);
		REQUIRE(s1.C_Str() == nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello World");
		REQUIRE(s2.Size() == 11);
		REQUIRE(s2.Capacity() == 11);
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);
		REQUIRE(s2.Data() != nullptr);
		REQUIRE(s2.C_Str() != nullptr);

		REQUIRE(s1.Data() != s2.Data());
	}
}

#pragma endregion Assignment

#pragma region Misc operators

TEST_CASE("String - Operator+", "[String]")
{
	SECTION("String")
	{
		String s1("Hello");
		char* pData = s1.Data();
		String s2(" World");
		s1 = s1 + s2;
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
	}
	SECTION("char array")
	{
		String s1("Hello");
		char* pData = s1.Data();
		s1 = s1 + " World";
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
	}
}

TEST_CASE("String - Operator+=", "[String]")
{
	SECTION("String")
	{
		String s1("Hello");
		char* pData = s1.Data();
		String s2(" World");
		s1 += s2;
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
	}
	SECTION("char array")
	{
		String s1("Hello");
		char* pData = s1.Data();
		s1 += " World";
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
		REQUIRE(s1.Size() == 11);
		REQUIRE(s1.Capacity() == 11);
	}
}

TEST_CASE("String - Operator== & Operator!=", "[String]")
{
	SECTION("String")
	{
		String s1("HellO");
		String s2("HeLlo");
		String s3("HellO");
		REQUIRE(s1 != s2);
		REQUIRE(s1 == s3);
		REQUIRE(s2 != s3);

		REQUIRE(!(s1 == s2));
		REQUIRE(!(s1 != s3));
		REQUIRE(!(s2 == s3));
	}
	SECTION("char array")
	{
		String s1("Hello");
		REQUIRE(s1 != "HeLlo");
		REQUIRE(s1 == "Hello");

		REQUIRE(!(s1 == "HeLlo"));
		REQUIRE(!(s1 != "Hello"));
	}
	SECTION("wchar_t array")
	{
		String s1("Hello");
		REQUIRE(s1 != L"HeLlo");
		REQUIRE(s1 == L"Hello");

		REQUIRE(!(s1 == L"HeLlo"));
		REQUIRE(!(s1 != L"Hello"));
	}
}

TEST_CASE("String - Accessors", "[String]")
{
	String s1("Hello World");
	REQUIRE(s1[2] == 'l');
	REQUIRE(s1[10] == 'd');

	REQUIRE(s1.At(2) == 'l');
	REQUIRE(s1.At(10) == 'd');
}

#pragma endregion Misc operators

#pragma region Sizing

TEST_CASE("String - Clear", "[String]")
{
	SECTION("Empty")
	{
		String s;
		REQUIRE(s.Size() == 0);
		REQUIRE(s.Capacity() == 0);
		s.Clear();
		REQUIRE(s.Size() == 0);
		REQUIRE(s.Capacity() == 0);
	}
	SECTION("Non-empty")
	{
		String s("Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != s.end());
		s.Clear();
		REQUIRE(s.Size() == 0);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() == s.end());
	}
}

TEST_CASE("String - Resize", "[String]")
{
	SECTION("Increase size and capacity")
	{
		String s = "Hello";
		char* pData = s.Data();
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 5);
		s.Resize(10);
		REQUIRE(pData != s.Data());
		pData = s.Data();
		REQUIRE(s.Size() == 10);
		REQUIRE(s.Capacity() == 10);
		for (size_t i = 5; i < 10; ++i)
		{
			REQUIRE(s[i] == 0);
		}
	}
	SECTION("Decrease size")
	{
		String s = "Hello";
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 5);
		s.Resize(2);
		REQUIRE(s.Size() == 2);
		REQUIRE(s.Capacity() == 2);
	}
}

TEST_CASE("String - Reserve", "[String]")
{
	SECTION("Increase capacity")
	{
		String s = "Hello";
		char* pData = s.Data();
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 5);
		s.Reserve(10);
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 10);
		REQUIRE(s.Data() != pData);
		pData = s.Data();
		s.Reserve(10);
		REQUIRE(s.Data() == pData);
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 10);
	}
	SECTION("Lower capacity")
	{
		String s = "Hello";
		char* pData = s.Data();
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 5);
		s.Reserve(2);
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 5);
		REQUIRE(s.Data() == pData);
		pData = s.Data();
		s.Reserve(10);
		REQUIRE(s.Data() != pData);
		REQUIRE(s.Size() == 5);
		REQUIRE(s.Capacity() == 10);
	}
}

TEST_CASE("String - ShrinkToFit", "[String]")
{
	SECTION("After push")
	{
		String s = "Hello";
		s.ShrinkToFit();
		REQUIRE(s.Capacity() == 5);
		s.Push('C');
		REQUIRE(s.Capacity() == 5 + String::CAPACITY_STEP_SIZE);
		s.ShrinkToFit();
		REQUIRE(s.Capacity() == 6);
	}
	SECTION("After resize")
	{
		String s = "Hello";
		s.ShrinkToFit();
		REQUIRE(s.Capacity() == 5);
		s.Resize(10);
		REQUIRE(s.Capacity() == 10);
		REQUIRE(s.Size() == 10);
		s.ShrinkToFit();
		REQUIRE(s.Capacity() == 10);
		REQUIRE(s.Size() == 10);
	}
	SECTION("After reserve")
	{
		String s = "Hello";
		s.ShrinkToFit();
		REQUIRE(s.Capacity() == 5);
		s.Reserve(10);
		s.ShrinkToFit();
		REQUIRE(s.Capacity() == 5);
		REQUIRE(s.Size() == 5);
	}
}

#pragma endregion Sizing

#pragma region Addition/Deletion

TEST_CASE("String - Push", "[String]")
{
	SECTION("Empty String")
	{
		String s;
		REQUIRE(s.Size() == 0);
		s.Push('O');
		REQUIRE(s.Size() == 1);
		REQUIRE(s.Capacity() == String::CAPACITY_STEP_SIZE);
		REQUIRE(s[0] == 'O');
		for (size_t i = 0; i < String::CAPACITY_STEP_SIZE - 1; ++i)
		{
			s.Push((char)('A' + i));
		}
		REQUIRE(s.Size() == String::CAPACITY_STEP_SIZE);
		REQUIRE(s.Capacity() == String::CAPACITY_STEP_SIZE);
		s.Push('I');
		REQUIRE(s.Size() == String::CAPACITY_STEP_SIZE + 1);
		REQUIRE(s.Capacity() == 2 * String::CAPACITY_STEP_SIZE);
		REQUIRE(s.Back() == 'I');
	}
	SECTION("Non-empty String")
	{
		String s = "Hel";
		REQUIRE(s.Size() == 3);
		char* pData = s.Data();
		s.Push('P');
		REQUIRE(pData != s.Data());
		pData = s.Data();
		REQUIRE(s.Size() == 4);
		REQUIRE(s.Capacity() == 3 + String::CAPACITY_STEP_SIZE);
		for (size_t i = 0; i < String::CAPACITY_STEP_SIZE - 1; ++i)
		{
			s.Push((char)(i + 'A'));
		}
		REQUIRE(pData == s.Data());
		pData = s.Data();
		REQUIRE(s.Capacity() == 3 + String::CAPACITY_STEP_SIZE);
		s.Push('O');
		REQUIRE(s.Capacity() == 3 + 2 * String::CAPACITY_STEP_SIZE);
		REQUIRE(pData != s.Data());
	}
}

TEST_CASE("String - Pop", "[String]")
{
	String s = "Hello";
	REQUIRE(s.Size() == 5);
	REQUIRE(s.Pop() == 'o');
	REQUIRE(s.Size() == 4);
	REQUIRE(s.Pop() == 'l');
	REQUIRE(s.Size() == 3);
	REQUIRE(s.Pop() == 'l');
	REQUIRE(s.Size() == 2);
	REQUIRE(s.Pop() == 'e');
	REQUIRE(s.Size() == 1);
	REQUIRE(s.Pop() == 'H');
	REQUIRE(s.Size() == 0);
}

TEST_CASE("String - Swap", "[String]")
{
	SECTION("Both filled")
	{
		String s1 = "Hello";
		String s2 = "Lol";
		char* pData1 = s1.Data();
		char* pData2 = s2.Data();
		REQUIRE(s1.Size() == 5);
		REQUIRE(s1.Capacity() == 5);
		REQUIRE(s2.Size() == 3);
		REQUIRE(s2.Capacity() == 3);
		s1.Swap(s2);
		REQUIRE(s1.Size() == 3);
		REQUIRE(s1.Capacity() == 3);
		REQUIRE(s2.Size() == 5);
		REQUIRE(s2.Capacity() == 5);
		REQUIRE(s1.Data() == pData2);
		REQUIRE(s2.Data() == pData1);
	}
	SECTION("One filled")
	{
		String s1 = "Hello";
		String s2;
		char* pData1 = s1.Data();
		char* pData2 = s2.Data();
		REQUIRE(s1.Size() == 5);
		REQUIRE(s1.Capacity() == 5);
		REQUIRE(s2.Size() == 0);
		REQUIRE(s2.Capacity() == 0);
		s1.Swap(s2);
		REQUIRE(s1.Size() == 0);
		REQUIRE(s1.Capacity() == 0);
		REQUIRE(s2.Size() == 5);
		REQUIRE(s2.Capacity() == 5);
		REQUIRE(s1.Data() == pData2);
		REQUIRE(s2.Data() == pData1);
	}
}

TEST_CASE("String - Assign multiple", "[String]")
{
	SECTION("No capacity")
	{
		String s = "Hello";
		REQUIRE(s.Size() == 5);
		char* pData = s.Data();
		s.Assign(4, 10);
		REQUIRE(pData != s.Data());
		REQUIRE(s.Size() == 9);
		REQUIRE(s.Capacity() == 9);
		for (size_t i = 5; i < s.Size(); ++i)
		{
			REQUIRE(s[i] == 10);
		}
	}
	SECTION("Has capacity")
	{
		String s = "Hello";
		REQUIRE(s.Size() == 5);
		s.Reserve(100);
		REQUIRE(s.Capacity() == 100);
		REQUIRE(s.Size() == 5);
		char* pData = s.Data();
		s.Assign(4, 10);
		REQUIRE(pData == s.Data());
		REQUIRE(s.Size() == 9);
		REQUIRE(s.Capacity() == 100);
		for (size_t i = 5; i < s.Size(); ++i)
		{
			REQUIRE(s[i] == 10);
		}
	}
}

TEST_CASE("String - EraseAt", "[String]")
{
	String s = "Hello";
	s.EraseAt(2);
	REQUIRE(s.Size() == 4);
	REQUIRE(s[0] == 'H');
	REQUIRE(s[1] == 'e');
	REQUIRE(s[2] == 'l');
	REQUIRE(s[3] == 'o');
}

TEST_CASE("String - Insert", "[String]")
{
	SECTION("char - No capacity - In the middle")
	{
		String s = "Hello";
		char* pData = s.Data();
		s.Insert(2, 'A');
		REQUIRE(pData != s.Data());
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'A');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'l');
		REQUIRE(s[5] == 'o');
		REQUIRE(s.Size() == 6);
	}
	SECTION("char - Has capacity - In the middle")
	{
		String s = "Hello";
		s.Reserve(100);
		REQUIRE(s.Capacity() == 100);
		char* pData = s.Data();
		s.Insert(2, 'A');
		REQUIRE(pData == s.Data());
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'A');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'l');
		REQUIRE(s[5] == 'o');
		REQUIRE(s.Size() == 6);
		REQUIRE(s.Capacity() == 100);
	}
	SECTION("char - No capacity - At the end")
	{
		String s = "Hello";
		s.Insert(5, 'A');
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'l');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'o');
		REQUIRE(s[5] == 'A');
		REQUIRE(s.Size() == 6);
	}
	SECTION("char - Has capacity - At the end")
	{
		String s = "Hello";
		s.Reserve(100);
		REQUIRE(s.Capacity() == 100);
		char* pData = s.Data();
		s.Insert(5, 'A');
		REQUIRE(pData == s.Data());
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'l');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'o');
		REQUIRE(s[5] == 'A');
		REQUIRE(s.Size() == 6);
		REQUIRE(s.Capacity() == 100);
	}
	SECTION("char array - In the middle")
	{
		String s = "Hello";
		s.Insert(2, " world");
		REQUIRE(s == "He worldllo");
		REQUIRE(s.Size() == 11);
	}
	SECTION("char array - At the end")
	{
		String s = "Hello";
		s.Insert(5, " world");
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
	}
	SECTION("char array - From empty string")
	{
		String s;
		s.Append("Hello world");
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.begin() != s.end());
	}
}

TEST_CASE("String - Append", "[String]")
{
	SECTION("char array - From non-empty string")
	{
		String s("Hello");
		s.Append(" world");
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.begin() != s.end());
	}
	SECTION("wchar_t array - From empty string")
	{
		String s;
		s.Append(L"Hello world");
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.begin() != s.end());
	}
	SECTION("wchar_t array - From non-empty string")
	{
		String s("Hello");
		s.Append(L" world");
		REQUIRE(s == "Hello world");
		REQUIRE(s.Size() == 11);
		REQUIRE(s.Capacity() == 11);
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.begin() != s.end());
	}
}

TEST_CASE("String - Substring", "[String]")
{
	SECTION("")
	{
		String s;
		String output = s.Substring(0);
		REQUIRE(output.Size() == 0);
		REQUIRE(output.Capacity() == 0);
		REQUIRE(output.Data() == nullptr);
	}
	SECTION("No length")
	{
		String s = "Hello World";
		String output = s.Substring(6);
		REQUIRE(output.Size() == 5);
		REQUIRE(output.Capacity() == 5);
		REQUIRE(output.Data() != nullptr);
		REQUIRE(output == "World");
	}
	SECTION("With length")
	{
		String s = "Hello World";
		String output = s.Substring(6, 5);
		REQUIRE(output.Size() == 5);
		REQUIRE(output.Capacity() == 5);
		REQUIRE(output.Data() != nullptr);
		REQUIRE(output == "World");
	}
}
#pragma endregion Addition/Deletion

#pragma region Search

TEST_CASE("String - Find", "[String]")
{
	SECTION("char")
	{
		String s1 = "Hello World";
		REQUIRE(s1.Find('l') == 2);
		REQUIRE(s1.Find('x') == String::Npos);

		String s2;
		REQUIRE(s2.Find('a') == String::Npos);
	}
	SECTION("wchar_t")
	{
		String s1 = "Hello World";
		REQUIRE(s1.Find(L'l') == 2);
		REQUIRE(s1.Find(L'x') == String::Npos);

		String s2;
		REQUIRE(s2.Find(L'a') == String::Npos);
	}
	SECTION("String")
	{
		String s1 = "Hello World Hello";
		REQUIRE(s1.Find(String("llo")) == 2);
		REQUIRE(s1.Find(String("Test")) == String::Npos);

		String s2;
		REQUIRE(s2.Find(String("llo")) == String::Npos);
	}
}

TEST_CASE("String - RFind", "[String]")
{
	SECTION("char")
	{
		String s1 = "Hello World";
		REQUIRE(s1.RFind('l') == 9);
		REQUIRE(s1.RFind('x') == String::Npos);

		String s2;
		REQUIRE(s2.RFind('g') == String::Npos);
	}
	SECTION("wchar_t")
	{
		String s1 = "Hello World";
		REQUIRE(s1.RFind(L'l') == 9);
		REQUIRE(s1.RFind(L'x') == String::Npos);

		String s2;
		REQUIRE(s2.RFind(L'g') == String::Npos);
	}
	SECTION("String")
	{
		String s1 = "Hello World Hello";
		REQUIRE(s1.RFind(String("llo")) == 14);
		REQUIRE(s1.RFind(String("Test")) == String::Npos);

		String s2;
		REQUIRE(s2.RFind(String("llo")) == String::Npos);
	}
	SECTION("char array")
	{
		String s1 = "Hello World Hello";
		REQUIRE(s1.RFind("llo") == 14);
		REQUIRE(s1.RFind("Test") == String::Npos);

		String s2;
		REQUIRE(s2.RFind("llo") == String::Npos);
	}
}

#pragma endregion Search

#pragma region Misc

TEST_CASE("String - Printf", "[String]")
{
	SECTION("Empty")
	{
		String s1 = String::Printf("");
		REQUIRE(s1 == "");
	}
	SECTION("Non-Empty")
	{
		String s1 = String::Printf("Hello World %i %s", 123, "Test");
		REQUIRE(s1 == "Hello World 123 Test");
	}
}

#pragma endregion