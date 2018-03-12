#pragma once
#include "KeyValuePair.h"
#include "Utility.h"
#include "BlockAllocator.h"

namespace FluxStd
{
	template<typename K, typename V, typename KeyCompare = FluxStd::LessThan<K>>
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

			Node(const K& key, V&& value) :
				Pair(key, Forward<V>(value))
			{}
		};

	public:
		struct Iterator
		{
			Iterator(Node* pNode) :
				pNode(pNode)
			{}

			Iterator(const Iterator& other) :
				pNode(other.pNode)
			{
			}

			Iterator& operator=(const Iterator& other)
			{
				pNode = other.pNode;
				return *this;
			}

			Iterator& operator++()
			{
				if (pNode)
					pNode = pNode->pNext;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator* pIt = this;
				if (pNode)
					pNode = pNode->pNext;
				return pIt;
			}

			Iterator& operator--()
			{
				if (pNode)
					pNode = pNode->pPrev;
				return *this;
			}

			Iterator operator--(int)
			{
				Iterator* pIt = this;
				if (pNode)
					pNode = pNode->pPrev;
				return pIt;
			}

			bool operator==(const Iterator& other) const { return pNode == other.pNode; }
			bool operator!=(const Iterator& other) const { return pNode != other.pNode; }

			KeyValuePair<K, V>* operator->() const { return &pNode->Pair; }
			KeyValuePair<K, V>& operator*() const { return pNode->Pair; }

			Node* pNode;
		};

		struct ConstIterator
		{
			ConstIterator(Node* pNode) :
				pNode(pNode)
			{}

			ConstIterator(const ConstIterator& other) :
				pNode(other.pNode)
			{
			}

			ConstIterator& operator=(const ConstIterator& other)
			{
				pNode = other.pNode;
				return *this;
			}

			ConstIterator& operator++()
			{
				if (pNode)
					pNode = pNode->pNext;
				return *this;
			}

			ConstIterator operator++(int)
			{
				ConstIterator* pIt = this;
				if (pNode)
					pNode = pNode->pNext;
				return pIt;
			}

			ConstIterator& operator--()
			{
				if (pNode)
					pNode = pNode->pPrev;
				return *this;
			}

			ConstIterator operator--(int)
			{
				ConstIterator* pIt = this;
				if (pNode)
					pNode = pNode->pPrev;
				return pIt;
			}

			bool operator==(const ConstIterator& other) const { return pNode == other.pNode; }
			bool operator!=(const ConstIterator& other) const { return pNode != other.pNode; }

			const KeyValuePair<K, V>* operator->() const { return &pNode->Pair; }
			const KeyValuePair<K, V>& operator*() const { return pNode->Pair; }

			Node* pNode;
		};

		Map() :
			m_pRoot(nullptr), m_pHead(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), 2);
			m_pNil = ReserveNode();
			new (m_pNil) Node(K());
			m_pNil->pParent = m_pNil->pLeft = m_pNil->pRight = m_pNil;
			m_pNil->Color = BLACK;
		}

		Map(const std::initializer_list<KeyValuePair<K, V>>& list) :
			m_pRoot(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), list.size() + 2);
			m_pNil = ReserveNode();
			new (m_pNil) Node(K());
			m_pNil->pParent = m_pNil->pLeft = m_pNil->pRight = m_pNil;
			m_pNil->Color = BLACK;
			for (const KeyValuePair<K, V>* pPair = list.begin(); pPair != list.end(); ++pPair)
			{
				Insert_Internal(pPair->Key, pPair->Value);
			}
		}

		Map(const Map& other) :
			m_pRoot(nullptr), m_Size(0)
		{
			m_pBlock = BlockAllocator::Initialize(sizeof(Node), other.m_Size + 2);
			m_pNil = ReserveNode();
			new (m_pNil) Node(K());
			m_pNil->pParent = m_pNil->pLeft = m_pNil->pRight = m_pNil;
			m_pNil->Color = BLACK;
			CopyFrom(other);
		}

		Map(const Map&& other)
		{
			Swap(other);
		}

		Map& operator=(const Map& other)
		{
			if (m_pNil == nullptr)
			{
				m_pNil = ReserveNode();
				new (m_pNil) Node(K());
				m_pNil->pParent = m_pNil->pLeft = m_pNil->pRight = m_pNil;
				m_pNil->Color = BLACK;
			}
			Clear();
			CopyFrom(other);
			return *this;
		}

		~Map()
		{
			Clear();
			FreeNode(m_pNil);
			BlockAllocator::Uninitialize(m_pBlock);
		}

		void Swap(Map& other)
		{
			FluxStd::Swap(m_pRoot, other.m_pRoot);
			FluxStd::Swap(m_pHead, other.m_pHead);
			FluxStd::Swap(m_pNil, other.m_pNil);
			FluxStd::Swap(m_Size, other.m_Size);
			FluxStd::Swap(m_pBlock, other.m_pBlock);
		}

		void CopyFrom(const Map& other)
		{
			Clear();
			for (Node *pNode = other.MinNode(); pNode; pNode = pNode->pNext)
				Insert_Internal(pNode->Pair.Key, pNode->Pair.Value);
		}

		void Clear() 
		{ 
			if (m_pRoot != nullptr)
			{
				DestroyTreeHelper(m_pRoot->pLeft);
				m_pRoot->pLeft = m_pNil;
				m_Size = 0;
				DeleteRoot();
			}
		}

		bool Contains(const K& key) const
		{
			return Find_Internal(key) != nullptr;
		}

		template<typename ...Args>
		Iterator Insert(const K& key, Args&&... args)
		{
			return Iterator(Insert_Internal(key, Forward<Args>(args)...));
		}

		Iterator Insert(const KeyValuePair<K, V>& pair)
		{
			return Iterator(Insert_Internal(pair.Key, pair.Value));
		}

		void Insert(const Map& other)
		{
			for (ConstIterator pIt = other.Begin(); pIt != other.End(); ++pIt)
				Insert_Internal(pIt->Key, pIt->Value);
		}

		Iterator Erase(const K& key)
		{
			Node* pNode = Find_Internal(key);
			if (pNode == nullptr)
				return Iterator(nullptr);
			Iterator it = Erase_Internal(pNode);
			if (m_Size == 0 && m_pRoot)
				DeleteRoot();
			return it;
		}

		Iterator Find(const K& key)
		{
			Node* pNode = Find_Internal(key);
			return pNode ? Iterator(pNode) : End();
		}

		ConstIterator Find(const K& key) const
		{
			Node* pNode = Find_Internal(key);
			return pNode ? ConstIterator(pNode) : End();
		}

		bool operator==(const Map& other) const
		{
			if (m_Size != other.m_Size)
				return false;
			if (m_pRoot == nullptr && other.m_pRoot == nullptr)
				return true;
			Node* pA = m_pRoot->pLeft;
			Node* pB = other.m_pRoot->pLeft;
			while (pA != nullptr)
			{
				if (pB == nullptr || pA->Pair != pB->Pair)
					return false;
				pA = pA->pNext;
				pB = pB->pNext;
			}
			return true;
		}

		bool operator!=(const Map& other) const
		{
			if (m_Size != other.m_Size)
				return true;
			if (m_pRoot == nullptr && other.m_pRoot == nullptr)
				return false;
			Node* pA = m_pRoot->pLeft;
			Node* pB = other.m_pRoot->pLeft;
			while (pA != nullptr)
			{
				if (pB == nullptr || pA->Pair != pB->Pair)
					return true;
				pA = pA->pNext;
				pB = pB->pNext;
			}
			return false;
		}

		V& operator[](const K& key)
		{
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

		size_t GetDepth() const
		{
			if (m_pRoot == nullptr)
				return 0;
			size_t depth = 0;
			GetDepth_Internal(m_pRoot, depth, 0);
			return depth;
		}

		size_t Size() const { return m_Size; }
		bool IsEmpty() const { return m_Size == 0; }
		static constexpr size_t MaxSize() { return ~(size_t)0; }

		const V& Front() const { assert(m_Size); return MaxNode()->Pair.Value; }
		const V& Back() const { assert(m_Size); return MinNode()->Pair.Value; }
		V& Front() { assert(m_Size); return MaxNode()->Pair.Value; }
		V& Back() { assert(m_Size); return MinNode()->Pair.Value; }

		Iterator Begin() { return Iterator(MinNode()); }
		Iterator End() { return Iterator(nullptr); }
		ConstIterator Begin() const { return ConstIterator(MinNode()); }
		ConstIterator End() const { return ConstIterator(nullptr); }

		Iterator begin() { return Iterator(MinNode()); }
		Iterator end() { return Iterator(nullptr); }
		ConstIterator begin() const { return ConstIterator(MinNode()); }
		ConstIterator end() const { return ConstIterator(nullptr); }

	private:
		//Get the node with the smallest key value
		Node * MinNode() const
		{
			if (m_pRoot == nullptr)
				return nullptr;
			Node* pNode = m_pRoot->pLeft;
			while (pNode->pLeft != m_pNil)
				pNode = pNode->pLeft;
			return pNode;
		}

		//Get the node with the biggest key value
		Node* MaxNode() const
		{
			if (m_pRoot == nullptr)
				return nullptr;
			Node* pNode = m_pRoot->pLeft;
			while (pNode->pRight != m_pNil)
				pNode = pNode->pRight;
			return pNode;
		}

		inline void DestroyTree()
		{
			DestroyTreeHelper(m_pRoot->pLeft);
			FreeNode(m_pRoot);
			FreeNode(m_pNil);
		}

		void DestroyTreeHelper(Node* pNode)
		{
			if (pNode != m_pNil)
			{
				DestroyTreeHelper(pNode->pLeft);
				DestroyTreeHelper(pNode->pRight);
				FreeNode(pNode);
			}
		}

		//Get the depth of the tree
		void GetDepth_Internal(Node *pNode, size_t &maxDepth, size_t depth) const 
		{
			if (pNode == m_pNil)
				return;
			GetDepth_Internal(pNode->pLeft, maxDepth, depth + 1);
			GetDepth_Internal(pNode->pRight, maxDepth, depth + 1);

			if (depth > maxDepth)
				maxDepth = depth;
		}

		Node* Find_Internal(const K& key) const
		{
			if (m_pRoot == nullptr)
				return nullptr;

			KeyCompare compare;
			Node* pNode = m_pRoot->pLeft;
			while (pNode != m_pNil)
			{
				if (compare(key, pNode->Pair.Key))
					pNode = pNode->pLeft;
				else if (compare(pNode->Pair.Key, key))
					pNode = pNode->pRight;
				else
					return pNode;
			}
			return nullptr;
		}

		template<typename ...Args>
		Node* Insert_Internal(const K& key, Args... args)
		{
			if (m_pRoot == nullptr)
				CreateRoot();

			Node *pNewParent = m_pRoot;
			Node *pNode = m_pRoot->pLeft;

			KeyCompare compare;
			while (pNode != m_pNil)
			{
				pNewParent = pNode;
				if (compare(key, pNode->Pair.Key))
					pNode = pNode->pLeft;
				else if (compare(pNode->Pair.Key, key))
					pNode = pNode->pRight;
				else
				{
					pNode->Pair.Value = Move(args...);
					return pNode;
				}
			}

			Node *pNewNode = ReserveNode();
			new (pNewNode) Node(key, Forward<Args>(args)...);

			pNewNode->pParent = pNewParent;
			pNewNode->pRight = m_pNil;
			pNewNode->pLeft = m_pNil;

			if (pNewParent == m_pRoot || compare(key, pNewParent->Pair.Key))
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
			m_pHead = MinNode();
			return pNewNode;
		}
		
		//Rebalance the tree after insertion
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

		Iterator Erase_Internal(Node *pNode)
		{
			Node* pNext = pNode->pNext;
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

			FreeNode(pNode);
			m_Size--;
			return Iterator(pNext);
		}

		//Rebalance the tree after erasing
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
	private:
		inline void CreateRoot()
		{
			m_pRoot = ReserveNode();
			new (m_pRoot) Node(K());
			m_pRoot->pParent = m_pRoot->pLeft = m_pRoot->pRight = m_pNil;
			m_pRoot->Color = BLACK;
		}

		inline void DeleteRoot()
		{
			if (m_pRoot)
				FreeNode(m_pRoot);
		}

		inline void RotateLeft(Node* pNode)
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

		inline void RotateRight(Node* pNode)
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

		//The node with the closest bigger key
		inline Node* Successor(Node *pNode) const
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

		//The node with the closest smaller key
		inline Node* Predecessor(Node *pNode) const
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

		///////////Allocations///////////

		inline Node* ReserveNode()
		{
			Node* pNode = static_cast<Node*>(BlockAllocator::Alloc(m_pBlock));
			return pNode;
		}

		inline void FreeNode(Node*& pNode)
		{
			(pNode)->~Node();
			BlockAllocator::Free(m_pBlock, pNode);
			pNode = nullptr;
		}

	private:
		//The root of the tree, the left node of the root is the root of the actual tree
		Node* m_pRoot;
		//Null value so we don't have to add extra cases
		Node* m_pNil;
		//The node with the smallest key
		Node* m_pHead;
		//The allocator
		BlockAllocator::Block* m_pBlock;
		size_t m_Size;
	};

	template<typename K, typename V, typename KeyCompare>
	inline void Swap(Map<K, V, KeyCompare>& a, Map<K, V, KeyCompare>& b)
	{
		a.Swap(b);
	}
}