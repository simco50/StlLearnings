#include "../catch.hpp"
#include "../Std/Map.h"

#include <iostream>
#include <string>

using namespace FluxStd;
using namespace std;

TEST_CASE("Map - Constructor - All", "[Map]")
{
	Map<int, int> test1;
	for (int i = 0; i < 10 ; i++)
	{
		test1.Insert(i, i);
	}
}