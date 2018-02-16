#pragma once
#include "Casting.h"

namespace FluxStd
{
	template<typename T, typename U>
	struct Pair
	{
		Pair() : First(T()), Second(U())
		{}

		Pair(const T& first, const U& second) :
			First(first), Second(second)
		{}

		Pair(const T&& first, const U&& second) :
			First(Move(first)), Second(Move(second))
		{}

		Pair(const Pair& other) :
			First(other.First), Second(other.Second)
		{
		}

		Pair& operator=(const Pair& other)
		{
			First = other.First;
			Second = other.Second;
			return *this;
		}

		bool operator==(const Pair& other) const
		{
			if (First != other.First)
				return false;
			return Second == other.Second;
		}

		bool operator!=(const Pair& other) const
		{
			return First != other.First || Second != other.Second;
		}

		T First;
		U Second;
	};
}