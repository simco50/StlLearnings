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
			Insert(key, m_pRoot)->Pair.Value = value;
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
		Node* Insert(const K& key, Node*& pCurrent)
		{
			//Insert BST
			Node* pNewNode = new Node(key);
			m_pRoot = BSTInsert(pCurrent, pNewNode);
			FixViolation(m_pRoot, pNewNode);
			return pNewNode;
		}

		void FixViolation(Node*& pRoot, Node*& pNode)
		{
			Node* pParent = nullptr;
			Node* pGrandParent = nullptr;

			while (pNode != pRoot && pNode->Red && IsRed(pNode->pParent))
			{
				pParent = pNode->pParent;
				pGrandParent = pParent->pParent;

				//Case A
				if (pParent == pGrandParent->pLeft)
				{
					//Case 1
					Node* pUncle = pGrandParent->pRight;
					if (pUncle && pUncle->Red)
					{
						pGrandParent->Red = true;
						pParent->Red = false;
						pUncle->Red = false;
						pNode = pGrandParent;
					}
					else
					{
						//Case 2
						if (pNode == pParent->pRight)
						{
							RotateLeft(pRoot, pParent);
							pNode = pParent;
							pParent = pNode->pParent;
						}
						//Case 3
						RotateRight(pRoot, pGrandParent);
						Swap(pParent->Red, pGrandParent->Red);
						pNode = pParent;
					}
				}
				//Case B
				else
				{
					Node* pUncle = pGrandParent->pLeft;
					//Case 1
					if (pUncle && pUncle->Red)
					{
						pGrandParent->Red = true;
						pParent->Red = false;
						pUncle->Red = false;
						pNode = pGrandParent;
					}
					else
					{
						//Case 2
						if (pNode == pParent->pLeft)
						{
							RotateRight(pRoot, pParent);
							pNode = pParent;
							pParent = pNode->pParent;
						}

						RotateLeft(pRoot, pGrandParent);
						Swap(pParent->Red, pGrandParent->Red);
						pNode = pParent;
					}
				}
			}
			pRoot->Red = false;
		}

		Node* BSTInsert(Node* pRoot, Node* pPtr)
		{
			/* If the tree is empty, return a new node */
			if (pRoot == nullptr)
				return pPtr;

			/* Otherwise, recur down the tree */
			if (pPtr->Pair.Key < pRoot->Pair.Key)
			{
				pRoot->pLeft = BSTInsert(pRoot->pLeft, pPtr);
				pRoot->pLeft->pParent = pRoot;
			}
			else if (pPtr->Pair.Key > pRoot->Pair.Key)
			{
				pRoot->pRight = BSTInsert(pRoot->pRight, pPtr);
				pRoot->pRight->pParent = pRoot;
			}

			/* return the (unchanged) node pointer */
			return pRoot;
		}

		void Erase(const K& key)
		{
			Node* pNode = EraseNode(m_pRoot, key);
		}

		Node* EraseNode(Node* pRoot, const K& key)
		{
			if (pRoot == nullptr)
				return nullptr;
			if (key < pRoot->Pair.Key)
				return EraseNode(pRoot->pLeft, key);
			if (key > pRoot->Pair.Key)
				return EraseNode(pRoot->pRight, key);
			if (pRoot->pLeft == nullptr || pRoot->pRight == nullptr)
				return pRoot;
			Node* pTemp = MinValueNode(pRoot->pRight);
			pRoot->Pair->Key = pTemp->Pair.Key;
			return EraseNode(pRoot->pRight, pTemp->Pair.Key);
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
		inline bool IsRed(Node* pNode) 
		{ 
			return pNode != nullptr && pNode->Red; 
		}

		void RotateLeft(Node *&pRoot, Node *&pNode)
		{
			Node *pRight = pNode->pRight;
			pNode->pRight = pRight->pLeft;
			if (pNode->pRight != nullptr)
				pNode->pRight->pParent = pNode;
			pRight->pParent = pNode->pParent;
			if (pNode->pParent == nullptr)
				pRoot = pRight;
			else if (pNode == pNode->pParent->pLeft)
				pNode->pParent->pLeft = pRight;
			else
				pNode->pParent->pRight = pRight;
			pRight->pLeft = pNode;
			pNode->pParent = pRight;
		}

		void RotateRight(Node *&pRoot, Node *&pNode)
		{
			Node *pt_pLeft = pNode->pLeft;
			pNode->pLeft = pt_pLeft->pRight;
			if (pNode->pLeft != nullptr)
				pNode->pLeft->pParent = pNode;
			pt_pLeft->pParent = pNode->pParent;
			if (pNode->pParent == nullptr)
				pRoot = pt_pLeft;
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