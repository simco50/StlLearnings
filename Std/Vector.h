#pragma once
#include <initializer_list>
#include <assert.h>
#include "Iterator.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename T>
	class Vector
	{
	public:
		using Iterator = RandomAccessIterator<T>;
		using ConstIterator = RandomAccessConstIterator<T>;
		using SizeType = size_t;

	public:
		Vector() noexcept
			: m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{}
		explicit Vector(const SizeType size)
			: m_pBuffer(Allocate(size)), m_Size(size), m_Capacity(size)
		{
			ConstructElements(Buffer(), size);
		}

		Vector(const SizeType size, const T& value)
			: m_pBuffer(Allocate(size)), m_Size(size), m_Capacity(size)
		{
			ConstructElements(Buffer(), size);
			for (SizeType i = 0; i < size; ++i)
			{
				Buffer()[i] = value;
			}
		}

		Vector(const T* pData, const SizeType size)
			: m_pBuffer(Allocate(size)), m_Size(size), m_Capacity(size)
		{
			ConstructElements(Buffer(), pData, size);
		}

		Vector(std::initializer_list<T> list)
			: m_pBuffer(Allocate(list.size())), m_Size(list.size()), m_Capacity(list.size())
		{
			ConstructElements(Buffer(), list.begin(), list.size());
		}

		//Move semantics
		Vector(Vector&& other) noexcept
			: m_pBuffer(other.m_pBuffer), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_Size = 0;
			other.m_Capacity = 0;
			other.m_pBuffer = nullptr;
		}

		//Deep copy
		Vector(const Vector& other)
			: m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			m_pBuffer = Allocate(other.m_Size);
			ConstructElements(Buffer(), other.Buffer(), other.m_Size);
		}

		~Vector()
		{
			if (m_pBuffer)
			{
				DestructElements(Buffer(), m_Size);
				Free(m_pBuffer);
			}
		}

		//Deep copy
		Vector& operator=(const Vector& other)
		{
			if (m_pBuffer)
			{
				DestructElements(Buffer(), m_Capacity);
				if (m_Capacity != other.m_Capacity)
				{
					Free(m_pBuffer);
				}
			}
			if (m_pBuffer == nullptr && other.m_Capacity > 0)
			{
				m_pBuffer = Allocate(other.m_Size);
			}
			m_Capacity = other.m_Capacity;
			m_Size = other.m_Size;

			ConstructElements(Buffer(), other.Buffer(), other.Size());
			return *this;
		}

		//Move operation
		Vector& operator=(Vector&& other)
		{
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_Capacity, other.m_Capacity);
			FluxStd::Swap(m_pBuffer, other.m_pBuffer);
			return *this;
		}

		//Keeps the items in memory but reset the size
		void Clear(bool shrink = false)
		{
			DestructElements(Buffer(), m_Size);
			m_Size = 0;
			if (shrink)
			{
				m_Capacity = 0;
				Free(m_pBuffer);
			}
		}

		void Resize(const SizeType size)
		{
			if (m_pBuffer)
			{
				unsigned char* pNewBuffer = Allocate(size);
				SizeType copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, sizeof(T) * copyWidth);
				//Shrinking
				if (size < m_Size)
				{
					DestructElements(Buffer() + copyWidth, m_Size - size);
					Free(m_pBuffer);
					m_pBuffer = pNewBuffer;
				}
				//Enlarging
				else
				{
					Free(m_pBuffer);
					m_pBuffer = pNewBuffer;
					ConstructElements(Buffer() + copyWidth, size - m_Size);
				}
			}
			else
			{
				m_pBuffer = Allocate(size);
				ConstructElements(Buffer(), size);
			}
			m_Size = size;
			m_Capacity = size;
		}

		void Reserve(const SizeType size)
		{
			if (size > m_Capacity)
			{
				if (m_pBuffer != nullptr)
				{
					unsigned char* pNewBuffer = Allocate(size);
					SizeType copyWidth = size > m_Size ? m_Size : size;
					memcpy(pNewBuffer, m_pBuffer, sizeof(T) * copyWidth);
					Free(m_pBuffer);
					m_pBuffer = pNewBuffer;
				}
				else
				{
					m_pBuffer = Allocate(size);
				}
				m_Capacity = size;
			}
		}

		void ShrinkToFit()
		{
			Resize(m_Size);
		}

		//If input is r-value
		//T will be int so it uses int&& for Forward
		//If input is l-value
		//V& is used for deduction
		//It uses int& && for Forward which callapses to int&
		void Push(const T& value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			new (Buffer() + m_Size) T(value);
			++m_Size;
		}

		void Push(T&& value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			new (Buffer() + m_Size) T(FluxStd::Forward<T>(value));
			++m_Size;
		}

		void PushUnique(const T& value)
		{
			if (!Find(value))
			{
				Push(value);
			}
		}

		void PushUnique(T&& value)
		{
			if (!Find(value))
			{
				Push(std::forward<T>(value));
			}
		}

		void Append(const T* pOther, SizeType count)
		{
			Reserve(m_Size + count);
			ConstructElements(Buffer() + m_Size, pOther, count);
			m_Size += count;
		}

		void Append(const Vector& other)
		{
			Append(other.Data(), other.Size());
		}

		//Generally better way to add to an array because the object gets created in the method itself by forwarding the arguments
		template<typename... Args>
		void Emplace(Args&&... args)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			new (Buffer() + m_Size) T(FluxStd::Forward<Args>(args)...);
			++m_Size;
		}

		T Pop()
		{
			assert(m_Size > 0);
			T value = Move(Back());
			DestructElements(Buffer() + m_Size - 1, 1);
			--m_Size;
			return value;
		}

		void Swap(Vector& other)
		{
			FluxStd::Swap(m_pBuffer, other.m_pBuffer);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_Capacity, other.m_Capacity);
		}

		void Swap(SizeType a, SizeType b)
		{
			assert(a < Size());
			assert(b < Size());
			FluxStd::Swap(Data()[a], Data()[b]);
		}

		void Assign(const SizeType amount, const T& value)
		{
			if (m_Size + amount > m_Capacity)
			{
				Reserve(m_Size + amount);
			}
			for (SizeType i = 0; i < amount; ++i)
			{
				ConstructElements(Buffer() + m_Size + i, &value, 1);
			}
			m_Size += amount;
		}

		bool Remove(const T& value)
		{
			SizeType i = IndexOf(value);
			if (i != MaxSize())
			{
				RemoveAt(i);
				return true;
			}
			return false;
		}

		bool RemoveSwap(const T& value)
		{
			SizeType i = IndexOf(value);
			if (i != MaxSize())
			{
				FluxStd::Swap(Buffer()[i], Buffer()[m_Size - 1]);
				--m_Size;
				return true;
			}
			return false;
		}

		void RemoveAll(const T& value)
		{
			for(;;)
			{
				if (!Remove(value))
				{
					return;
				}
			}
		}

		void RemoveAllSwap(const T& value)
		{
			for (;;)
			{
				if (!RemoveSwap(value))
				{
					return;
				}
			}
		}

		template<typename Predicate>
		bool RemoveByPredicate(Predicate&& predicate)
		{
			for (;;)
			{
				SizeType i = IndexOfByPredicate(predicate);
				if (i != MaxSize())
				{
					RemoveAt(i);
				}
				else
				{
					return;
				}
			}
		}

		template<typename Predicate>
		bool RemoveSwapByPredicate(Predicate&& predicate)
		{
			for (;;)
			{
				SizeType i = IndexOfByPredicate(predicate);
				if (i != MaxSize())
				{
					RemoveSwapAt(i);
				}
				else
				{
					return;
				}
			}
		}

		void RemoveSwapAt(const SizeType index)
		{
			assert(IsValidIndex(index));
			FluxStd::Swap(Buffer()[index], Back());
			DestructElements(Buffer() + m_Size - 1, 1);
			--m_Size;
		}

		Iterator RemoveAt(const SizeType index)
		{
			assert(IsValidIndex(index));
			DestructElements(Buffer() + index, 1);
			MoveRange(index, index + 1, m_Size - index - 1);
			--m_Size;
			return Iterator(Buffer() + index);
		}

		Iterator Insert(const SizeType index, const T& value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
			{
				Reserve(m_Size + 1);
			}

			MoveRange(index + 1, index, m_Size - index);
			new (Buffer() + index) T(value);
			++m_Size;
			return Iterator(Buffer() + index);
		}

		Iterator Insert(const SizeType index, T&& value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
			{
				Reserve(m_Size + 1);
			}

			MoveRange(index + 1, index, m_Size - index);
			new (Buffer() + index) T(Forward<T>(value));
			++m_Size;
			return Iterator(Buffer() + index);
		}

		T* Find(const T& value)
		{
			for (Iterator it = begin(); it != end(); ++it)
			{
				if (*it == value)
				{
					return it.pPtr;
				}
			}
			return nullptr;
		}

		const T* Find(const T& value) const
		{
			for (ConstIterator it = begin(); it != end(); ++it)
			{
				if (*it == value)
				{
					return it.pPtr;
				}
			}
			return nullptr;
		}

		SizeType IndexOf(const T& value) const
		{
			for (SizeType i = 0; i < m_Size; ++i)
			{
				if (Data()[i] == value)
				{
					return i;
				}
			}
			return -1;
		}

		template<typename Predicate>
		SizeType IndexOfByPredicate(Predicate&& predicate) const
		{
			for (SizeType i = 0; i < m_Size; ++i)
			{
				if (predicate(Data()[i]))
				{
					return i;
				}
			}
			return -1;
		}

		template<typename Predicate>
		T* FindByPredicate(Predicate&& predicate)
		{
			for (Iterator it = begin(); it != end(); ++it)
			{
				if (predicate(*it))
				{
					return it.pPtr;
				}
			}
			return nullptr;
		}

		template<typename Predicate>
		const T* FindByPredicate(Predicate&& predicate) const
		{
			for (ConstIterator it = begin(); it != end(); ++it)
			{
				if (predicate(*it))
				{
					return it.pPtr;
				}
			}
			return nullptr;
		}

		//Very simple shuffle
		void Shuffle()
		{
			for (SizeType i = 0; i < m_Size; ++i)
			{
				SizeType j = rand() % m_Size;
				FluxStd::Swap(m_pBuffer[i], m_pBuffer[j]);
			}
		}

		bool operator==(const Vector<T>& other) const
		{
			if (m_Size != other.m_Size)
			{
				return false;
			}
			for (SizeType i = 0; i < m_Size; ++i)
			{
				if (Buffer()[i] != other.Buffer()[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator!=(const Vector& other) const
		{
			if (m_Size != other.m_Size)
			{
				return true;
			}
			for (SizeType i = 0; i < m_Size; ++i)
			{
				if (Buffer()[i] != other.Buffer()[i])
				{
					return true;
				}
			}
			return false;
		}

		inline Vector& operator+=(const Vector& other)
		{
			Append(other);
			return *this;
		}

		inline operator bool() const noexcept
		{
			return m_Size > 0;
		}

		//Non-const. No assert random access
		inline T& operator[](const SizeType index)
		{
			return Buffer()[index];
		}

		//Const.No assert random access
		inline const T& operator[](const SizeType index) const
		{
			return Buffer()[index];
		}

		//Non-const. Assert random access
		T& At(const SizeType index)
		{
			assert(IsValidIndex(index));
			return Buffer()[index];
		}

		//Const. Assert random access
		const T& At(const SizeType index) const
		{
			assert(IsValidIndex(index));
			return Buffer()[index];
		}

		inline bool IsValidIndex(const SizeType index) const
		{
			return index < m_Size;
		}

		inline const T* Data() const 
		{ 
			return Buffer();
		}

		inline T* Data() 
		{ 
			return Buffer();
		}

		inline SizeType Size() const 
		{
			return m_Size; 
		}

		inline SizeType Capacity() const 
		{ 
			return m_Capacity; 
		}

		inline bool Empty() const
		{ 
			return m_Size == 0;
		}

		inline Iterator begin() 
		{ 
			return Iterator(Data()); 
		}

		inline Iterator end()
		{ 
			return Iterator(Data() + m_Size);
		}

		inline ConstIterator begin() const 
		{
			return ConstIterator(Data());
		}

		inline ConstIterator end() const 
		{ 
			return ConstIterator(Data() + m_Size);
		}

		inline Iterator Begin() 
		{ 
			return Iterator(Buffer()); 
		}

		inline Iterator End() 
		{ 
			return Iterator(Buffer() + m_Size); 
		}

		inline ConstIterator Begin() const 
		{
			return ConstIterator(Buffer());
		}

		inline ConstIterator End() const 
		{
			return ConstIterator(Buffer() + m_Size);
		}

		inline T& Front() 
		{
			assert(m_pBuffer); 
			return *Buffer();
		}

		inline const T& Front() const 
		{ 
			assert(m_pBuffer);
			return *Buffer();
		}

		inline T& Back() 
		{
			assert(m_Size > 0); 
			return *(Buffer() + m_Size - 1);
		}

		inline const T& Back() const 
		{
			assert(m_Size > 0); 
			return *(Buffer() + m_Size - 1); 
		}

		//Move elements
		inline void MoveRange(const SizeType dest, const SizeType src, const SizeType count)
		{
			if (count)
			{
				memmove(Buffer() + dest, Buffer() + src, count * sizeof(T));
			}
		}

		constexpr static SizeType MaxSize() { return ~(SizeType)0; }

	private:
		inline SizeType CalculateGrowth(const SizeType oldSize)
		{
			SizeType newSize = (SizeType)floor(oldSize * 1.5);
			return newSize == m_Capacity ? newSize + 1 : newSize;
		}

		inline T* Buffer() const noexcept
		{
			return reinterpret_cast<T*>(m_pBuffer);
		}

		inline unsigned char* Allocate(const SizeType size)
		{
			return new unsigned char[sizeof(T) * size];
		}

		inline void Free(unsigned char* pBuffer)
		{
			delete[] pBuffer;
			m_pBuffer = nullptr;
		}

		//Call constructor on elements with source data
		void ConstructElements(T* pDestination, const T* pSource, const SizeType count)
		{
			for (unsigned i = 0; i < count; ++i)
			{
				new(pDestination + i) T(pSource[i]);
			}
		}

		//Call empty constructor on elements
		inline void ConstructElements(T* pDestination, const SizeType count)
		{
			for (unsigned i = 0; i < count; ++i)
			{
				new(pDestination + i) T();
			}
		}

		//Call destructor on elements
		void DestructElements(T* pDestination, SizeType count)
		{
			while (count--)
			{
				pDestination->~T();
				++pDestination;
			}
		}

		unsigned char* m_pBuffer;
		SizeType m_Size;
		SizeType m_Capacity;
	};

	template<typename T>
	inline void Swap(Vector<T>& a, Vector<T>& b)
	{
		a.Swap(b);
	}
}