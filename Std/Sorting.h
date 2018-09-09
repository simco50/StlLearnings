#pragma once
#include "Algorithm.h"
#include "Iterator.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename T>
	void BubbleSort(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd)
	{
		for (RandomAccessIterator<T> i = pBegin; i != pEnd; i++)
		{
			for (RandomAccessIterator<T> j = pBegin; j < i; j++)
			{
				if (*i < *j)
				{
					Swap(*i, *j);
				}
			}
		}
	}

	template<typename T, typename CompareFunctor>
	void BubbleSort(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, CompareFunctor compare)
	{
		for (RandomAccessIterator<T> i = pBegin; i != pEnd; i++)
		{
			for (RandomAccessIterator<T> j = pBegin; j < i; j++)
			{
				if (compare(*i, *j))
				{
					Swap(*i, *j);
				}
			}
		}
	}

	template<typename T>
	void InsertionSort(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd)
	{
		if (pBegin == pEnd)
		{
			return;
		}
		for (RandomAccessIterator<T> i = pBegin + 1; i != pEnd; ++i)
		{
			T temp = *i;
			RandomAccessIterator<T> j = i;
			while (temp < *(j - 1) && j > pBegin)
			{
				*j = *(j - 1);
				--j;
			}
			*j = temp;
		}
	}

	template<typename T, typename CompareFunctor>
	void InsertionSort(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, CompareFunctor compare)
	{
		if (pBegin == pEnd)
		{
			return;
		}
		for (RandomAccessIterator<T> i = pBegin + 1; i != pEnd; ++i)
		{
			T temp = *i;
			RandomAccessIterator<T> j = i;
			while (compare(temp, *(j - 1)) && j > pBegin)
			{
				*j = *(j - 1);
				--j;
			}
			*j = temp;
		}
	}

	template<typename T>
	RandomAccessIterator<T> Partition(RandomAccessIterator<T> begin, RandomAccessIterator<T> end)
	{
		RandomAccessIterator<T> pivot = begin + rand() % Distance(begin, end);
		Swap(*begin, *pivot);
		pivot = begin;
		RandomAccessIterator<T> i = begin , j = begin;
		for (; j != end; j++)
		{
			if (*j < *pivot)
			{
				Swap(*j, *++i);
			}
		}
		Swap(*pivot, *i);
		return i;
	}

	template<typename T>
	void QuickSort(RandomAccessIterator<T> begin, RandomAccessIterator<T> end)
	{
		if (begin == end)
		{
			return;
		}
		RandomAccessIterator<T> elementAtCorrectPosition = Partition(begin, end);
		QuickSort(begin, elementAtCorrectPosition);
		QuickSort(++elementAtCorrectPosition, end);
	}

	template<typename T>
	bool IsSorted(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd)
	{
		if (pBegin == pEnd)
		{
			return true;
		}
		while(++pBegin != pEnd)
		{
			if (*pBegin < *(pBegin - 1))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	bool IsSorted(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd)
	{
		if (pBegin == pEnd)
		{
			return true;
		}
		while (++pBegin != pEnd)
		{
			if (*pBegin < *(pBegin - 1))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, typename CompareFunctor>
	bool IsSorted(RandomAccessIterator<T> pBegin, RandomAccessIterator<T> pEnd, CompareFunctor compare)
	{
		if (pBegin == pEnd)
		{
			return true;
		}
		while (++pBegin != pEnd)
		{
			if (compare(*pBegin, *(pBegin - 1)))
			{
				return false;
			}			
		}
		return true;
	}

	template<typename T, typename CompareFunctor>
	bool IsSorted(RandomAccessConstIterator<T> pBegin, RandomAccessConstIterator<T> pEnd, CompareFunctor compare)
	{
		if (pBegin == pEnd)
		{
			return true;
		}
		while (++pBegin != pEnd)
		{
			if (compare(*pBegin, *(pBegin - 1)))
			{
				return false;
			}
		}
		return true;
	}
}