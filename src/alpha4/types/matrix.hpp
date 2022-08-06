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

#ifndef ALPHA4_TYPES_MATRIX_HPP
#define ALPHA4_TYPES_MATRIX_HPP
#include "alpha4/types/util.hpp"
#include "alpha4/types/vector.hpp"

#include <iostream>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

namespace alp {
enum matrix_storage_type_t {
	ROW_MAJOR    = 0,
	COLUMN_MAJOR = 1,
};

template<typename T, matrix_storage_type_t stor> struct Matrix4_base {};

template<typename T> struct Matrix4_base<T, ROW_MAJOR> {
public:
	// clang-format off
			T a11, a12, a13, a14,
				a21, a22, a23, a24,
				a31, a32, a33, a34,
				a41, a42, a43, a44;
	// clang-format on
	Matrix4_base() :
		// clang-format off
			a11(1), a12(0), a13(0), a14(0), 
			a21(0), a22(1), a23(0), a24(0), 
			a31(0), a32(0), a33(1), a34(0), 
			a41(0), a42(0), a43(0), a44(1) { }
	// clang-format on

	Matrix4_base(
		// clang-format off
			T a, T b, T c, T d,
			T e, T f, T g, T h,
			T i, T j, T k, T l,
			T m, T n, T o, T p) :
			a11(a), a12(b), a13(c), a14(d),
			a21(e), a22(f), a23(g), a24(h),
			a31(i), a32(j), a33(k), a34(l),
			a41(m), a42(n), a43(o), a44(p) { }
	// clang-format on
};

template<typename T> struct Matrix4_base<T, COLUMN_MAJOR> {
public:
	// clang-format off
      T a11, a21, a31, a41,
        a12, a22, a32, a42,
        a13, a23, a33, a43,
        a14, a24, a34, a44;
	// clang-format on
	Matrix4_base() :
		// clang-format off
			a11(1), a21(0), a31(0), a41(0), 
			a12(0), a22(1), a32(0), a42(0), 
			a13(0), a23(0), a33(1), a43(0), 
			a14(0), a24(0), a34(0), a44(1) { }
	// clang-format on

	Matrix4_base(
		// clang-format off
			T a, T b, T c, T d,
			T e, T f, T g, T h,
			T i, T j, T k, T l,
			T m, T n, T o, T p) :

			a11(a), a21(e), a31(i), a41(m),
			a12(b), a22(f), a32(j), a42(n),
			a13(c), a23(g), a33(k), a43(o),
			a14(d), a24(h), a34(l), a44(p) { }
	// clang-format on
};

template<class T, matrix_storage_type_t stor = ROW_MAJOR>
struct Matrix4 : public Matrix4_base<T, stor> {
	using Matrix4_base<T, stor>::a11;
	using Matrix4_base<T, stor>::a12;
	using Matrix4_base<T, stor>::a13;
	using Matrix4_base<T, stor>::a14;
	using Matrix4_base<T, stor>::a21;
	using Matrix4_base<T, stor>::a22;
	using Matrix4_base<T, stor>::a23;
	using Matrix4_base<T, stor>::a24;
	using Matrix4_base<T, stor>::a31;
	using Matrix4_base<T, stor>::a32;
	using Matrix4_base<T, stor>::a33;
	using Matrix4_base<T, stor>::a34;
	using Matrix4_base<T, stor>::a41;
	using Matrix4_base<T, stor>::a42;
	using Matrix4_base<T, stor>::a43;
	using Matrix4_base<T, stor>::a44;

public:
	using Matrix4_base<T, stor>::Matrix4_base;

	void set(Matrix4 m) {
		// clang-format off
			a11=m.a11; a12=m.a12; a13=m.a13; a14=m.a14;
			a21=m.a21; a22=m.a22; a23=m.a23; a24=m.a24;
			a31=m.a31; a32=m.a32; a33=m.a33; a34=m.a34;
			a41=m.a41; a42=m.a42; a43=m.a43; a44=m.a44;
		// clang-format on
	}

