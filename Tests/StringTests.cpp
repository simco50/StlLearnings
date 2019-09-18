#include "../catch.hpp"
#include "../Std/String.h"
using namespace FluxStd;

#pragma region StrLen

TEST_CASE("String - StrLen", "[String]")
{
	SECTION("Non-empty char")
	{
		char pData[] = "Hello world";
		REQUIRE(String::StrLen(pData) == 11);
	}
	SECTION("Empty char")
	{
		char pData[] = "";
		REQUIRE(String::StrLen(pData) == 0);
	}
	SECTION("Non-empty wchar_t")
	{
		wchar_t pData[] = L"Hello world";
		REQUIRE(String::StrLen(pData) == 11);
	}
	SECTION("Empty wchar_t")
	{
		wchar_t pData[] = L"";
		REQUIRE(String::StrLen(pData) == 0);
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
		REQUIRE(s.begin() == nullptr);
		REQUIRE(s.end() == nullptr);
	}
	SECTION("char array")
	{
		String s("Hello world");
		REQUIRE(!s.Empty());
		REQUIRE(s == "Hello world");
		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
	}
	SECTION("begin/end")
	{
		char test[] = TEXT("Hello world");
		String s(&test[0], &test[0] + 11);
		REQUIRE(!s.Empty());
		REQUIRE(s == "Hello world");

		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
	}
	SECTION("Move semantics")
	{
		String s1("Hello world");
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);

		String s2(Move(s1));
		REQUIRE(s1.Empty());
		REQUIRE(s1 == "");
		REQUIRE(s1.begin() == nullptr);
		REQUIRE(s1.end() == nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello world");
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);
	}
	SECTION("Deep copy")
	{
		String s1("Hello world");
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);

		String s2(s1);
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello world");
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);

		REQUIRE(s1.Data() != s2.Data());
	}
}

#pragma endregion Constructors

#pragma region Assignment

TEST_CASE("String - Assignment", "[String]")
{
	SECTION("Deep copy")
	{
		String s1("Hello world");
		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);

		String s2;
		s2 = s1;

		REQUIRE(!s1.Empty());
		REQUIRE(s1 == "Hello world");
		REQUIRE(s1.begin() != nullptr);
		REQUIRE(s1.end() != nullptr);

		REQUIRE(!s2.Empty());
		REQUIRE(s2 == "Hello world");
		REQUIRE(s2.begin() != nullptr);
		REQUIRE(s2.end() != nullptr);
		REQUIRE(s1.Data() != s2.Data());
	}
	SECTION("Deep copy empty")
	{
		String s1;
		String s2 = "Hello World";
		s2 = s1;

		REQUIRE(s1.Empty());
		REQUIRE(s1 == "");
		REQUIRE(s1.begin() == nullptr);
		REQUIRE(s1.end() == nullptr);

		REQUIRE(s2.Empty());
		REQUIRE(s2 == "");
	}
}

#pragma endregion Assignment

#pragma region Misc operators

TEST_CASE("String - Operator+", "[String]")
{
	SECTION("String")
	{
		String s1("Hello");
		const char* pData = s1.Data();
		String s2(" World");
		s1 = s1 + s2;
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
	}
	SECTION("char array")
	{
		String s1("Hello");
		const char* pData = s1.Data();
		s1 = s1 + " World";
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
	}
}

TEST_CASE("String - Operator+=", "[String]")
{
	SECTION("String")
	{
		String s1("Hello");
		const char* pData = s1.Data();
		String s2(" World");
		s1 += s2;
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
	}
	SECTION("char array")
	{
		String s1("Hello");
		const char* pData = s1.Data();
		s1 += " World";
		REQUIRE(s1 == "Hello World");
		REQUIRE(s1.Data() != pData);
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
		s.Clear();
	}
	SECTION("Non-empty")
	{
		String s("Hello world");
		REQUIRE(s.begin() != s.end());
		s.Clear();
		String::Iterator a = s.Begin();
		String::Iterator b = s.End();
		REQUIRE(s.begin() == s.end());
	}
}

