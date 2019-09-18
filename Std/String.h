#pragma once
#include <assert.h>
#include <cstdio>
#include "Iterator.h"
#include "Algorithm.h"
#include "Utility.h"
#include "Hash.h"
#include "Vector.h"

#ifdef UNICODE
#ifndef TEXT
#define TEXT(t) L t
#endif
#else
#ifndef TEXT
#define TEXT(t) t
#endif
#endif

namespace FluxStd
{
	template<typename CharType>
	class TString;

	using String = TString<char>;
	using WString = TString<wchar_t>;

	template<typename ChrType>
	class TString
	{
	public:
		using CharType = ChrType;
		using SizeType = size_t;
		using Iterator = RandomAccessIterator<CharType>;
		using ConstIterator = RandomAccessConstIterator<CharType>;

	public:
		//Empty string, no buffer
		TString() 
		{}

		//Create string from c-string
		TString(const CharType* pData)
			: m_Buffer(pData, StrLen(pData) + 1)
		{
		}

		//Create string from foreign c-string
		template<typename OtherCharType>
		TString(const OtherCharType* pData)
		{
			Append(pData);
		}

		//Create string from begin and end iterator
		explicit TString(const CharType* pBegin, const CharType* pEnd)
			: m_Buffer(pBegin, pEnd - pBegin + 1)
		{
		}

		//Move semantics
		TString(TString&& other) noexcept
			: m_Buffer(Move(other.m_Buffer))
		{
		}

		//Deep copy constructor
		TString(const TString& other)
			: m_Buffer(other.m_Buffer)
		{
		}

		//Create string from foreign string
		template<typename OtherCharType>
		explicit TString(const TString<OtherCharType>& other)
		{
			Append(other.Data());
		}

		//Deep copy assignment
		TString& operator=(const TString& other)
		{
			m_Buffer = other.m_Buffer;
			return *this;
		}

		//Deep copy assignment from foreign string
		template<typename OtherCharType>
		TString& operator=(const TString<OtherCharType>& other)
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
			
		}

		//Set the size to 0 but keep the buffer or resize to 0
		void Clear(const bool hard = true)
		{
			m_Buffer.Clear(hard);
		}

		//Create a buffer large enough to fit the given size
		void Reserve(const SizeType size)
		{
			m_Buffer.Reserve(size + 1);
		}

		//Resizes the buffer so that the characters just without
		void ShrinkToFit()
		{
			m_Buffer.ShrinkToFit();
		}

		CharType Pop()
		{
			assert(Length() > 0);
			CharType value = m_Buffer[m_Buffer.Size() - 2];
			m_Buffer.Pop();
			m_Buffer.Back() = TEXT('\0');
		}

		void Swap(TString& other)
		{
			m_Buffer.Swap(other.m_Buffer);
		}

		void EraseAt(const SizeType index)
		{
			assert(index < Length());
			m_Buffer.RemoveAt(index);
		}

		void Insert(const SizeType index, const CharType value)
		{
			assert(index <= Length());
			m_Buffer.Insert(index, value);
		}

		void Insert(const SizeType index, const CharType* pData)
		{
			SizeType len = StrLen(pData);
			assert(index <= Length());

			SizeType previousLen = Length();
			m_Buffer.Resize(len + Length() + 1);
			m_Buffer.MoveRange(index + len, index, previousLen - index);
			memcpy(m_Buffer.Data() + index, pData, len);
			m_Buffer.Back() = TEXT('\0');
		}

		inline void Append(const TString& other)
		{
			Append(other.Data());
		}

		void Append(const CharType value)
		{
			m_Buffer.Push(value);
			if (Length() > 0)
			{
				m_Buffer.Swap(m_Buffer.Size() - 1, m_Buffer.Size() - 2);
			}
			else
			{
				m_Buffer.Push(TEXT('\0'));
			}
		}

		template<typename OtherCharType>
		void Append(const OtherCharType value)
		{
			m_Buffer.Push((CharType)value);
			if (Length() > 0)
			{
				m_Buffer.Swap(m_Buffer.Size() - 1, m_Buffer.Size() - 2);
			}
			else
			{
				m_Buffer.Push(TEXT('\0'));
			}
		}