	void set(
		// clang-format off
			T a, T b, T c, T d,
			T e, T f, T g, T h,
			T i, T j, T k, T l,
			T m, T n, T o, T p) {
			a11=a; a12=b; a13=c; a14=d;
			a21=e; a22=f; a23=g; a24=h;
			a31=i; a32=j; a33=k; a34=l;
			a41=m; a42=n; a43=o; a44=p;
		// clang-format on
	}

	void setDiagonal(T a, T b, T c, T d) {
		// clang-format off
			a11=a; a12=0; a13=0; a14=0;
			a21=0; a22=b; a23=0; a24=0;
			a31=0; a32=0; a33=c; a34=0;
			a41=0; a42=0; a43=0; a44=d;
		// clang-format on
	}

	static Matrix4 Diagonal(T x, T y, T z, T w) {
		// clang-format off
			return Matrix4(
				x,0,0,0,
				0,y,0,0,
				0,0,z,0,
				0,0,0,w);
		// clang-format on
	}

	void setIdentity() {
		// clang-format off
			a11=1; a12=0; a13=0; a14=0;
			a21=0; a22=1; a23=0; a24=0;
			a31=0; a32=0; a33=1; a34=0;
			a41=0; a42=0; a43=0; a44=1;
		// clang-format on
	}

	void setPerspective(T fov, T aspectInv, T zNear, T zFar) {
		T a = T(1.0 / tan(fov * (constants<T>::pi_360)));
		T b = T(1.0 / (zNear - zFar));
		// clang-format off
			set(
				0              ,-a,0          ,0,
				0              ,0 ,a*aspectInv,0,
				-(zFar+zNear)*b,0 ,0          ,2*zFar*zNear*b,
				1              ,0 ,0          ,0);
		// clang-format on
	}

	static Matrix4 Perspective(T fov, T aspectInv, T zNear, T zFar) {
		T a = T(1.0 / tan(fov * (constants<T>::pi_360)));
		T b = T(1.0 / (zNear - zFar));
		// clang-format off
			return Matrix4(
				0              ,-a,0          ,0,
				0              ,0 ,a*aspectInv,0,
				-(zFar+zNear)*b,0 ,0          ,2*zFar*zNear*b,
				1              ,0 ,0          ,0);
		// clang-format on
	}

	void setOrthogonalProjection(T l, T r, T b, T t, T n, T f) {
		// clang-format off
			set(
				T(2.0/(r-l)),0           ,0            ,-(r+l)/(r-l),
				0           ,T(2.0/(t-b)),0            ,-(t+b)/(t-b),
				0           ,0           ,T(-2.0/(f-n)),-(f+n)/(f-n),
				0           ,0           ,0            ,1           );
		// clang-format on
	}

	static Matrix4 OrthogonalProjection(T l, T r, T b, T t, T n, T f) {
		// clang-format off
			return Matrix4(
				T(2.0/(r-l)),0           ,0            ,-(r+l)/(r-l),
				0           ,T(2.0/(t-b)),0            ,-(t+b)/(t-b),
				0           ,0           ,T(-2.0/(f-n)),-(f+n)/(f-n),
				0           ,0           ,0            ,1           );
		// clang-format on
	}

