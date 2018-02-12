#include "../catch.hpp"
#include "../Std/Map.h"

#include <iostream>

using namespace FluxStd;
using namespace std;

TEST_CASE("Map - Constructor - All", "[Map]")
{
	Map<char*, int> test1;
	test1.Iterate([](const KeyValuePair<char*, int>& pair) 
	{
		cout << pair.Key << ": " << pair.Value << endl;
	});
}