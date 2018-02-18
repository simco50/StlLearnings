#pragma once
#include "KeyValuePair.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename K, typename V>
	class Map
	{
	private:
		struct Node
		{
			Node* pParent = nullptr;
			Node* pLeft = nullptr;
			Node* pRight = nullptr;
			Node* pNext = nullptr;
			Node* pPrev = nullptr;
			bool Red = true;
			KeyValuePair<K, V> Pair;

			Node(const K& key) :
				Pair(key)
			{}

			Node(const K& key, const V& value) :
				Pair(key, value)
			{}
		};

	public:
		struct Iterator
		{
			Iterator(Node* pNode) :
				pNode(pNode)
			{}

			Iterator& operator++()
			{
				if (pNode)
					pNode = pNode->pNext;
				return *this;
			}

			Iterator& operator--()
			{
				if (pNode)
					pNode = pNode->pPrev;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator it(pNode);
				if (pNode)
					pNode = pNode->pNext;
				return it;
			}

			Iterator operator--(int)
			{
				Iterator it(pNode);
				if (pNode)
					pNode = pNode->pPrev;
				return it;
			}

			bool operator!=(const Iterator& other) const { return pNode != other.pNode; }
			bool operator==(const Iterator& other) const { return pNode == other.pNode; }

			KeyValuePair<K, V>& operator*() const { return pNode->Pair; }
			KeyValuePair<K, V>* operator->() const { return &pNode->Pair; }

			Node* pNode;
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

		size_t Size() const { return m_Size; }
		bool Contains(const K& key) const { return Get(key, m_pRoot) != nullptr; }
		bool IsEmpty() const { return m_Size == 0; }
		static constexpr size_t MaxSize() { return ~(size_t)0; }

		KeyValuePair<K, V>* Find(const K& key) 
		{ 
			Node* pNode = Get(key, m_pRoot); 
			if (pNode) 
				return &pNode->Pair; 
			return nullptr; 
		}

		void Insert(const K& key, const V& value)
		{
			Insert_Internal(key, value);
		}

		Iterator begin() { return Iterator(MinNode()); }
		Iterator end() { return Iterator(MaxNode()->pNext); }

	private:

		Node * Successor(Node* pNode)
		{
			if (pNode == nullptr)
				return nullptr;
			if (pNode->pRight)
			{
				pNode = pNode->pRight;
				while (pNode->pLeft != nullptr)
					pNode = pNode->pLeft;
				return pNode;
			}
			else
			{
				while (pNode->pParent && pNode == pNode->pParent->pRight)
					pNode = pNode->pParent;
				return pNode->pParent;
			}
		}

		Node* Predecessor(Node* pNode)
		{
			if (pNode == nullptr)
				return nullptr;
			if (pNode->pLeft)
			{
				pNode = pNode->pLeft;
				while (pNode->pRight != nullptr)
					pNode = pNode->pRight;
				return pNode;
			}
			else
			{
				while (pNode->pParent && pNode == pNode->pParent->pLeft)
					pNode = pNode->pParent;
				return pNode->pParent;
			}
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

		void FixViolation(Node*& pNode)
		{
			Node* pParent = nullptr;
			Node* pGrandParent = nullptr;

			while (pNode != m_pRoot && pNode->Red && IsRed(pNode->pParent))
			{
				pParent = pNode->pParent;
				pGrandParent = pParent->pParent;
				if (pGrandParent == nullptr)
					break;

				//Case A
				if (pParent == pGrandParent->pLeft)
				{
					//Case 1
					Node* pUncle = pGrandParent->pRight;
					if (pUncle && pUncle->Red)
					{
						pGrandParent->Red = true;
						pParent->Red = false;
						pGrandParent->pRight->Red = false;
						pNode = pGrandParent;
						pParent = pNode->pParent;
					}
					else
					{
						//Case 2
						if (pNode == pParent->pRight)
						{
							RotateLeft(pParent);
							pNode = pParent;
							pParent = pNode->pParent;
						}
						//Case 3
						pParent->Red = false;
						pGrandParent->Red = true;
						RotateRight(pGrandParent);
					}
				}
				//Case B
				else
				{
					Node* pUncle = pGrandParent->pLeft;
					//Case 1
					if (pUncle && pUncle->Red)
					{
						pParent->Red = false;
						pUncle->Red = false;
						pGrandParent->Red = true;
						pNode = pGrandParent;
						pParent = pNode->pParent;
					}
					else
					{
						//Case 2
						if (pNode == pParent->pLeft)
						{
							RotateRight(pParent);
							pNode = pParent;
							pParent = pNode->pParent;
						}
						pParent->Red = false;
						pGrandParent->Red = true;
						RotateLeft(pGrandParent);
					}
				}
			}
			m_pRoot->Red = false;
		}

		Node* Insert_Internal(const K& key, const V& value)
		{
			if (m_pRoot == nullptr)
			{
				m_pRoot = new Node(key, value);
				return m_pRoot;
			}

			Node* pNewParent = m_pRoot;
			Node* pNode = m_pRoot;

			while (pNode != nullptr)
			{
				pNewParent = pNode;
				if (key < pNode->Pair.Key)
					pNode = pNode->pLeft;
				else if (pNode->Pair.Key < key)
					pNode = pNode->pRight;
				else
				{
					pNode->Pair.Value = value;
					return pNode;
				}
			}
			Node* pNewNode = new Node(key, value);
			pNewNode->pParent = pNewParent;
			if (key < pNewParent->Pair.Key)
				pNewParent->pLeft = pNewNode;
			else
				pNewParent->pRight = pNewNode;

			pNewNode->pNext = Successor(pNewNode);
			pNewNode->pPrev = Predecessor(pNewNode);

			if (pNewNode->pNext)
				pNewNode->pNext->pPrev = pNewNode;
			if (pNewNode->pPrev)
				pNewNode->pPrev->pNext = pNewNode;

			++m_Size;
			FixViolation(pNewNode);
			return pNewNode;
		}

		/*void Erase(const K& key)
		{
			Node* pNode = EraseNode(m_pRoot, key);
		}

		Node* EraseNode(Node* pRoot, const K& key)
		{
			if (pRoot == nullptr)
				return nullptr;
			if (key < pRoot->Pair.Key)
				pRoot = EraseNode(pRoot->pLeft, key);
			else if (key > pRoot->Pair.Key)
				pRoot = EraseNode(pRoot->pRight, key);
			else if (pRoot->pLeft == nullptr || pRoot->pRight == nullptr)
			{
			}
			return pRoot;
		}*/

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
		Node * MinNode()
		{
			if (!m_pRoot)
				return nullptr;
			Node* pMin = m_pRoot;
			while (pMin->pLeft)
				pMin = pMin->pLeft;
			return pMin;
		}
		Node * MaxNode()
		{
			if (!m_pRoot)
				return nullptr;
			Node* pMax = m_pRoot;
			while (pMax->pRight)
				pMax = pMax->pRight;
			return pMax;
		}

		inline bool IsRed(Node* pNode) 
		{ 
			return pNode != nullptr && pNode->Red; 
		}

		void RotateLeft(Node *&pNode)
		{
			Node *pRight = pNode->pRight;
			pNode->pRight = pRight->pLeft;
			if (pNode->pRight != nullptr)
				pNode->pRight->pParent = pNode;
			pRight->pParent = pNode->pParent;
			if (pNode->pParent == nullptr)
				m_pRoot = pRight;
			else if (pNode == pNode->pParent->pLeft)
				pNode->pParent->pLeft = pRight;
			else
				pNode->pParent->pRight = pRight;
			pRight->pLeft = pNode;
			pNode->pParent = pRight;
		}

		void RotateRight(Node *&pNode)
		{
			Node *pt_pLeft = pNode->pLeft;
			pNode->pLeft = pt_pLeft->pRight;
			if (pNode->pLeft != nullptr)
				pNode->pLeft->pParent = pNode;
			pt_pLeft->pParent = pNode->pParent;
			if (pNode->pParent == nullptr)
				m_pRoot = pt_pLeft;
			else if (pNode == pNode->pParent->pLeft)
				pNode->pParent->pLeft = pt_pLeft;
			else
				pNode->pParent->pRight = pt_pLeft;
			pt_pLeft->pRight = pNode;
			pNode->pParent = pt_pLeft;
		}

	private:
		Node* m_pRoot;
		size_t m_Size;
	};
}