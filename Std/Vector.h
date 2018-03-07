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
			m_pBuffer(new unsigned char[size * sizeof(T)]), m_Size(size), m_Capacity(size)
		{
			memset(m_pBuffer, 0, sizeof(T) * size);
		}

		Vector(const size_t size, const T& value) :
			m_pBuffer(new unsigned char[size * sizeof(T)]), m_Size(size), m_Capacity(size)
		{
			for (size_t i = 0; i < size; ++i)
				Buffer()[i] = value;
		}

		Vector(T* pData, const size_t size) :
			m_pBuffer(new unsigned char[size * sizeof(T)]), m_Size(size), m_Capacity(size)
		{
			memcpy(m_pBuffer, pData, sizeof(T) * size);
		}

		Vector(std::initializer_list<T> list) :
			m_pBuffer(new unsigned char[list.size() * sizeof(T)]), m_Size(list.size()), m_Capacity(list.size())
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
			m_pBuffer = new unsigned char[other.m_Size * sizeof(T)];
			memcpy(m_pBuffer, other.m_pBuffer, sizeof(T) * other.m_Size);
		}

		~Vector()
		{
			if (m_pBuffer)
			{
				DestructElements(Buffer(), m_Capacity);
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
				m_pBuffer = new unsigned char[other.m_Size * sizeof(T)];
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
				if (Buffer()[i] != other.Buffer()[i])
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
			return Buffer()[index];
		}

		const T& operator[](const size_t index) const
		{
			return Buffer()[index];
		}

		T& At(const size_t index)
		{
			assert(index < m_Size);
			return Buffer()[index];
		}

		const T& At(const size_t index) const
		{
			assert(index < m_Size);
			return Buffer()[index];
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
				unsigned char* pNewBuffer = new unsigned char[size * sizeof(T)];
				copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, sizeof(T) * copyWidth);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
				ConstructElements(Buffer() + copyWidth, nullptr, size - copyWidth);
			}
			else
			{
				m_pBuffer = new unsigned char[size * sizeof(T)];
				ConstructElements(Buffer(), nullptr, size);
			}
			m_Size = size;
			m_Capacity = size;
		}

		void Reserve(const size_t size)
		{
			if (size <= m_Capacity)
				return;

			if (m_pBuffer != nullptr)
			{
				unsigned char* pNewBuffer = new unsigned char[size * sizeof(T)];
				size_t copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, sizeof(T) * copyWidth);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
				ConstructElements(Buffer() + copyWidth, nullptr, size - copyWidth);
			}
			else
			{
				m_pBuffer = new unsigned char[size * sizeof(T)];
				ConstructElements(Buffer(), nullptr, size);
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
				Reserve(CalculateGrowth(m_Size));
			}
			*(Buffer() + m_Size) = value;
			++m_Size;
		}

		void Push(T&& value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			*(Buffer() + m_Size) = Move(value);
			++m_Size;
		}

		T Pop()
		{
			assert(m_Size > 0);
			RandomAccessIterator<T> pValue = End() - 1;
			--m_Size;
			return *pValue;
		}

		void Swap(Vector<T>& other)
		{
			FluxStd::Swap(m_pBuffer, other.m_pBuffer);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_Capacity, other.m_Capacity);
		}

		void Assign(const size_t amount, T value)
		{
			if (m_Size + amount > m_Capacity)
				Reserve(m_Size + amount);
			for (size_t i = 0; i < amount; ++i)
			{
				memcpy(Buffer() + m_Size + i, &value, sizeof(T));
			}
			m_Size += amount;
		}

		void SwapEraseAt(const size_t index)
		{
			assert(index < m_Size);
			FluxStd::Swap(Buffer()[index], Back());
			--m_Size;
		}

		Iterator EraseAt(const size_t index)
		{
			assert(index < m_Size);
			for (size_t i = index; i < m_Size - 1; ++i)
				Buffer()[i] = Move(Buffer()[i + 1]);
			--m_Size;
			return Iterator(Buffer() + index);
		}

		Iterator Insert(const size_t index, const T& value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
				Reserve(m_Size + 1);

			for (size_t i = m_Size; i > index; --i)
				Buffer()[i] = Move(Buffer()[i - 1]);
			Buffer()[index] = value;
			++m_Size;
			return Iterator(Buffer() + index);
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

		const T* Data() const { return Buffer(); }
		T* Data() { return Buffer(); }
		size_t Size() const { return m_Size; }
		size_t Capacity() const { return m_Capacity; }
		bool Empty() const { return m_Size == 0; }

		Iterator begin() { return Iterator(Buffer()); }
		Iterator end() { return Iterator(Buffer() + m_Size); }
		ConstIterator begin() const { return ConstIterator(Buffer()); }
		ConstIterator end() const { return ConstIterator(Buffer() + m_Size); }

		Iterator Begin() { return Iterator(Buffer()); }
		Iterator End() { return Iterator(Buffer() + m_Size); }
		ConstIterator Begin() const { return ConstIterator(Buffer()); }
		ConstIterator End() const { return ConstIterator(Buffer() + m_Size); }

		T& Front() { assert(m_pBuffer); return *Buffer(); }
		const T& Front() const { assert(m_pBuffer); return *Buffer(); }
		T& Back() { assert(m_Size > 0); return *(Buffer() + m_Size - 1); }
		const T& Back() const { assert(m_Size > 0); return *(Buffer() + m_Size - 1); }

		constexpr static size_t MaxSize() { return Npos; }
		static const size_t Npos = ~(size_t)0;

	private:
		size_t CalculateGrowth(const size_t oldSize)
		{
			size_t newSize = (size_t)floor(oldSize * 1.5);
			return newSize == m_Capacity ? newSize + 1 : newSize;
		}

		T* Buffer() const
		{
			return reinterpret_cast<T*>(m_pBuffer);
		}

		void ConstructElements(T* pDestination, T* pSource, const size_t count)
		{
			if (!pSource)
			{
				for (unsigned i = 0; i < count; ++i)
					new(pDestination + i) T();
			}
			else
			{
				for (unsigned i = 0; i < count; ++i)
					new(pDestination + i) T(*(pSource + i));
			}
		}

		void CopyElements(T* pDestination, const T* pSource, int count)
		{
			while (count--)
				*pDestination++ = *pSource++;
		}

		void DestructElements(T* pDestination, size_t count)
		{
			while (count--)
			{
				pDestination->~T();
				++pDestination;
			}
		}

		unsigned char * m_pBuffer;
		size_t m_Size;
		size_t m_Capacity;
	};

	template<typename T>
	inline void Swap(Vector<T>& a, Vector<T>& b)
	{
		a.Swap(b);
	}
}