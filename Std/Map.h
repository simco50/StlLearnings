#pragma once

namespace FluxStd
{
	template<typename K, typename V>
	struct KeyValuePair
	{
		KeyValuePair() :
			Key(K())
		{}

		KeyValuePair(const K& key) :
			Key(key)
		{}

		KeyValuePair(const K& key, const V& value) :
			Key(key), Value(value)
		{}

		KeyValuePair(const KeyValuePair& other) :
			Key(other.Key), Value(other.Value)
		{}
		
		KeyValuePair& operator=(const KeyValuePair& other)
		{
			Key = other.Key;
			Value = other.Value;
		}

		bool operator==(const KeyValuePair& other) { return Key == other.Key && Value == other.Value; }
		bool operator!=(const KeyValuePair& other) { return Key != other.Key || Value != other.Value; }

		K Key;
		V Value;
	};

	template<typename K, typename V>
	class Map
	{
	public:
		struct Node
		{
			Node* pParent = nullptr;
			Node* pLeft = nullptr;
			Node* pRight = nullptr;
			bool Red = true;

			KeyValuePair<K, V> Pair;

			Node(const K& key) :
				Pair(key)
			{}

			Node(const K& key, const V& value) :
				Pair(key, value)
			{}

			Node(const Node& other) :
				Pair(other.Pair)
			{}

			static bool IsRed(Node* pNode) { return pNode != nullptr && pNode->Red; }

			static Node* RotateLeft(Node *pNode)
			{
				Node *pX = pNode->pRight;
				p->pRight = pX->pLeft;
				pX->pLeft = pNode;
				pX->Red = pX->pLeft->Red;
				pX->pLeft->Red = true;
				return pX;
			}

			static Node* RotateRight(Node *pNode)
			{
				Node *pX = pNode->pLeft;
				p->pLeft = pX->pRight;
				pX->pRight = pNode;
				pX->Red = pX->pRight->Red;
				pX->pRight->Red = true;
				return pX;
			}

			static void FlipColor(Node* pNode)
			{
				if (pNode == nullptr)
					return;
				pNode->Red = !pNode->Red;
				pNode->pLeft->Red = !pNode->pLeft->Red;
				pNode->pRight->Red = !pNode->pRight->Red;
			}
		};

		Map() :
			m_pRoot(nullptr), m_Size(0)
		{}

		~Map()
		{
			DestroyTree(m_pRoot);
		}

		Map(const Map& other) :
			m_Size(other.m_Size), m_pRoot(nullptr)
		{
			DeepCopy(other.m_pRoot, m_pRoot);
		}

		Map& operator=(const Map& other)
		{
			DestroyTree(m_pRoot);
			m_Size = other.m_Size;
			DeepCopy(other.m_pRoot, m_pRoot);
			return *this;
		}

		Map(Map&& other) :
			m_pRoot(other.m_pRoot), m_Size(other.m_Size)
		{
			other.m_pRoot = nullptr;
			other.m_Size = 0;
		}

		template<typename FunctorType>
		void Iterate(FunctorType functor)
		{
			Iterate<FunctorType>(m_pRoot, functor);
		}

		void Clear() const 
		{ 
			DestroyTree(m_pRoot);
			m_Size = 0;
		}

		void Insert(const K& key, const V& value)
		{
			Insert(key, value, m_pRoot);
		}

		size_t Size() const { return m_Size; }
		bool Contains(const K& key) const { return Get(key, m_pRoot) != nullptr; }
		bool IsEmpty() const { return m_Size == 0; }
		static constexpr size_t MaxSize() { return ~(size_t)0; }

		const V& At(const K& key) const { return Get(key, m_pRoot)->Pair.Value; }
		V& At(const K& key) { return Get(key, m_pRoot)->Pair.Value; }

		V& operator[](const K& key) { return Insert(key, m_pRoot)->Pair.Value; }

		KeyValuePair<K, V>* Find(const K& key) 
		{ 
			Node* pNode = Get(key, m_pRoot); 
			if (pNode) 
				return &pNode->Pair; 
			return nullptr; 
		}

	private:

		void DeepCopy(const Node* pSource, Node*& pDestination)
		{
			if (pSource == nullptr)
				return;
			pDestination = new Node(*pSource);
			DeepCopy(pSource->pLeft, pDestination->pLeft);
			DeepCopy(pSource->pRight, pDestination->pRight);
		}

		void DestroyTree(Node*& pNode)
		{
			if (pNode)
			{
				DestroyTree(pNode->pLeft);
				DestroyTree(pNode->pRight);
				delete pNode;
				pNode = nullptr;
			}
		}

		//Searches the node for a key
		Node* Get(const K& key, Node* pNode) const
		{
			while (pNode != nullptr)
			{
				if (key < pNode->Pair.Key)
					pNode = pNode->pLeft;
				else if (key > pNode->Pair.Key)
					pNode = pNode->pRight;
				else
					return pNode;
			}
			return nullptr;
		}

		//Inserts node or returns an already existing one
		Node* Insert(const K& key, const V& value, Node*& pCurrent)
		{
			if (pCurrent == nullptr)
			{
				pCurrent = new Node(key, value);
				++m_Size;
				return pCurrent;
			}
			if (key < pCurrent->Pair.Key)
				return Insert(key, pCurrent->pLeft);
			else if(key > pCurrent->Pair.Key)
				return Insert(key, pCurrent->pRight);
			return pCurrent;
		}

		//Inserts node or returns an already existing one
		Node* Insert(const K& key, Node*& pCurrent)
		{
			if (pCurrent == nullptr)
			{
				pCurrent = new Node(key);
				++m_Size;
				return pCurrent;
			}
			if (key < pCurrent->Pair.Key)
				return Insert(key, pCurrent->pLeft);
			else if (key > pCurrent->Pair.Key)
				return Insert(key, pCurrent->pRight);
			return pCurrent;
		}

		template<typename FunctorType>
		void Iterate(Node* pNode, FunctorType functor)
		{
			if (pNode == nullptr)
				return;
			Iterate(pNode->pLeft, functor);
			functor(pNode->Pair);
			Iterate(pNode->pRight, functor);
		}

	private:

		Node* m_pRoot;
		size_t m_Size;
	};
}