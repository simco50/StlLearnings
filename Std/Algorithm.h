#pragma once

namespace FluxStd
{
	template <class T>
	inline void Swap(T& first, T& second)
	{
		T temp = first;
		first = second;
		second = temp;
	}

	template <class T>
	inline void IteratorSwap(T pFirst, T pSecond)
	{
		Swap(*pFirst, *pSecond);
	}

	template<typename T, typename UnaryPredicate>
	void ForEach(T* pBegin, T* pEnd, UnaryPredicate functor)
	{
		while (pBegin != pEnd)
		{
			functor(*pBegin);
			++pBegin;
		}
	}

	template<typename T>
	T* Find(T* pBegin, T* pEnd, const T& value)
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
	T* FindIf(T* pBegin, T* pEnd, UnaryPredicate functor)
	{
		while (pBegin != pEnd)
		{
			if (functor(*pBegin))
				return pBegin;
			++pBegin;
		}
		return pEnd;
	}

	template<class T>
	void Reverse(T* pBegin, T* pEnd)
	{
		while (pBegin != pEnd && pBegin != --pEnd)
		{
			IteratorSwap(pBegin++, pEnd);
		}
	}

	template<class T>
	void Replace(T* pBegin, T* pEnd, const T& oldValue, const T& newValue)
	{
		while (pBegin != pEnd)
		{
			if (*pBegin == oldValue)
				*pBegin = newValue;
			++pBegin;
		}
	}

	template<class T, typename UnaryPredicate>
	void ReplaceIf(T* pBegin, T* pEnd, UnaryPredicate predicate, const T& newValue)
	{
		while (pBegin != pEnd)
		{
			if (predicate(*pBegin))
				*pBegin = newValue;
			++pBegin;
		}
	}

	template<class T>
	const T& Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	template<class T, typename Compare>
	const T& Max(const T& a, const T& b, Compare compare)
	{
		return compare(a, b) ? b : a;
	}

	template<class T>
	T* MaxElement(const T* pBegin, const T* pEnd)
	{
		if (pBegin == pEnd)
			return pEnd;
		T* pLargest = pBegin;
		++pBegin;
		while (pBegin != pEnd)
		{
			if (*pLargest > *pBegin)
				pLargest = pBegin;
			++pBegin;
		}
		return pLargest;
	}

	template<class T, typename Compare>
	T* MaxElement(const T* pBegin, const T* pEnd, Compare compare)
	{
		if (pBegin == pEnd)
			return pEnd;
		T* pLargest = pBegin;
		++pBegin;
		while (pBegin != pEnd)
		{
			if (compare(*pLargest, *pBegin))
				pLargest = pBegin;
			++pBegin;
		}
		return pLargest;
	}
}