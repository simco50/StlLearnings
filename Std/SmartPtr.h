#pragma once
#include <intrin0.h>
#include "Utility.h"

#define MT_INC(x) \
_InterlockedIncrement(reinterpret_cast<volatile long*>(&x))

#define MT_DECR(x) \
_InterlockedDecrement(reinterpret_cast<volatile long*>(&x))

namespace FluxStd
{
	template<typename T>
	struct DefaultDeleter
	{
		void operator()(T* pObj)
		{
			delete pObj;
		}
	};

	//The base for a control block
	//Has StrongRefs and WeakRefs
	//If StrongRefs is 0, the object gets deleted
	//If WeakRefs is 0, the controlblock gets deleted
	class RefCountBase
	{
	public:
		virtual ~RefCountBase() noexcept
		{
		}

		void DecRef()
		{
			if (MT_DECR(StrongRefs) == 0)
			{
				Destroy();
				DecWeakRef();
			}
		}

		inline void IncRef()
		{
			MT_INC(StrongRefs);
		}

		void DecWeakRef()
		{
			if (MT_DECR(WeakRefs) == 0)
			{
				DeleteThis();
			}
		}

		inline void IncWeakRef()
		{
			MT_INC(WeakRefs);
		}

		inline long RefCount() const { return StrongRefs; }

	protected:
		RefCountBase()
			: StrongRefs(1), WeakRefs(1)
		{
		}

		virtual void Destroy() = 0;
		virtual void DeleteThis() noexcept = 0;

	private:
		long StrongRefs;
		long WeakRefs;
	};

	template<typename T>
	class RefCount : public RefCountBase
	{
	public:
		RefCount(T* pPtr)
			: m_pPtr(pPtr)
		{
		}

	protected:
		virtual void Destroy() override
		{
			delete m_pPtr;
		}

		virtual void DeleteThis() noexcept override
		{
			delete this;
		}

	private:
		T * m_pPtr;
	};

	template<typename T>
	class RefCountWithObject : public RefCountBase
	{
	public:
		template<typename... Args>
		RefCountWithObject(Args&&... args) :
			RefCountBase()
		{
			new (m_Object) T(FluxStd::Forward<Args>(args)...);
		}

		inline T* GetPtr()
		{
			return (T*)m_Object;
		}

	protected:
		virtual void Destroy() override
		{
			GetPtr()->~T();
		}

		virtual void DeleteThis() noexcept override
		{
			delete this;
		}

	private:
		char m_Object[sizeof(T)];
	};

	template<typename T>
	class RefCounted
	{
	public:
		bool operator<(const RefCounted& other) const noexcept { return m_pPtr < other.m_pPtr; }
		bool operator==(const RefCounted& other) const noexcept { return m_pPtr == other.m_pPtr; }
		bool operator!=(const RefCounted& other) const noexcept { return m_pPtr != other.m_pPtr; }

		inline unsigned char HardRefs() const noexcept { return m_pRefCount->StrongRefs; }
		inline unsigned char WeakRefs() const noexcept { return m_pRefCount->WeakRefs; }

	protected:
		RefCounted(T* pPtr, RefCountBase* pRefCount) noexcept
			: m_pPtr(pPtr), m_pRefCount(pRefCount)
		{}

		T* m_pPtr;
		RefCountBase* m_pRefCount;
	};

	//forward declare
	template<typename T>
	class SharedPtr;

	template<typename T>
	class WeakPtr;

	template<typename T, typename Deleter = DefaultDeleter<T>>
	class UniquePtr;

	template<typename T>
	class WeakPtr : public RefCounted<T>
	{
	private:
		using SharedPtrT = SharedPtr<T>;

	public:
		explicit WeakPtr(const SharedPtrT& shared) :
			RefCounted(shared.m_pPtr, shared.m_pRefCount)
		{
			if (m_pRefCount)
			{
				m_pRefCount->IncWeakRef();
			}
		}

		WeakPtr(const WeakPtr& other)
			: RefCount(other.m_pPtr, other.m_pRefCount)
		{
			if (m_pRefCount)
			{
				m_pRefCount->IncWeakRef();
			}
		}