	void setRotationX(T ang) {
		T s = T(sin(ang)), c = T(cos(ang));
		// clang-format off
			set(
				1, 0,  0, 0,
				0, c, -s, 0,
				0, s,  c, 0,
				0, 0,  0, 1);
		// clang-format on
	}
	static Matrix4 RotationX(T ang) {
		T s = T(sin(ang)), c = T(cos(ang));
		// clang-format off
			return Matrix4(
				1, 0,  0, 0,
				0, c, -s, 0,
				0, s,  c, 0,
				0, 0,  0, 1);
		// clang-format on
	}
	void setRotationY(T ang) {
		T s = T(sin(ang)), c = T(cos(ang));
		// clang-format off
			set(
				c, 0, s, 0,
				0, 1, 0, 0,
				-s, 0, c, 0,
				0, 0, 0, 1);
		// clang-format on
	}
	static Matrix4 RotationY(T ang) {
		T s = T(sin(ang)), c = T(cos(ang));
		// clang-format off
			return Matrix4(
				c, 0, s, 0,
				0, 1, 0, 0,
				-s, 0, c, 0,
				0, 0, 0, 1);
		// clang-format on
	}
	void setRotationZ(T ang) {
		T s = T(sin(ang)), c = T(cos(ang));
		// clang-format off
			set(
				c, -s, 0, 0,
				s,  c, 0, 0,
				0,  0, 1, 0,
				0,  0, 0, 1);
		// clang-format on
	}
	static Matrix4 RotationZ(T ang) {
		T s = T(sin(ang)), c = T(cos(ang));
		// clang-format off
			return Matrix4(
				c, -s, 0, 0,
				s,  c, 0, 0,
				0,  0, 1, 0,
				0,  0, 0, 1);
		// clang-format on
	}

	void setRotation(T ang, const Vector<3, T> &axis) {
		Vector<3, T> axis_u = axis.normalized();
		T            cs = T(cos(ang)), sn = T(sin(ang));
		T            x = axis_u.x(), y = axis_u.y(), z = axis_u.z();
		T            cn = 1 - cs;
		// clang-format off
			set(
				cn*x*x+  cs, cn*x*y-z*sn, cn*x*z+y*sn, 0,
				cn*x*y+z*sn, cn*y*y+  cs, cn*y*z-x*sn, 0,
				cn*x*z-y*sn, cn*y*z+x*sn, cn*z*z+  cs, 0,
				0          , 0          , 0          , 1);
		// clang-format on
	}

	static Matrix4 Rotation(T ang, const Vector<3, T> &axis) {
		Vector<3, T> axis_u = axis.normalized();
		T            cs = T(cos(ang)), sn = T(sin(ang));
		T            x = axis_u.x(), y = axis_u.y(), z = axis_u.z();
		T            cn = 1 - cs;
		// clang-format off
			return Matrix4(
				cn*x*x+  cs, cn*x*y-z*sn, cn*x*z+y*sn, 0,
				cn*x*y+z*sn, cn*y*y+  cs, cn*y*z-x*sn, 0,
				cn*x*z-y*sn, cn*y*z+x*sn, cn*z*z+  cs, 0,
				0          , 0          , 0          , 1);
		// clang-format on
	}

	void setTranslation(T x, T y, T z) {
		// clang-format off
			set(
				1,0,0,x,
				0,1,0,y,
				0,0,1,z,
				0,0,0,1);
		// clang-format on
	}
	static Matrix4 Translation(T x, T y, T z) {
		// clang-format off
			return Matrix4(
				1,0,0,x,
				0,1,0,y,
				0,0,1,z,
				0,0,0,1);
		// clang-format on
	}

	void setTranslation(const Vector<3, T> &v) {
		// clang-format off
			set(
				1,0,0,v.x(),
				0,1,0,v.y(),
				0,0,1,v.z(),
				0,0,0,1);
		// clang-format on
	}
	static Matrix4 Translation(const Vector<3, T> &v) {
		// clang-format off
			return Matrix4(
				1,0,0,v.x(),
				0,1,0,v.y(),
				0,0,1,v.z(),
				0,0,0,1);
		// clang-format on
	}

	static Matrix4 TwoPointTransform(
		const Vector<2, T> &a0,
		const Vector<2, T>  b0,
		const Vector<2, T> &a1,
		const Vector<2, T>  b1) {
		Vector<2, T> u1 = b1 - a1, u0 = b0 - a0;

		T angle = atan2(u0.x(), u0.y()) - atan2(u1.x(), u1.y());
		T sn = sin(angle), cs = cos(angle);

		T s = sqrt(u1.square() / u0.square());

		// clang-format off
			return 
				Matrix4(
					cs*s, -sn*s, 0, a1.x(),
					sn*s,  cs*s, 0, a1.y(),
						0,     0, 1, 0,
						0,     0, 0, 1) *

				Translation(-a0.x(),-a0.y(),0);
		// clang-format on
	}

