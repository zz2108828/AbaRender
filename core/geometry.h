#pragma once
#include <math.h>
#include <vector>

namespace aba
{
// Vetor3
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
	friend Vector3<T> operator *(const T& t, const Vector3<T>& rhs) { return rhs * t; }
	Vector3<T> operator *(const Vector3<T>& v) const;
	Vector3<T> operator /(const Vector3<T>& v) const;
	Vector3<T> operator /(const T& t) const;
	T& operator [](const int t);
	Vector3<T> normalize();
};

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
inline T& Vector3<T>::operator[](const int t)
{
	return rows[t];
}

template<class T>
inline Vector3<T> Vector3<T>::normalize()
{
	Vector3<T> result;
	T len = sqrt(dot(*this, *this));
	result =(*this)/len;
	return result;
}

template <typename T> Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2)
{
	T _x = v1.y * v2.z - v1.z * v2.y;
	T _y = v1.z * v2.x - v1.x * v2.z;
	T _z = v1.x * v2.y - v1.y * v2.x;
	return Vector3<T>(_x, _y, _z);
}
template <typename T> T dot(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

typedef Vector3<float> Vec3f;
typedef Vector3<int> Vec3i;

class Matrix {
private:
	std::vector<std::vector<float> > matrix_;
	int rows_, cols_;
public:
	Matrix(uint32_t r = 4, uint32_t c = 4);
	Matrix(const Vec3f& v);
	uint32_t getRows();
	uint32_t getCols();
	std::vector<float> operator[] (const uint32_t i) const;
	Matrix operator*(const Matrix& b);
	Matrix operator*(const int b);
	Matrix transpose();
	Matrix inverse();
	static Matrix identity(int dimensions);
};


}
