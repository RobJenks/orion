#pragma once

namespace Orion
{
	template <typename T>
	struct Vec2
	{
	public:

		T x;
		T y;

		Vec2();
		Vec2(T _x, T _y);

		Vec2<T> operator+(Vec2<T> other) const;
		Vec2<T> & operator+=(const Vec2<T>& other);

	};


	// Implementation

	template <typename T>
	inline Vec2<T>::Vec2()
	{
	}

	template <typename T>
	inline Vec2<T>::Vec2(T _x, T _y)
		:
		x(_x),
		y(_y)
	{
	}

	template <typename T>
	inline Vec2<T> Vec2<T>::operator+(Vec2<T> other) const
	{
		other += *this;
		return other;
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}
}
