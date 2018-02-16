#pragma once
#include <vector>
#include "Algorithm.h"
#include "Iterator.h"

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
}