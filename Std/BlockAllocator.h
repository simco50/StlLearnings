#pragma once

namespace FluxStd
{ 
	class BlockAllocator
	{
	private:
		struct BlockNode
		{
			BlockNode* pNext;
		};

	public:
		struct Block
		{
			size_t NodeSize;
			size_t Capacity;
			BlockNode* pFree;
			Block* pNext;
		};

	public:
		static Block* Initialize(size_t nodeSize, size_t capacity = 1)
		{
			Block* pBlock = AllocateBlock(nullptr, nodeSize, capacity);
			return pBlock;
		}

		static void Uninitialize(Block* pAllocator)
		{
			while (pAllocator)
			{
				Block* pNext = pAllocator->pNext;
				delete[] reinterpret_cast<char*>(pAllocator);
				pAllocator = pNext;
			}
		}

		static void* Alloc(Block* pAllocator)
		{
			if (pAllocator == nullptr)
				return nullptr;

			if (pAllocator->pFree == nullptr)
			{
				size_t newCapacity = (pAllocator->Capacity + 1) >> 1;
				AllocateBlock(pAllocator, pAllocator->NodeSize, newCapacity);
				pAllocator->Capacity += newCapacity;
			}
			BlockNode* pFree = pAllocator->pFree;
			void* pPtr = reinterpret_cast<char*>(pFree) + sizeof(BlockNode);
			pAllocator->pFree = pFree->pNext;
			pFree->pNext = nullptr;
			return pPtr;
		}

		static void Free(Block* pAllocator, void* pPtr)
		{
			if (pAllocator == nullptr || pPtr == nullptr)
				return;
			char* pData = reinterpret_cast<char*>(pPtr);
			BlockNode* pNode = reinterpret_cast<BlockNode*>(pData - sizeof(BlockNode));

			pNode->pNext = pAllocator->pFree;
			pAllocator->pFree = pNode;
		}

	private:
		static Block* AllocateBlock(Block* pAllocator, size_t nodeSize, size_t capacity)
		{
			if (capacity == 0)
				capacity = 1;

			char* pBlockPtr = new char[sizeof(Block) + capacity * (sizeof(BlockNode) + nodeSize)];
			Block* pBlock = reinterpret_cast<Block*>(pBlockPtr);
			pBlock->NodeSize = nodeSize;
			pBlock->Capacity = capacity;
			pBlock->pFree = nullptr;
			pBlock->pNext = nullptr;

			if (!pAllocator)
			{
				pAllocator = pBlock;
			}
			else
			{
				pBlock->pNext = pAllocator->pNext;
				pAllocator->pNext = pBlock;
			}

			char* pNodePtr = pBlockPtr + sizeof(Block);
			BlockNode* pNode = reinterpret_cast<BlockNode*>(pNodePtr);
			for (size_t i = 0; i < capacity - 1; ++i)
			{
				BlockNode* pNewNode = reinterpret_cast<BlockNode*>(pNodePtr);
				pNewNode->pNext = reinterpret_cast<BlockNode*>(pNodePtr + sizeof(BlockNode) + nodeSize);
				pNodePtr += sizeof(BlockNode) + nodeSize;
			}
			{
				BlockNode* pNewNode = reinterpret_cast<BlockNode*>(pNodePtr);
				pNewNode->pNext = nullptr;
			}

			pAllocator->pFree = pNode;
			return pBlock;
		}
	};
}