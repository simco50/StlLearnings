#pragma once
#include <assert.h>
#include "BlockAllocator.h"
#include "KeyValuePair.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename K, typename V>
	struct HashNode
	{
		HashNode() :
			Pair(K()), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		HashNode(const K& key, const V& value) :
			Pair(key, value), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}
		HashNode(const K& key) :
			Pair(key), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		KeyValuePair<K, V> Pair;
		//The previous node in the linked list
		HashNode* pPrev;
		//The next node in the linked list
		HashNode* pNext;
		//The next node in the bucket
		HashNode* pDown;
	};

	template<typename K, typename V>
	struct HashIterator
	{
		using Node = HashNode<K, V>;

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
				pNode = pNode->pNext;
			return *this;
		}

		HashIterator operator++(int)
		{
			HashIterator* pIt = this;
			if (pNode)
				pNode = pNode->pNext;
			return pIt;
		}

		HashIterator& operator--()
		{
			if (pNode)
				pNode = pNode->pPrev;
			return *this;
		}

		HashIterator operator--(int)
		{
			HashIterator* pIt = this;
			if (pNode)
				pNode = pNode->pPrev;
			return pIt;
		}

		bool operator==(const HashIterator& other) const { return pNode == other.pNode; }
		bool operator!=(const HashIterator& other) const { return pNode != other.pNode; }

		KeyValuePair<K, V>* operator->() const { return &pNode->Pair; }
		KeyValuePair<K, V>& operator*() const { return pNode->Pair; }

		Node* pNode;
	};

	template<typename K, typename V>
	struct HashConstIterator
	{
		using Node = HashNode<K, V>;

		HashConstIterator(Node* pNode) :
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
				pNode = pNode->pNext;
			return *this;
		}

		HashConstIterator operator++(int)
		{
			HashConstIterator* pIt = this;
			if (pNode)
				pNode = pNode->pNext;
			return pIt;
		}

		HashConstIterator& operator--()
		{
			if (pNode)
				pNode = pNode->pPrev;
			return *this;
		}

		HashConstIterator operator--(int)
		{
			HashConstIterator* pIt = this;
			if (pNode)
				pNode = pNode->pPrev;
			return pIt;
		}

		bool operator==(const HashConstIterator& other) const { return pNode == other.pNode; }
		bool operator!=(const HashConstIterator& other) const { return pNode != other.pNode; }

		const KeyValuePair<K, V>* operator->() const { return &pNode->Pair; }
		const KeyValuePair<K, V>& operator*() const { return pNode->Pair; }

		Node* pNode;
	};

	template<typename K, typename V, typename HashType = std::hash<K>, typename KeyEqual = FluxStd::EqualTo<K>>
	class HashMap
	{
	public:
		using Iterator = HashIterator<K, V>;
		using ConstIterator = HashConstIterator<K, V>;
		using Node = HashNode<K, V>;

	public:
		HashMap() :
			m_pTable(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node));
			AllocateBuckets(START_BUCKETS);
			m_pHead = ReserveNode();
			m_pTail = m_pHead;
		}

		HashMap(const std::initializer_list<KeyValuePair<K, V>>& list) :
			m_pTable(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), list.size() + 1);
			m_pHead = ReserveNode();
			m_pTail = m_pHead;
			for (const KeyValuePair<K, V>* pPair = list.begin(); pPair != list.end(); ++pPair)
			{
				Insert(pPair->Key, pPair->Value);
			}
		}

		HashMap(const HashMap& other) :
			m_pTable(nullptr), m_Size(0), m_BucketCount(other.m_BucketCount)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), other.m_Size + 1);
			AllocateBuckets(START_BUCKETS);
			m_pHead = ReserveNode();
			m_pTail = m_pHead;
			Insert(other);
		}

		HashMap(HashMap&& other) :
			m_BucketCount(other.m_BucketCount), m_Size(other.m_Size), m_pHead(other.m_pHead), m_pTail(other.m_pTail), m_pTable(other.m_pTable), m_pBlock(other.m_pBlock), m_Hasher(other.m_Hasher)
		{
			other.m_BucketCount = 0;
			other.m_Size = 0;
			other.m_pHead = nullptr;
			other.m_pTail = nullptr;
			other.m_pTable = nullptr;
			other.m_pBlock = nullptr;
		}

		~HashMap()
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

		HashMap& operator=(const HashMap& other)
		{
			//If not self
			if (this != &other)
			{
				Clear();
				Insert(other);
			}
			return *this;
		}

		HashMap& operator+=(const KeyValuePair<K, V>& pair)
		{
			Insert(pair);
			return *this;
		}

		HashMap& operator+=(const HashMap& map)
		{
			//If not self
			if (this != &map)
				Insert(map);
			return *this;
		}

		bool operator==(const HashMap& other) const
		{
			if (m_Size != other.m_Size)
				return false;

			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				ConstIterator pFound = other.Find(pIt->Key);
				if (pFound == pEnd || pFound->Value != pIt->Value)
					return false;
				++pIt;
			}
			return true;
		}

		bool operator!=(const HashMap& other) const
		{
			if (m_Size != other.m_Size)
				return true;

			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				ConstIterator pFound = other.Find(pIt->Key);
				if (pFound == pEnd || pFound->Value != pIt->Value)
					return true;
				++pIt;
			}
			return false;
		}

		void Insert(const HashMap& map)
		{
			ConstIterator pIt = map.begin();
			while (pIt != map.end())
			{
				Insert(pIt->Key, pIt->Value);
				++pIt;
			}
		}

		void Swap(HashMap& other)
		{
			FluxStd::Swap(m_BucketCount, other.m_BucketCount);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_pHead, other.m_pHead);
			FluxStd::Swap(m_pTail, other.m_pTail);
			FluxStd::Swap(m_pTable, other.m_pTable);
			FluxStd::Swap(m_pBlock, other.m_pBlock);
		}

		Iterator Insert(const KeyValuePair<K, V>& pair)
		{
			Iterator pIt = GetOrCreate_Internal(pair.Key);
			pIt.pNode->Pair.Value = pair.Value;
			return pIt;
		}

		Iterator Insert(const K& key, const V& value)
		{	
			Iterator pIt = GetOrCreate_Internal(key);
			pIt.pNode->Pair.Value = value;
			return pIt;
		}

		Iterator Erase(const K& key)
		{
			size_t hash = Hash(key);
			Node* pNode = m_pTable[hash];
			if (pNode == nullptr)
				return Iterator(m_pTail);
			Node* pUp = nullptr;
			Node* pNext = pNode->pNext;
			KeyEqual equal;
			while (pNode != nullptr)
			{
				if (equal(key, pNode->Pair.Key))
				{
					if (pUp)
					{
						//Delete from the bucket
						pUp->pDown = pNode->pDown;
					}
					//Delete from the linked list
					Node* pPrev = pNode->pPrev;
					if(pPrev)
						pPrev->pNext = pNode->pNext;
					if(pNode->pNext)
						pNode->pNext->pPrev = pPrev;

					//If it was the head, replace the head
					if (pNode == m_pHead)
						m_pHead = pNode->pNext;

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
			return Erase(it->Key);
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

		Iterator Find(const K& key)
		{
			const size_t hash = Hash(key);
			Node* pNode = m_pTable[hash];
			KeyEqual equal;
			while (pNode)
			{
				if (equal(pNode->Pair.Key, key))
					return Iterator(pNode);
				pNode = pNode->pDown;
			}
			return Iterator(m_pTail);
		}

		ConstIterator Find(const K& key) const
		{
			const size_t hash = Hash(key);
			Node* pNode = m_pTable[hash];
			KeyEqual equal;
			while (pNode)
			{
				if (equal(pNode->Pair.Key, key))
					return ConstIterator(pNode);
				pNode = pNode->pDown;
			}
			return ConstIterator(m_pTail);
		}

		bool Contains(const K& key) const
		{
			return Find(key) != m_pTail;
		}

		const V& operator[](const K& key) const { return Get(key)->Value; }
		V& operator[](const K& key) { return GetOrCreate_Internal(key)->Value; }

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

		size_t Bucket(const K& key) const
		{
			assert(Find(key) != End());
			return Hash(key);
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
		Iterator GetOrCreate_Internal(const K& key)
		{
			if (m_pTable == nullptr)
				AllocateBuckets(START_BUCKETS);

			//If it exists, change that
			Iterator pExists = Find(key);
			if (pExists != m_pTail)
			{
				return pExists;
			}

			Node* pNewNode = ReserveNode(key);

			//Add node to linked listed
			Node* pPrev = m_pTail->pPrev;
			pNewNode->pPrev = pPrev;
			pNewNode->pNext = m_pTail;
			m_pTail->pPrev = pNewNode;
			if (pPrev)
				pPrev->pNext = pNewNode;

			//If the tail is the head, replace the head
			if (m_pTail == m_pHead)
				m_pHead = pNewNode;

			//Add node to right bucket
			size_t hash = Hash(key);
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
		inline size_t Hash(const K& key) const
		{
			return m_Hasher(key) % m_BucketCount;
		}

		void AllocateBuckets(const size_t count)
		{
			if (m_pTable)
				delete[] m_pTable;
			m_pTable = new Node*[count];
			m_BucketCount = count;

			for (size_t i = 0; i < count; ++i)
				m_pTable[i] = nullptr;
		}

		//Rehash all the elements and place them in the right bucket
		void Rehash()
		{
			for (Iterator pCurrent = begin(); pCurrent != end(); ++pCurrent)
			{
				Node* pNode = pCurrent.pNode;
				size_t hash = Hash(pCurrent->Key);
				pNode->pDown = m_pTable[hash];
				m_pTable[hash] = pNode;
			}
		}

		///////////Allocations///////////

		Node* ReserveNode()
		{
			Node* pNode = static_cast<Node*>(BlockAllocator::Alloc(m_pBlock));
			new(pNode) Node();
			return pNode;
		}

		Node* ReserveNode(const K& key)
		{
			Node* pNode = static_cast<Node*>(BlockAllocator::Alloc(m_pBlock));
			new(pNode) Node(key);
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
		//The hash functor
		HashType m_Hasher;
	};

	template<typename K, typename V, typename Hasher>
	inline void Swap(HashMap<K, V, Hasher>& a, HashMap<K, V, Hasher>& b)
	{
		a.Swap(b);
	}
}