		WeakPtr(WeakPtr&& other) noexcept
			: RefCounted(other.m_pPtr, other.m_pRefCount)
		{
			other.m_pPtr = nullptr;
			other.m_pRefCount = nullptr;
		}

		WeakPtr& operator=(const WeakPtr& other)
		{
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			if (m_pRefCount)
			{
				m_pRefCount->IncWeakRef();
			}
			return *this;
		}

		WeakPtr& operator=(WeakPtr&& other)
		{
			m_pPtr = other.m_pPtr;
			other.m_pPtr = nullptr;
			m_pRefCount = other.m_pRefCount;
			other.m_pRefCount = nullptr;
			return *this;
		}

		~WeakPtr()
		{
			Release();
		}

		SharedPtrT Pin()
		{
			if (IsValid())
			{
				m_pRefCount->IncRef();
				return SharedPtrT(m_pPtr, m_pRefCount);
			}
			else
			{
				return SharedPtrT(nullptr);
			}
		}

		inline bool IsValid() const noexcept
		{
			return m_pRefCount && m_pRefCount->RefCount() > 0;
		}

		inline operator bool() const noexcept
		{
			return IsValid();
		}

		void Release()
		{
			if (m_pRefCount)
			{
				m_pRefCount->DecWeakRef();
				m_pRefCount = nullptr;
			}
		}
	};

	template<typename T>
	class SharedPtr : public RefCounted<T>
	{
	private:
		using WeakPtrT = WeakPtr<T>;
		using UniquePtrT = UniquePtr<T>;
		friend class WeakPtrT;

	public:
		SharedPtr()
			: RefCounted(nullptr, new RefCount<T>(nullptr))
		{}

		explicit SharedPtr(T* pPtr)
			: RefCounted(pPtr, new RefCount<T>(nullptr))
		{}

		SharedPtr(const SharedPtr& other)
			: RefCounted(other.m_pPtr, other.m_pRefCount)
		{
			if (m_pRefCount)
			{
				m_pRefCount->IncRef();
			}
		}

		SharedPtr(SharedPtr&& other) noexcept
			: RefCounted(other.m_pPtr, other.m_pRefCount)
		{
			other.m_pPtr = nullptr;
			other.m_pRefCount = nullptr;
		}

		explicit SharedPtr(UniquePtrT&& other) noexcept
			: RefCounted(other.m_pPtr, new RefCount<T>(other.m_pPtr))
		{
			other.m_pPtr = nullptr;
		}

		~SharedPtr() noexcept
		{
			Release();
		}

		SharedPtr& operator=(const SharedPtr& other)
		{
			Release();
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			if (m_pRefCount)
			{
				m_pRefCount->IncRef();
			}
			return *this;
		}

		SharedPtr& operator=(SharedPtr&& other) noexcept
		{
			FluxStd::Swap(m_pRefCount, other.m_pRefCount);
			FluxStd::Swap(m_pPtr, other.m_pPtr);
			return *this;
		}

		SharedPtr(T* pPtr, RefCountBase* pRefCount) :
			RefCounted(pPtr, pRefCount)
		{
		}

		inline bool IsValid() const noexcept
		{
			return m_pPtr != nullptr;
		}

		inline operator bool() const
		{
			return IsValid();
		}

		void Release()
		{
			if (m_pRefCount)
			{
				m_pRefCount->DecRef();
				m_pRefCount = nullptr;
			}
			m_pPtr = nullptr;
		}

		inline T * operator->() { return m_pPtr; }
		inline const T* operator->() const { return m_pPtr; }
		inline T& operator*() { return *m_pPtr; }
		inline const T& operator*() const { return *m_pPtr; }

		inline T* Get() { return m_pPtr; }
		inline const T* Get() const { return m_pPtr; }

		inline T** GetAddressOf() { return &m_pPtr; }
		inline T* const* GetAddressOf() const { return &m_pPtr; }
	};

	struct DummyArgA
	{};

	struct DummyArgB
	{};