TEST_CASE("String - Reserve", "[String]")
{
	SECTION("Increase capacity")
	{
		String s = "Hello";
		const char* pData = s.Data();
		s.Reserve(10);
		REQUIRE(s.Data() != pData);
		pData = s.Data();
		s.Reserve(10);
		REQUIRE(s.Data() == pData);
	}
	SECTION("Lower capacity")
	{
		String s = "Hello";
		const char* pData = s.Data();
		s.Reserve(2);
		REQUIRE(s.Data() == pData);
		pData = s.Data();
		s.Reserve(10);
		REQUIRE(s.Data() != pData);
	}
}

TEST_CASE("String - ShrinkToFit", "[String]")
{
	SECTION("After push")
	{
		String s = "Hello";
		s.ShrinkToFit();
		s.Append('C');
		s.ShrinkToFit();
	}

	SECTION("After reserve")
	{
		String s = "Hello";
		s.ShrinkToFit();
		s.Reserve(10);
		s.ShrinkToFit();

	}
}

#pragma endregion Sizing

#pragma region Addition/Deletion

TEST_CASE("String - Append - Single", "[String]")
{
	SECTION("Empty String")
	{
		String s;
		s.Append('O');
	}
	SECTION("Non-empty String")
	{
		String s = "Hel";
		const char* pData = s.Data();
		s.Append('P');
		REQUIRE(pData != s.Data());
		pData = s.Data();
	}
}

TEST_CASE("String - Swap", "[String]")
{
	SECTION("Both filled")
	{
		String s1 = "Hello";
		String s2 = "Lol";
		const char* pData1 = s1.Data();
		const char* pData2 = s2.Data();
		s1.Swap(s2);
		REQUIRE(s1.Data() == pData2);
		REQUIRE(s2.Data() == pData1);
	}
	SECTION("One filled")
	{
		String s1 = "Hello";
		String s2;
		const char* pData1 = s1.Data();
		const char* pData2 = s2.Data();
		s1.Swap(s2);
		REQUIRE(s1.Data() == pData2);
		REQUIRE(s2.Data() == pData1);
	}
}

TEST_CASE("String - EraseAt", "[String]")
{
	String s = "Hello";
	s.EraseAt(2);

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
		const char* pData = s.Data();
		s.Insert(2, 'A');
		REQUIRE(pData != s.Data());
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'A');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'l');
		REQUIRE(s[5] == 'o');

	}
	{
		String s = "Hello";
		s.Reserve(100);
		const char* pData = s.Data();
		s.Insert(2, 'A');
		REQUIRE(pData == s.Data());
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'A');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'l');
		REQUIRE(s[5] == 'o');

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

	}
	{
		String s = "Hello";
		s.Reserve(100);
		const char* pData = s.Data();
		s.Insert(5, 'A');
		REQUIRE(pData == s.Data());
		REQUIRE(s[0] == 'H');
		REQUIRE(s[1] == 'e');
		REQUIRE(s[2] == 'l');
		REQUIRE(s[3] == 'l');
		REQUIRE(s[4] == 'o');
		REQUIRE(s[5] == 'A');

	}
	SECTION("char array - In the middle")
	{
		String s = "Hello";
		s.Insert(2, " world");
		REQUIRE(s == "He worldllo");
	}
	SECTION("char array - At the end")
	{
		String s = "Hello";
		s.Insert(5, " world");
		REQUIRE(s == "Hello world");

	}
	SECTION("char array - From empty string")
	{
		String s;
		s.Append("Hello world");
		REQUIRE(s == "Hello world");

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

		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.begin() != s.end());
	}
	SECTION("wchar_t array - From empty string")
	{
		String s;
		s.Append(L"Hello world");
		REQUIRE(s == "Hello world");

		REQUIRE(s.begin() != nullptr);
		REQUIRE(s.end() != nullptr);
		REQUIRE(s.begin() != s.end());
	}
	SECTION("wchar_t array - From non-empty string")
	{
		String s("Hello");
		s.Append(L" world");
		REQUIRE(s == "Hello world");

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
	}
	SECTION("No length")
	{
		String s = "Hello World";
		String output = s.Substring(6);
		REQUIRE(output == "World");
	}
	SECTION("With length")
	{
		String s = "Hello World";
		String output = s.Substring(6, 5);
		REQUIRE(output == "World");
	}
}
#pragma endregion Addition/Deletion

#pragma region Manipulation

