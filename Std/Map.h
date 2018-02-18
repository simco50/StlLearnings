#pragma once
#include "KeyValuePair.h"
#include "Utility.h"

namespace FluxStd
{
	template<typename K, typename V>
	class Map
	{
	private:
		enum Color
		{
			RED,
			BLACK,
		};

		struct Node
		{
			Node* pParent = nullptr;
			Node* pLeft = nullptr;
			Node* pRight = nullptr;
			Node* pNext = nullptr;
			Node* pPrev = nullptr;
			int Color = RED;
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
		{
			m_pNil = new Node(K());
			m_pNil->pParent = m_pNil->pLeft = m_pNil->pRight = m_pNil;
			m_pNil->Color = BLACK;
		}

		~Map()
		{
			DestroyTree();
		}

		template<typename FunctorType>
		void Iterate(FunctorType functor)
		{
			Iterate<FunctorType>(m_pRoot, functor);
		}

		void Clear() 
		{ 
			DestroyTreeHelper(m_pRoot->pLeft);
			m_pRoot->pLeft = m_pNil;
			m_Size = 0;
			DeleteRoot();
		}

		size_t Size() const { return m_Size; }
		bool IsEmpty() const { return m_Size == 0; }
		static constexpr size_t MaxSize() { return ~(size_t)0; }

		bool Contains(const K& key) const
		{
			return Find_Internal(key) != nullptr;
		}

		void Insert(const K& key, const V& value)
		{
			if (m_pRoot == nullptr)
				CreateRoot();
			Insert_Internal(key, value);
		}

		bool Erase(const K& key)
		{
			if (m_pRoot == nullptr)
				return false;
			Node* pNode = Find_Internal(key);
			if (pNode == nullptr)
				return false;
			Erase_Internal(pNode);
			if (m_Size == 0 && m_pRoot)
				DeleteRoot();
			return true;
		}

		const V* Find(const K& key) const
		{
			Node* pNode = Find_Internal(key);
			pNode ? pNode->Pair.Key : nullptr;
		}

		V& operator[](const K& key)
		{
			if (m_pRoot == nullptr)
				CreateRoot();
			Node* pNode = Find_Internal(key);
			if (pNode == nullptr)
				pNode = Insert_Internal(key, V());
			return pNode->Pair.Value;
		}

		const V& operator[](const K& key) const
		{
			assert(m_pRoot);
			const Node* pNode = Find_Internal(key);
			assert(pNode);
			return pNode->Pair.Value;
		}

	private:

		void DestroyTree()
		{
			DestroyTreeHelper(m_pRoot->pLeft);
			delete m_pRoot;
			delete m_pNil;
		}

		void DestroyTreeHelper(Node* pNode)
		{
			if (pNode != m_pNil)
			{
				DestroyTreeHelper(pNode->pLeft);
				DestroyTreeHelper(pNode->pRight);
				delete pNode;
			}
		}

