#include "../catch.hpp"
#include "../Std/Map.h"

#include <iostream>
#include <string>

using namespace FluxStd;
using namespace std;

TEST_CASE("Map - Constructor - All", "[Map]")
{
	Map<int, int> test1;
	size_t size = 50;
	std::vector<int> keys(size);
	for (int i = 0; i < (int)size; i++)
		keys[i] = i * 2;
	std::random_shuffle(keys.begin(), keys.end());
	for (int i = 0; i < (int)size; i++)
	{
		test1[keys[i]] = i;
	}
	test1.Clear();
	for (int i = 0; i < (int)size; i++)
	{
		test1[keys[i]] = i;
	}
	REQUIRE(test1.Contains(20));
	cout << test1.Erase(20) << endl;
	REQUIRE(!test1.Contains(20));
}