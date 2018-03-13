#pragma once
#include "Iterator.h"

namespace FluxStd
{
	template<typename T, typename UnaryPredicate>
	void ForEach(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, UnaryPredicate functor)
	{
		while (pBegin != pEnd)
		{
			functor(*pBegin);
			++pBegin;
		}
	}

	template<typename T, typename UnaryPredicate>
	void ForEach(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd, UnaryPredicate functor)
	{
		while (pBegin != pEnd)
		{
			functor(*pBegin);
			++pBegin;
		}
	}

	template<typename T, typename Generator>
	void Generate(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, Generator generator)
	{
		while (pBegin != pEnd)
		{
			*pBegin = generator();
			++pBegin;
		}
	}

	template<typename T>
	void Fill(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, const T& value)
	{
		while (pBegin != pEnd)
		{
			*pBegin = value;
			++pBegin;
		}
	}

#pragma region Search

	template<typename T>
	RandomAccessIterator<T> Find(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, const T& value)
	{
		while (pBegin != pEnd)
		{
			if (*pBegin == value)
				return pBegin;
			++pBegin;
		}
		return pEnd;
	}

	template<typename T>
	RandomAccessConstIterator<T> Find(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd, const T& value)
	{
		while (pBegin != pEnd)
		{
			if (*pBegin == value)
				return pBegin;
			++pBegin;
		}
		return pEnd;
	}

	template<typename T, typename UnaryPredicate>
	RandomAccessIterator<T> FindIf(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, UnaryPredicate functor)
	{
		while (pBegin != pEnd)
		{
			if (functor(*pBegin))
				return pBegin;
			++pBegin;
		}
		return pEnd;
	}

	template<typename T, typename UnaryPredicate>
	RandomAccessConstIterator<T> FindIf(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd, UnaryPredicate functor)
	{
		while (pBegin != pEnd)
		{
			if (functor(*pBegin))
				return pBegin;
			++pBegin;
		}
		return pEnd;
	}

#pragma region Search

	template<class T>
	void Reverse(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd)
	{
		while (pBegin != pEnd && pBegin != --pEnd)
		{
			Swap(*pBegin, *pEnd);
			++pBegin;
		}
	}

	template<class T>
	void Replace(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, const T& oldValue, const T& newValue)
	{
		while (pBegin != pEnd)
		{
			if (*pBegin == oldValue)
				*pBegin = newValue;
			++pBegin;
		}
	}

	template<class T, typename UnaryPredicate>
	void ReplaceIf(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, UnaryPredicate predicate, const T& newValue)
	{
		while (pBegin != pEnd)
		{
			if (predicate(*pBegin))
				*pBegin = newValue;
			++pBegin;
		}
	}

#pragma region Comparison

	template<class T>
	const T& Max(const T& a, const T& b)
	{
		return a < b ? b : a;
	}

	template<class T, typename Compare>
	const T& Max(const T& a, const T& b, Compare compare)
	{
		return compare(a, b) ? b : a;
	}

	template<class T>
	RandomAccessIterator<T> MaxElement(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessIterator<T> pLargest = pBegin++;
		while (pBegin != pEnd)
		{
			if (*pLargest < *pBegin)
				pLargest = pBegin;
			++pBegin;
		}
		return pLargest;
	}

	template<class T>
	RandomAccessConstIterator<T> MaxElement(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessConstIterator<T> pLargest = pBegin++;
		while (pBegin != pEnd)
		{
			if (*pLargest < *pBegin)
				pLargest = pBegin;
			++pBegin;
		}
		return pLargest;
	}

	template<class T, typename Compare>
	RandomAccessIterator<T> MaxElement(RandomAccessIterator<T> pBegin, const RandomAccessIterator<T> pEnd, Compare compare)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessIterator<T> pLargest = pBegin++;
		while (pBegin != pEnd)
		{
			//pLargest < pBegin
			if (compare(*pLargest, *pBegin))
				pLargest = pBegin;
			++pBegin;
		}
		return pLargest;
	}

	template<class T, typename Compare>
	RandomAccessConstIterator<T> MaxElement(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd, Compare compare)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessConstIterator<T> pLargest = pBegin++;
		while (pBegin != pEnd)
		{
			//pLargest < pBegin
			if (compare(*pLargest, *pBegin))
				pLargest = pBegin;
			++pBegin;
		}
		return pLargest;
	}

	template<class T>
	const T& Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<class T, typename Compare>
	const T& Min(const T& a, const T& b, Compare compare)
	{
		return compare(a, b) ? a : b;
	}

	template<class T>
	RandomAccessIterator<T> MinElement(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessIterator<T> pSmallest = pBegin++;
		while (pBegin != pEnd)
		{
			if (*pBegin < *pSmallest)
				pSmallest = pBegin;
			++pBegin;
		}
		return pSmallest;
	}

	template<class T>
	RandomAccessConstIterator<T> MinElement(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessConstIterator<T> pSmallest = pBegin++;
		while (pBegin != pEnd)
		{
			if (*pBegin < *pSmallest)
				pSmallest = pBegin;
			++pBegin;
		}
		return pSmallest;
	}

	template<class T, typename Compare>
	RandomAccessIterator<T> MinElement(RandomAccessIterator<T> pBegin, const RandomAccessIterator<T> pEnd, Compare compare)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessIterator<T> pSmallest = pBegin++;
		while (pBegin != pEnd)
		{
			if (compare(*pBegin, *pSmallest))
				pSmallest = pBegin;
			++pBegin;
		}
		return pSmallest;
	}

	template<class T, typename Compare>
	RandomAccessConstIterator<T> MinElement(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd, Compare compare)
	{
		if (pBegin == pEnd)
			return pEnd;
		RandomAccessConstIterator<T> pSmallest = pBegin++;
		while (pBegin != pEnd)
		{
			if (compare(*pBegin, *pSmallest))
				pSmallest = pBegin;
			++pBegin;
		}
		return pSmallest;
	}
#pragma endregion Comparison
}