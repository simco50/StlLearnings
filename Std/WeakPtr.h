#pragma once
#include "SharedPtr.h"

namespace FluxStd
{
	template<typename T>
	class WeakPtr
	{
	public:
		WeakPtr() :
			m_pPtr(nullptr), m_pRefCount(nullptr)
		{}

		WeakPtr(SharedPtr<T>& other)
		{
			m_pRefCount = other.m_pRefCount;
			AddRef();
			m_pPtr = other.m_pPtr;
		}

		WeakPtr& operator=(SharedPtr<T>& other)
		{
			m_pRefCount = other.m_pRefCount;
			m_pPtr = other.m_pPtr;
			AddRef();
			return *this;
		}

		bool operator<(const WeakPtr<T>& other) const { return m_pPtr < other.m_pPtr; }
		bool operator==(const WeakPtr<T>& other) const { return m_pPtr == other.m_pPtr; }
		bool operator!=(const WeakPtr<T>& other) const { return m_pPtr != other.m_pPtr; }
		operator bool() const { return m_pPtr != nullptr && m_pRefCount->HardRefs > 0; }

		size_t Hash() const { return (size_t)m_pPtr / sizeof(T); }

		~WeakPtr()
		{
			Release();
		}

		SharedPtr<T> Pin()
		{
			if (IsValid())
			{
				return SharedPtr<T>(m_pPtr, m_pRefCount);
			}
			return SharedPtr<int>(nullptr);
		}

		void Release()
		{
			if (m_pRefCount == nullptr)
				return;
			RemoveRef();
			if (m_pRefCount->WeakRefs == 0)
			{
				delete m_pRefCount;
				m_pRefCount = nullptr;
			}
			m_pPtr = nullptr;
		}

		bool IsValid()
		{
			return m_pPtr != nullptr && m_pRefCount->HardRefs > 0;
		}

		int GetRefCount() const
		{
			return m_pRefCount ? m_pRefCount->HardRefs : 0;
		}

		int GetWeakRefCount() const
		{
			return m_pRefCount ? m_pRefCount->WeakRefs : 0;
		}

	private:
		void AddRef()
		{
			if (m_pRefCount)
				m_pRefCount->WeakRefs++;
		}
		void RemoveRef()
		{
			if (m_pRefCount)
				m_pRefCount->WeakRefs--;
		}

		RefCount * m_pRefCount;
		T* m_pPtr = nullptr;
	};
}