#pragma once
#include <initializer_list>
#include <assert.h>
#include "Iterator.h"
#include "Algorithm.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename T>
	class Vector
	{
	public:
		using Iterator = RandomAccessIterator<T>;
		using ConstIterator = RandomAccessConstIterator<T>;

	public:
		Vector() :
			m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{}
		Vector(const size_t size) :
			m_pBuffer(new T[size]), m_Size(size), m_Capacity(size)
		{
			memset(m_pBuffer, 0, sizeof(T) * size);
		}

		Vector(const size_t size, const T& value) :
			m_pBuffer(new T[size]), m_Size(size), m_Capacity(size)
		{
			for (size_t i = 0; i < size; ++i)
				m_pBuffer[i] = value;
		}

		Vector(T* pData, const size_t size) :
			m_pBuffer(new T[size]), m_Size(size), m_Capacity(size)
		{
			memcpy(m_pBuffer, pData, sizeof(T) * size);
		}

		Vector(std::initializer_list<T> list) :
			m_pBuffer(new T[list.size()]), m_Size(list.size()), m_Capacity(list.size())
		{
			memcpy(m_pBuffer, list.begin(), sizeof(T) * list.size());
		}

		//Move semantics
		Vector(Vector<T>&& other) :
			m_pBuffer(other.m_pBuffer), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_pBuffer = nullptr;
		}

		//Deep copy
		Vector(const Vector<T>& other) :
			m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			m_pBuffer = new T[other.m_Size];
			memcpy(m_pBuffer, other.m_pBuffer, sizeof(T) * other.m_Size);
		}

		~Vector()
		{
			if (m_pBuffer)
			{
				delete[] m_pBuffer;
				m_pBuffer = nullptr;
			}
		}

		//Deep copy
		Vector& operator=(const Vector<T>& other)
		{
			if (m_pBuffer && m_Capacity != other.m_Capacity)
			{
				delete[] m_pBuffer;
				m_pBuffer = nullptr;
			}
			if (m_pBuffer == nullptr && other.m_Capacity > 0)
			{
				m_pBuffer = new T[other.m_Size];
			}
			m_Capacity = other.m_Capacity;

			m_Size = other.m_Size;
			memcpy(m_pBuffer, other.m_pBuffer, sizeof(T) * other.m_Size);
			return *this;
		}

		bool operator==(const Vector<T>& other) const
		{
			if (m_Size != other.m_Size)
				return false;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_pBuffer[i] != other.m_pBuffer[i])
					return false;
			}
			return true;
		}

		bool operator!=(const Vector& other) const
		{
			return !operator==(other);
		}

		operator bool() const
		{
			return m_Size > 0;
		}

		T& operator[](const size_t index)
		{
			return m_pBuffer[index];
		}

		const T& operator[](const size_t index) const
		{
			return m_pBuffer[index];
		}

		T& At(const size_t index)
		{
			assert(index < m_Size);
			return m_pBuffer[index];
		}

		const T& At(const size_t index) const
		{
			assert(index < m_Size);
			return m_pBuffer[index];
		}

		//Keeps the items in memory but reset the size
		void Clear()
		{
			m_Size = 0;
		}

		void Resize(const size_t size)
		{
			size_t copyWidth = 0;
			if (m_pBuffer)
			{
				T* pNewBuffer = new T[size];
				copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, sizeof(T) * copyWidth);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new T[size];
			}
			memset(m_pBuffer + copyWidth, 0, (size - copyWidth) * sizeof(T));
			m_Size = size;
			m_Capacity = size;
		}

		void Reserve(const size_t size)
		{
			if (size <= m_Capacity)
				return;

			if (m_pBuffer != nullptr)
			{
				T* pNewBuffer = new T[size];
				size_t copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, sizeof(T) * copyWidth);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new T[size];
			}
			m_Capacity = size;
		}

		void ShrinkToFit()
		{
			Resize(m_Size);
		}

		void Push(const T& value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(m_Size + CAPACITY_STEP_SIZE);
			}
			*(m_pBuffer + m_Size) = value;
			++m_Size;
		}

		void Push(T&& value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(m_Size + CAPACITY_STEP_SIZE);
			}
			*(m_pBuffer + m_Size) = Move(value);
			++m_Size;
		}

		T Pop()
		{
			assert(m_Size > 0);
			RandomAccessIterator<T> pValue = End() - 1;
			--m_Size;
			return *pValue;
		}

		void SwapVector(Vector<T>& other)
		{
			Swap(m_pBuffer, other.m_pBuffer);
			Swap(m_Size, other.m_Size);
			Swap(m_Capacity, other.m_Capacity);
		}

		void Assign(const size_t amount, T value)
		{
			if (m_Size + amount > m_Capacity)
				Reserve(m_Size + amount);
			for (size_t i = 0; i < amount; ++i)
			{
				memcpy(&m_pBuffer[m_Size + i], &value, sizeof(T));
			}
			m_Size += amount;
		}

		void SwapEraseAt(const size_t index)
		{
			assert(index < m_Size);
			Swap(m_pBuffer[index], Back());
			--m_Size;
		}

		Iterator EraseAt(const size_t index)
		{
			assert(index < m_Size);
			for (size_t i = index; i < m_Size - 1; ++i)
				m_pBuffer[i] = Move(m_pBuffer[i + 1]);
			--m_Size;
			return Iterator(m_pBuffer + index);
		}

		Iterator Insert(const size_t index, const T& value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
				Reserve(m_Size + 1);

			for (size_t i = m_Size; i > index; --i)
				m_pBuffer[i] = Move(m_pBuffer[i - 1]);
			m_pBuffer[index] = value;
			++m_Size;
			return Iterator(m_pBuffer + index);
		}

		ConstIterator Find(const T& value) const
		{
			ConstIterator pIt = Begin();
			while (pIt != End() && *pIt != value)
				++pIt;
			return pIt;
		}

		Iterator Find(const T& value)
		{
			Iterator pIt = Begin();
			while (pIt != End() && *pIt != value)
				++pIt;
			return pIt;
		}

		const T* Data() const { return m_pBuffer; }
		T* Data() { return m_pBuffer; }
		size_t Size() const { return m_Size; }
		size_t Capacity() const { return m_Capacity; }
		bool Empty() const { return m_Size == 0; }

		Iterator begin() { return Iterator(m_pBuffer); }
		Iterator end() { return Iterator(m_pBuffer + m_Size); }
		ConstIterator begin() const { return ConstIterator(m_pBuffer); }
		ConstIterator end() const { return ConstIterator(m_pBuffer + m_Size); }

		Iterator Begin() { return Iterator(m_pBuffer); }
		Iterator End() { return Iterator(m_pBuffer + m_Size); }
		ConstIterator Begin() const { return ConstIterator(m_pBuffer); }
		ConstIterator End() const { return ConstIterator(m_pBuffer + m_Size); }

		T& Front() { assert(m_pBuffer); return *m_pBuffer; }
		const T& Front() const { assert(m_pBuffer); return *m_pBuffer; }
		T& Back() { assert(m_Size > 0); return *(m_pBuffer + m_Size - 1); }
		const T& Back() const { assert(m_Size > 0); return *(m_pBuffer + m_Size - 1); }

		constexpr static size_t MaxSize() { return Npos; }
		static const size_t Npos = ~(size_t)0;
		static const int CAPACITY_STEP_SIZE = 4;

	private:
		T * m_pBuffer;
		size_t m_Size;
		size_t m_Capacity;
	};

	template<typename T>
	void Swap(Vector<T>& a, Vector<T>& b)
	{
		a.SwapVector(b);
	}
}