	Matrix4 operator+(const Matrix4 &m) const {
		Matrix4 r;
		// clang-format off
			r.a11=a11+m.a11; r.a12=a12+m.a12; r.a13=a13+m.a13; r.a14=a14+m.a14;
			r.a21=a21+m.a21; r.a22=a22+m.a22; r.a23=a23+m.a23; r.a24=a24+m.a24;
			r.a31=a31+m.a31; r.a32=a32+m.a32; r.a33=a33+m.a33; r.a34=a34+m.a34;
			r.a41=a41+m.a41; r.a42=a42+m.a42; r.a43=a43+m.a43; r.a44=a44+m.a44;
		// clang-format on
		return r;
	}

	void operator+=(const Matrix4 &m) {
		// clang-format off
			a11+=m.a11; a12+=m.a12; a13+=m.a13; a14+=m.a14;
			a21+=m.a21; a22+=m.a22; a23+=m.a23; a24+=m.a24;
			a31+=m.a31; a32+=m.a32; a33+=m.a33; a34+=m.a34;
			a41+=m.a41; a42+=m.a42; a43+=m.a43; a44+=m.a44;
		// clang-format on
	}

	Matrix4 operator-(const Matrix4 &m) const {
		Matrix4 r;
		// clang-format off
			r.a11=a11-m.a11; r.a12=a12-m.a12; r.a13=a13-m.a13; r.a14=a14-m.a14;
			r.a21=a21-m.a21; r.a22=a22-m.a22; r.a23=a23-m.a23; r.a24=a24-m.a24;
			r.a31=a31-m.a31; r.a32=a32-m.a32; r.a33=a33-m.a33; r.a34=a34-m.a34;
			r.a41=a41-m.a41; r.a42=a42-m.a42; r.a43=a43-m.a43; r.a44=a44-m.a44;
		// clang-format on
		return r;
	}

	void operator-=(const Matrix4 &m) {
		// clang-format off
			a11-=m.a11; a12-=m.a12; a13-=m.a13; a14-=m.a14;
			a21-=m.a21; a22-=m.a22; a23-=m.a23; a24-=m.a24;
			a31-=m.a31; a32-=m.a32; a33-=m.a33; a34-=m.a34;
			a41-=m.a41; a42-=m.a42; a43-=m.a43; a44-=m.a44;
		// clang-format on
	}

	Matrix4 operator-() {
		Matrix4 r;
		// clang-format off
			r.a11=-a11; r.a12=-a12; r.a13=-a13; r.a14=-a14;
			r.a21=-a21; r.a22=-a22; r.a23=-a23; r.a24=-a24;
			r.a31=-a31; r.a32=-a32; r.a33=-a33; r.a34=-a34;
			r.a41=-a41; r.a42=-a42; r.a43=-a43; r.a44=-a44;
		// clang-format on
		return r;
	}

	Matrix4 operator*(T f) const {
		Matrix4 r;
		// clang-format off
			r.a11=a11*f; r.a12=a12*f; r.a13=a13*f; r.a14=a14*f;
			r.a21=a21*f; r.a22=a22*f; r.a23=a23*f; r.a24=a24*f;
			r.a31=a31*f; r.a32=a32*f; r.a33=a33*f; r.a34=a34*f;
			r.a41=a41*f; r.a42=a42*f; r.a43=a43*f; r.a44=a44*f;
		// clang-format on
		return r;
	}
	friend Matrix4 operator*(T f, const Matrix4 &m) {
		Matrix4 r;
		// clang-format off
			r.a11=m.a11*f; r.a12=m.a12*f; r.a13=m.a13*f; r.a14=m.a14*f;
			r.a21=m.a21*f; r.a22=m.a22*f; r.a23=m.a23*f; r.a24=m.a24*f;
			r.a31=m.a31*f; r.a32=m.a32*f; r.a33=m.a33*f; r.a34=m.a34*f;
			r.a41=m.a41*f; r.a42=m.a42*f; r.a43=m.a43*f; r.a44=m.a44*f;
		// clang-format on
		return r;
	}

