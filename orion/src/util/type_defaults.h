#pragma once

template <typename T>
class TypeDefault
{
public:

	inline static T zero() { return (T)0; }
};

template <>
class TypeDefault<int>
{
public:

	inline static int zero() { return 0; }
};


template <>
class TypeDefault<size_t>
{
public:

	inline static size_t zero() { return 0; }
};
