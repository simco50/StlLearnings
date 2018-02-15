#pragma once

template<typename T>
struct RandomAccessIterator
{
	RandomAccessIterator(T* pPtr) :
		pPtr(pPtr)
	{}

	RandomAccessIterator(const RandomAccessIterator& other) :
		pPtr(other.pPtr)
	{}

	RandomAccessIterator& operator=(const RandomAccessIterator& other)
	{
		pPtr = other.pPtr;
		return *this;
	}

	RandomAccessIterator& operator++()
	{
		++pPtr;
		return *this;
	}

	RandomAccessIterator& operator--()
	{
		--pPtr;
		return *this;
	}

	RandomAccessIterator operator++(int)
	{
		RandomAccessIterator<T> pIt = *this;
		++pPtr;
		return pIt;
	}

	RandomAccessIterator operator--(int)
	{
		RandomAccessIterator<T> pIt = *this;
		--pPtr;
		return pIt;
	}

	RandomAccessIterator& operator+=(const size_t distance)
	{
		pPtr += distance;
		return *this;
	}

	RandomAccessIterator& operator-=(const size_t distance)
	{
		pPtr -= distance;
		return *this;
	}

	RandomAccessIterator operator+(const size_t distance) const { return RandomAccessIterator(pPtr + distance); }
	RandomAccessIterator operator-(const size_t distance) const { return RandomAccessIterator(pPtr - distance); }

	size_t operator+(const RandomAccessIterator& other) const { return (size_t)(pPtr + other.pPtr); }
	size_t operator-(const RandomAccessIterator& other) const { return (size_t)(pPtr - other.pPtr); }
	
	bool operator==(const RandomAccessIterator& other) const { return pPtr == other.pPtr; }
	bool operator!=(const RandomAccessIterator& other) const { return pPtr != other.pPtr; }
	bool operator>(const RandomAccessIterator& other) const { return pPtr > other.pPtr; }
	bool operator<(const RandomAccessIterator& other) const { return pPtr < other.pPtr; }
	bool operator>=(const RandomAccessIterator& other) const { return pPtr >= other.pPtr; }
	bool operator<=(const RandomAccessIterator& other) const { return pPtr <= other.pPtr; }

	T* operator->() const { return pPtr; }
	T& operator*() const { return *pPtr; }

	T* pPtr;
};

template<typename T>
struct RandomAccessConstIterator
{
	RandomAccessConstIterator(T* pPtr) :
		pPtr(pPtr)
	{}

	RandomAccessConstIterator(const RandomAccessConstIterator& other) :
		pPtr(other.pPtr)
	{}

	RandomAccessConstIterator& operator=(const RandomAccessConstIterator& other)
	{
		pPtr = other.pPtr;
		return *this;
	}

	RandomAccessConstIterator& operator++()
	{
		--pPtr;
		return *this;
	}

	RandomAccessConstIterator& operator--()
	{
		--pPtr;
		return *this;
	}

	RandomAccessConstIterator operator++(int)
	{
		RandomAccessConstIterator<T> pIt = *this;
		++pPtr;
		return pIt;
	}

	RandomAccessConstIterator operator--(int)
	{
		RandomAccessConstIterator<T> pIt = *this;
		--pPtr;
		return pIt;
	}

	RandomAccessConstIterator& operator+=(const size_t distance)
	{
		pPtr += distance;
		return *this;
	}

	RandomAccessConstIterator& operator-=(const size_t distance)
	{
		pPtr -= distance;
		return *this;
	}

	RandomAccessConstIterator operator+(const size_t distance) const { return RandomAccessConstIterator(pPtr + distance); }
	RandomAccessConstIterator operator-(const size_t distance) const { return RandomAccessConstIterator(pPtr - distance); }

	size_t operator+(const RandomAccessConstIterator& other) const { return (size_t)(pPtr + other.pPtr); }
	size_t operator-(const RandomAccessConstIterator& other) const { return (size_t)(pPtr - other.pPtr); }

	bool operator==(const RandomAccessConstIterator& other) const { return pPtr == other.pPtr; }
	bool operator!=(const RandomAccessConstIterator& other) const { return pPtr != other.pPtr; }
	bool operator>(const RandomAccessConstIterator& other) const { return pPtr > other.pPtr; }
	bool operator<(const RandomAccessConstIterator& other) const { return pPtr < other.pPtr; }
	bool operator>=(const RandomAccessConstIterator& other) const { return pPtr >= other.pPtr; }
	bool operator<=(const RandomAccessConstIterator& other) const { return pPtr <= other.pPtr; }

	const T* operator->() const { return pPtr; }
	const T& operator*() const { return *pPtr; }

	T* pPtr;
};