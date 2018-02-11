#pragma once
#include <assert.h>
#include <string>
#include <iostream>

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
		String() :
			m_pBuffer(nullptr), m_pCurrent(nullptr), m_Size(0), m_Capacity(0)
		{}

		String(const size_t size, const char value = '\0') :
			m_pBuffer(new char[size + 1]), m_pCurrent(m_pBuffer + m_Size), m_Size(size), m_Capacity(size)
		{
			memset(m_pBuffer, value, m_Size);
			*m_pCurrent = '\0';
		}

		String(const char* pData) :
			m_pBuffer(nullptr), m_pCurrent(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(pData);
		}

		explicit String(const wchar_t* pData) :
			m_pBuffer(nullptr), m_pCurrent(nullptr), m_Size(0)
		{
			Append(pData);
		}

		//Move semantics
		String(String&& other) :
			m_pBuffer(other.m_pBuffer), m_pCurrent(other.m_pCurrent), m_Size(other.m_Size), m_Capacity(other.m_Capacity)
		{
			other.m_pBuffer = nullptr;
			other.m_pCurrent = nullptr;
			other.m_Size = 0;
			other.m_Capacity = 0;
		}

		//Deep copy
		String(const String& other) :
			m_pBuffer(nullptr), m_pCurrent(nullptr), m_Size(0), m_Capacity(0)
		{
			Append(other.Data());
		}

		String& operator=(const String& other)
		{
			m_pBuffer = nullptr;
			m_pCurrent = nullptr;
			m_Size = 0;
			m_Capacity = 0;
			Append(other.Data());
		}

		explicit String(const char* pBegin, const char* pEnd)
		{
			assert(pBegin < pEnd);
			m_Size = pEnd - pBegin;
			m_Capacity = m_Size;
			m_pBuffer = new char[m_Size + 1];
			memcpy(m_pBuffer, pBegin, m_Size);
			m_pBuffer[m_Size] = '\0';
		}

		~String()
		{
			if (m_pBuffer)
			{
				delete[] m_pBuffer;
				m_pBuffer = nullptr;
			}
		}

		void Append(const char* pData)
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
			m_pCurrent = m_pBuffer + m_Size;
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
			m_pCurrent = m_pBuffer + m_Size;
		}

		void Clear()
		{
			m_pCurrent = m_pBuffer;
			m_Size = 0;
		}

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
			m_pCurrent = m_pBuffer + size;
			m_pBuffer[m_Size] = '\0';
		}

		void Reserve(const size_t size)
		{
			if (size <= m_Capacity)
				return;

			if (m_pBuffer != nullptr)
			{
				char* pNewBuffer = new char[size + 1];
				size_t copyWidth = size > m_Size ? m_Size : size;
				memcpy(pNewBuffer, m_pBuffer, copyWidth);
				delete[] m_pBuffer;
				m_pBuffer = pNewBuffer;
			}
			else
			{
				m_pBuffer = new char[size + 1];
			}
			m_Capacity = size;
			m_pCurrent = m_pBuffer + m_Size;
			m_pBuffer[m_Size] = '\0';
		}

		void Push(const char value)
		{
			if (m_Size >= m_Capacity)
			{
				Reserve(m_Size + 4);
			}
			*m_pCurrent = std::move(value);
			++m_Size;
			++m_pCurrent;
			m_pBuffer[m_Size] = '\0';
		}

		char Pop()
		{
			assert(m_Size > 0);
			char* pValue = --m_pCurrent;
			--m_Size;
			m_pBuffer[m_Size] = '\0';
			return *pValue;
		}

		void Swap(String& other)
		{
			char* pTempBuffer = m_pBuffer;
			m_pBuffer = other.m_pBuffer;
			other.m_pBuffer = pTempBuffer;

			pTempBuffer = m_pCurrent;
			m_pCurrent = other.m_pCurrent;
			other.m_pCurrent = pTempBuffer;

			size_t tempSize = m_Size;
			m_Size = other.m_Size;
			other.m_Size = tempSize;

			tempSize = m_Capacity;
			m_Capacity = other.m_Capacity;
			other.m_Capacity = tempSize;
		}

		void Assign(const size_t amount, const char value)
		{
			if (m_Size + amount > m_Capacity)
				Reserve(m_Size + amount);
			for (size_t i = 0; i < amount; ++i, ++m_pCurrent)
			{
				memcpy(m_pCurrent, &value, sizeof(char));
			}
			m_Size += amount;
			m_pBuffer[m_Size] = '\0';
		}

		void EraseAt(const size_t index)
		{
			assert(index < m_Size);
			for (size_t i = index; i < m_Size - 1; ++i)
				m_pBuffer[i] = std::move(m_pBuffer[i + 1]);
			--m_Size;
			--m_pCurrent;
			m_pBuffer[m_Size] = '\0';
		}

		void Insert(const size_t index, const char value)
		{
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
				Reserve(m_Size + 1);

			for (size_t i = m_Size; i > index; --i)
				m_pBuffer[i] = std::move(m_pBuffer[i - 1]);
			m_pBuffer[index] = value;
			++m_Size;
			++m_pCurrent;
			m_pBuffer[m_Size] = '\0';
		}

		void Insert(const size_t index, const char* pData)
		{
			size_t len = StrLen(pData);
			assert(index <= m_Size);
			if (m_Size == m_Capacity)
				Reserve(m_Size + len);

			for (size_t i = m_Size + len; i > index; --i)
				m_pBuffer[i] = std::move(m_pBuffer[i - len]);
			memcpy(m_pBuffer + index, pData, len);
			m_Size += len;
			m_pCurrent += len;
		}

		String Substring(const unsigned int from, const int length = String::Npos)
		{
			if (length == -1)
			{
				assert(from < m_Size);
				return String(m_pBuffer + from, m_pCurrent);
			}
			else
			{
				assert(length - from < m_Size);
				return String(m_pBuffer + from, m_pBuffer + from + length);
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

		bool operator==(const String& other)
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

		bool operator!=(const String& other)
		{
			return !operator==(other);
		}

		bool operator==(const char* pData)
		{
			if (m_Size != StrLen(pData))
				return false;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_pBuffer[i] != pData[i])
					return false;
			}
			return true;
		}

		bool operator==(const wchar_t* pData)
		{
			if (m_Size != StrLen(pData))
				return false;
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_pBuffer[i] != (wchar_t)pData[i])
					return false;
			}
			return true;
		}

		bool operator!=(const char* pData)
		{
			return !operator==(pData);
		}

		const char& operator[](const size_t index) const { return m_pBuffer[index]; }
		char& operator[](const size_t index) { return m_pBuffer[index]; }

		const char& At(const size_t index) const { assert(index < m_Size); m_pBuffer[index]; }
		char& At(const size_t index) { assert(index < m_Size); m_pBuffer[index]; }

		int Find(const char c)
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				if (m_pBuffer[i] == c)
					return (int)i;
			}
			return String::Npos;
		}

		int RFind(const char c)
		{
			for (size_t i = m_Size - 1; i >= 0; --i)
			{
				if (m_pBuffer[i] == c)
					return (int)i;
			}
			return String::Npos;
		}

		int Find(const String& str)
		{
			return Find(str.Data());
		}

		int Find(const char* c)
		{
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

		int RFind(const char* c)
		{
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
						return (int)(i - len + 1);
				}
			}
			return String::Npos;
		}

		int RFind(const String& str)
		{
			return RFind(str.Data());
		}

		void ShrinkToFit()
		{
			Resize(m_Size);
		}

		template<typename ...Args>
		static String Printf(const char* format, Args... args)
		{
			auto size = std::snprintf(nullptr, 0, format, args...);
			String output(size + 1, '\0');
			sprintf_s(&output[0], output.Size(), format, args...);
			return std::move(output);
		}

		operator bool() const { return m_Size > 0; }
		bool Empty() const { return m_Size > 0; }
		size_t Size() const { return m_Size; }

		const char* CStr() const { return m_pBuffer; }
		const char* Data() const { return m_pBuffer; }
		char* Data() { return m_pBuffer; }

		char& Front() { assert(m_pBuffer); return *m_pBuffer; }
		const char& Front() const { assert(m_pBuffer); return *m_pBuffer; }
		char& Back() { assert(m_pCurrent); return *(m_pCurrent - 1); }
		const char& Back() const { assert(m_pCurrent); return *(m_pCurrent - 1); }
		const char* begin() const { return m_pBuffer; }
		const char* end() const { return m_pCurrent; }

		constexpr size_t MaxSize() const { size_t nr = 0; nr = ~nr; return nr; }

		friend std::ostream& operator<<(std::ostream& os, const String& string)
		{
			os << string.Data();
			return os;
		}

		static const int Npos = -1;
	private:
		char* m_pBuffer;
		char* m_pCurrent;
		size_t m_Size;
		size_t m_Capacity;
	};
}