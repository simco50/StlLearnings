#pragma once
#include <assert.h>

namespace FluxStd
{
	template<typename K, typename V>
	struct KeyValuePair
	{
		KeyValuePair()
		{}

		KeyValuePair(const K& key, const V& value) :
			Key(key), Value(value)
		{}
		KeyValuePair(const K& key) :
			Key(key)
		{}

		K Key;
		V Value;
	};

	template<typename K, typename V>
	struct HashNode
	{
		HashNode() :
			pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		HashNode(const K& key, const V& value) :
			Pair(key, value), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}
		HashNode(const K& key) :
			Pair(key), pPrev(nullptr), pNext(nullptr), pDown(nullptr)
		{}

		KeyValuePair<K, V> Pair;
		HashNode* pPrev;
		HashNode* pNext;
		HashNode* pDown;
	};

	template<typename K, typename V>
	struct HashIteratorBase
	{
		HashIteratorBase(HashNode<K, V>* pNode) :
			pNode(pNode)
		{}

		HashIteratorBase& operator++()
		{
			if (pNode)
				pNode = pNode->pNext;
			return *this;
		}

		HashIteratorBase operator++(int)
		{
			HashIteratorBase* pIt = this;
			if (pNode)
				pNode = pNode->pNext;
			return pIt;
		}

		HashIteratorBase& operator--()
		{
			if (pNode)
				pNode = pNode->pPrev;
			return *this;
		}

		HashIteratorBase operator--(int)
		{
			HashIteratorBase* pIt = this;
			if (pNode)
				pNode = pNode->pPrev;
			return pIt;
		}

		bool operator==(const HashIteratorBase& other) const { return pNode == other.pNode; }
		bool operator!=(const HashIteratorBase& other) const { return pNode != other.pNode; }

		HashNode<K, V>* pNode;
	};

	template<typename K, typename V, typename HashType = std::hash<K>>
	class HashMap
	{
	public:
		using Node = HashNode<K, V>;

		struct Iterator : public HashIteratorBase<K, V>
		{
			Iterator(Node* pNode) : HashIteratorBase<K, V>(pNode)
			{}

			KeyValuePair<K, V>* operator->() { return &pNode->Pair; }
			KeyValuePair<K, V>& operator*() { return pNode->Pair; }
		};

		struct ConstIterator : public HashIteratorBase<K, V>
		{
			ConstIterator(Node* pNode) : HashIteratorBase<K, V>(pNode)
			{}

			const KeyValuePair<K, V>* operator->() const { return &pNode->Pair; }
			const KeyValuePair<K, V>& operator*() const { return pNode->Pair; }
		};

		HashMap() :
			m_pTable(nullptr), m_Size(0)
		{
			AllocateBuckets(START_BUCKETS);
			m_pHead = new Node();
			m_pTail = m_pHead;
		}

		~HashMap()
		{
			if (m_pTable)
			{
				Clear();
				delete m_pHead;
				m_pHead = nullptr;
				delete[] m_pTable;
				m_pTable = nullptr;
			}
		}

		Iterator Insert(const K& key, const V& value)
		{	
			Iterator pIt = GetOrCreate_Internal(key);
			pIt->Value = value;
			return pIt;
		}

		void Erase(const K& key)
		{
			size_t hash = Hash(key);
			Node* pNode = m_pTable[hash];
			Node* pUp = nullptr;
			while (pNode != nullptr)
			{
				if (key == pNode->Pair.Key)
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

					delete pNode;
					--m_Size;
					return;
				}
				pUp = pNode;
				pNode = pNode->pDown;
			}
		}

		void Clear()
		{
			Node* pNode = m_pHead;
			while (pNode != m_pTail)
			{
				Node* pNext = pNode->pNext;
				delete pNode;
				pNode = pNext;
			}
			m_pHead = m_pTail;
			m_Size = 0;
		}

		Iterator Find(const K& key)
		{
			return Get_Internal(key);
		}

		ConstIterator Find(const K& key) const
		{
			return Get_Internal(key);
		}

		Iterator Get(const K& key) const
		{
			size_t hash = Hash(key);
			Node* pNode = m_pTable[hash];
			while (pNode)
			{
				if (pNode->Pair.Key == key)
					return Iterator(pNode);
				pNode = pNode->pDown;
			}
			return Iterator(m_pTail);
		}

		const V& operator[](const K& key) const { return Get(key)->Value; }
		V& operator[](const K& key) { return GetOrCreate_Internal(key)->Value; }

		size_t Size() const { return m_Size; }
		size_t BucketCount() const { return m_BucketCount; }

		bool Contains(const K& key) const
		{
			return Find(key) != m_pTail;
		}

		Iterator begin() { return Iterator(m_pHead); }
		ConstIterator begin() const { return ConstIterator(m_pHead); }
		Iterator end() { return Iterator(m_pTail); }
		ConstIterator end() const { return ConstIterator(m_pTail); }
	
	private:
		Iterator GetOrCreate_Internal(const K& key)
		{
			//If it exists, change that
			Iterator pExists = Get(key);
			if (pExists != m_pTail)
			{
				return pExists;
			}

			Node* pNewNode = new Node(key);

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

			if (m_Size > m_BucketCount * MAX_BUCKET_LOAD)
			{
				AllocateBuckets(m_BucketCount << 1);
				Rehash();
			}

			return Iterator(pNewNode);
		}

		size_t Hash(const K& key) const
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

		Node** m_pTable;
		size_t m_BucketCount;
		size_t m_Size;
		HashType m_Hasher;
		Node* m_pHead;
		Node* m_pTail;

		static const size_t START_BUCKETS = 8;
		static const size_t MAX_BUCKET_LOAD = 4;
	};
}