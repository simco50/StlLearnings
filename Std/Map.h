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

			static Node* Parent(Node* pNode)
			{
				if (pNode)
					return pNode->pParent;
				return nullptr;
			}

			static Node* Sibling(Node* pNode)
			{
				if (pNode == nullptr)
					return nullptr;
				Node* pParent = pNode->pParent;
				if (pParent == nullptr)
					return nullptr;
				if (pParent->pLeft == pNode)
					return pParent->pRight;
				return pParent->pLeft;
			}

			static Node* Uncle(Node* pNode)
			{
				Node* pParent = Parent(pNode);
				return Sibling(pParent);
			}

			static Node* GrandParent(Node* pNode)
			{
				if (pNode == nullptr)
					return nullptr;
				Node* pParent = pNode->pParent;
				if (pParent == nullptr)
					return nullptr;
				return pParent->pParent;
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

		size_t Size() const { return m_Size; }
		void Clear() const 
		{ 
			return DestroyTree(m_pRoot);
			m_Size = 0;
		}

		const V& Get(const K& key) const { return GetValue(key, m_pRoot); }
		V& Get(const K& key) { return GetValue(key, m_pRoot); }

		V& operator[](const K& key) { return GetOrCreate(key, m_pRoot); }

	private:

		V & GetValue(const K& key, Node* pNode) const
		{
			Node* pFoundNode = GetNode(key, pNode);
			assert(pFoundNode);
			pFoundNode->Value;
		}

		V& GetOrCreate(const K& key, Node* pNode)
		{
			Node* pFoundNode = GetNode(key, pNode);
			if (pFoundNode)
				return pFoundNode->Pair.Value;
			return InsertPair(key)->Pair.Value;
		}

		Node* GetNode(const K& key, Node* pNode)
		{
			if (pNode == nullptr)
				return nullptr;

			if (key < pNode->Pair.Key)
				return GetNode(key, pNode->pLeft);
			else if (key > pNode->Pair.Key)
				return GetNode(key, pNode->pRight);
			return pNode;
		}

		Node* InsertPair(const K& key, const V& value)
		{
			Node* pNewNode = new Node(key, value);
			InsertNode(pNewNode);
			return pNewNode;
		}

		Node* InsertPair(const K& key)
		{
			Node* pNewNode = new Node(key);
			InsertNode(pNewNode, m_pRoot);
			return pNewNode;
		}

		void InsertNode(Node* pNode, Node*& pCurrent)
		{
			if (pCurrent == nullptr)
			{
				pCurrent = pNode;
				return;
			}
			if (pNode->Pair.Key < pCurrent->Pair.Key)
				InsertNode(pNode, pCurrent->pLeft);
			else
				InsertNode(pNode, pCurrent->pRight);
		}

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
		Node* RotateLeft(Node *pNode)
		{
			Node *pX = pNode->pRight;
			p->pRight = pX->pLeft;
			pX->pLeft = pNode;
			pX->Red = pX->pLeft->Red;
			pX->pLeft->Red = true;
			return pX;
		}

		Node* RotateRight(Node *pNode)
		{
			Node *pX = pNode->pLeft;
			p->pLeft = pX->pRight;
			pX->pRight = pNode;
			pX->Red = pX->pRight->Red;
			pX->pRight->Red = true;
			return pX;
		}

		void FlipColor(Node* pNode)
		{
			if (pNode == nullptr)
				return;
			pNode->Red = !pNode->Red;
			pNode->pLeft->Red = !pNode->pLeft->Red;
			pNode->pRight->Red = !pNode->pRight->Red;
		}

	private:
		Node* m_pRoot;
		size_t m_Size;
	};
}