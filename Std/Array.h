#pragma once
#include <assert.h>
#include "Iterator.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename T, int size>
	class Array
	{
	public:
		using Iterator = RandomAccessIterator<T>;
		using ConstIterator = RandomAccessConstIterator<T>;
		using SizeType = size_t;

	public:
		void Fill(const T& value)
		{
			for (SizeType i = 0; i < size; ++i)
			{
				m_Data[i] = value;
			}
		}

		void Swap(Array& other)
		{
			for (Iterator i = begin(); i != end(); ++i)
			{
				FluxStd::Swap<T>(m_Data[0], other.m_Data[0]);
			}
		}

		T& operator[](SizeType index) { return m_Data[index]; }
		const T& operator[](SizeType index) const { return m_Data[index]; }

		T& At(SizeType index) { assert(index < size); return m_Data[index]; }
		const T& At(SizeType index) const { assert(index < size); return m_Data[index]; }

		T& Front() { assert(size > 0);  return m_Data[0]; }
		T& Back() { assert(size > 0);  return m_Data[size - 1]; }

		T* Data() { return &m_Data[0]; }
		const T* Data() const { return &m_Data[0]; }

		Iterator begin() { return Iterator(m_Data); }
		Iterator end() { return Iterator(m_Data + size - 1); }
		ConstIterator begin() const { return ConstIterator(m_Data); }
		ConstIterator end() const { return ConstIterator(m_Data + size - 1); }

		Iterator Begin() { return Iterator(m_Data); }
		Iterator End() { return Iterator(m_Data + size - 1); }
		ConstIterator Begin() const { return ConstIterator(m_Data); }
		ConstIterator End() const { return ConstIterator(m_Data + size - 1); }

		constexpr SizeType Size() const { return size; }
		constexpr SizeType MaxSize() const { SizeType nr = 0; nr = ~nr; return nr; }
		constexpr bool Empty() const { return size == 0; }

	private:
		T m_Data[size];
	};

	template<typename T, int size>
	inline void Swap(Array<T, size>& a, Array<T, size>& b)
	{
		a.Swap(b);
	}
}