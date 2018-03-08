#pragma once
#include <assert.h>
#include <iostream>
#include "Iterator.h"
#include "Algorithm.h"
#include "Utility.h"
#include "Hash.h"
#include "Vector.h"

namespace FluxStd
{
	inline size_t StrLen(const char* pData)
	{
		const char* pCurrent = pData;
		for (; *pCurrent; ++pCurrent)
		{
		}
		return pCurrent - pData;
	}

	inline size_t StrLen(const wchar_t* pData)
	{
		const wchar_t* pCurrent = pData;
		for (; *pCurrent; ++pCurrent)
		{
		}
		return pCurrent - pData;
	}

	class String
	{
	public:
		using Iterator = RandomAccessIterator<char>;
		using ConstIterator = RandomAccessConstIterator<char>;
	public:
		//Empty string, no buffer
		String() :
			m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{}

		//Create string filled with defined value
		String(const size_t size, const char value = '\0') :
			m_pBuffer(new char[size + 1]), m_Size(size), m_Capacity(size)
		{
			memset(m_pBuffer, value, m_Size);
			m_pBuffer[size] = '\0';
		}

		//Create string from c-string
		String(const char* pData) :
			m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(pData);
		}

		//Create string from unicode c-string
		explicit String(const wchar_t* pData) :
			m_pBuffer(nullptr), m_Size(0)
		{
			Append(pData);
		}

		//Create string from begin and end iterator
		explicit String(const char* pBegin, const char* pEnd)
		{
			if (pBegin == nullptr || pEnd == nullptr)
			{
				m_pBuffer = nullptr;
				m_Capacity = 0;
				m_Size = 0;
			}
			else
			{
				m_Size = pEnd - pBegin;
				m_Capacity = m_Size;
				m_pBuffer = new char[m_Size + 1];
				memcpy(m_pBuffer, pBegin, m_Size);
				m_pBuffer[m_Size] = '\0';
			}
		}