	Matrix4 operator*=(T f) {
		Matrix4 r;
		// clang-format off
			a11*=f; a12*=f; a13*=f; a14*=f;
			a21*=f; a22*=f; a23*=f; a24*=f;
			a31*=f; a32*=f; a33*=f; a34*=f;
			a41*=f; a42*=f; a43*=f; a44*=f;
		// clang-format on

		return r;
	}

	Vector<4, T> operator*(const Vector<4, T> &v) const {
		Vector<4, T> r;
		// clang-format off
			r.x() = a11*v.x() + a12*v.y() + a13*v.z() + a14*v.w(); 
			r.y() = a21*v.x() + a22*v.y() + a23*v.z() + a24*v.w(); 
			r.z() = a31*v.x() + a32*v.y() + a33*v.z() + a34*v.w(); 
			r.w() = a41*v.x() + a42*v.y() + a43*v.z() + a44*v.w();
		// clang-format on
		return r;
	}

	// assume (x,y,z,1) coordinates here, useful for transformation
	Vector<3, T> operator*(const Vector<3, T> &v) const {
		Vector<3, T> r;
		// clang-format off
			r.x()=a11*v.x()+a12*v.y()+a13*v.z()+a14; 
			r.y()=a21*v.x()+a22*v.y()+a23*v.z()+a24; 
			r.z()=a31*v.x()+a32*v.y()+a33*v.z()+a34;
		// clang-format on
		return r;
	}

	// assume (x,y,0,1) coordinates here, useful for transformation
	Vector<2, T> operator*(const Vector<2, T> &v) const {
		Vector<2, T> r;
		r.x() = a11 * v.x() + a12 * v.y() + a14;
		r.y() = a21 * v.x() + a22 * v.y() + a24;
		return r;
	}

	Matrix4 operator*(const Matrix4 &m) const {
		Matrix4 r;
		// clang-format off
			r.a11 = a11*m.a11 + a12*m.a21 + a13*m.a31 + a14*m.a41;
			r.a12 = a11*m.a12 + a12*m.a22 + a13*m.a32 + a14*m.a42;
			r.a13 = a11*m.a13 + a12*m.a23 + a13*m.a33 + a14*m.a43;
			r.a14 = a11*m.a14 + a12*m.a24 + a13*m.a34 + a14*m.a44;
			
			r.a21 = a21*m.a11 + a22*m.a21 + a23*m.a31 + a24*m.a41;
			r.a22 = a21*m.a12 + a22*m.a22 + a23*m.a32 + a24*m.a42;
			r.a23 = a21*m.a13 + a22*m.a23 + a23*m.a33 + a24*m.a43;
			r.a24 = a21*m.a14 + a22*m.a24 + a23*m.a34 + a24*m.a44;
			
			r.a31 = a31*m.a11 + a32*m.a21 + a33*m.a31 + a34*m.a41;
			r.a32 = a31*m.a12 + a32*m.a22 + a33*m.a32 + a34*m.a42;
			r.a33 = a31*m.a13 + a32*m.a23 + a33*m.a33 + a34*m.a43;
			r.a34 = a31*m.a14 + a32*m.a24 + a33*m.a34 + a34*m.a44;
			
			r.a41 = a41*m.a11 + a42*m.a21 + a43*m.a31 + a44*m.a41;
			r.a42 = a41*m.a12 + a42*m.a22 + a43*m.a32 + a44*m.a42;
			r.a43 = a41*m.a13 + a42*m.a23 + a43*m.a33 + a44*m.a43;
			r.a44 = a41*m.a14 + a42*m.a24 + a43*m.a34 + a44*m.a44;
		// clang-format on

		return r;
	}

