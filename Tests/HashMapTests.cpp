#include "../catch.hpp"
#include "../Std/HashMap.h"
#include <string>
#include <iostream>
using namespace FluxStd;
using namespace std;

TEST_CASE("HashMap - Constructor - Empty", "[HashMap]")
{
	HashMap<string, double> map;
	REQUIRE(map.Size() == 0);
	REQUIRE(map.BucketCount() == HashMap<string, double>::START_BUCKETS);
	REQUIRE(map.Begin() == map.End());
	REQUIRE(map.begin() == map.Begin());
	REQUIRE(map.end() == map.End());
}

TEST_CASE("HashMap - Initializer list constructor - Empty", "[HashMap]")
{
	HashMap<string, double> map = {};
	REQUIRE(map.Size() == 0);
	REQUIRE(map.BucketCount() == HashMap<string, double>::START_BUCKETS);
	REQUIRE(map.Begin() == map.End());
}

TEST_CASE("HashMap - Initializer list constructor - Non-Empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
	REQUIRE(map.BucketCount() == HashMap<string, double>::START_BUCKETS);
	REQUIRE(map.Size() == 2);
	REQUIRE(map.Begin() != map.End());
}

TEST_CASE("HashMap - Initializer list constructor - Non-Empty exceed load", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = { 
		P("Hello", 1.23), 
		P("World", 2.46), 
		P("Lorem", 6.89),
		P("Ipsum", 4.87),
		P("Foo", 7.57),
		P("Bar", 8.41),
		P("C++", 1.35),
		P("C#", 2.85),
	};
	REQUIRE(map.BucketCount() > HashMap<string, double>::START_BUCKETS);
	REQUIRE(map.Size() == 8);
	REQUIRE(map.Begin() != map.End());
}

TEST_CASE("HashMap - Copy constructor - From empty", "[HashMap]")
{
	HashMap<string, double> map;
	HashMap<string, double> map2(map);
	REQUIRE(map.Size() == 0);
	REQUIRE(map.Begin() == map.End());
	REQUIRE(map2.Size() == 0);
}

TEST_CASE("HashMap - Copy constructor - From non-empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
	HashMap<string, double> map2(map);
	REQUIRE(map.Size() == 2);
	REQUIRE(map.Begin() != map.End());
	REQUIRE(map2.Size() == 2);
}

TEST_CASE("HashMap - Assignment - From empty to empty", "[HashMap]")
{
	HashMap<string, double> map;
	HashMap<string, double> map2;
	REQUIRE(map.Size() == 0);
	REQUIRE(map2.Size() == 0);
	map = map2;
	REQUIRE(map.Size() == 0);
	REQUIRE(map2.Size() == 0);
}

TEST_CASE("HashMap - Assignment - From non-empty to empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
	HashMap<string, double> map2;
	REQUIRE(map.Size() == 2);
	REQUIRE(map2.Size() == 0);
	map2 = map;
	REQUIRE(map.Size() == 2);
	REQUIRE(map2.Size() == 2);
}

TEST_CASE("HashMap - Assignment - From non-empty to non-empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = { P("Hello", 1.23), P("World", 2.46) };
	HashMap<string, double> map2 = { P("CPP", 4.87), P("Foo", 1.23), P("Bar", 2.46) };
	REQUIRE(map2.Size() == 3);
	REQUIRE(map.Size() == 2);
	map2 = map;
	REQUIRE(map2.Size() == 2);
	REQUIRE(map2.Size() == 2);
	REQUIRE(map.Size() == 2);
}

TEST_CASE("HashMap - Find - Empty", "[HashMap]")
{
	HashMap<string, double> map;
	REQUIRE(map.Size() == 0);

	REQUIRE(map.Find("Poo") == map.End());
	REQUIRE(map.Find("Hey") == map.End());
	REQUIRE(map.Find("Thing") == map.End());
	REQUIRE(map.Find("No") == map.End());
}

TEST_CASE("HashMap - Find - Non empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = {
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

TEST_CASE("HashMap - Insert - Empty", "[HashMap]")
{
	HashMap<string, double> map;
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

TEST_CASE("HashMap - Insert - Non empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = { P("Foo", 1.23), P("Bar", 2.46) };
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

TEST_CASE("HashMap - Accessors - Get", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = {
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

TEST_CASE("HashMap - Accessors - Modify  & Insert", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = {
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

TEST_CASE("HashMap - Accessors - Erase", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = {
		P("Hello", 1.23),
		P("World", 2.46),
		P("Lorem", 6.89),
		P("Ipsum", 4.87),
	};
	REQUIRE(map.Size() == 4);

	REQUIRE(map.Contains("Hello"));
	HashMap<string, double>::Iterator it = map.Erase("Hello");
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

TEST_CASE("HashMap - Comparison - Empty", "[HashMap]")
{
	HashMap<string, double> map;
	HashMap<string, double> map2;
	REQUIRE(map == map2);
	REQUIRE(!(map != map2));
}

TEST_CASE("HashMap - Comparison - One empty", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = {
		P("Hello", 1.23),
		P("World", 2.46),
		P("Lorem", 6.89),
		P("Ipsum", 4.87),
	};
	HashMap<string, double> map2;
	REQUIRE(map != map2);
	REQUIRE(!(map == map2));
}

TEST_CASE("HashMap - Comparison - Non-empty - Equal", "[HashMap]")
{
	using P = KeyValuePair<string, double>;
	HashMap<string, double> map = {
		P("Hello", 1.23),
		P("World", 2.46),
		P("Lorem", 6.89),
		P("Ipsum", 4.87),
	};
	HashMap<string, double> map2 = {
		P("Hello", 1.23),
		P("World", 2.46),
		P("Lorem", 6.89),
		P("Ipsum", 4.87),
	};
	REQUIRE(map == map2);
	REQUIRE(!(map != map2));
}

TEST_CASE("HashMap - Clear - Empty", "[HashMap]")
{
	HashMap<string, double> map;
	REQUIRE(map.Size() == 0);
	map.Clear();
	REQUIRE(map.Size() == 0);
}

TEST_CASE("HashMap - Clear - Non-empty", "[HashMap]")
{
	HashMap<string, double> map;
	REQUIRE(map.Size() == 0);
	map.Clear();
	REQUIRE(map.Size() == 0);
}