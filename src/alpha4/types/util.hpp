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

#ifndef ALPHA_TYPES_UTIL_HPP
#define ALPHA_TYPES_UTIL_HPP

namespace alp {
template<typename T> struct constants {};

template<> struct constants<double> {
	static constexpr const double pi           = 3.14159265358979323846;
	static constexpr const double pi_2         = 1.57079632679489661923;
	static constexpr const double pi_4         = 0.785398163397448309616;
	static constexpr const double pi_inv       = 0.318309886183790671538;
	static constexpr const double pi_2_inv     = 0.636619772367581343076;
	static constexpr const double sqrtpi_2_inv = 1.12837916709551257390;
	static constexpr const double sqrt2        = 1.41421356237309504880;
	static constexpr const double sqrt1_2      = 0.707106781186547524401;
	static constexpr const double e            = 2.71828182845904523536;
	static constexpr const double log2e        = 1.44269504088896340736;
	static constexpr const double log10e       = 0.434294481903251827651;
	static constexpr const double ln2          = 0.693147180559945309417;
	static constexpr const double ln10         = 2.30258509299404568402;

	static constexpr const double pi_90      = 3.49065850398865915384e-2;
	static constexpr const double pi_180     = 1.74532925199432957692e-2;
	static constexpr const double pi_360     = 8.72664625997164788461e-3;
	static constexpr const double pi_90_inv  = 2.86478897565411604383e+1;
	static constexpr const double pi_180_inv = 5.72957795130823208767e+1;
	static constexpr const double pi_360_inv = 1.14591559026164641753e+2;
};
template<> struct constants<float> {
	static constexpr const float pi           = 3.14159265358979323846f;
	static constexpr const float pi_2         = 1.57079632679489661923f;
	static constexpr const float pi_4         = 0.785398163397448309616f;
	static constexpr const float pi_inv       = 0.318309886183790671538f;
	static constexpr const float pi_2_inv     = 0.636619772367581343076f;
	static constexpr const float sqrtpi_2_inv = 1.12837916709551257390f;
	static constexpr const float sqrt2        = 1.41421356237309504880f;
	static constexpr const float sqrt1_2      = 0.707106781186547524401f;
	static constexpr const float e            = 2.71828182845904523536f;
	static constexpr const float log2e        = 1.44269504088896340736f;
	static constexpr const float log10e       = 0.434294481903251827651f;
	static constexpr const float ln2          = 0.693147180559945309417f;
	static constexpr const float ln10         = 2.30258509299404568402f;

	static constexpr const float pi_90      = 3.49065850398865915384e-2f;
	static constexpr const float pi_180     = 1.74532925199432957692e-2f;
	static constexpr const float pi_360     = 8.72664625997164788461e-3f;
	static constexpr const float pi_90_inv  = 2.86478897565411604383e+1f;
	static constexpr const float pi_180_inv = 5.72957795130823208767e+1f;
	static constexpr const float pi_360_inv = 1.14591559026164641753e+2f;
};
} // namespace alp
#endif