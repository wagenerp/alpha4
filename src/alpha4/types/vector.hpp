/* Copyright 2022 Peter Wagener <mail@peterwagener.net>

This file is part of the Alpha4 library.

Alpha4 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

Alpha4 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Alpha4. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ALPHA_TYPES_VECTOR_HPP
#define ALPHA_TYPES_VECTOR_HPP
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <stdint.h>

namespace alp {

template<size_t D_, typename Scalar_> struct Vector;

template<typename T> concept VectorType = requires() {
	{ T::D }
	->std::convertible_to<size_t>;
};
template<typename T> concept ScalarType = (!VectorType<T>);

template<typename T> struct VectorTraits {
	static constexpr const bool IsVector = VectorType<T>;
	typedef T                   type;

	static constexpr size_t Dimensions() {
		if constexpr (IsVector) {
			return T::D;
		} else {
			return 1;
		}
	}
};

template<size_t D_, typename Scalar_>
struct Vector : public std::array<Scalar_, D_> {
	static constexpr const size_t D = D_;
	typedef Scalar_               Scalar;

	constexpr Scalar_ &      x() { return (*this)[0]; }
	constexpr const Scalar_ &x() const { return (*this)[0]; }
	constexpr Scalar_ &      y() requires(D_ > 1) { return (*this)[1]; }
	constexpr const Scalar_ &y() const requires(D_ > 1) { return (*this)[1]; }
	constexpr Scalar_ &      z() requires(D_ > 2) { return (*this)[2]; }
	constexpr const Scalar_ &z() const requires(D_ > 2) { return (*this)[2]; }
	constexpr Scalar_ &      w() requires(D_ > 3) { return (*this)[3]; }
	constexpr const Scalar_ &w() const requires(D_ > 3) { return (*this)[3]; }

	constexpr Vector<2, Scalar_> xy() const requires(D_ > 1) {
		return {x(), y()};
	}
	constexpr Vector<2, Scalar_> yx() const requires(D_ > 1) {
		return {y(), x()};
	}

	constexpr Vector<2, Scalar_> xz() const requires(D_ > 2) {
		return {x(), z()};
	}
	constexpr Vector<2, Scalar_> zx() const requires(D_ > 2) {
		return {z(), x()};
	}
	constexpr Vector<2, Scalar_> yz() const requires(D_ > 2) {
		return {y(), z()};
	}
	constexpr Vector<2, Scalar_> zy() const requires(D_ > 2) {
		return {z(), y()};
	}

	constexpr Vector<3, Scalar_> xyz() const requires(D > 2) {
		return {x(), y(), z()};
	}
	constexpr Vector<3, Scalar_> xzy() const requires(D > 2) {
		return {x(), z(), y()};
	}
	constexpr Vector<3, Scalar_> yxz() const requires(D > 2) {
		return {y(), x(), z()};
	}
	constexpr Vector<3, Scalar_> zxy() const requires(D > 2) {
		return {z(), x(), y()};
	}
	constexpr Vector<3, Scalar_> yzx() const requires(D > 2) {
		return {y(), z(), x()};
	}
	constexpr Vector<3, Scalar_> zyx() const requires(D > 2) {
		return {z(), y(), x()};
	}

	template<size_t... I>
	constexpr Vector<sizeof...(I), Scalar_> composed() const
		requires(std::max({size_t(0), I...}) < D_) {
		return Vector<sizeof...(I), Scalar>((*this)[I]...);
	}

	template<typename... Composite>
	static constexpr size_t CompositeDimensions() {
		return (VectorTraits<Composite>::Dimensions() + ... + 0);
	}

	template<size_t offset, VectorType Head>
	constexpr void
	assignComposite(const Head &v) requires(offset + Head::D <= D) {
		for (size_t i = offset; i < offset + Head::D; i++)
			(*this)[i] = v[i - offset];
	}

	template<size_t offset, ScalarType Head>
	constexpr void assignComposite(const Head &v) requires(offset + 1 <= D) {
		(*this)[offset] = Scalar(v);
	}

	template<size_t offset, typename Head, typename... Tail>
	constexpr void assignComposite(const Head &v, const Tail &... tail) {
		assignComposite<offset, Head>(v);
		assignComposite<offset + CompositeDimensions<Head>(), Tail...>(tail...);
	}

	constexpr Vector() {
		for (auto &v : *this) {
			v = Scalar(0);
		}
	}
	template<typename... Args>
	constexpr Vector(const Args &... args) requires(
		CompositeDimensions<Args...>() == D) {
		assignComposite<0, Args...>(args...);
	}

	template<typename Scalar2 = Scalar_, typename ScalarRes = Scalar_>
	constexpr Vector<D_, ScalarRes> zippedWith(
		const Vector<D_, Scalar2> &                b,
		std::function<ScalarRes(Scalar_, Scalar2)> op) const {
		Vector<D_, ScalarRes> res;

		auto ita = this->begin();
		auto itb = b.begin();
		auto itr = res.begin();

		for (unsigned i = 0; i < D_; i++, ++ita, ++itb, ++itr) {
			*itr = op(*ita, *itb);
		}
		return res;
	}
	template<typename Scalar2 = Scalar_>
	constexpr Vector &zipWith(
		const Vector<D_, Scalar2> &b, std::function<Scalar_(Scalar_, Scalar2)> op) {
		auto ita = this->begin();
		auto itb = b.begin();

		for (unsigned i = 0; i < D_; i++, ++ita, ++itb) {
			*ita = op(*ita, *itb);
		}
		return *this;
	}
	template<typename Scalar2 = Scalar_, typename ScalarRes = Scalar_>
	constexpr ScalarRes reducedWith(
		const Vector<D_, Scalar2> &                           b,
		std::function<ScalarRes(Scalar_, Scalar2, ScalarRes)> op,
		ScalarRes init = ScalarRes()) const {
		auto ita = this->begin();
		auto itb = b.begin();

		for (unsigned i = 0; i < D_; i++, ++ita, ++itb) {
			init = op(*ita, *itb, init);
		}
		return init;
	}

	template<typename Scalar2 = Scalar_>
	constexpr Vector<D_, Scalar2>
	filtered(std::function<Scalar_(Scalar_)> op) const {
		Vector<D_, Scalar2> res;
		auto                ita = this->begin();
		auto                itr = res.begin();
		for (unsigned i = 0; i < D_; i++, ++ita, ++itr) {
			*itr = op(*ita);
		}
		return res;
	}
	constexpr Vector &filter(std::function<Scalar_(Scalar_)> op) {
		for (auto &v : *this)
			v = op(v);
		return *this;
	}

	template<typename ScalarRes = Scalar_>
	constexpr ScalarRes reduce(
		std::function<ScalarRes(Scalar_, ScalarRes)> op,
		ScalarRes                                    init = ScalarRes()) const {
		for (const auto v : *this) {
			init = op(v, init);
		}
		return init;
	}
	template<unsigned Axis>
	requires(Axis < D_) constexpr Vector set(Scalar_ v) const {
		auto res  = *this;
		res[Axis] = v;
		return res;
	}

	template<typename Scalar2, typename ScalarRes = Scalar_>
	constexpr Vector<D_, ScalarRes>
	operator+(const Vector<D_, Scalar2> &b) const {
		return zippedWith<Scalar2, ScalarRes>(
			b, [](auto a, auto b) constexpr { return a + b; });
	}
	template<typename Scalar2, typename ScalarRes = Scalar_>
	constexpr Vector<D_, ScalarRes>
	operator-(const Vector<D_, Scalar2> &b) const {
		return zippedWith<Scalar2, ScalarRes>(
			b, [](auto a, auto b) constexpr { return a - b; });
	}
	template<typename Scalar2 = Scalar_, typename ScalarRes = Scalar_>
	constexpr ScalarRes operator*(const Vector<D_, Scalar2> &b) const {
		return reducedWith<Scalar2, ScalarRes>(
			b, [](Scalar_ a, Scalar2 b, ScalarRes c) constexpr { return c + a * b; });
	}

	constexpr Vector operator*(Scalar_ f) const {
		return filtered<Scalar_>([f](auto a) constexpr { return a * f; });
	}

	friend constexpr Vector operator*(Scalar_ f, const Vector &v) {
		return v * f;
	}

	template<typename Scalar2, typename ScalarRes = Scalar_>
	constexpr Vector<3, ScalarRes> operator%(const Vector<3, Scalar2> &b) const
		requires(D_ == 3) {
		const auto &a = *this;
		return {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0],
		};
	}
	template<typename Scalar2>
	constexpr Vector &operator%=(const Vector<3, Scalar2> &b) requires(D_ == 3) {
		const auto &a = *this;
		*this         = {
      a[1] * b[2] - a[2] * b[1],
      a[2] * b[0] - a[0] * b[2],
      a[0] * b[1] - a[1] * b[0],
    };

		return *this;
	}

	template<typename Scalar2> constexpr Vector operator/(Scalar2 f) const {
		return filtered<Scalar_>([f](auto a) constexpr { return a / f; });
	}

	template<typename Scalar2>
	constexpr Vector &operator+=(const Vector<D_, Scalar2> &b) {
		return zipWith<Scalar2>(
			b, [](auto a, auto b) constexpr { return a + b; });
	}
	template<typename Scalar2>
	constexpr Vector &operator-=(const Vector<D_, Scalar2> &b) {
		return zipWith<Scalar2>(
			b, [](auto a, auto b) constexpr { return a - b; });
	}
	template<typename Scalar2> constexpr Vector &operator*=(Scalar2 f) {
		return filter([f](auto a) constexpr { return a * f; });
	}

	template<typename Scalar2> constexpr Vector &operator/=(Scalar2 f) {
		return filter([f](auto a) constexpr { return a / f; });
	}

	constexpr Scalar_ norm() const { return std::sqrt(square()); }
	constexpr Scalar_ square() const {
		return reduce<Scalar_>([](auto a, auto b) constexpr { return b + a * a; });
	}
	constexpr Vector &normalize() { return *this /= norm(); }
	constexpr Vector  normalized() const { return *this / norm(); }

	constexpr Vector abs() const {
		return filtered([](auto a) constexpr { return std::abs(a); });
	}
	constexpr Scalar_ max() const {
		return reduce<Scalar_>(
			[](auto a, auto b) constexpr { return std::max(a, b); }, (*this)[0]);
	}
	constexpr Scalar_ min() const {
		return reduce<Scalar_>(
			[](auto a, auto b) { return std::min(a, b); }, (*this)[0]);
	}
	constexpr Scalar_ absmax() const {
		return reduce<Scalar_>(
			[](auto a, auto b) constexpr {
				return std::max((Scalar_)std::abs(a), b);
			},
			0);
	}
	constexpr Scalar_ absmin() const {
		return reduce<Scalar_>(
			[](auto a, auto b) constexpr {
				return std::min((Scalar_)std::abs(a), b);
			},
			0);
	}

	constexpr Scalar_ reduce_mul(Scalar_ init = 1) const {
		return reduce<Scalar_>(
			[](auto a, auto b) constexpr { return b * a; }, init);
	}
	constexpr Scalar_ reduce_add(Scalar_ init = 0) const {
		return reduce<Scalar_>(
			[](auto a, auto b) constexpr { return b + a; }, init);
	}
	constexpr Scalar_ reduce_sub(Scalar_ init = 0) const {
		return reduce<Scalar_>(
			[](auto a, auto b) constexpr { return b - a; }, init);
	}

	constexpr unsigned count_positive() const {
		return reduce<unsigned>(
			[](auto a, auto b) constexpr { return (a > 0) ? b + 1 : b; }, 0);
	}
	constexpr unsigned count_negative() const {
		return reduce<unsigned>(
			[](auto a, auto b) constexpr { return (a < 0) ? b + 1 : b; }, 0);
	}
	constexpr unsigned count_nonPositive() const {
		return reduce<unsigned>(
			[](auto a, auto b) constexpr { return !(a > 0) ? b + 1 : b; }, 0);
	}
	constexpr unsigned count_nonNegative() const {
		return reduce<unsigned>(
			[](auto a, auto b) constexpr { return !(a < 0) ? b + 1 : b; }, 0);
	}

	constexpr Vector cmin(const Vector &b) const {
		return zippedWith<Scalar_, Scalar_>(
			b, [](auto a, auto b) constexpr { return std::min(a, b); });
	}
	constexpr Vector cmax(const Vector &b) const {
		return zippedWith<Scalar_, Scalar_>(
			b, [](auto a, auto b) constexpr { return std::max(a, b); });
	}

	constexpr Vector floor() const {
		return filtered([](auto a) constexpr { return std::floor(a); });
	}
	constexpr Vector ceil() const {
		return filtered([](auto a) constexpr { return std::ceil(a); });
	}

	constexpr unsigned maxComponent() const {
		unsigned res = 0;
		Scalar_  v   = (*this)[0];
		for (unsigned i = 1; i < D_; i++) {
			if ((*this)[i] > v) {
				v   = (*this)[i];
				res = i;
			}
		}
		return res;
	}
	constexpr unsigned minComponent() const {
		unsigned res = 0;
		Scalar_  v   = (*this)[0];
		for (unsigned i = 1; i < D_; i++) {
			if ((*this)[i] < v) {
				v   = (*this)[i];
				res = i;
			}
		}
		return res;
	}

	friend std::ostream &operator<<(std::ostream &f, const Vector &v) {
		for (const auto &c : v) {
			f << c << " ";
		}
		return f;
	}
};

} // namespace alp

extern template struct alp::Vector<2, double>;
extern template struct alp::Vector<2, float>;
extern template struct alp::Vector<2, int32_t>;
extern template struct alp::Vector<2, int64_t>;
extern template struct alp::Vector<2, int16_t>;
extern template struct alp::Vector<2, int8_t>;
extern template struct alp::Vector<3, double>;
extern template struct alp::Vector<3, float>;
extern template struct alp::Vector<3, int32_t>;
extern template struct alp::Vector<3, int64_t>;
extern template struct alp::Vector<3, int16_t>;
extern template struct alp::Vector<3, int8_t>;
extern template struct alp::Vector<4, double>;
extern template struct alp::Vector<4, float>;
extern template struct alp::Vector<4, int32_t>;
extern template struct alp::Vector<4, int64_t>;
extern template struct alp::Vector<4, int16_t>;
extern template struct alp::Vector<4, int8_t>;

namespace alp {
typedef Vector<2, double>  vec2d;
typedef Vector<2, float>   vec2f;
typedef Vector<2, int>     vec2i;
typedef Vector<2, long>    vec2l;
typedef Vector<2, int32_t> vec2i32;
typedef Vector<2, int64_t> vec2i64;
typedef Vector<2, int16_t> vec2i16;
typedef Vector<2, int8_t>  vec2i8;
typedef Vector<3, double>  vec3d;
typedef Vector<3, float>   vec3f;
typedef Vector<3, int>     vec3i;
typedef Vector<3, long>    vec3l;
typedef Vector<3, int32_t> vec3i32;
typedef Vector<3, int64_t> vec3i64;
typedef Vector<3, int16_t> vec3i16;
typedef Vector<3, int8_t>  vec3i8;
typedef Vector<4, double>  vec4d;
typedef Vector<4, float>   vec4f;
typedef Vector<4, int>     vec4i;
typedef Vector<4, long>    vec4l;
typedef Vector<4, int32_t> vec4i32;
typedef Vector<4, int64_t> vec4i64;
typedef Vector<4, int16_t> vec4i16;
typedef Vector<4, int8_t>  vec4i8;
} // namespace alp

namespace std {
template<unsigned D, typename S> struct hash<alp::Vector<D, S>> {
	size_t operator()(const alp::Vector<D, S> &v) const {
		hash<S> subhash;
		size_t  res = 0;
		for (unsigned i = 0; i < D; i++) {
			res = ((res << 8) | (res >> 56)) ^ subhash(v[i]);
		}
		return res;
	}
};
} // namespace std
#endif