	void operator*=(const Matrix4 &m) {
		// clang-format off
			set(
				a11*m.a11 + a12*m.a21 + a13*m.a31 + a14*m.a41,
				a11*m.a12 + a12*m.a22 + a13*m.a32 + a14*m.a42,
				a11*m.a13 + a12*m.a23 + a13*m.a33 + a14*m.a43,
				a11*m.a14 + a12*m.a24 + a13*m.a34 + a14*m.a44,
				
				a21*m.a11 + a22*m.a21 + a23*m.a31 + a24*m.a41,
				a21*m.a12 + a22*m.a22 + a23*m.a32 + a24*m.a42,
				a21*m.a13 + a22*m.a23 + a23*m.a33 + a24*m.a43,
				a21*m.a14 + a22*m.a24 + a23*m.a34 + a24*m.a44,
				
				a31*m.a11 + a32*m.a21 + a33*m.a31 + a34*m.a41,
				a31*m.a12 + a32*m.a22 + a33*m.a32 + a34*m.a42,
				a31*m.a13 + a32*m.a23 + a33*m.a33 + a34*m.a43,
				a31*m.a14 + a32*m.a24 + a33*m.a34 + a34*m.a44,
				
				a41*m.a11 + a42*m.a21 + a43*m.a31 + a44*m.a41,
				a41*m.a12 + a42*m.a22 + a43*m.a32 + a44*m.a42,
				a41*m.a13 + a42*m.a23 + a43*m.a33 + a44*m.a43,
				a41*m.a14 + a42*m.a24 + a43*m.a34 + a44*m.a44);
		// clang-format on
	}

	Matrix4 operator/(T f) const {
		Matrix4 r;
		// clang-format off
			r.a11=a11/f; r.a12=a12/f; r.a13=a13/f; r.a14=a14/f;
			r.a21=a21/f; r.a22=a22/f; r.a23=a23/f; r.a24=a24/f;
			r.a31=a31/f; r.a32=a32/f; r.a33=a33/f; r.a34=a34/f;
			r.a41=a41/f; r.a42=a42/f; r.a43=a43/f; r.a44=a44/f;
		// clang-format on
		return r;
	}

	Matrix4 operator/=(T f) {
		Matrix4 r;
		// clang-format off
			a11/=f; a12/=f; a13/=f; a14/=f;
			a21/=f; a22/=f; a23/=f; a24/=f;
			a31/=f; a32/=f; a33/=f; a34/=f;
			a41/=f; a42/=f; a43/=f; a44/=f;
		// clang-format on

		return r;
	}

	Matrix4 transposed() const {
		// clang-format off
			return Matrix4(
				a11, a21, a31, a41,
				a12, a22, a32, a42,
				a13, a23, a33, a43,
				a14, a24, a34, a44);
		// clang-format on
	}

	void transpose() {
		// clang-format off
			set(
				a11, a21, a31, a41,
				a12, a22, a32, a42,
				a13, a23, a33, a43,
				a14, a24, a34, a44);
		// clang-format on
	}

	Matrix4 getRowMajor() const {
		if constexpr (stor == ROW_MAJOR)
			return *this;
		else
			return transposed();
	}
	Matrix4 getColMajor() const {
		if constexpr (stor == COLUMN_MAJOR)
			return *this;
		else
			return transposed();
	}

	T det() const {
		// clang-format off
			return 
				a14*a23*a32*a41 - a13*a24*a32*a41 - a14*a22*a33*a41 + a12*a24*a33*a41 + 
				a13*a22*a34*a41 - a12*a23*a34*a41 - a14*a23*a31*a42 + a13*a24*a31*a42 + 
				a14*a21*a33*a42 - a11*a24*a33*a42 - a13*a21*a34*a42 + a11*a23*a34*a42 + 
				a14*a22*a31*a43 - a12*a24*a31*a43 - a14*a21*a32*a43 + a11*a24*a32*a43 + 
				a12*a21*a34*a43 - a11*a22*a34*a43 - a13*a22*a31*a44 + a12*a23*a31*a44 + 
				a13*a21*a32*a44 - a11*a23*a32*a44 - a12*a21*a33*a44 + a11*a22*a33*a44;
		// clang-format on
	}

