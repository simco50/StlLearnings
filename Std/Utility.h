#pragma once

namespace FluxStd
{
	template<typename T>
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

	template<class T>
	struct RemoveReference
	{
		// remove reference
		typedef T Type;
	};

	template<class T>
	struct RemoveReference<T&>
	{
		// remove reference
		typedef T Type;
	};

	template<class T>
	struct RemoveReference<T&&>
	{
		// remove rvalue reference
		typedef T Type;
	};

	template<class T>
	inline typename RemoveReference<T>::Type&& Move(T&& arg)
	{
		// forward arg as movable
		return static_cast<typename RemoveReference<T>::Type&&>(arg);
	}

	template <class T>
	inline T&& Forward(typename RemoveReference<T>::Type& t)
	{
		return static_cast<T&&>(t);
	}

	template <class T>
	inline T&& Forward(typename RemoveReference<T>::Type&& t)
	{
		return static_cast<T&&>(t);
	}

	template<typename T>
	struct LessThan
	{
		constexpr bool operator()(const T& a, const T& b) const { return a < b; }
	};
	template<typename T>
	struct GreaterThan
	{
		constexpr bool operator()(const T& a, const T& b) const { return a > b; }
	};
	template<typename T>
	struct EqualTo
	{
		constexpr bool operator()(const T& a, const T& b) const { return a == b; }
	};

	template< class T> 
	struct AddConst
	{ 
		typedef const T Type; 
	};
	template< class T >
	using AddConstT = typename AddConst<T>::Type;

	template <class T>
	constexpr AddConstT<T>& AsConst(T& value) noexcept
	{
		return value;
	}

	inline constexpr char ToLower(const char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + 32 : c;
	}

	inline constexpr char ToUpper(const char c)
	{
		return (c >= 'a' && c <= 'z') ? c - 32 : c;
	}

	inline constexpr bool IsLetter(const char c)
	{
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
	}

	inline constexpr bool IsDigit(const char c)
	{
		return (c >= '0' && c <= '9');
	}
}