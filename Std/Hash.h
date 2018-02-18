#pragma once

#ifdef _WIN64
#define FNV_PRIME 1099511628211
#define FNV_OFFSET_BASIS 0xcbf29ce484222325
#elif defined(_WIN32)
#define FNV_PRIME 16777619
#define FNV_OFFSET_BASIS 0x811c9dc5
#endif

namespace FluxStd
{
	inline size_t FNV1Hash(const char* pChar, const size_t length)
	{
		size_t hash = FNV_OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash = hash * FNV_PRIME;
			hash = hash ^ pChar[i];
		}
		return hash;
	}

	inline size_t FNV1aHash(const char* pChar, const size_t length)
	{
		size_t hash = FNV_OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash = hash ^ pChar[i];
			hash = hash * FNV_PRIME;
		}
		return hash;
	}
}

#undef FNV_PRIME
#undef FNV_OFFSET_BASIS