	T tr() const { return a11 + a12 + a13 + a14; }

	Matrix4 inverse() const {
		Matrix4 r;
		T       d = det();
		// If a matrix is not invertible, a wrong result is favorable to -inf.
		if (d == 0)
			d = 1;
		else
			d = T(1.0 / d);

		// clang-format off
			r.a11 = d*(
				a23*a34*a42 - a24*a33*a42 + a24*a32*a43 - 
				a22*a34*a43 - a23*a32*a44 + a22*a33*a44);
			r.a12 = d*(
				a14*a33*a42 - a13*a34*a42 - a14*a32*a43 + 
				a12*a34*a43 + a13*a32*a44 - a12*a33*a44);
			r.a13 = d*(
				a13*a24*a42 - a14*a23*a42 + a14*a22*a43 - 
				a12*a24*a43 - a13*a22*a44 + a12*a23*a44);
			r.a14 = d*(
				a14*a23*a32 - a13*a24*a32 - a14*a22*a33 + 
				a12*a24*a33 + a13*a22*a34 - a12*a23*a34);
			r.a21 = d*(
				a24*a33*a41 - a23*a34*a41 - a24*a31*a43 + 
				a21*a34*a43 + a23*a31*a44 - a21*a33*a44);
			r.a22 = d*(
				a13*a34*a41 - a14*a33*a41 + a14*a31*a43 - 
				a11*a34*a43 - a13*a31*a44 + a11*a33*a44);
			r.a23 = d*(
				a14*a23*a41 - a13*a24*a41 - a14*a21*a43 + 
				a11*a24*a43 + a13*a21*a44 - a11*a23*a44);
			r.a24 = d*(
				a13*a24*a31 - a14*a23*a31 + a14*a21*a33 - 
				a11*a24*a33 - a13*a21*a34 + a11*a23*a34);
			r.a31 = d*(
				a22*a34*a41 - a24*a32*a41 + a24*a31*a42 - 
				a21*a34*a42 - a22*a31*a44 + a21*a32*a44);
			r.a32 = d*(
				a14*a32*a41 - a12*a34*a41 - a14*a31*a42 + 
				a11*a34*a42 + a12*a31*a44 - a11*a32*a44);
			r.a33 = d*(
				a12*a24*a41 - a14*a22*a41 + a14*a21*a42 - 
				a11*a24*a42 - a12*a21*a44 + a11*a22*a44);
			r.a34 = d*(
				a14*a22*a31 - a12*a24*a31 - a14*a21*a32 + 
				a11*a24*a32 + a12*a21*a34 - a11*a22*a34);
			r.a41 = d*(
				a23*a32*a41 - a22*a33*a41 - a23*a31*a42 + 
				a21*a33*a42 + a22*a31*a43 - a21*a32*a43);
			r.a42 = d*(
				a12*a33*a41 - a13*a32*a41 + a13*a31*a42 - 
				a11*a33*a42 - a12*a31*a43 + a11*a32*a43);
			r.a43 = d*(
				a13*a22*a41 - a12*a23*a41 - a13*a21*a42 + 
				a11*a23*a42 + a12*a21*a43 - a11*a22*a43);
			r.a44 = d*(
				a12*a23*a31 - a13*a22*a31 + a13*a21*a32 - 
				a11*a23*a32 - a12*a21*a33 + a11*a22*a33);
		// clang-format on
		return r;
	}

