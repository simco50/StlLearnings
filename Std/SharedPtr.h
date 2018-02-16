#pragma once
#include "Casting.h"

namespace FluxStd
{
	struct RefCount
	{
		RefCount(int hardRefs, int weakRefs) : HardRefs(hardRefs), WeakRefs(weakRefs)
		{}
		int HardRefs = 0;
		int WeakRefs = 0;
	};

	template<typename T>
	class SharedPtr
	{
	public:
		SharedPtr() :
			m_pPtr(nullptr),
			m_pRefCount(new RefCount(1, 1))
		{
		}

		explicit SharedPtr(T* pPtr) :
			m_pPtr(pPtr),
			m_pRefCount(new RefCount(1, 1))
		{
		}

		~SharedPtr()
		{
			Release();
		}

		SharedPtr(const SharedPtr<T>& other)
		{
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			AddRef();
		}

		SharedPtr& operator=(const SharedPtr<T>& other)
		{
			Release();
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			AddRef();
			return *this;
		}

		SharedPtr& operator=(T* pOther)
		{
			Release();
			m_pPtr = pOther;
			m_pRefCount = new RefCount(1, 1);
			return *this;
		}

		T* operator->() const { return m_pPtr; }
		T& operator*() const { return *m_pPtr; }

		T* Get() const { return m_pPtr; }

		bool operator<(const SharedPtr<T>& other) const { return m_pPtr < other.m_pPtr; }
		bool operator==(const SharedPtr<T>& other) const { return m_pPtr == other.m_pPtr; }
		bool operator!=(const SharedPtr<T>& other) const { return m_pPtr != other.m_pPtr; }
		operator bool() const { return m_pPtr != nullptr; }

		size_t Hash() const { return (size_t)m_pPtr / sizeof(T); }

		bool IsValid() const { return m_pPtr != nullptr; }

		void Release()
		{
			if (m_pRefCount == nullptr)
				return;

			RemoveRef();
			if (m_pRefCount->HardRefs == 0)
			{
				m_pRefCount->WeakRefs--;
				if (m_pPtr)
				{
					delete m_pPtr;
					m_pPtr = nullptr;
				}

			}
			if (m_pRefCount->WeakRefs == 0)
			{
				delete m_pRefCount;
				m_pRefCount = nullptr;
			}
			m_pPtr = nullptr;
		}

		int GetRefCount() const
		{
			return m_pRefCount ? m_pRefCount->HardRefs : -1;
		}

		int GetWeakRefCount() const
		{
			return m_pRefCount ? m_pRefCount->WeakRefs : -1;
		}

	private:
		template<typename T>
		friend class WeakPtr;

		SharedPtr(T* pPtr, RefCount* pRefCount) :
			m_pPtr(pPtr), m_pRefCount(pRefCount)
		{
			AddRef();
		}

		void AddRef()
		{
			m_pRefCount->HardRefs++;
		}
		void RemoveRef()
		{
			m_pRefCount->HardRefs--;
		}

		T * m_pPtr;
		RefCount* m_pRefCount;
	};

	template<typename T, typename ... Args>
	SharedPtr<T> MakeShared(Args... args)
	{
		return SharedPtr<T>(new T(Forward<Args>(args)...));
	}
}