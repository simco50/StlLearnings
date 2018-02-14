#pragma once
#include <vector>

namespace FluxStd
{
	namespace Comparison
	{
		template<typename T>
		struct Less
		{
			bool operator()(const T& a, const T& b) const { return a < b; }
		};

		template<typename T>
		struct Greater
		{
			bool operator()(const T& a, const T& b) const { return a > b; }
		};
	}

	template <class T> 
	inline void Swap(T& first, T& second)
	{
		T temp = first;
		first = second;
		second = temp;
	}

	template<typename T>
	void BubbleSort(T* pBegin, T* pEnd)
	{
		size_t size = pEnd - pBegin;
		for (size_t i = 0; i < size - 1; ++i)
		{
			for (size_t j = 0; j < size - i - 1; ++j)
			{
				if (pBegin[j + 1] < pBegin[j])
				{
					Swap(pBegin[i], pBegin[i + 1])
				}
			}
		}
	}

	template<typename T, typename CompareFunctor>
	void BubbleSort(T* pBegin, T* pEnd, CompareFunctor compare)
	{
		size_t size = pEnd - pBegin;
		for (size_t i = 0; i < size - 1; ++i)
		{
			for (size_t j = 0; j < size - i - 1; ++j)
			{
				if (compare(pBegin[j + 1], pBegin[j]))
				{
					Swap(pBegin[i], pBegin[i + 1])
				}
			}
		}
	}

	template<typename T>
	void InsertionSort(T* pBegin, T* pEnd)
	{
		size_t size = pEnd - pBegin;
		for (size_t i = 1; i < size; ++i)
		{
			size_t j = i;
			while (pBegin[j] < pBegin[j - 1] && j > 0)
			{
				Swap(pBegin[j], pBegin[j - 1]);
				--j;
			}
		}
	}

	template<typename T, typename CompareFunctor>
	void InsertionSort(T* pBegin, T* pEnd, CompareFunctor compare)
	{
		CompareFunctor compare;
		size_t size = pEnd - pBegin;
		for (size_t i = 1; i < size; ++i)
		{
			size_t j = i;
			while (compare(pBegin[j], pBegin[j - 1]) && j > 0)
			{
				Swap(pBegin[j], pBegin[j - 1]);
				--j;
			}
		}
	}

	template<typename T>
	void QuickSort(T* pBegin, T* pEnd)
	{
		if (pEnd - pBegin <= 1)
			return;
		int* pPivot = pEnd - 1;
		int* pWall = pBegin;
		for (int* pCurrent = pBegin + 1; pCurrent != pEnd; ++pCurrent)
		{
			if (*pCurrent < *pPivot)
			{
				Swap(*pCurrent, *pWall);
				++pWall;
			}
		}
		Swap(*pPivot, *pWall);
		QuickSort(pBegin, pWall);
		QuickSort(pWall + 1, pEnd);
	}

	template<typename T, typename CompareFunctor>
	void QuickSort(T* pBegin, T* pEnd, CompareFunctor compare)
	{
		CompareFunctor compare;
		if (pEnd - pBegin <= 1)
			return;
		int* pPivot = pEnd - 1;
		int* pWall = pBegin;
		for (int* pCurrent = pBegin + 1; pCurrent != pEnd; ++pCurrent)
		{
			if (compare(*pCurrent, *pPivot))
			{
				Swap(*pCurrent, *pWall);
				++pWall;
			}
		}
		Swap(*pPivot, *pWall);
		QuickSort(pBegin, pWall);
		QuickSort(pWall + 1, pEnd);
	}
}