	void invert() {
		T d = det();
		if (d == 0) return;
		d = T(1.0 / d);
		// clang-format off
			set(
				d*(a23*a34*a42 - a24*a33*a42 + a24*a32*a43 -
				   a22*a34*a43 - a23*a32*a44 + a22*a33*a44),
				d*(a14*a33*a42 - a13*a34*a42 - a14*a32*a43 +
				   a12*a34*a43 + a13*a32*a44 - a12*a33*a44),
				d*(a13*a24*a42 - a14*a23*a42 + a14*a22*a43 -
				   a12*a24*a43 - a13*a22*a44 + a12*a23*a44),
				d*(a14*a23*a32 - a13*a24*a32 - a14*a22*a33 +
				   a12*a24*a33 + a13*a22*a34 - a12*a23*a34),
				d*(a24*a33*a41 - a23*a34*a41 - a24*a31*a43 +
				   a21*a34*a43 + a23*a31*a44 - a21*a33*a44),
				d*(a13*a34*a41 - a14*a33*a41 + a14*a31*a43 -
				   a11*a34*a43 - a13*a31*a44 + a11*a33*a44),
				d*(a14*a23*a41 - a13*a24*a41 - a14*a21*a43 +
				   a11*a24*a43 + a13*a21*a44 - a11*a23*a44),
				d*(a13*a24*a31 - a14*a23*a31 + a14*a21*a33 -
				   a11*a24*a33 - a13*a21*a34 + a11*a23*a34),
				d*(a22*a34*a41 - a24*a32*a41 + a24*a31*a42 -
				   a21*a34*a42 - a22*a31*a44 + a21*a32*a44),
				d*(a14*a32*a41 - a12*a34*a41 - a14*a31*a42 +
				   a11*a34*a42 + a12*a31*a44 - a11*a32*a44),
				d*(a12*a24*a41 - a14*a22*a41 + a14*a21*a42 -
				   a11*a24*a42 - a12*a21*a44 + a11*a22*a44),
				d*(a14*a22*a31 - a12*a24*a31 - a14*a21*a32 +
				   a11*a24*a32 + a12*a21*a34 - a11*a22*a34),
				d*(a23*a32*a41 - a22*a33*a41 - a23*a31*a42 +
				   a21*a33*a42 + a22*a31*a43 - a21*a32*a43),
				d*(a12*a33*a41 - a13*a32*a41 + a13*a31*a42 -
				   a11*a33*a42 - a12*a31*a43 + a11*a32*a43),
				d*(a13*a22*a41 - a12*a23*a41 - a13*a21*a42 +
				   a11*a23*a42 + a12*a21*a43 - a11*a22*a43),
				d*(a12*a23*a31 - a13*a22*a31 + a13*a21*a32 -
				   a11*a23*a32 - a12*a21*a33 + a11*a22*a33));
		// clang-format on
	}

	Vector<4, T> col1() const { return Vector<4, T>(a11, a21, a31, a41); }
	Vector<4, T> col2() const { return Vector<4, T>(a12, a22, a32, a42); }
	Vector<4, T> col3() const { return Vector<4, T>(a13, a23, a33, a43); }
	Vector<4, T> col4() const { return Vector<4, T>(a14, a24, a34, a44); }

	Vector<4, T> row1() const { return Vector<4, T>(a11, a12, a13, a14); }
	Vector<4, T> row2() const { return Vector<4, T>(a21, a22, a23, a24); }
	Vector<4, T> row3() const { return Vector<4, T>(a31, a32, a33, a34); }
	Vector<4, T> row4() const { return Vector<4, T>(a41, a42, a43, a44); }

	friend std::ostream &operator<<(std::ostream &o, const Matrix4<T, stor> &m) {
		// clang-format off
			return o
			  << m.a11 << " "  << m.a12 << " " << m.a13 << " " << m.a14 << std::endl
			  << m.a21 << " "  << m.a22 << " " << m.a23 << " " << m.a24 << std::endl
			  << m.a31 << " "  << m.a32 << " " << m.a33 << " " << m.a34 << std::endl
			  << m.a41 << " "  << m.a42 << " " << m.a43 << " " << m.a44 << std::endl;
			// clm.ang-format on
		}
};

typedef Matrix4<float>  mat4f;
typedef Matrix4<double> mat4d;
}

extern template struct alp::Matrix4<float>;
extern template struct alp::Matrix4<double>;
#endif