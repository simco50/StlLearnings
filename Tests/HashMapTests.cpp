#include "../catch.hpp"
#include "../Std/HashMap.h"
#include <string>
#include <iostream>
using namespace FluxStd;
using namespace std;

TEST_CASE("HashMap - Constructor - Empty", "[HashMap]")
{
	HashMap<int, int> map;

	for (int a = 0; a <= 100; ++a)
	{
		map.Insert(a, 5 * a);
	}
	cout << endl;
	for (const KeyValuePair<int, int>& it : map)
	{
		std::cout << it.Key << ": " << it.Value << ", ";
	}
	cout << endl;
}