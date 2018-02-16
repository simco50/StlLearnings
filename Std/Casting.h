#pragma once

namespace FluxStd
{
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
}