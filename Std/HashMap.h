#pragma once
#include <assert.h>
#include "HashSet.h"

namespace FluxStd
{
	template<typename K, typename V, typename HashType = std::hash<K>, typename KeyEqual = FluxStd::EqualTo<K>>
	class HashMap
	{
	private:
		using ValueType = KeyValuePair<K, V>;
	public:
		using Iterator = HashIterator<ValueType>;
		using ConstIterator = HashConstIterator<ValueType>;
		using Node = HashNode<ValueType>;

	public:
		HashMap()
		{
		}

		HashMap(const std::initializer_list<KeyValuePair<K, V>>& list)
			: m_Set(list)
		{
		}

		HashMap(const HashMap& other) :
			m_Set(other.m_Set)
		{
		}

		HashMap(HashMap&& other) noexcept
		{
			m_Set.Swap(other.m_Set);
		}

		~HashMap()
		{
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

		HashMap& operator=(HashMap&& other) noexcept
		{
			m_Set.Swap(other.m_Set);
			return *this;
		}

		HashMap& operator+=(const KeyValuePair<K, V>& pair)
		{
			m_Set.Insert(pair);
			return *this;
		}

		HashMap& operator+=(const HashMap& map)
		{
			m_Set.Insert(map.m_Set);
			return *this;
		}

		bool operator==(const HashMap& other) const
		{
			return m_Set == other.m_Set;

		}

		bool operator!=(const HashMap& other) const
		{
			return m_Set != other.m_Set;
		}

		void Insert(const HashMap& map)
		{
			m_Set.Insert(map.m_Set);
		}

		void Swap(HashMap& other)
		{
			m_Set.Swap(other.m_Set);
		}

		inline Iterator Insert(const KeyValuePair<K, V>& pair)
		{
			return m_Set.Insert(pair);
		}

		inline Iterator Insert(const K& key, const V& value)
		{	
			return m_Set.Insert(ValueType(key, value));
		}

		inline Iterator Insert(const K& key, V&& value)
		{
			return m_Set.Insert(ValueType(key, FluxStd::Forward<V>(value)));
		}

		inline Iterator Erase(const Iterator& it)
		{
			return m_Set.Erase(it->Key);
		}

		inline Iterator Erase(const K& key)
		{
			Iterator it = Find(key);
			return m_Set.Erase(it);
		}

		inline void Clear()
		{
			m_Set.Clear();
		}

		inline Iterator Find(const K& key)
		{
			return m_Set.Find(key);
		}

		inline ConstIterator Find(const K& key) const
		{
			return m_Set.Find(key);
		}

		inline bool Contains(const K& key) const
		{
			return Find(key) != End();
		}

		inline const V& operator[](const K& key) const { return Find(key)->Value; }
		inline V& operator[](const K& key) { return Insert(key, V())->Value; }

		inline size_t Size() const { return m_Set.Size(); }
		inline size_t BucketCount() const { return m_Set.BucketCount(); }
		inline constexpr float MaxLoadFactor() const { return m_Set.MaxLoadFactor(); }
		inline float LoadFactor() const { return m_Set.LoadFactor(); }

		inline size_t BucketSize(const size_t idx) const
		{
			return m_Set.BucketSize(idx);
		}

		inline Iterator Begin() { return m_Set.Begin(); }
		inline ConstIterator Begin() const { return m_Set.Begin(); }
		inline Iterator End() { return m_Set.End(); }
		inline ConstIterator End() const { return m_Set.End(); }

		//Support range based for-loop
		inline Iterator begin() { return m_Set.Begin(); }
		inline ConstIterator begin() const { return m_Set.Begin(); }
		inline Iterator end() { return m_Set.End(); }
		inline ConstIterator end() const { return m_Set.End(); }

	private:
		struct KVPEqual
		{
			bool operator()(const KeyValuePair<K, V>& a, const KeyValuePair<K, V>& b) { return KeyEqual{}(a.Key, b.Key); }
		};

		struct KVPHash
		{
			size_t operator()(const KeyValuePair<K, V>& a) { return HashType{}(a.Key); }
		};

		HashSet<ValueType, KVPHash, KVPEqual> m_Set;
	};

	template<typename K, typename V, typename Hasher>
	inline void Swap(HashMap<K, V, Hasher>& a, HashMap<K, V, Hasher>& b)
	{
		a.Swap(b);
	}
}