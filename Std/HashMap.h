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

		KeyValuePair(const KeyValuePair& other) :
			Key(other.Key), Value(other.Value)
		{}

		KeyValuePair& operator=(const KeyValuePair& other) = delete;

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
		//The previous node in the linked list
		HashNode* pPrev;
		//The next node in the linked list
		HashNode* pNext;
		//The next node in the bucket
		HashNode* pDown;
	};

	template<typename K, typename V>
	struct HashIteratorBase
	{
		HashIteratorBase(HashNode<K, V>* pNode) :
			pNode(pNode)
		{}

		HashIteratorBase(const HashIteratorBase& other) :
			pNode(other.pNode)
		{
		}

		HashIteratorBase& operator=(const HashIteratorBase& other)
		{
			pNode = other.pNode;
			return *this;
		}

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

		HashMap(const std::initializer_list<KeyValuePair<K, V>>& list)
		{
			for (KeyValuePair<K, V>* pPair = list.begin(); pPair != list.end(); ++pPair)
			{
				Insert(pPair->Key, pPair->Value);
			}
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

		HashMap& operator=(const HashMap& other)
		{
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
			Insert(map);
			return *this;
		}

		bool operator==(const HashMap& other)
		{
			if (m_Size != other.m_Size)
				return false;

			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				ConstIterator pFound = other.Find(pIt->Key);
				if (pFound != pEnd)
					return false;
			}
			return true;
		}

		bool operator!=(const HashMap& other)
		{
			if (m_Size == other.m_Size)
				return false;

			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				ConstIterator pFound = other.Find(pIt->Key);
				if (pFound == pEnd)
					return true;
			}
			return false;
		}

		void Insert(const HashMap& map)
		{
			ConstIterator pIt = ConstIterator(m_pHead);
			ConstIterator pEnd = ConstIterator(m_pTail);
			while (pIt != pEnd)
			{
				Insert(pIt->Key, pIt->Value);
				++pIt;
			}
		}

		Iterator Insert(const KeyValuePair<K, V>& pair)
		{
			Iterator pIt = GetOrCreate_Internal(pair.Key);
			pIt->Value = pair.Value;
			return pIt;
		}

		Iterator Insert(const K& key, const V& value)
		{	
			Iterator pIt = GetOrCreate_Internal(key);
			pIt->Value = value;
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
					break;
				}
				pUp = pNode;
				pNode = pNode->pDown;
			}
			return Iterator(pNext);
		}

		Iterator Erase(const Iterator& it)
		{
			Node* pNode = it.pNode;
			if (it == Iterator(m_pTail) || pNode == nullptr)
				return Iterator(m_pTail);
			Node* pUp;
			Node* pNext = pNode->pNext;
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
					if (pPrev)
						pPrev->pNext = pNode->pNext;
					if (pNode->pNext)
						pNode->pNext->pPrev = pPrev;

					//If it was the head, replace the head
					if (pNode == m_pHead)
						m_pHead = pNode->pNext;

					delete pNode;
					--m_Size;
					break;
				}
				pUp = pNode;
				pNode = pNode->pDown;
			}
			return Iterator(pNext);
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

		Iterator Find(const K& key) const
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
		constexpr size_t StartBuckets() const { return START_BUCKETS; }
		constexpr size_t MaxLoadFactor() const { return MAX_LOAD_FACTOR; }

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
			Iterator pExists = Find(key);
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

			if (m_Size > m_BucketCount * MAX_LOAD_FACTOR)
			{
				AllocateBuckets(m_BucketCount << 1);
				Rehash();
			}

			return Iterator(pNewNode);
		}

		//Hash a key using the hash functor
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

		//Array of pointers representing the table used for random access
		Node** m_pTable;
		//The amount of buckets
		size_t m_BucketCount;
		//The amount of elements in the map
		size_t m_Size;
		//The hash functor
		HashType m_Hasher;
		//The head of the linked list
		Node* m_pHead;
		//The tail of the linked list
		Node* m_pTail;

		//The amount of buckets to start with
		static const size_t START_BUCKETS = 8;
		//The maximum capacity of 1 bucket, if it exceeds, allocate more buckets and rehash
		static const size_t MAX_LOAD_FACTOR = 4;
	};
}