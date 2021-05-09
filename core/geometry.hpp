#pragma once
#include <math.h>
//Vetor3
template <class T>
class Vector3 {
public:
	union {
		struct {
			T x, y, z;
		};
		T rows[3];
	};
	
	Vector3();
	Vector3(T x, T y, T z = 0);
	Vector3<T> operator +(const Vector3<T>& v) const;
	Vector3<T> operator -(const Vector3<T>& v) const;
	Vector3<T> operator *(const T& t) const;
	Vector3<T> operator *(const Vector3<T>& v) const;
	Vector3<T> operator /(const Vector3<T>& v) const;
	Vector3<T> operator /(const T& t) const;
	Vector3<T>& normalize();
};

typedef Vector3<float> Vec3f;
typedef Vector3<int> Vec3i;

template<class T>
inline Vector3<T>::Vector3() : x(0), y(0), z(0)
{
}

template<class T>
inline Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z)
{
}

template<class T>
inline Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template<class T>
inline Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const
{
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template<class T>
inline Vector3<T> Vector3<T>::operator*(const T& t) const
{
	return Vector3<T>(x * t, y * t, z * t);
}

template<class T>
inline Vector3<T> Vector3<T>::operator*(const Vector3<T>& v) const
{
	return Vector3<T>(x * v.x, y * v.y, z * v.z);
}


template<class T>
inline Vector3<T> Vector3<T>::operator/(const Vector3<T>& v) const
{
	return Vector3<T>(x / v.x, y / v.y, z / v.z);
}

template<class T>
inline Vector3<T> Vector3<T>::operator/(const T& t) const
{
	return Vector3<T>(x / t, y / t, z / t);
}

template<class T>
inline Vector3<T>& Vector3<T>::normalize()
{
	T len = sqrt(dot(*this, *this));
	*this =(*this)/len;
	return *this;
}
