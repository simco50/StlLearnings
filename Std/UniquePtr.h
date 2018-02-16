#pragma once
#include "Casting.h"

namespace FluxStd
{
	template<typename T>
	class UniquePtr
	{
	public:
		UniquePtr() : m_pPtr(nullptr) {}
		explicit UniquePtr(T* pPtr) : m_pPtr(pPtr) {}

		UniquePtr(const UniquePtr<T>& other) = delete;
		UniquePtr& operator=(UniquePtr<T>& other) = delete;

		UniquePtr(UniquePtr<T>&& other) : m_pPtr(other.Detach()) {}

		UniquePtr& operator=(UniquePtr<T>&& other)
		{
			Release();
			m_pPtr = other.Detach();
			return *this;
		}

		~UniquePtr()
		{
			Release();
		}

		T* operator->() const { return m_pPtr; }
		T& operator*() const { return *m_pPtr; }

		bool operator<(const UniquePtr<T>& other) const {
			return m_pPtr < other.m_pPtr;
		}
		bool operator==(const UniquePtr<T>& other) const { return m_pPtr == other.m_pPtr; }
		bool operator!=(const UniquePtr<T>& other) const { return m_pPtr != other.m_pPtr; }
		operator bool() const { return m_pPtr != nullptr; }

		size_t Hash() const { return (size_t)m_pPtr / sizeof(T); }

		T* Get() const { return m_pPtr; }

		void Swap(UniquePtr<T>& other)
		{
			T* pTemp = m_pPtr;
			m_pPtr = other.m_pPtr;
			other.m_pPtr = pTemp;
		}

		T* Detach()
		{
			T* pPtr = m_pPtr;
			m_pPtr = nullptr;
			return pPtr;
		}

		void Release()
		{
			if (m_pPtr)
			{
				delete m_pPtr;
				m_pPtr = nullptr;
			}
		}

		bool IsValid() const { return m_pPtr != nullptr; }

	private:
		T* m_pPtr;
	};

	template<typename T, typename ...Args>
	UniquePtr<T> MakeUnique(Args &&... args)
	{
		return UniquePtr<T>(new T(Forward<Args>(args)...));
	}
}