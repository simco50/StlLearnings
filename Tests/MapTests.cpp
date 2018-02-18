#include "../catch.hpp"
#include "../Std/Map.h"

using namespace FluxStd;
using namespace std;

TEST_CASE("Map - Constructor", "[Map]")
{
	SECTION("Empty")
	{
		Map<string, int> map;
		REQUIRE(map.Size() == 0);
		REQUIRE(map.Begin() == map.End());
		REQUIRE(map.begin() == map.Begin());
		REQUIRE(map.end() == map.End());
	}
	SECTION("Initializer list constructor - Empty")
	{
		Map<string, double> map = {};
		REQUIRE(map.Size() == 0);
		REQUIRE(map.Begin() == map.End());
	}
	SECTION("Initializer list constructor - Non-empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
		REQUIRE(map.Size() == 2);
		REQUIRE(map.Begin() != map.End());
	}
	SECTION("Initializer list constructor - Non-Empty exceed load")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
			P("Foo", 7.57),
			P("Bar", 8.41),
			P("C++", 1.35),
			P("C#", 2.85),
		};
		REQUIRE(map.Size() == 8);
		REQUIRE(map.Begin() != map.End());
	}
}

TEST_CASE("Map - Copy constructor", "[HashMap]")
{
	SECTION("From empty")
	{
		Map<string, double> map;
		Map<string, double> map2(map);
		REQUIRE(map.Size() == 0);
		REQUIRE(map.Begin() == map.End());
		REQUIRE(map2.Size() == 0);
	}
	SECTION("From non-empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
		Map<string, double> map2(map);
		REQUIRE(map.Size() == 2);
		REQUIRE(map.Begin() != map.End());
		REQUIRE(map2.Size() == 2);
	}
}

TEST_CASE("Map - Assignment", "[HashMap]")
{
	SECTION("From empty to empty")
	{
		Map<string, double> map;
		Map<string, double> map2;
		REQUIRE(map.Size() == 0);
		REQUIRE(map2.Size() == 0);
		map = map2;
		REQUIRE(map.Size() == 0);
		REQUIRE(map2.Size() == 0);
	}
	SECTION("From non-empty to empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
		Map<string, double> map2;
		REQUIRE(map.Size() == 2);
		REQUIRE(map2.Size() == 0);
		map2 = map;
		REQUIRE(map.Size() == 2);
		REQUIRE(map2.Size() == 2);
	}
	SECTION("From non-empty to non-empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
		Map<string, double> map2 = { P("CPP", 4.87), P("Foo", 1.23), P("Bar", 2.46) };
		REQUIRE(map2.Size() == 3);
		REQUIRE(map.Size() == 2);
		map2 = map;
		REQUIRE(map2.Size() == 2);
		REQUIRE(map2.Size() == 2);
		REQUIRE(map.Size() == 2);
	}
}

TEST_CASE("Map - Find", "[HashMap]")
{
	SECTION("Empty")
	{
		Map<string, double> map;
		REQUIRE(map.Size() == 0);

		REQUIRE(map.Find("Poo") == map.End());
		REQUIRE(map.Find("Hey") == map.End());
		REQUIRE(map.Find("Thing") == map.End());
		REQUIRE(map.Find("No") == map.End());
	}
	SECTION("Non-empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
			P("Foo", 7.57),
			P("Bar", 8.41),
			P("C++", 1.35),
			P("C#", 2.85),
		};
		REQUIRE(map.Size() == 8);

		REQUIRE(map.Find("Hello") != map.End());
		REQUIRE(map.Find("World") != map.End());
		REQUIRE(map.Find("Lorem") != map.End());
		REQUIRE(map.Find("Ipsum") != map.End());
		REQUIRE(map.Find("Foo") != map.End());
		REQUIRE(map.Find("Bar") != map.End());
		REQUIRE(map.Find("C++") != map.End());
		REQUIRE(map.Find("C#") != map.End());

		REQUIRE(map.Find("Poo") == map.End());
		REQUIRE(map.Find("Hey") == map.End());
		REQUIRE(map.Find("Thing") == map.End());
		REQUIRE(map.Find("No") == map.End());
	}
}

TEST_CASE("Map - Swap", "[HashMap]")
{
	SECTION("1 Empty - Member function")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map1;
		Map<string, double> map2 = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
		};
		map1.Swap(map2);
		REQUIRE(map1.Size() == 3);
		REQUIRE(map2.Size() == 0);
	}
	SECTION("1 Empty - Global function")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map1;
		Map<string, double> map2 = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
		};
		Swap(map1, map2);
		REQUIRE(map1.Size() == 3);
		REQUIRE(map2.Size() == 0);
	}
	SECTION("No empty - Member function")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map1 = {
			P("Lorem", 6.89),
		};
		Map<string, double> map2 = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
		};
		map1.Swap(map2);
		REQUIRE(map1.Size() == 3);
		REQUIRE(map2.Size() == 1);
	}
	SECTION("No empty - Global function")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map1 = {
			P("Lorem", 6.89),
		};
		Map<string, double> map2 = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
		};
		Swap(map1, map2);
		REQUIRE(map1.Size() == 3);
		REQUIRE(map2.Size() == 1);
	}
}