		//Move semantics
		String(String&& other) :
			m_pBuffer(other.m_pBuffer), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_pBuffer = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		//Deep copy constructor
		String(const String& other) :
			m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(other.Data());
		}

		//Deep copy assignment
		String& operator=(const String& other)
		{
			if(other.m_pBuffer != m_pBuffer)
				Append(other.Data());
			return *this;
		}

		~String()
		{
			if (m_pBuffer)
			{
				delete[] m_pBuffer;
				m_pBuffer = nullptr;
			}
		}

		//Set the size to 0 but keep the buffer or resize to 0
		void Clear(const bool hard = false)
		{
			if (hard == false)
				m_Size = 0;
			else
				Resize(0);
		}

		//Hard resize the buffer
		void Resize(const size_t size)
		{
			size_t copyWidth = 0;
			if (m_pBuffer)
			{
				char* pNewBuffer = new char[size + 1];
				copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, copyWidth);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new char[size + 1];
			}
			memset(m_pBuffer + copyWidth, 0, (size - copyWidth));
			m_Size = size;
			m_Capacity = size;
			m_pBuffer[m_Size] = '\0';
		}

		//Create a buffer large enough to fit the given size
		void Reserve(const size_t size)
		{
			if (size <= m_Capacity)
				return;

			if (m_pBuffer != nullptr)
			{
				char* pNewBuffer = new char[size + 1];
				size_t copyWidth = size > m_Size ? m_Size: size;
				memcpy(pNewBuffer, m_pBuffer, copyWidth + 1);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new char[size + 1];
			}
			m_Capacity = size;
		}

		//Resizes the buffer so that the characters just without
		void ShrinkToFit()
		{
			Resize(m_Size);
		}

		char Pop()
		{
			assert(m_Size > 0);
			char value = Back();
			--m_Size;
			m_pBuffer[m_Size] = '\0';
			return value;
		}

		void Swap(String& other)
		{
			FluxStd::Swap(m_pBuffer, other.m_pBuffer);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_Capacity, other.m_Capacity);
		}

		void Assign(const size_t amount, const char value)
		{
			if (m_Size + amount > m_Capacity)
				Reserve(m_Size + amount);
			for (size_t i = 0; i < amount; ++i)
			{
				memcpy(m_pBuffer + m_Size + i, &value, sizeof(char));
			}
			m_Size += amount;
			m_pBuffer[m_Size] = '\0';
		}

		void EraseAt(const size_t index)
		{
			assert(index < m_Size);
			for (size_t i = index; i < m_Size - 1; ++i)
				m_pBuffer[i] = m_pBuffer[i + 1];
			--m_Size;
			m_pBuffer[m_Size] = '\0';
		}

		void Insert(const size_t index, const char value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
				Reserve(m_Size + 1);

			for (size_t i = m_Size; i > index; --i)
				m_pBuffer[i] = m_pBuffer[i - 1];
			m_pBuffer[index] = value;
			++m_Size;
			m_pBuffer[m_Size] = '\0';
		}

		void Insert(const size_t index, const char* pData)
		{
			size_t len = StrLen(pData);
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
				Reserve(m_Size + len);

			for (size_t i = m_Size + len; i > index; --i)
				m_pBuffer[i] = m_pBuffer[i - len];
			memcpy(m_pBuffer + index, pData, len);
			m_Size += len;
		}

		void Append(const char value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			*(m_pBuffer + m_Size) = value;
			++m_Size;
			m_pBuffer[m_Size] = '\0';
		}

		void Append(const char* pData)
		{
			if (pData)
			{
				const size_t dataSize = StrLen(pData);			
				char* pNewBuffer = new char[m_Size + dataSize + 1];
				memcpy(pNewBuffer + m_Size, pData, dataSize);
				if (m_pBuffer)
				{
					memcpy(pNewBuffer, m_pBuffer, m_Size);
					delete[] m_pBuffer;
				}
				m_Size += dataSize;
				m_Capacity = m_Size;
				m_pBuffer = pNewBuffer;
				m_pBuffer[m_Size] = '\0';
			}
		}

		void Append(const wchar_t* pData)
		{
			const size_t dataSize = StrLen(pData);
			char* pNewBuffer = new char[m_Size + dataSize + 1];
			for (size_t i = 0; i < dataSize; ++i)
			{
				pNewBuffer[i + m_Size] = (char)pData[i];
			}
			if (m_pBuffer)
			{
				memcpy(pNewBuffer, m_pBuffer, m_Size);
				delete[] m_pBuffer;
			}
			m_Size += dataSize;
			m_Capacity = m_Size;
			m_pBuffer = pNewBuffer;
			m_pBuffer[m_Size] = '\0';
		}

		String Substring(const size_t from, const size_t length = String::Npos) const
		{
			if (length == String::Npos)
			{
				assert(from <= m_Size);
				return String(m_pBuffer + from, m_pBuffer + m_Size);
			}
			else
			{
				assert(length + from <= m_Size);
				return String(m_pBuffer + from, m_pBuffer + from + length);
			}
		}

		void ToUpperInline()
		{
			if (m_pBuffer)
			{
				for (char& c : *this)
					c = FluxStd::ToUpper(c);
			}
		}

		String ToUpper()
		{
			String str;
			if (m_pBuffer == nullptr)
				return str;
			str.m_pBuffer = new char[m_Capacity + 1];
			str.m_Capacity = m_Capacity;
			str.m_Size = m_Size;
			for (size_t i = 0; i < m_Size + 1; i++)
				str.m_pBuffer[i] = FluxStd::ToUpper(m_pBuffer[i]);
			return str;
		}

		void ToLowerInline()
		{
			if (m_pBuffer)
			{
				for (char& c : *this)
					c = FluxStd::ToLower(c);
			}
		}

		String ToLower()
		{
			String str;
			if (m_pBuffer == nullptr)
				return str;
			str.m_pBuffer = new char[m_Capacity + 1];
			str.m_Capacity = m_Capacity;
			str.m_Size = m_Size;
			for (size_t i = 0; i < m_Size + 1; i++)
				str.m_pBuffer[i] = FluxStd::ToLower(m_pBuffer[i]);
			return str;
		}

		size_t Find(const char c) const
		{
			if (m_Size > 0)
			{
				for (size_t i = 0; i < m_Size; ++i)
				{
					if (m_pBuffer[i] == c)
						return i;
				}
			}
			return String::Npos;
		}

		size_t RFind(const char c) const
		{
			if (m_Size > 0)
			{
				for (size_t i = m_Size - 1; i != String::Npos; --i)
				{
					if (m_pBuffer[i] == c)
						return i;
				}
			}
			return String::Npos;
		}

		size_t Find(const wchar_t c) const
		{
			if (m_Size > 0)
			{
				for (size_t i = 0; i < m_Size; ++i)
				{
					if (m_pBuffer[i] == (char)c)
						return i;
				}
			}
			return String::Npos;
		}

		size_t RFind(const wchar_t c) const
		{
			if (m_Size > 0)
			{
				for (size_t i = m_Size - 1; i != String::Npos; --i)
				{
					if (m_pBuffer[i] == (char)c)
						return i;
				}
			}
			return String::Npos;
		}

		size_t Find(const String& str) const
		{
			return Find(str.Data());
		}

		size_t RFind(const String& str) const
		{
			return RFind(str.Data());
		}

		size_t Find(const char* c) const
		{
			if (m_Size == 0)
				return String::Npos;

			size_t len = StrLen(c);
			if (len == 0)
				return String::Npos;

			for (size_t i = 0; i < m_Size - len; ++i)
			{
				if (m_pBuffer[i] == c[0])
				{
					bool match = true;
					for (size_t j = 1; j < len; ++j)
					{
						if (m_pBuffer[i + j] != c[j])
						{
							match = false;
							break;
						}
					}
					if (match)
						return (int)i;
				}
			}
			return String::Npos;
		}

		size_t RFind(const char* c) const
		{
			if (m_Size == 0)
				return String::Npos;

			size_t len = StrLen(c);
			if (len == 0)
				return String::Npos;

			for (size_t i = m_Size - 1; i >= len - 1; --i)
			{
				if (m_pBuffer[i] == c[len - 1])
				{
					bool match = true;
					for (int j = (int)len - 1; j >= 0; --j)
					{
						if (m_pBuffer[i - len + j + 1] != c[j])
						{
							match = false;
							break;
						}
					}
					if (match)
						return i - len + 1;
				}
			}
			return String::Npos;
		}

		bool StartsWith(const char* pStr) const
		{
			const size_t len = StrLen(pStr);
			if (len > m_Size)
				return false;
			for (size_t i = 0; i < len; i++)
			{
				if (m_pBuffer[i] != pStr[i])
					return false;
			}
			return true;
		}

		bool EndsWith(const char* pStr) const
		{
			const size_t len = StrLen(pStr);
			if (len > m_Size)
				return false;
			for (size_t i = 0; i < len ; i++)
			{
				if (m_pBuffer[m_Size - len + i] != pStr[i])
					return false;
			}
			return true;
		}

		Vector<String> Split(const char delimiter) const
		{
			return Split(&delimiter, 1);
		}

		Vector<String> Split(const char* pDelimiters, const size_t delimiterCount) const
		{
			Vector<String> out;
			size_t start = 0;
			for (size_t i = 0; i < m_Size; i++)
			{
				for (size_t j = 0; j < delimiterCount ; j++)
				{
					if (m_pBuffer[i] == pDelimiters[j])
					{
						out.Push(Substring(start, i - start));
						start = i + 1;
						break;
					}
				}
			}
			out.Push(Substring(start));
			return out;
		}

		void Replace(const char toReplace, const char replaceWith)
		{
			for (char& c : *this)
			{
				if (c == toReplace)
					c = replaceWith;
			}
		}

		String& operator+(const String& other)
		{
			Append(other.Data());
			return *this;
		}

		String& operator+(const char* pData)
		{
			Append(pData);
			return *this;
		}

		String& operator+(const char c)
		{
			Append(c);
			return *this;
		}

		String& operator+=(const String& other)
		{
			Append(other.Data());
			return *this;
		}

		String& operator+=(const char* pData)
		{
			Append(pData);
			return *this;
		}

		String& operator+=(const char c)
		{
			Append(c);
			return *this;
		}

		bool operator==(const String& other) const
		{
			size_t len = Length();
			if (len != other.m_Size)
				return false;
			for (size_t i = 0; i < len; ++i)
			{
				if (m_pBuffer[i] != other.m_pBuffer[i])
					return false;
			}
			return true;
		}

		bool operator!=(const String& other) const
		{
			return !operator==(other);
		}

		bool operator==(const char* pData) const
		{
			size_t len = Length();
			if (len != StrLen(pData))
				return false;
			for (size_t i = 0; i < len; ++i)
			{
				if (m_pBuffer[i] != pData[i])
					return false;
			}
			return true;
		}

		bool operator!=(const char* pData) const
		{
			return !operator==(pData);
		}

		bool operator==(const wchar_t* pData) const
		{
			size_t len = Length();
			if (len != StrLen(pData))
				return false;
			for (size_t i = 0; i < len; ++i)
			{
				if (m_pBuffer[i] != (wchar_t)pData[i])
					return false;
			}
			return true;
		}

		bool operator!=(const wchar_t* pData) const
		{
			return !operator==(pData);
		}

		const char& operator[](const size_t index) const { return m_pBuffer[index]; }
		char& operator[](const size_t index) { return m_pBuffer[index]; }

		const char& At(const size_t index) const { assert(index < m_Size); return m_pBuffer[index]; }
		char& At(const size_t index) { assert(index < m_Size); return m_pBuffer[index]; }

		size_t GetHash() const
		{
			return FNV1aHash(m_pBuffer, m_Size);
		}
		struct Hash
		{
			size_t operator()(const String& other) const { return other.GetHash(); }
		};

		template<typename ...Args>
		static String Printf(const char* format, Args... args)
		{
			auto size = std::snprintf(nullptr, 0, format, args...);
			String output(size + 1, '\0');
			sprintf_s(&output[0], output.Size(), format, args...);
			return Move(output);
		}

		friend std::ostream& operator<<(std::ostream& os, const String& string)
		{
			if(string.m_pBuffer)
				os << string.Data();
			return os;
		}

		const char* Data() const { return m_pBuffer; }
		char* Data() { return m_pBuffer; }

		operator bool() const { return m_Size > 0; }
		inline bool Empty() const { return m_Size == 0; }
		inline size_t Size() const { return m_Size; }
		inline size_t Length() const { return m_pBuffer ? StrLen(m_pBuffer) : 0; }
		inline size_t Capacity() const { return m_Capacity; }

		inline char& Front() { assert(m_pBuffer); return *m_pBuffer; }
		inline const char& Front() const { assert(m_pBuffer); return *m_pBuffer; }
		inline char& Back() { assert(m_Size > 0); return *(m_pBuffer + m_Size - 1); }
		inline const char& Back() const { assert(m_Size > 0); return *(m_pBuffer + m_Size - 1); }
		
		inline Iterator begin() { return Iterator(m_pBuffer); }
		inline Iterator end() { return Iterator(m_pBuffer + m_Size); }
		inline ConstIterator begin() const { return ConstIterator(m_pBuffer); }
		inline ConstIterator end() const { return ConstIterator(m_pBuffer + m_Size); }

		inline Iterator Begin() { return Iterator(m_pBuffer); }
		inline Iterator End() { return Iterator(m_pBuffer + m_Size); }
		inline ConstIterator Begin() const { return ConstIterator(m_pBuffer); }
		inline ConstIterator End() const { return ConstIterator(m_pBuffer + m_Size); }
		
		constexpr static size_t MaxSize() { return Npos; }
		static constexpr size_t Npos = ~(size_t)0;

	private:
		inline size_t CalculateGrowth(const size_t oldSize)
		{
			size_t newSize = (size_t)floor(oldSize * 1.5);
			return newSize == m_Capacity ? newSize + 1 : newSize;
		}

		char* m_pBuffer;
		size_t m_Size;
		size_t m_Capacity;
	};

	template<>
	inline void Swap(String& a, String& b)
	{
		a.Swap(b);
	}
}