		void Append(const CharType* pData)
		{
			if (pData)
			{
				SizeType currentSize = Length();
				SizeType dataSize = StrLen(pData);
				m_Buffer.Resize(Length() + dataSize + 1);
				Strcpy(m_Buffer.Data() + currentSize, pData, dataSize);
			}
		}

		template<typename OtherCharType>
		void Append(const OtherCharType* pData)
		{
			if (pData)
			{
				SizeType currentSize = Length();
				SizeType dataSize = StrLen(pData);
				m_Buffer.Resize(Length() + dataSize + 1);
				for (SizeType i = 0; i < dataSize; ++i)
				{
					m_Buffer[currentSize + i] = (CharType)pData[i];
				}
				m_Buffer.Back() = TEXT('\0');
			}
		}

		TString Substring(const SizeType from, const SizeType length = TString::Npos) const
		{
			if (Length() == 0)
			{
				return TString();
			}
			if (length == TString::Npos)
			{
				assert(from <= Length());
				return TString(Data() + from, m_Buffer.Data() + Length());
			}
			else
			{
				assert(length + from <= Length());
				return TString(Data() + from, Data() + from + length);
			}
		}

		void ToUpperInline()
		{
			for (CharType& c : *this)
			{
				c = FluxStd::ToUpper(c);
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
			for (CharType& c : *this)
			{
				c = FluxStd::ToLower(c);
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
			if (Length() > 0)
			{
				from = from == TString::Npos ? 0 : from;
				for (SizeType i = from; i < Length(); ++i)
				{
					if (m_Buffer[i] == c)
					{
						return i;
					}
				}
			}
			return TString::Npos;
		}

		SizeType RFind(const CharType c, SizeType from = TString::Npos) const
		{
			if (Length() > 0)
			{
				from = from == TString::Npos ? Length() - 1 : from;
				for (SizeType i = from; i != TString::Npos; --i)
				{
					if (m_Buffer[i] == c)
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

		SizeType Find(const CharType* c, SizeType from = TString::Npos) const
		{
			SizeType len = StrLen(c);
			if (Length() == 0 || len == 0)
			{
				return TString::Npos;
			}

			from = from == TString::Npos ? 0 : from;
			for (SizeType i = from; i <= Length() - len; ++i)
			{
				if (m_Buffer[i] == c[0])
				{
					bool match = true;
					for (SizeType j = 1; j < len; ++j)
					{
						if (m_Buffer[i + j] != c[j])
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
			return TString::Npos;
		}

		SizeType RFind(const CharType* c) const
		{
			SizeType len = StrLen(c);
			if (Length() == 0 || len == 0)
			{
				return TString::Npos;
			}

			for (SizeType i = Length() - 1; i >= len - 1; --i)
			{
				if (m_Buffer[i] == c[len - 1])
				{
					bool match = true;
					for (int j = (int)len - 1; j >= 0; --j)
					{
						if (m_Buffer[i - len + j + 1] != c[j])
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
			if (StrLen(pStr) > Length())
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
			const SizeType appendLen = StrLen(pStr);
			if (appendLen > Length())
			{
				return false;
			}
			return Find(pStr, Length() - appendLen) != TString::Npos;
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
			SizeType length = Length();
			for (SizeType i = 0; i < length; i++)
			{
				for (SizeType j = 0; j < delimiterCount ; j++)
				{
					if (m_Buffer[i] == pDelimiters[j])
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

		template<typename Range>
		static TString Join(const Range& values, CharType* pSeparator)
		{
			TString output;
			bool first = true;
			for (const auto& element : values)
			{
				if (first)
				{
					first = false;
				}
				else
				{
					output.Append(pSeparator);
				}
				output.Append(element);
				
			}
			return output;
		}

		void ReplaceInline(const CharType toReplace, const CharType replaceWith)
		{
			for (CharType& c : *this)
			{
				if (c == toReplace)
				{
					c = replaceWith;
				}
			}
		}

		inline TString Replace(const CharType toReplace, const CharType replaceWith) const
		{
			TString copy = TString(*this);
			copy.ReplaceInline(toReplace, replaceWith);
			return copy;
		}

		void ReplaceInline(const CharType* pToReplace, const CharType* pReplaceWith)
		{
			SizeType findLen = StrLen(pToReplace);
			SizeType replaceLen = StrLen(pReplaceWith);

			SizeType idx = 0;
			for (;;)
			{
				idx = Find(pToReplace, idx);
				if (idx != Npos)
				{
					if (findLen == replaceLen)
					{
						memcpy(m_Buffer.Data() + idx, pReplaceWith, replaceLen);
					}
					else if (findLen > replaceLen)
					{
						m_Buffer.MoveRange(idx, idx + findLen - replaceLen, m_Buffer.Size() - idx);
						if (replaceLen > 0)
						{
							memcpy(m_Buffer.Data() + idx, pReplaceWith, replaceLen);
						}
					}
					else
					{
						m_Buffer.Resize(m_Buffer.Size() + replaceLen - findLen);
						m_Buffer.MoveRange(idx + replaceLen - findLen, idx, m_Buffer.Size() - (idx + replaceLen - findLen));
						memcpy(m_Buffer.Data() + idx, pReplaceWith, replaceLen);
					}
				}
				else
				{
					break;
				}
			}
		}

		inline TString Replace(const CharType* pToReplace, const CharType* pReplaceWith) const
		{
			TString copy = TString(*this);
			copy.ReplaceInline(pToReplace, pReplaceWith);
			return copy;
		}

		void ReverseInline()
		{
			SizeType halfSize = Length() / 2;
			for (SizeType i = 0; i < halfSize; ++i)
			{
				m_Buffer.Swap(i, Length() - i - 1);
			}
		}

		inline TString Reverse() const
		{
			TString copy = TString(*this);
			copy.ReverseInline();
			return copy;
		}

		bool IsNumeric() const
		{
			bool floatingPoint = false;
			for (SizeType i = 0; i < Length(); ++i)
			{
				CharType c = Data()[i];
				if (!FluxStd::IsDigit(c))
				{
					if (i == 0 && c == TEXT('-'))
					{
						continue;
					}
					if (floatingPoint == false && c == TEXT('.'))
					{
						floatingPoint = true;
						continue;
					}
					return false;
				}
			}
			return true;
		}

		static float ToFloat(const TString& string)
		{
			int value = 0;
			int sign = 1;
			int floatingPoint = 1;
			for (SizeType i = 0; i < string.Length(); ++i)
			{
				CharType c = string.Data()[string.Length() - 1 - i];
				if (i == string.Length() - 1 && c == TEXT('-'))
				{
					assert(sign == 1);
					sign = -1;
					continue;
				}
				if (c == TEXT('.'))
				{
					assert(floatingPoint == 1);
					floatingPoint = i;
					continue;
				}
				int pos = floatingPoint > 1 ? i - 1 : i;
				value += (int)(c - TEXT('0')) * (int)pow(10, pos);
			}
			return (float)value / (int)pow(10, floatingPoint) * sign;
		}

		static TString Format(const CharType* pFormat, const Vector<TString>& arguments)
		{
			TString string;
			SizeType len = StrLen(pFormat);
			string.m_Buffer.Reserve(len);
			for (SizeType i = 0; i < len; ++i)
			{
				CharType c = pFormat[i];
				if (c == '{')
				{
					int argIdx = pFormat[i + 1] - TEXT('0');
					const TString& arg = arguments[argIdx];
					for (CharType argC : arg)
					{
						if (argC == TEXT('\0'))
						{
							break;
						}
						string.m_Buffer.Push(argC);
					}
					i += 2;
				}
				else
				{
					string.m_Buffer.Push(c);
				}
			}
			string.m_Buffer.Push(TEXT('\0'));
			return string;
		}

		void RemoveFromEnd(const CharType* pText)
		{
			if (EndsWith(pText))
			{
				m_Buffer.Resize(m_Buffer.Size() - StrLen(pText));
				m_Buffer[Length()] = TEXT('\0');
			}
		}

		inline void RemoveFromEnd(const TString& other)
		{
			RemoveFromEnd(other.Data());
		}

		void RemoveFromStart(const CharType* pText)
		{
			if (StartsWith(pText))
			{
				SizeType len = StrLen(pText);
				m_Buffer.MoveRange(0, len, m_Buffer.Size() - StrLen(pText));
				m_Buffer.Resize(m_Buffer.Size() - len);
			}
		}

		inline void RemoveFromStart(const TString& other)
		{
			RemoveFromStart(other.Data());
		}

		static void Strcpy(CharType* pDestination, const CharType* pSource, SizeType length)
		{
			while (*pSource != TEXT('\0') && length > 0)
			{
				*pDestination = *pSource;
				++pDestination;
				++pSource;
				--length;
			}
			*pDestination = TEXT('\0');
		}

		static SizeType StrLen(const CharType* pData)
		{
			const CharType* pCurrent = pData;
			for (; *pCurrent; ++pCurrent)
			{
			}
			return pCurrent - pData;
		}

		template<typename OtherCharType>
		static SizeType StrLen(const OtherCharType* pData)
		{
			const OtherCharType* pCurrent = pData;
			for (; *pCurrent; ++pCurrent)
			{
			}
			return pCurrent - pData;
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
			if (len != other.Length())
			{
				return false;
			}
			for (SizeType i = 0; i < len; ++i)
			{
				if (m_Buffer[i] != other.m_Buffer[i])
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
				if (m_Buffer[i] != pData[i])
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

		const CharType& operator[](const SizeType index) const { return m_Buffer[index]; }
		CharType& operator[](const SizeType index) { return m_Buffer[index]; }

		const CharType& At(const SizeType index) const { assert(index < Length()); return m_Buffer[index]; }
		CharType& At(const SizeType index) { assert(index < Length()); return m_Buffer[index]; }

		SizeType GetHash() const
		{
			return FNV1aHash(Data(), Length());
		}

		template<typename ...Args>
		static TString Printf(const CharType* format, Args... args)
		{
			auto size = std::snprintf(nullptr, 0, format, args...);
			TString output;
			if (size > 0)
			{
				output.m_Buffer.Resize(size + 1);
				sprintf_s(&output[0], output.Length() + 1, format, args...);
			}
			return output;
		}

		friend std::ostream& operator<<(std::ostream& os, const TString& string)
		{
			os << string.Data();
			return os;
		}

		const CharType* Data() const { return m_Buffer.Size() > 0 ? m_Buffer.Data() : GetEmpty<CharType>(); }

		operator bool() const { return !Empty(); }
		inline bool Empty() const { return m_Buffer.Size() <= 1; }
		inline SizeType Length() const { return m_Buffer.Size() > 0 ? m_Buffer.Size() - 1 : 0; }

		inline CharType& Front() { return m_Buffer.Front(); }
		inline const CharType& Front() const { return m_Buffer.Front(); }
		inline CharType& Back() { return m_Buffer.Back(); }
		inline const CharType& Back() const { return m_Buffer.Back(); }
		
		inline Iterator begin() { return m_Buffer.begin(); }
		inline Iterator end() { return m_Buffer.Empty() ? m_Buffer.End() : m_Buffer.End() + 1; }
		inline ConstIterator begin() const { return m_Buffer.begin(); }
		inline ConstIterator end() const { return m_Buffer.Empty() ? m_Buffer.End() : m_Buffer.End() + 1; }

		inline Iterator Begin() { return m_Buffer.Begin(); }
		inline Iterator End() { return m_Buffer.Empty() ? m_Buffer.End() : m_Buffer.End() + 1; }
		inline ConstIterator Begin() const { return m_Buffer.Begin(); }
		inline ConstIterator End() const { return m_Buffer.Empty() ? m_Buffer.End() : m_Buffer.End() + 1; }
		
		static constexpr SizeType Npos = ~(SizeType)0;

	private:
		Vector<CharType> m_Buffer;

		template<typename T>
		static constexpr const T* GetEmpty() { return ""; }
		template<>
		static constexpr const wchar_t* GetEmpty() { return L""; }
	};

	template<typename CharType, typename SizeType>
	inline void Swap(TString<CharType>& a, TString<CharType>& b)
	{
		a.Swap(b);
	}
}