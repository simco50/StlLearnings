#pragma once
#include "Utility.h"

namespace FluxStd
{
	template<typename K, typename V>
	struct KeyValuePair
	{
		KeyValuePair()
			: Key(K()), Value(V())
		{}

		KeyValuePair(const K& key)
			: Key(key), Value(V())
		{}

		KeyValuePair(K&& key, const V& value) 
			: Key(Forward<K>(key)), Value(value)
		{}

		KeyValuePair(const K& key, V&& value) 
			: Key(key), Value(Forward<V>(value))
		{}

		KeyValuePair(K&& key, V&& value) noexcept
			: Key(Forward<K>(key)), Value(Forward<V>(value))
		{}

		KeyValuePair(const K& key, const V& value) 
			: Key(key), Value(value)
		{}

		KeyValuePair(const KeyValuePair& other) 
			: Key(other.Key), Value(other.Value)
		{}

		KeyValuePair(KeyValuePair&& other) noexcept
		{
			FluxStd::Swap(Key, other.Key);
			FluxStd::Swap(Value, other.Value);
		}

		KeyValuePair& operator=(const KeyValuePair& other)
		{
			Key = other.Key;
			Value = other.Value;
			return *this;
		}

		KeyValuePair& operator=(KeyValuePair&& other)
		{
			Key = Move(other.Key);
			Value = Move(other.Value);
			return *this;
		}

		bool operator==(const KeyValuePair& other) const { return Key == other.Key && Value == other.Value; }
		bool operator!=(const KeyValuePair& other) const { return Key != other.Key || Value != other.Value; }

		K Key;
		V Value;
	};
}