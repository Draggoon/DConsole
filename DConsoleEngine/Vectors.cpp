#include "Vectors.h"

template class Draggoon::Vector2D<char>;
template class Draggoon::Vector2D<short>;
template class Draggoon::Vector2D<long>;
template class Draggoon::Vector2D<long long>;
template class Draggoon::Vector2D<int>;
template class Draggoon::Vector2D<float>;
template class Draggoon::Vector2D<double>;
template class Draggoon::Vector2D<long double>;


template <class T>
Draggoon::Vector2D<T>::Vector2D(const T& t_x, const T& t_y) : m_x(t_x), m_y(t_y) {
	if (std::is_unsigned<T>::value)
		throw "Draggoon::Vector2D::Vector2D: Unsigned value used!";
}

template<class T>
T Draggoon::Vector2D<T>::getX() const {
	return m_x;
}

template<class T>
T Draggoon::Vector2D<T>::getY() const {
	return m_y;
}

template<class T>
void Draggoon::Vector2D<T>::setX(const T& t_x) {
	m_x = t_x;
}

template<class T>
void Draggoon::Vector2D<T>::setY(const T& t_y) {
	m_y = t_y;
}

template<class T>
T Draggoon::Vector2D<T>::getLength() const {
	return (T)sqrt(m_x*m_x + m_y*m_y);
}

template<class T>
T Draggoon::Vector2D<T>::getArea() const {
	return m_x * m_y;
}

template<class T>
T Draggoon::Vector2D<T>::distanceTo(const Vector2D<T> t_v) {
	T d_x(abs(t_v.m_x - m_x)), d_y(abs(t_v.m_y - m_y));
	return (T)sqrt(d_x*d_x + d_y*d_y);
}

template<class T>
bool Draggoon::Vector2D<T>::contains(const Vector2D<T> t_v) const {
	bool x(false), y(false);
	if (m_x >= 0 && t_v.m_x >= 0) {
		x = t_v.m_x <= m_x;
	}
	else if (m_x <= 0 && t_v.m_x <= 0) {
		x = t_v.m_x >= m_x;
	}
	if (m_y >= 0 && t_v.m_y >= 0) {
		y = t_v.m_y <= m_y;
	}
	else if (m_y <= 0 && t_v.m_y <= 0) {
		y = t_v.m_y >= m_y;
	}
	return x && y;
}

template<class T>
bool Draggoon::Vector2D<T>::isContainedIn(const Vector2D<T> t_v) const {
	bool x(false), y(false);
	if (m_x >= 0 && t_v.m_x >= 0) {
		x = t_v.m_x >= m_x;
	}
	else if (m_x <= 0 && t_v.m_x <= 0) {
		x = t_v.m_x <= m_x;
	}
	if (m_y >= 0 && t_v.m_y >= 0) {
		y = t_v.m_y >= m_y;
	}
	else if (m_y <= 0 && t_v.m_y <= 0) {
		y = t_v.m_y <= m_y;
	}
	return x && y;
}

template<class T>
bool Draggoon::Vector2D<T>::operator==(const Vector2D<T>& t_v) const {
	return t_v.m_x == m_x && t_v.m_y == m_y;
}

template<class T>
bool Draggoon::Vector2D<T>::operator!=(const Vector2D<T>& t_v) const {
	return t_v.m_x != m_x || t_v.m_y != m_y;
}


template class Draggoon::Vector3D<char>;
template class Draggoon::Vector3D<short>;
template class Draggoon::Vector3D<long>;
template class Draggoon::Vector3D<long long>;
template class Draggoon::Vector3D<int>;
template class Draggoon::Vector3D<float>;
template class Draggoon::Vector3D<double>;
template class Draggoon::Vector3D<long double>;


template<class T>
Draggoon::Vector3D<T>::Vector3D(const T& t_x, const T& t_y, const T& t_z) : Vector2D<T>(t_x, t_y), m_z(t_z) {

}

template<class T>
T Draggoon::Vector3D<T>::getZ() const {
	return m_z;
}

template<class T>
void Draggoon::Vector3D<T>::setZ(const T & t_z) {
	m_z = t_z;
}

template<class T>
T Draggoon::Vector3D<T>::getVolume() const {
	return this->m_x*this->m_y*m_z;
}

template<class T>
Draggoon::Vector2D<T> Draggoon::Vector3D<T>::projectionXY() const {
	return {this->m_x, this->m_y};
}

template<class T>
bool Draggoon::Vector3D<T>::contains(const Vector3D<T> t_v) const {
	bool z(false);
	if (m_z >= 0 && t_v.m_z >= 0)
		z = t_v.m_z <= m_z;
	else if (m_z <= 0 && t_v.m_z <= 0)
		z = t_v.m_z >= m_z;
	return projectionXY().contains({t_v.projectionXY()}) && z;
}

template<class T>
bool Draggoon::Vector3D<T>::isContainedIn(const Vector3D<T> t_v) const {
	bool z(false);
	if (m_z >= 0 && t_v.m_z >= 0)
		z = t_v.m_z >= m_z;
	else if (m_z <= 0 && t_v.m_z <= 0)
		z = t_v.m_z <= m_z;
	return projectionXY().isContainedIn({t_v.projectionXY()}) && z;
}

template<class T>
bool Draggoon::Vector3D<T>::operator==(const Vector3D<T>& t_v) const {
	return projectionXY() == t_v.projectionXY() && m_z == t_v.m_z;
}

template<class T>
bool Draggoon::Vector3D<T>::operator!=(const Vector3D<T>& t_v) const {
	return projectionXY() != t_v.projectionXY() || m_z != t_v.m_z;
}