		Node* Find_Internal(const K& key) const
		{
			Node* pNode = m_pRoot->pLeft;
			while (pNode != m_pNil)
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

		Node* Insert_Internal(const K& key, const V& value) 
		{
			Node *pNewParent = m_pRoot;
			Node *pNode = m_pRoot->pLeft;

			while (pNode != m_pNil) 
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
			Node *pNewNode = new Node(key, value);
			pNewNode->pParent = pNewParent;
			pNewNode->pRight = m_pNil;
			pNewNode->pLeft = m_pNil;

			if (pNewParent == m_pRoot || key < pNewParent->Pair.Key)
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
			InsertFix(pNewNode);
			return pNewNode;
		}

		void InsertFix(Node *pNewNode)
		{
			Node *pNode = pNewNode;
			Node *pParent = pNode->pParent;
			Node *pGrandParent;

			while (pParent->Color == RED)
			{
				pGrandParent = pParent->pParent;

				if (pParent == pGrandParent->pLeft)
				{
					if (pGrandParent->pRight->Color == RED)
					{
						SetColor(pParent, BLACK);
						SetColor(pGrandParent->pRight, BLACK);
						SetColor(pGrandParent, RED);
						pNode = pGrandParent;
						pParent = pNode->pParent;
					}
					else
					{
						if (pNode == pParent->pRight)
						{
							RotateLeft(pParent);
							pNode = pParent;
							pParent = pNode->pParent;
						}
						SetColor(pParent, BLACK);
						SetColor(pGrandParent, RED);
						RotateRight(pGrandParent);
					}
				}
				else
				{
					if (pGrandParent->pLeft->Color == RED)
					{
						SetColor(pParent, BLACK);
						SetColor(pGrandParent->pLeft, BLACK);
						SetColor(pGrandParent, RED);
						pNode = pGrandParent;
						pParent = pNode->pParent;
					}
					else
					{
						if (pNode == pParent->pLeft)
						{
							RotateRight(pParent);
							pNode = pParent;
							pParent = pNode->pParent;
						}
						SetColor(pParent, BLACK);
						SetColor(pGrandParent, RED);
						RotateLeft(pGrandParent);
					}
				}
			}
			SetColor(m_pRoot->pLeft, BLACK);
		}

		void Erase_Internal(Node *pNode)
		{
			Node *pRp = ((pNode->pLeft == m_pNil) || (pNode->pRight == m_pNil)) ? pNode : pNode->pNext;
			Node *pTemp = (pRp->pLeft == m_pNil) ? pRp->pRight : pRp->pLeft;

			Node *pSibling;
			if (pRp == pRp->pParent->pLeft)
			{
				pRp->pParent->pLeft = pTemp;
				pSibling = pRp->pParent->pRight;
			}
			else
			{
				pRp->pParent->pRight = pTemp;
				pSibling = pRp->pParent->pLeft;
			}

			if (pTemp->Color == RED)
			{
				pTemp->pParent = pRp->pParent;
				SetColor(pTemp, BLACK);
			}
			else if (pRp->Color == BLACK && pRp->pParent != m_pRoot)
				EraseFix(pSibling);

			if (pRp != pNode)
			{
				pRp->pLeft = pNode->pLeft;
				pRp->pRight = pNode->pRight;
				pRp->pParent = pNode->pParent;
				pRp->Color = pNode->Color;
				if (pNode->pLeft != m_pNil)
					pNode->pLeft->pParent = pRp;
				if (pNode->pRight != m_pNil)
					pNode->pRight->pParent = pRp;

				if (pNode == pNode->pParent->pLeft)
					pNode->pParent->pLeft = pRp;
				else
					pNode->pParent->pRight = pRp;
			}
			if (pNode->pNext)
				pNode->pNext->pPrev = pNode->pPrev;
			if (pNode->pPrev)
				pNode->pPrev->pNext = pNode->pNext;

			delete pNode;
			m_Size--;
		}

		void EraseFix(Node *pNode)
		{
			Node *pRoot = m_pRoot->pLeft;
			Node *pTemp = m_pNil;
			Node *pSibling = pNode;
			Node *pParent = pSibling->pParent;

			while (pTemp != pRoot)
			{
				if (pSibling->Color == RED)
				{
					SetColor(pSibling, BLACK);
					SetColor(pParent, RED);
					if (pSibling == pParent->pRight)
					{
						pSibling = pSibling->pLeft;
						RotateLeft(pParent);
					}
					else
					{
						pSibling = pSibling->pRight;
						RotateRight(pParent);
					}
				}
				if ((pSibling->pLeft->Color == BLACK) && (pSibling->pRight->Color == BLACK))
				{
					SetColor(pSibling, RED);
					if (pParent->Color == RED)
					{
						SetColor(pParent, BLACK);
						break;
					}
					else
					{
						pTemp = pParent;
						pParent = pTemp->pParent;
						pSibling = (pTemp == pParent->pLeft) ? pParent->pRight : pParent->pLeft;
					}
				}
				else
				{
					if (pSibling == pParent->pRight)
					{
						if (pSibling->pRight->Color == BLACK)
						{
							SetColor(pSibling->pLeft, BLACK);
							SetColor(pSibling, RED);
							RotateRight(pSibling);
							pSibling = pSibling->pParent;
						}
						SetColor(pSibling, pParent->Color);
						SetColor(pParent, BLACK);
						SetColor(pSibling->pRight, BLACK);
						RotateLeft(pParent);
						break;
					}
					else
					{
						if (pSibling->pLeft->Color == BLACK)
						{
							SetColor(pSibling->pRight, BLACK);
							SetColor(pSibling, RED);
							RotateLeft(pSibling);
							pSibling = pSibling->pParent;
						}
						SetColor(pSibling, pParent->Color);
						SetColor(pParent, BLACK);
						SetColor(pSibling->pLeft, BLACK);
						RotateRight(pParent);
						break;
					}
				}
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
		void CreateRoot()
		{
			m_pRoot = new Node(K());
			m_pRoot->pParent = m_pRoot->pLeft = m_pRoot->pRight = m_pNil;
			m_pRoot->Color = BLACK;
		}

		void DeleteRoot()
		{
			if (m_pRoot)
			{
				delete m_pRoot;
				m_pRoot = nullptr;
			}
		}

		void RotateLeft(Node* pNode)
		{
			Node* pRight = pNode->pRight;
			pNode->pRight = pRight->pLeft;
			if (pRight->pLeft != m_pNil)
				pRight->pLeft->pParent = pNode;
			pRight->pParent = pNode->pParent;
			if (pNode == pNode->pParent->pLeft)
				pNode->pParent->pLeft = pRight;
			else
				pNode->pParent->pRight = pRight;

			pRight->pLeft = pNode;
			pNode->pParent = pRight;
		}

		void RotateRight(Node* pNode)
		{
			Node* pLeft = pNode->pLeft;
			pNode->pLeft = pLeft->pRight;
			if (pLeft->pRight != m_pNil)
				pLeft->pRight->pParent = pNode;
			pLeft->pParent = pNode->pParent;
			if (pNode == pNode->pParent->pRight)
				pNode->pParent->pRight = pLeft;
			else
				pNode->pParent->pLeft = pLeft;
			pLeft->pRight = pNode;
			pNode->pParent = pLeft;
		}

		Node* Successor(Node *pNode) const 
		{
			Node *pTemp = pNode;
			if (pTemp->pRight != m_pNil) 
			{
				pTemp = pTemp->pRight;
				while (pTemp->pLeft != m_pNil)
					pTemp = pTemp->pLeft;
				return pTemp;
			}
			else
			{
				while (pTemp == pTemp->pParent->pRight) 
					pTemp = pTemp->pParent;

				if (pTemp->pParent == m_pRoot)
					return nullptr;
				return pTemp->pParent;
			}
		}

		Node* Predecessor(Node *pNode) const
		{
			Node *pTemp = pNode;

			if (pTemp->pLeft != m_pNil)
			{
				pTemp = pTemp->pLeft;
				while (pTemp->pRight != m_pNil)
					pTemp = pTemp->pRight;
				return pTemp;
			}
			else
			{
				while (pTemp == pTemp->pParent->pLeft)
					pTemp = pTemp->pParent;

				if (pTemp == m_pRoot)
					return nullptr;
				return pTemp->pParent;
			}
		}

		inline void SetColor(Node* pNode, int color) 
		{
			pNode->Color = color;
		}

	private:
		Node* m_pRoot;
		Node* m_pNil;
		size_t m_Size;
	};
}