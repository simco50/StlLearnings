#include "../catch.hpp"
#include "../Std/HashMap.h"
#include <string>
#include <iostream>
using namespace FluxStd;
using namespace std;

TEST_CASE("HashMap - Constructor - Empty", "[HashMap]")
{
	HashMap<char, int> map(20);

	for (char a = 'A'; a < 127; ++a)
	{
		map[a] = (int)a;
	}
	for (const KeyValuePair<char, int>& it : map)
	{
		std::cout << it.Key << ": " << it.Value << ", ";
	}
	cout << endl;
	cout << map.LargestBucket() << endl;
}