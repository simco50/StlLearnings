#pragma once
#include <assert.h>
#include <iostream>
#include "Iterator.h"
#include "Algorithm.h"
#include "Utility.h"
#include "Hash.h"
#include "Vector.h"

#define TXT(t) t

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

	template<typename CharType, typename OtherCharType, typename SizeType>
	class TString;

	using String = TString<char, wchar_t, size_t>;
	using WString = TString<wchar_t, char, size_t>;

	template<typename CharType, typename OtherCharType, typename SizeType>
	class TString
	{
	public:
		using Iterator = RandomAccessIterator<char>;
		using ConstIterator = RandomAccessConstIterator<char>;

	public:
		//Empty string, no buffer
		TString() 
			: m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{}

		//Create string filled with defined value
		TString(const SizeType size, const CharType value = TXT('\0'))
			: m_pBuffer(new CharType[size + 1]), m_Size(size), m_Capacity(size)
		{
			memset(m_pBuffer, value, m_Size);
			m_pBuffer[size] = TXT('\0');
		}

		//Create string from c-string
		TString(const CharType* pData)
			: m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(pData);
		}

		//Create string from begin and end iterator
		explicit TString(const CharType* pBegin, const CharType* pEnd)
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
				m_pBuffer = new CharType[m_Size + 1];
				memcpy(m_pBuffer, pBegin, m_Size * sizeof(CharType));
				m_pBuffer[m_Size] = TXT('\0');
			}
		}

		//Move semantics
		TString(TString&& other) noexcept
			: m_pBuffer(other.m_pBuffer), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_pBuffer = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		//Deep copy constructor
		explicit TString(const TString& other)
			: m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(other.Data());
		}

		//Create string from foreign string
		explicit TString(const TString<OtherCharType, CharType, SizeType>& other)
			: m_pBuffer(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(other.Data());
		}

		//Deep copy assignment
		TString& operator=(const TString& other)
		{
			if (other.m_pBuffer != m_pBuffer)
			{
				Clear();
				Append(other.Data());
			}
			return *this;
		}

		//Deep copy assignment from foreign string
		TString& operator=(const TString<OtherCharType, CharType, SizeType>& other)
		{
			Clear();
			Append(other.Data());
			return *this;
		}

		//Move assignment
		TString& operator=(TString&& other) noexcept
		{
			Swap(other);
			return *this;
		}

		~TString()
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
			{
				m_Size = 0;
			}
			else
			{
				Resize(0);
			}
		}

		//Hard resize the buffer
		void Resize(const SizeType size)
		{
			SizeType copyWidth = 0;
			if (m_pBuffer)
			{
				CharType* pNewBuffer = new CharType[size + 1];
				copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, copyWidth * sizeof(CharType));
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new CharType[size + 1];
			}
			memset(m_pBuffer + copyWidth, 0, (size - copyWidth) * sizeof(CharType));
			m_Size = size;
			m_Capacity = size;
			m_pBuffer[m_Size] = TXT('\0');
		}

		//Create a buffer large enough to fit the given size
		void Reserve(const SizeType size)
		{
			if (size <= m_Capacity)
			{
				return;
			}

			if (m_pBuffer != nullptr)
			{
				CharType* pNewBuffer = new CharType[size + 1];
				SizeType copyWidth = size > m_Size ? m_Size: size;
				memcpy(pNewBuffer, m_pBuffer, (copyWidth + 1) * sizeof(CharType));
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new CharType[size + 1];
			}
			m_Capacity = size;
		}

		//Resizes the buffer so that the characters just without
		void ShrinkToFit()
		{
			Resize(m_Size);
		}

		CharType Pop()
		{
			assert(m_Size > 0);
			CharType value = Back();
			--m_Size;
			m_pBuffer[m_Size] = TXT('\0');
			return value;
		}

		void Swap(TString& other)
		{
			FluxStd::Swap(m_pBuffer, other.m_pBuffer);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_Capacity, other.m_Capacity);
		}

		void Assign(const SizeType amount, const CharType value)
		{
			if (m_Size + amount > m_Capacity)
			{
				Reserve(m_Size + amount);
			}
			for (SizeType i = 0; i < amount; ++i)
			{
				memcpy(m_pBuffer + m_Size + i, &value, sizeof(CharType));
			}
			m_Size += amount;
			m_pBuffer[m_Size] = TXT('\0');
		}

		void EraseAt(const SizeType index)
		{
			assert(index < m_Size);
			for (SizeType i = index; i < m_Size - 1; ++i)
			{
				m_pBuffer[i] = m_pBuffer[i + 1];
			}
			--m_Size;
			m_pBuffer[m_Size] = TXT('\0');
		}

		void Insert(const SizeType index, const CharType value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
			{
				Reserve(m_Size + 1);
			}

			for (SizeType i = m_Size; i > index; --i)
			{
				m_pBuffer[i] = m_pBuffer[i - 1];
			}
			m_pBuffer[index] = value;
			++m_Size;
			m_pBuffer[m_Size] = TXT('\0');
		}

		void Insert(const SizeType index, const CharType* pData)
		{
			SizeType len = StrLen(pData);
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
			{
				Reserve(m_Size + len);
			}

			for (SizeType i = m_Size + len; i > index; --i)
			{
				m_pBuffer[i] = m_pBuffer[i - len];
			}
			memcpy(m_pBuffer + index, pData, len * sizeof(CharType));
			m_Size += len;
		}

		void Append(const CharType value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			*(m_pBuffer + m_Size) = value;
			++m_Size;
			m_pBuffer[m_Size] = TXT('\0');
		}

		void Append(const OtherCharType value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(CalculateGrowth(m_Size));
			}
			*(m_pBuffer + m_Size) = (CharType)value;
			++m_Size;
			m_pBuffer[m_Size] = TXT('\0');
		}

		void Append(const CharType* pData)
		{
			if (pData)
			{
				const SizeType dataSize = StrLen(pData);			
				CharType* pNewBuffer = new CharType[m_Size + dataSize + 1];
				memcpy(pNewBuffer + m_Size, pData, dataSize * sizeof(CharType));
				if (m_pBuffer)
				{
					memcpy(pNewBuffer, m_pBuffer, m_Size * sizeof(CharType));
					delete[] m_pBuffer;
				}
				m_Size += dataSize;
				m_Capacity = m_Size;
				m_pBuffer = pNewBuffer;
				m_pBuffer[m_Size] = TXT('\0');
			}
		}

		void Append(const OtherCharType* pData)
		{
			if (pData)
			{
				const SizeType dataSize = StrLen(pData);
				CharType* pNewBuffer = new CharType[m_Size + dataSize + 1];

				for (SizeType i = 0; i < dataSize; ++i)
				{
					pNewBuffer[m_Size + i] = (CharType)pData[i];
				}

				if (m_pBuffer)
				{
					memcpy(pNewBuffer, m_pBuffer, m_Size * sizeof(CharType));
					delete[] m_pBuffer;
				}
				m_Size += dataSize;
				m_Capacity = m_Size;
				m_pBuffer = pNewBuffer;
				m_pBuffer[m_Size] = TXT('\0');
			}
		}

		TString Substring(const SizeType from, const SizeType length = TString::Npos) const
		{
			if (length == TString::Npos)
			{
				assert(from <= m_Size);
				return TString(m_pBuffer + from, m_pBuffer + m_Size);
			}
			else
			{
				assert(length + from <= m_Size);
				return TString(m_pBuffer + from, m_pBuffer + from + length);
			}
		}

		void ToUpperInline()
		{
			if (m_pBuffer)
			{
				for (CharType& c : *this)
				{
					c = FluxStd::ToUpper(c);
				}
			}
		}

		TString ToUpper()
		{
			TString str = TString(Data());
			str.ToUpperInline();
			return str;
		}

		void ToLowerInline()
		{
			if (m_pBuffer)
			{
				for (CharType& c : *this)
				{
					c = FluxStd::ToLower(c);
				}
			}
		}

		TString ToLower()
		{
			TString str = TString(Data());
			str.ToLowerInline();
			return str;
		}

		SizeType Find(const CharType c, SizeType from = TString::Npos) const
		{
			if (m_Size > 0)
			{
				from = from == TString::Npos ? 0 : from;
				for (SizeType i = from; i < m_Size; ++i)
				{
					if (m_pBuffer[i] == c)
					{
						return i;
					}
				}
			}
			return TString::Npos;
		}

		SizeType RFind(const CharType c, SizeType from = TString::Npos) const
		{
			if (m_Size > 0)
			{
				from = from == TString::Npos ? m_Size - 1 : from;
				for (SizeType i = from; i != TString::Npos; --i)
				{
					if (m_pBuffer[i] == c)
					{
						return i;
					}
				}
			}
			return TString::Npos;
		}

		SizeType Find(const TString& str) const
		{
			return Find(str.Data());
		}

		SizeType RFind(const TString& str) const
		{
			return RFind(str.Data());
		}

		SizeType Find(const CharType* c, SizeType from = String::Npos) const
		{
			SizeType len = StrLen(c);
			if (m_Size == 0 || len == 0)
			{
				return String::Npos;
			}

			from = from == TString::Npos ? 0 : from;
			for (SizeType i = from; i <= m_Size - len; ++i)
			{
				if (m_pBuffer[i] == c[0])
				{
					bool match = true;
					for (SizeType j = 1; j < len; ++j)
					{
						if (m_pBuffer[i + j] != c[j])
						{
							match = false;
							break;
						}
					}
					if (match)
					{
						return i;
					}
				}
			}
			return String::Npos;
		}

		SizeType RFind(const CharType* c) const
		{
			SizeType len = StrLen(c);
			if (m_Size == 0 || len == 0)
			{
				return String::Npos;
			}

			for (SizeType i = m_Size - 1; i >= len - 1; --i)
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
					{
						return i - len + 1;
					}
				}
			}
			return TString::Npos;
		}

		bool StartsWith(const CharType* pStr) const
		{
			const SizeType len = StrLen(pStr);
			if (len > m_Size)
			{
				return false;
			}
			return Find(pStr) == 0;
		}

		bool StartsWith(const TString& other) const
		{
			return StartsWith(other.Data());
		}

		bool EndsWith(const CharType* pStr) const
		{
			const SizeType len = StrLen(pStr);
			if (len > m_Size)
			{
				return false;
			}
			return Find(pStr, m_Size - len) != String::Npos;
		}

		bool EndsWith(const TString& other) const
		{
			return EndsWith(other.Data());
		}

		Vector<TString> Split(const CharType delimiter) const
		{
			return Split(&delimiter, 1);
		}

		Vector<TString> Split(const CharType* pDelimiters, const SizeType delimiterCount) const
		{
			Vector<TString> out;
			SizeType start = 0;
			for (SizeType i = 0; i < m_Size; i++)
			{
				for (SizeType j = 0; j < delimiterCount ; j++)
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

		void Replace(const CharType toReplace, const CharType replaceWith)
		{
			for (CharType& c : *this)
			{
				if (c == toReplace)
				{
					c = replaceWith;
				}
			}
		}

		TString& operator+(const TString& other)
		{
			Append(other.Data());
			return *this;
		}

		TString& operator+(const CharType* pData)
		{
			Append(pData);
			return *this;
		}

		TString& operator+(const CharType c)
		{
			Append(c);
			return *this;
		}

		TString& operator+=(const TString& other)
		{
			Append(other.Data());
			return *this;
		}

		TString& operator+=(const CharType* pData)
		{
			Append(pData);
			return *this;
		}

		TString& operator+=(const CharType c)
		{
			Append(c);
			return *this;
		}

		bool operator==(const TString& other) const
		{
			SizeType len = Length();
			if (len != other.m_Size)
			{
				return false;
			}
			for (SizeType i = 0; i < len; ++i)
			{
				if (m_pBuffer[i] != other.m_pBuffer[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator!=(const TString& other) const
		{
			return !operator==(other);
		}

		bool operator==(const CharType* pData) const
		{
			SizeType len = Length();
			if (len != StrLen(pData))
			{
				return false;
			}
			for (SizeType i = 0; i < len; ++i)
			{
				if (m_pBuffer[i] != pData[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator!=(const CharType* pData) const
		{
			return !operator==(pData);
		}

		const CharType& operator[](const SizeType index) const { return m_pBuffer[index]; }
		CharType& operator[](const SizeType index) { return m_pBuffer[index]; }

		const CharType& At(const SizeType index) const { assert(index < m_Size); return m_pBuffer[index]; }
		CharType& At(const SizeType index) { assert(index < m_Size); return m_pBuffer[index]; }

		SizeType GetHash() const
		{
			return FNV1aHash(m_pBuffer, m_Size);
		}

		template<typename ...Args>
		static TString Printf(const CharType* format, Args... args)
		{
			auto size = std::snprintf(nullptr, 0, format, args...);
			TString output(size + 1, '\0');
			sprintf_s(&output[0], output.Size(), format, args...);
			return output;
		}

		friend std::ostream& operator<<(std::ostream& os, const TString& string)
		{
			if (string.m_pBuffer)
			{
				os << string.Data();
			}
			return os;
		}

		const CharType* Data() const { return m_pBuffer; }
		CharType* Data() { return m_pBuffer; }

		operator bool() const { return m_Size > 0; }
		inline bool Empty() const { return m_Size == 0; }
		inline SizeType Size() const { return m_Size; }
		inline SizeType Length() const { return m_pBuffer ? Min(m_Size, StrLen(m_pBuffer)) : 0; }
		inline SizeType Capacity() const { return m_Capacity; }

		inline CharType& Front() { assert(m_pBuffer); return *m_pBuffer; }
		inline const CharType& Front() const { assert(m_pBuffer); return *m_pBuffer; }
		inline CharType& Back() { assert(m_Size > 0); return *(m_pBuffer + m_Size - 1); }
		inline const CharType& Back() const { assert(m_Size > 0); return *(m_pBuffer + m_Size - 1); }
		
		inline Iterator begin() { return Iterator(m_pBuffer); }
		inline Iterator end() { return Iterator(m_pBuffer + m_Size); }
		inline ConstIterator begin() const { return ConstIterator(m_pBuffer); }
		inline ConstIterator end() const { return ConstIterator(m_pBuffer + m_Size); }

		inline Iterator Begin() { return Iterator(m_pBuffer); }
		inline Iterator End() { return Iterator(m_pBuffer + m_Size); }
		inline ConstIterator Begin() const { return ConstIterator(m_pBuffer); }
		inline ConstIterator End() const { return ConstIterator(m_pBuffer + m_Size); }
		
		constexpr static SizeType MaxSize() { return Npos; }
		static constexpr SizeType Npos = ~(SizeType)0;

	private:
		inline SizeType CalculateGrowth(const SizeType oldSize)
		{
			SizeType newSize = (SizeType)floor(oldSize * 1.5);
			return newSize == m_Capacity ? newSize + 1 : newSize;
		}

		CharType* m_pBuffer;
		SizeType m_Size;
		SizeType m_Capacity;
	};

	template<typename CharType, typename OtherCharType, typename SizeType>
	inline void Swap(TString<CharType, OtherCharType, SizeType>& a, TString<CharType, OtherCharType, SizeType>& b)
	{
		a.Swap(b);
	}
}