TEST_CASE("Map - Insert", "[HashMap]")
{
	SECTION("Empty")
	{
		Map<string, double> map;
		REQUIRE(map.Size() == 0);
		map.Insert(KeyValuePair<string, double>("Hello", 1.0));
		REQUIRE(map.Size() == 1);
		REQUIRE(map.Find("Hello") != map.End());
		map.Insert("World", 5.0);
		REQUIRE(map.Size() == 2);
		REQUIRE(map.Find("World") != map.End());
		map.Insert("Hello", 5.0);
		REQUIRE(map.Find("Hello") != map.End());
		REQUIRE(map.Size() == 2);
	}
	SECTION("Non-empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = { P("Foo", 1.23), P("Bar", 2.46) };
		REQUIRE(map.Size() == 2);
		map.Insert(KeyValuePair<string, double>("Hello", 1.0));
		REQUIRE(map.Size() == 3);
		REQUIRE(map.Find("Hello") != map.End());
		map.Insert("World", 5.0);
		REQUIRE(map.Size() == 4);
		REQUIRE(map.Find("World") != map.End());
		map.Insert(KeyValuePair<string, double>("Hello", 5.0));
		REQUIRE(map.Size() == 4);
		REQUIRE(map.Find("Hello") != map.End());
	}
	SECTION("Empty - Empty Map")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map;
		REQUIRE(map.Size() == 0);
		Map<string, double> map2;
		map.Insert(map2);
		REQUIRE(map.Size() == 0);
	}
	SECTION("Empty - Non-empty Map")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map;
		REQUIRE(map.Size() == 0);
		Map<string, double> map2 = { P("Hello", 1.23), P("World", 2.46) };
		map.Insert(map2);
		REQUIRE(map.Size() == 2);
		REQUIRE(map.Contains("Hello"));
		REQUIRE(map.Contains("World"));
	}
	SECTION("Non-empty - Non-empty Map")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = { P("Foo", 1.23), P("Bar", 2.46) };
		REQUIRE(map.Size() == 2);
		Map<string, double> map2 = { P("Hello", 1.23), P("World", 2.46) };
		map.Insert(map2);
		REQUIRE(map.Size() == 4);
		REQUIRE(map.Contains("Foo"));
		REQUIRE(map.Contains("Bar"));
		REQUIRE(map.Contains("Hello"));
		REQUIRE(map.Contains("World"));
		map.Erase("Kut");
	}
}

TEST_CASE("Map - Accessors", "[HashMap]")
{
	SECTION("Get")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
			P("Foo", 7.57),
			P("Bar", 8.41),
			P("C++", 1.35),
			P("C#", 2.85),
		};
		REQUIRE(map.Size() == 8);

		REQUIRE(map["Hello"] == 1.23);
		REQUIRE(map["World"] == 2.46);
		REQUIRE(map["Lorem"] == 6.89);
		REQUIRE(map["Ipsum"] == 4.87);
		REQUIRE(map["Foo"] == 7.57);
		REQUIRE(map["Bar"] == 8.41);
		REQUIRE(map["C++"] == 1.35);
		REQUIRE(map["C#"] == 2.85);
	}
	SECTION("Modify & Insert")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		REQUIRE(map.Size() == 4);

		REQUIRE(map.Contains("Hello"));
		REQUIRE(map["Hello"] == 1.23);
		map["Hello"] = 5.0;
		REQUIRE(map["Hello"] == 5.0);

		REQUIRE(!map.Contains("Foo"));
		REQUIRE(map.Find("Foo") == map.End());
		map["Foo"] = 1.0;
		REQUIRE(map.Contains("Foo"));
		REQUIRE(map.Size() == 5);
		REQUIRE(map.Find("Foo") != map.End());
		REQUIRE(map["Foo"] == 1.0);
	}
	SECTION("Erase")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		REQUIRE(map.Size() == 4);

		REQUIRE(map.Contains("Hello"));
		Map<string, double>::Iterator it = map.Erase("Hello");
		REQUIRE(map.Size() == 3);
		REQUIRE(it != map.End());
		REQUIRE(map.Find("Hello") == map.End());
		REQUIRE(!map.Contains("Hello"));

		it = map.Erase("Foo");
		REQUIRE(map.Size() == 3);
		REQUIRE(it == map.End());
		REQUIRE(map.Find("Foo") == map.End());
		REQUIRE(!map.Contains("Foo"));

		map.Erase("World");
		map.Erase("Ipsum");
		map.Erase("Lorem");
		REQUIRE(map.Size() == 0);
		REQUIRE(map.Begin() == map.End());
	}
}

TEST_CASE("Map - Comparison", "[HashMap]")
{
	SECTION("Empty")
	{
		Map<string, double> map;
		Map<string, double> map2;
		REQUIRE(map == map2);
		REQUIRE(!(map != map2));
	}
	SECTION("One empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		Map<string, double> map2;
		REQUIRE(map != map2);
		REQUIRE(!(map == map2));
	}
	SECTION("Non-empty - Equal")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		Map<string, double> map2 = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		REQUIRE(map == map2);
		REQUIRE(!(map != map2));
	}
	SECTION("Non-empty - Not-equal")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Test", 4.87),
		};
		Map<string, double> map2 = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		REQUIRE(!(map == map2));
		REQUIRE(map != map2);
	}
}

TEST_CASE("Map - Clear", "[HashMap]")
{
	SECTION("Empty")
	{
		Map<string, double> map;
		REQUIRE(map.Size() == 0);
		map.Clear();
		REQUIRE(map.Size() == 0);
	}
	SECTION("Non-empty")
	{
		using P = KeyValuePair<string, double>;
		Map<string, double> map = {
			P("Hello", 1.23),
			P("World", 2.46),
			P("Lorem", 6.89),
			P("Ipsum", 4.87),
		};
		REQUIRE(map.Size() == 4);
		map.Clear();
		REQUIRE(map.Size() == 0);
	}
}
