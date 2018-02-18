#include "../catch.hpp"
#include "../Std/Map.h"

#include <iostream>
#include <string>

using namespace FluxStd;
using namespace std;

TEST_CASE("Map - Constructor - All", "[Map]")
{
	Map<int, int> test1;
	size_t size = 10;
	std::vector<int> keys(size);
	for (int i = 0; i < size; i++)
		keys[i] = i * 2;
	std::random_shuffle(keys.begin(), keys.end());
	for (int i = 0; i < size; i++)
	{
		test1.Insert(keys[i], i);
	}

	int count = 0;
	for (KeyValuePair<int, int>& pair : test1)
	{
		cout << pair.Key << ", " << pair.Value << endl;
		++count;
	}
	cout << count << endl;
}