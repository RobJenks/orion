#pragma once

#include <vector>
#include <iterator>
#include <tuple>
#include <numeric>
#include <algorithm>

namespace Func
{
	// Only suitable for small vectors with element types suitable for copy
	template <typename T, class Pred>
	std::vector<T> copyFilterVector(const std::vector<T> & in, Pred pred)
	{
		std::vector<T> out;
		std::copy_if(out.cbegin(), out.cend(), std::back_inserter(out), pred);

		return out;
	}

	template <typename T, typename U>
	void zipVectors(const std::vector<T> & v0, const std::vector<U> & v1,
		std::vector<std::tuple<T, U>>& dest)
	{
		std::transform(v0.cbegin(), v0.cend(), v1.cbegin(), std::back_inserter(dest),
			[](const T& e0, const U& e1) { return std::make_tuple(e0, e1); }
	}

	template <typename ExPo, typename T, typename Init, class BinaryOp>
	void reduceVector(ExPo policy, const std::vector<T> & v, Init init, BinaryOp op)
	{
		std::reduce(policy, v.cbegin(), v.cend(), init, op);
	}

	template <typename T, typename Init, class BinaryOp>
	void reduceVector(const std::vector<T> & v, Init init, BinaryOp op)
	{
		std::reduce(v.cbegin(), v.cend(), init, op);
	}

	// Only suitable for small vectors with element types suitable for copy
	template <typename T, typename U>
	void copyZipVectors(const std::vector<T> & v0, const std::vector<U> & v1,
		std::vector<std::tuple<T, U>>& dest)
	{
		std::vector<std::tuple<T, U>> out;
		zipVectors(v0, v1, out);

		return out;
	}

	// Only suitable for small vectors with element types suitable for copy
	template <typename T, typename U, class Fn>
	std::vector<U> copyMapVector(const std::vector<T>& v, Fn fn)
	{
		std::vector<U> out;
		std::transform(v.cbegin(), v.cend(), std::back_inserter(out), fn);

		return out;
	}

}
