#include "geometry.hpp"

template <typename T>
Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2)
{
	T _x = v1.y * v2.z - v1.z * v2.y;
	T _y = v1.z * v2.x - v1.x * v2.z;
	T _z = v1.x * v2.y - v1.y * v2.x;
	return Vector3<T>(_x, _y, _z);
}

template <typename T>
T dot(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