TEST_CASE("String - ToLower", "[String]")
{
	SECTION("Inline")
	{
		String s = "Hello World";
		s.ToLowerInline();
		REQUIRE(s == "hello world");
	}
	SECTION("Inline - Empty")
	{
		String s;
		s.ToLowerInline();
		REQUIRE(s == "");
	}
	SECTION("Copy")
	{
		String s = "Hello World";
		String low = s.ToLower();
		REQUIRE(low == "hello world");
	}
	SECTION("Copy - Empty")
	{
		String s;
		String low = s.ToLower();
		REQUIRE(low == "");
	}
}

TEST_CASE("String - ToUpper", "[String]")
{
	SECTION("Inline")
	{
		String s = "Hello World";
		s.ToUpperInline();
		REQUIRE(s == "HELLO WORLD");
	}
	SECTION("Inline - Empty")
	{
		String s;
		s.ToUpperInline();
		REQUIRE(s == "");
	}
	SECTION("Copy")
	{
		String s = "Hello World";
		String low = s.ToUpper();
		REQUIRE(low == "HELLO WORLD");
	}
	SECTION("Copy - Empty")
	{
		String s;
		String low = s.ToUpper();
		REQUIRE(low == "");
	}
}

#pragma endregion Manipulation

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

TEST_CASE("String - StartsWith", "[String]")
{
	SECTION("Empty")
	{
		String s1;
		REQUIRE(s1.StartsWith("Hi") == false);
	}
	SECTION("Non-Empty - Found")
	{
		String s1 = "Hello World";
		REQUIRE(s1.StartsWith("Hello"));
	}
	SECTION("Non-Empty - Not Found")
	{
		String s1 = "Hello World";
		REQUIRE(s1.StartsWith("World") == false);
	}
	SECTION("Non-Empty - Found - Same size")
	{
		String s1 = "Hello World";
		REQUIRE(s1.StartsWith("Hello World"));
	}
}

TEST_CASE("String - EndsWith", "[String]")
{
	SECTION("Empty")
	{
		String s1;
		REQUIRE(s1.EndsWith("Hi") == false);
	}
	SECTION("Non-Empty - Found")
	{
		String s1 = "Hello World";
		REQUIRE(s1.EndsWith("World"));
	}
	SECTION("Non-Empty - Not Found")
	{
		String s1 = "Hello World";
		REQUIRE(s1.EndsWith("Hello") == false);
	}
	SECTION("Non-Empty - Found - Same size")
	{
		String s1 = "Hello World";
		REQUIRE(s1.EndsWith("Hello World"));
	}
}

TEST_CASE("String - Split", "[String]")
{
	SECTION("Empty")
	{
		String s1;
		Vector<String> parts = s1.Split(',');
	}
	SECTION("Non-Empty - No split")
	{
		String s1 = "Hello World";
		Vector<String> parts = s1.Split(',');
		REQUIRE(parts[0] == "Hello World");
	}
	SECTION("Non-Empty - Split")
	{
		String s1 = "Hello World";
		Vector<String> parts = s1.Split(' ');
		REQUIRE(parts[0] == "Hello");
		REQUIRE(parts[1] == "World");
	}
	SECTION("Non-Empty - Split - Short")
	{
		String s1 = "H e l l o W o r l d";
		Vector<String> parts = s1.Split(' ');
	}
	SECTION("Non-Empty - Split - Duplicate delimiters")
	{
		String s1 = "Hello   World";
		Vector<String> parts = s1.Split(' ');
		REQUIRE(parts[0] == "Hello");
		REQUIRE(parts[1] == "");
		REQUIRE(parts[2] == "");
		REQUIRE(parts[3] == "World");
	}
	SECTION("Non-Empty - Split - Multiple delimiters")
	{
		String s1 = "Hello,World Hi";
		Vector<String> parts = s1.Split(", ", 2);
		REQUIRE(parts[0] == "Hello");
		REQUIRE(parts[1] == "World");
		REQUIRE(parts[2] == "Hi");
	}
}

TEST_CASE("String - Replace", "[String]")
{
	SECTION("Replace")
	{
		String s1 = "Hello World";
		s1.ReplaceInline('o', 'a');
		REQUIRE(s1[4] == 'a');
		REQUIRE(s1[7] == 'a');
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