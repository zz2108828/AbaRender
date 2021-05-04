#pragma once
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
	//vector3<t> operator +(const vector3<t>& v) const;
	//vector3<t> operator -(const vector3<t>& v) const;
	//vector3<t> operator *(const vector3<t>& v) const;
	//t normalize() const;
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