	template<typename T1, typename T2>
	class CompressedPair : private T1
	{
		using Base = T1;

	public:
		template<typename... Args1>
		CompressedPair(DummyArgA, Args1&&... args)
			: T1(), m_Second(FluxStd::Forward<Args1>(args)...)
		{}

		template<typename Args1, typename... Args2>
		CompressedPair(DummyArgB, Args1&& first, Args2&&... args)
			: T1(FluxStd::Forward<Args1>(first)), m_Second(FluxStd::Forward<Args2>(args)...)
		{}

		T1& GetFirst() { return *this; }
		T2& GetSecond() { return m_Second; }

		inline const Base& GetFirst() const { return *this; }
		inline const T2& GetSecond() const { return m_Second; }

	private:
		T2 m_Second;
	};

	template<typename T, typename Deleter>
	class UniquePtrBase
	{
	public:
		UniquePtrBase() noexcept
			: m_Object(DummyArgA(), nullptr)
		{}

		UniquePtrBase(T* pPtr) noexcept
			: m_Object(DummyArgA(), pPtr)
		{}

		UniquePtrBase(T* pPtr, const Deleter& deleter) noexcept
			: m_Object(DummyArgB(), FluxStd::Forward<const Deleter>(deleter), pPtr)
		{}

		~UniquePtrBase() noexcept
		{
			if (GetPtr())
			{
				GetDeleter()(GetPtr());
			}
		}

	protected:
		T * & GetPtr()
		{
			return m_Object.GetSecond();
		}

		const T* GetPtr() const
		{
			return m_Object.GetSecond();
		}

		Deleter& GetDeleter()
		{
			return m_Object.GetFirst();
		}

		CompressedPair<Deleter, T*> m_Object;

	};

	template<typename T, typename Deleter>
	class UniquePtr : public UniquePtrBase<T, Deleter>
	{
	public:
		friend class SharedPtr<T>;

		UniquePtr() noexcept
			: UniquePtrBase(nullptr)
		{
		}

		~UniquePtr() = default;

		UniquePtr(T* pPtr) noexcept
			: UniquePtrBase(pPtr)
		{
		}

		UniquePtr(T* pPtr, const Deleter& deleter) noexcept
			: UniquePtrBase(pPtr, FluxStd::Forward<const Deleter>(deleter))
		{
		}

		UniquePtr(const UniquePtr& other) = delete;
		UniquePtr& operator=(const UniquePtr& other) = delete;

		UniquePtr(UniquePtr&& other) noexcept
			: UniquePtrBase(other.Detach(), FluxStd::Forward<Deleter>(other.GetDeleter()))
		{
		}

		UniquePtr& operator=(UniquePtr&& other) noexcept
		{
			Reset(other.Detach());
			GetDeleter() = FluxStd::Forward<Deleter>(other.GetDeleter());
			return *this;
		}

		void Reset(T* pPointer = nullptr) noexcept
		{
			T* pOld = Get();
			GetPtr() = pPointer;
			if (pOld != pPointer)
			{
				GetDeleter()(pOld);
			}
		}

		inline bool IsValid() const
		{
			return Get() != nullptr;
		}

		T* Detach() noexcept
		{
			T* pPtr = Get();
			GetPtr() = nullptr;
			return pPtr;
		}

		inline T* operator->() { return Get(); }
		inline const T* operator->() const { return Get(); }
		inline T& operator*() { return *Get(); }
		inline const T& operator*() const { return *Get(); }

		inline T* Get() { return GetPtr(); }
		inline const T* Get() const { return GetPtr(); }
	};

	template<typename T, typename... Args>
	inline SharedPtr<T> MakeShared(Args... args)
	{
		RefCountWithObject<T>* pObject = new RefCountWithObject<T>(FluxStd::Forward<Args>(args)...);
		return SharedPtr<T>(pObject->GetPtr(), pObject);
	}

	template<typename T, typename... Args>
	inline UniquePtr<T> MakeUnique(Args... args)
	{
		return UniquePtr<T>(new T(FluxStd::Forward<Args>(args)...));
	}
}