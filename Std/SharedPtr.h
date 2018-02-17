#pragma once
#include <atomic>

namespace FluxStd
{
	enum class SharedPtrType
	{
		NonThreadSafe,
		ThreadSafe,
	};

	template<class T, SharedPtrType Mode = SharedPtrType::NonThreadSafe> class SharedPtr;
	template<class T, SharedPtrType Mode = SharedPtrType::NonThreadSafe> class WeakPtr;

	template<SharedPtrType type>
	struct RefCount;

	template<>
	struct RefCount<SharedPtrType::NonThreadSafe>
	{
		RefCount(size_t hardRefs, size_t weakRefs) : HardRefs(hardRefs), WeakRefs(weakRefs)
		{}
		size_t HardRefs = 0;
		size_t WeakRefs = 0;
	};

	template<>
	struct RefCount<SharedPtrType::ThreadSafe>
	{
		RefCount(size_t hardRefs, size_t weakRefs) : HardRefs(hardRefs), WeakRefs(weakRefs)
		{}
		std::atomic<size_t> HardRefs = 0;
		std::atomic<size_t> WeakRefs = 0;
	};

	template<typename T, SharedPtrType type>
	class SharedPtr
	{
	private:
		using TRefCount = RefCount<type>;

	public:
		SharedPtr() :
			m_pPtr(nullptr),
			m_pRefCount(new TRefCount(1, 1))
		{
		}

		explicit SharedPtr(T* pPtr) :
			m_pPtr(pPtr),
			m_pRefCount(new TRefCount(1, 1))
		{
		}

		~SharedPtr()
		{
			Release();
		}

		SharedPtr(const SharedPtr& other)
		{
			m_pPtr = other.m_pPtr;
			m_pRefCount = other.m_pRefCount;
			AddRef();
		}

		SharedPtr& operator=(const SharedPtr& other)
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
			m_pRefCount = new TRefCount(1, 1);
			return *this;
		}

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

		bool operator<(const SharedPtr& other) const { return m_pPtr < other.m_pPtr; }
		bool operator==(const SharedPtr& other) const { return m_pPtr == other.m_pPtr; }
		bool operator!=(const SharedPtr& other) const { return m_pPtr != other.m_pPtr; }
		operator bool() const { return m_pPtr != nullptr; }

		size_t Hash() const { return (size_t)m_pPtr / sizeof(T); }

		bool IsValid() const { return m_pPtr != nullptr; }

		size_t GetRefCount() const { return m_pRefCount ? m_pRefCount->HardRefs : -1; }
		size_t GetWeakRefCount() const { return m_pRefCount ? m_pRefCount->WeakRefs : -1; }

		T* operator->() const { return m_pPtr; }
		T& operator*() const { return *m_pPtr; }

		T** GetAddressOf() const { return &m_pPtr; }
		T* Get() const { return m_pPtr; }

	private:
		template<typename T, SharedPtrType type>
		friend class WeakPtr;

		SharedPtr(T* pPtr, TRefCount* pRefCount) :
			m_pPtr(pPtr), m_pRefCount(pRefCount)
		{
			AddRef();
		}

		void AddRef()
		{
			if(m_pRefCount)
				m_pRefCount->HardRefs++;
		}
		void RemoveRef()
		{
			if (m_pRefCount)
				m_pRefCount->HardRefs--;
		}

		T* m_pPtr;
		TRefCount* m_pRefCount;
	};

	template<typename T, SharedPtrType type = SharedPtrType::NonThreadSafe, typename ... Args>
	SharedPtr<T, type> MakeShared(Args... args)
	{
		return SharedPtr<T, type>(new T(Forward<Args>(args)...));
	}

	template<typename T, SharedPtrType type>
	class WeakPtr
	{
	private:
		using TSharedPtr = SharedPtr<T, type>;
		using TRefCount = RefCount<type>;
	public:
		WeakPtr() :
			m_pPtr(nullptr), m_pRefCount(nullptr)
		{}

		WeakPtr(TSharedPtr& other)
		{
			m_pRefCount = other.m_pRefCount;
			AddRef();
			m_pPtr = other.m_pPtr;
		}

		WeakPtr& operator=(TSharedPtr& other)
		{
			m_pRefCount = other.m_pRefCount;
			m_pPtr = other.m_pPtr;
			AddRef();
			return *this;
		}

		size_t Hash() const { return (size_t)m_pPtr / sizeof(T); }

		~WeakPtr()
		{
			Release();
		}

		TSharedPtr Pin()
		{
			if (IsValid())
			{
				return TSharedPtr(m_pPtr, m_pRefCount);
			}
			return TSharedPtr(nullptr);
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

		bool IsValid() { return m_pPtr != nullptr && m_pRefCount->HardRefs > 0; }
		size_t GetRefCount() const { return m_pRefCount ? m_pRefCount->HardRefs : 0; }
		size_t GetWeakRefCount() const { return m_pRefCount ? m_pRefCount->WeakRefs : 0; }

		bool operator<(const WeakPtr& other) const { return m_pPtr < other.m_pPtr; }
		bool operator==(const WeakPtr& other) const { return m_pPtr == other.m_pPtr; }
		bool operator!=(const WeakPtr& other) const { return m_pPtr != other.m_pPtr; }
		operator bool() const { return m_pPtr != nullptr && m_pRefCount->HardRefs > 0; }

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

		TRefCount* m_pRefCount;
		T* m_pPtr = nullptr;
	};
}