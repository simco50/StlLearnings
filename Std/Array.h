#pragma once
#include <assert.h>

namespace FluxStd
{
	template<typename T, size_t size>
	class Array
	{
	public:
		Array()
		{
			memset(&m_Data[0], 0, sizeof(T) * size);
		}

		~Array()
		{}

		T& operator[](size_t index) { return m_Data[index]; }
		const T& operator[](size_t index) const { return m_Data[index]; }

		T& At(size_t index) { assert(index < size); return m_Data[index]; }
		const T& At(size_t index) const { assert(index < size); return m_Data[index]; }

		T& Front() { assert(size > 0);  return m_Data[0]; }
		T& Back() { assert(size > 0);  return m_Data[size - 1]; }

		T* Data() { return &m_Data[0]; }
		const T* Data() const { return &m_Data[0]; }

		void Fill(const T& value)
		{
			for (size_t i = 0; i < size; ++i)
				m_Data[i] = value;
		}

		void Swap(Array& other)
		{
			T* pTemp = &m_Data;
			&m_Data = &other.m_Data;
			&other.m_Data = pTemp;
		}

		T* begin() const { return &m_Data[0]; }
		T* end() const { return &m_Data[size - 1]; }

		constexpr size_t Size() const { return size; }
		constexpr size_t MaxSize() const { size_t nr = 0; nr = ~nr; return nr; }
		constexpr bool Empty() const { return size == 0; }

	private:
		T m_Data[size];
	};
}