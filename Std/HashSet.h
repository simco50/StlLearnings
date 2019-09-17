#pragma once
#include <assert.h>
#include "BlockAllocator.h"
#include "KeyValuePair.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename T>
	struct HashNode
	{
		HashNode() 
			: Value(T()), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		HashNode(const T& value)
			: Value(value), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		HashNode(T&& value) noexcept
			: Value(Forward<T>(value)), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		T Value;
		//The previous node in the linked list
		HashNode* pPrev;
		//The next node in the linked list
		HashNode* pNext;
		//The next node in the bucket
		HashNode* pDown;
	};

	template<typename T>
	struct HashIterator
	{
		using Node = HashNode<T>;

		HashIterator(Node* pNode) :
			pNode(pNode)
		{}

		HashIterator(const HashIterator& other) :
			pNode(other.pNode)
		{
		}

		HashIterator& operator=(const HashIterator& other)
		{
			pNode = other.pNode;
			return *this;
		}

		HashIterator& operator++()
		{
			if (pNode)
			{
				pNode = pNode->pNext;
			}
			return *this;
		}

		HashIterator operator++(int)
		{
			HashIterator* pIt = this;
			if (pNode)
			{
				pNode = pNode->pNext;
			}
			return *this;
		}

		HashIterator& operator--()
		{
			if (pNode)
			{
				pNode = pNode->pPrev;
			}
			return *this;
		}

		HashIterator operator--(int)
		{
			HashIterator* pIt = this;
			if (pNode)
			{
				pNode = pNode->pPrev;
			}
			return *this;
		}

		bool operator==(const HashIterator& other) const { return pNode == other.pNode; }
		bool operator!=(const HashIterator& other) const { return pNode != other.pNode; }

		T* operator->() const { return &pNode->Value; }
		T& operator*() const { return pNode->Value; }

		Node* pNode;
	};

	template<typename T>
	struct HashConstIterator
	{
		using Node = HashNode<T>;

		HashConstIterator(const Node* pNode) :
			pNode(pNode)
		{}

		HashConstIterator(const HashConstIterator& other) :
			pNode(other.pNode)
		{
		}

		HashConstIterator& operator=(const HashConstIterator& other)
		{
			pNode = other.pNode;
			return *this;
		}

		HashConstIterator& operator++()
		{
			if (pNode)
			{
				pNode = pNode->pNext;
			}
			return *this;
		}

		HashConstIterator operator++(int)
		{
			HashConstIterator* pIt = this;
			if (pNode)
			{
				pNode = pNode->pNext;
			}
			return *this;
		}

		HashConstIterator& operator--()
		{
			if (pNode)
			{
				pNode = pNode->pPrev;
			}
			return *this;
		}

		HashConstIterator operator--(int)
		{
			HashConstIterator* pIt = this;
			if (pNode)
			{
				pNode = pNode->pPrev;
			}
			return *this;
		}

		bool operator==(const HashConstIterator& other) const { return pNode == other.pNode; }
		bool operator!=(const HashConstIterator& other) const { return pNode != other.pNode; }

		const T* operator->() const { return &pNode->Value; }
		const T& operator*() const { return pNode->Value; }

		const Node* pNode;
	};

	template<typename T, typename HashType = std::hash<T>, typename KeyEqual = FluxStd::EqualTo<T>>
	class HashSet
	{
	public:
		using Iterator = HashIterator<T>;
		using ConstIterator = HashConstIterator<T>;
		using Node = HashNode<T>;

	public:
		HashSet() :
			m_pTable(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node));
			AllocateBuckets(START_BUCKETS);
			m_pHead = ReserveNode();
			new(m_pHead) Node();
			m_pTail = m_pHead;
		}

		HashSet(const std::initializer_list<T>& list) 
			: m_pTable(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), list.size() + 1);
			m_pHead = ReserveNode();
			new(m_pHead) Node();
			m_pTail = m_pHead;
			for (const T* pValue = list.begin(); pValue != list.end(); ++pValue)
			{
				Insert(*pValue);
			}
		}

		HashSet(const HashSet& other) 
			: m_pTable(nullptr), m_Size(0), m_BucketCount(other.m_BucketCount)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), other.m_Size + 1);
			AllocateBuckets(START_BUCKETS);
			m_pHead = ReserveNode();
			new(m_pHead) Node();
			m_pTail = m_pHead;
			Insert(other);
		}

		HashSet(HashSet&& other) noexcept
			: m_BucketCount(other.m_BucketCount), m_Size(other.m_Size), m_pHead(other.m_pHead), m_pTail(other.m_pTail), m_pTable(other.m_pTable), m_pBlock(other.m_pBlock)
		{
			other.m_BucketCount = 0;
			other.m_Size = 0;
			other.m_pHead = nullptr;
			other.m_pTail = nullptr;
			other.m_pTable = nullptr;
			other.m_pBlock = nullptr;
		}

		~HashSet()
		{
			if (m_pTable)
			{
				Clear();
				FreeNode(m_pHead);
				m_pHead = nullptr;
				delete[] m_pTable;
				m_pTable = nullptr;

				BlockAllocator::Uninitialize(m_pBlock);
			}
		}

		HashSet& operator=(const HashSet& other)
		{
			//If not self
			if (this != &other)
			{
				Clear();
				Insert(other);
			}
			return *this;
		}

		HashSet& operator+=(const T& value)
		{
			Insert(value);
			return *this;
		}

		HashSet& operator+=(const HashSet& set)
		{
			//If not self
			if (this != &set)
				Insert(set);
			return *this;
		}

		bool operator==(const HashSet& other) const
		{
			if (m_Size != other.m_Size)
			{
				return false;
			}

			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				ConstIterator pFound = other.Find(*pIt);
				if (pFound == pEnd)
				{
					return false;
				}
				++pIt;
			}
			return true;
		}

		bool operator!=(const HashSet& other) const
		{
			if (m_Size != other.m_Size)
			{
				return true;
			}

			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				ConstIterator pFound = other.Find(*pIt);
				if (pFound == pEnd)
				{
					return true;
				}
				++pIt;
			}
			return false;
		}

		void Insert(const HashSet& map)
		{
			ConstIterator pIt = map.begin();
			while (pIt != map.end())
			{
				Insert(*pIt);
				++pIt;
			}
		}

		void Swap(HashSet& other)
		{
			FluxStd::Swap(m_BucketCount, other.m_BucketCount);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_pHead, other.m_pHead);
			FluxStd::Swap(m_pTail, other.m_pTail);
			FluxStd::Swap(m_pTable, other.m_pTable);
			FluxStd::Swap(m_pBlock, other.m_pBlock);
		}

		Iterator Insert(const T& value)
		{
			Iterator pIt = GetOrCreate_Internal(value);
			return pIt;
		}


		Iterator Insert(T&& value)
		{
			Iterator pIt = GetOrCreate_Internal(FluxStd::Forward<T>(value));
			return pIt;
		}

		Iterator Erase(const T& value)
		{
			size_t hash = Hash(value);
			Node* pNode = m_pTable[hash];
			if (pNode == nullptr)
			{
				return Iterator(m_pTail);
			}
			Node* pUp = nullptr;
			Node* pNext = pNode->pNext;
			KeyEqual equal;
			while (pNode != nullptr)
			{
				if (equal(value, pNode->Value))
				{
					if (pUp)
					{
						//Delete from the bucket
						pUp->pDown = pNode->pDown;
					}
					//Delete from the linked list
					Node* pPrev = pNode->pPrev;
					if (pPrev)
					{
						pPrev->pNext = pNode->pNext;
					}
					if (pNode->pNext)
					{
						pNode->pNext->pPrev = pPrev;
					}

					//If it was the head, replace the head
					if (pNode == m_pHead)
					{
						m_pHead = pNode->pNext;
					}

					FreeNode(pNode);
					--m_Size;
					break;
				}
				pUp = pNode;
				pNode = pNode->pDown;
			}
			return Iterator(pNext);
		}

		Iterator Erase(const Iterator& it)
		{
			return Erase(*it);
		}

		void Clear()
		{
			Node* pNode = m_pHead;
			while (pNode != m_pTail)
			{
				Node* pNext = pNode->pNext;
				FreeNode(pNode);
				pNode = pNext;
			}
			m_pHead = m_pTail;
			m_Size = 0;
		}

		Iterator Find(const T& value)
		{
			const size_t hash = Hash(value);
			Node* pNode = m_pTable[hash];
			KeyEqual equal;
			while (pNode)
			{
				if (equal(pNode->Value, value))
				{
					return Iterator(pNode);
				}
				pNode = pNode->pDown;
			}
			return Iterator(m_pTail);
		}

		ConstIterator Find(const T& value) const
		{
			const size_t hash = Hash(value);
			const Node* pNode = m_pTable[hash];
			KeyEqual equal;
			while (pNode)
			{
				if (equal(pNode->Value, value))
				{
					return ConstIterator(pNode);
				}
				pNode = pNode->pDown;
			}
			return ConstIterator(m_pTail);
		}

		bool Contains(const T& value) const
		{
			return Find(value) != m_pTail;
		}

		size_t Size() const { return m_Size; }
		size_t BucketCount() const { return m_BucketCount; }
		constexpr float MaxLoadFactor() const { return 0.75f; }
		float LoadFactor() const { return (float)m_Size / m_BucketCount; }

		size_t BucketSize(const size_t idx) const
		{
			assert(idx < m_BucketCount);
			Node* pNode = m_pTable[idx];
			size_t size = 0;
			while (pNode != nullptr)
			{
				++size;
				pNode = pNode->pDown;
			}
			return size;
		}

		size_t Bucket(const T& value) const
		{
			assert(Find(value) != End());
			return Hash(value);
		}


		//The amount of buckets to start with
		static const size_t START_BUCKETS = 8;

		Iterator Begin() { return Iterator(m_pHead); }
		ConstIterator Begin() const { return ConstIterator(m_pHead); }
		Iterator End() { return Iterator(m_pTail); }
		ConstIterator End() const { return ConstIterator(m_pTail); }

		//Support range based for-loop
		Iterator begin() { return Iterator(m_pHead); }
		ConstIterator begin() const { return ConstIterator(m_pHead); }
		Iterator end() { return Iterator(m_pTail); }
		ConstIterator end() const { return ConstIterator(m_pTail); }

	private:
		template<typename U>
		Iterator GetOrCreate_Internal(U&& value)
		{
			if (m_pTable == nullptr)
			{
				AllocateBuckets(START_BUCKETS);
			}

			//If it exists, change that
			Iterator pExists = Find(value);
			if (pExists != m_pTail)
			{
				return pExists;
			}

			Node* pNewNode = ReserveNode();
			new(pNewNode) Node(FluxStd::Forward<U>(value));

			//Add node to linked listed
			Node* pPrev = m_pTail->pPrev;
			pNewNode->pPrev = pPrev;
			pNewNode->pNext = m_pTail;
			m_pTail->pPrev = pNewNode;
			if (pPrev)
			{
				pPrev->pNext = pNewNode;
			}

			//If the tail is the head, replace the head
			if (m_pTail == m_pHead)
			{
				m_pHead = pNewNode;
			}

			//Add node to right bucket
			size_t hash = Hash(pNewNode->Value);
			pNewNode->pDown = m_pTable[hash];
			m_pTable[hash] = pNewNode;
			++m_Size;

			if (m_Size >= m_BucketCount * MaxLoadFactor())
			{
				AllocateBuckets(m_BucketCount << 1);
				Rehash();
			}

			return Iterator(pNewNode);
		}

		//Hash a key using the hash functor
		inline size_t Hash(const T& value) const
		{
			HashType hasher;
			return hasher(value) % m_BucketCount;
		}

		void AllocateBuckets(const size_t count)
		{
			if (m_pTable)
			{
				delete[] m_pTable;
			}
			m_pTable = new Node * [count];
			m_BucketCount = count;

			for (size_t i = 0; i < count; ++i)
			{
				m_pTable[i] = nullptr;
			}
		}

		//Rehash all the elements and place them in the right bucket
		void Rehash()
		{
			for (Iterator pCurrent = begin(); pCurrent != end(); ++pCurrent)
			{
				Node* pNode = pCurrent.pNode;
				size_t hash = Hash(pNode->Value);
				pNode->pDown = m_pTable[hash];
				m_pTable[hash] = pNode;
			}
		}

		///////////Allocations///////////

		Node* ReserveNode()
		{
			Node* pNode = static_cast<Node*>(BlockAllocator::Alloc(m_pBlock));
			return pNode;
		}

		void FreeNode(Node* pNode)
		{
			(pNode)->~Node();
			BlockAllocator::Free(m_pBlock, pNode);
		}

	private:
		//The amount of buckets
		size_t m_BucketCount;
		//The amount of elements in the map
		size_t m_Size;
		//The head of the linked list
		Node* m_pHead;
		//The tail of the linked list
		Node* m_pTail;
		//Array of pointers representing the table used for random access
		Node** m_pTable;
		//The allocator block
		BlockAllocator::Block* m_pBlock;
	};

	template<typename T, typename Hasher>
	inline void Swap(HashSet<T, Hasher>& a, HashSet<T, Hasher>& b)
	{
		a.Swap(b);
	}
}