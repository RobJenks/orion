#pragma once

#include <iostream>
#include <sstream>

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

		Vec2<T>& operator=(const Vec2<T>& other);
		bool operator==(const Vec2<T>& other);
		bool operator!=(const Vec2<T>& other);
		bool operator<(const Vec2<T>& other);
		bool operator<=(const Vec2<T>& other);
		bool operator>(const Vec2<T>& other);
		bool operator>=(const Vec2<T>& other);

		Vec2<T> & operator+=(const Vec2<T>& other);
		Vec2<T> & operator-=(const Vec2<T>& other);
		Vec2<T> & operator*=(const Vec2<T>& other);
		Vec2<T> & operator/=(const Vec2<T>& other);

		std::string str() const;
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
	Vec2<T>& Vec2<T>::operator=(const Vec2<T>& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	template <typename T>
	bool Vec2<T>::operator==(const Vec2<T>& other)
	{
		return x == other.x && y == other.y;
	}

	template <typename T>
	bool Vec2<T>::operator!=(const Vec2<T>& other)
	{
		return !(*this == other);
	}

	template <typename T>
	bool Vec2<T>::operator<(const Vec2<T>& other)
	{
		return x < other.x&& y < other.y;
	}
	

	template <typename T>
	bool Vec2<T>::operator<=(const Vec2<T>& other)
	{
		return x <= other.x && y <= other.y;
	}
	

	template <typename T>
	bool Vec2<T>::operator>(const Vec2<T>& other)
	{
		return x > other.x && y > other.y;
	}
	

	template <typename T>
	bool Vec2<T>::operator>=(const Vec2<T>& other)
	{
		return x >= other.x && y >= other.y;
	}

	template <typename T>
	inline Vec2<T> operator+(Vec2<T> lhs, const Vec2<T>& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	template <typename T>
	inline Vec2<T> operator-(Vec2<T> lhs, const Vec2<T>& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	template <typename T>
	inline Vec2<T> operator*(Vec2<T> lhs, const Vec2<T>& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	template <typename T>
	inline Vec2<T> operator/(Vec2<T> lhs, const Vec2<T>& rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	template <typename T>
	inline Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& other)
	{
		x /= other.x;
		y /= other.y;

		return *this;
	}

	template <typename T>
	inline std::ostream& operator<<(std::ostream& ss, Vec2<T> const& v) {
		ss << '(' << v.x << ',' << v.y << ')';
		return ss;
	}

	template<typename T>
	inline std::string Vec2<T>::str() const
	{
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

}
