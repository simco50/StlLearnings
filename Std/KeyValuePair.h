#pragma once

namespace FluxStd
{
	template<typename K, typename V>
	struct KeyValuePair
	{
		KeyValuePair(const K& key) :
			Key(key)
		{}

		KeyValuePair(const K& key, const V& value) :
			Key(key), Value(value)
		{}

		KeyValuePair(const KeyValuePair& other) :
			Key(other.Key), Value(other.Value)
		{}

		KeyValuePair& operator=(const KeyValuePair& other) = delete;

		bool operator==(const KeyValuePair& other) const { return Key == other.Key && Value == other.Value; }
		bool operator!=(const KeyValuePair& other) const { return Key != other.Key || Value != other.Value; }

		K Key;
		V Value;
	};
}