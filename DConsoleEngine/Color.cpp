#include "Color.h"

template class Draggoon::Color<bool>;
template class Draggoon::Color<char>;
template class Draggoon::Color<short>;
template class Draggoon::Color<unsigned char>;
template class Draggoon::Color<unsigned short>;
template class Draggoon::Color<long>;
template class Draggoon::Color<unsigned long>;
template class Draggoon::Color<long long>;
template class Draggoon::Color<unsigned long long>;
template class Draggoon::Color<int>;
template class Draggoon::Color<unsigned int>;
template class Draggoon::Color<float>;
template class Draggoon::Color<double>;
template class Draggoon::Color<long double>;

template<class T>
Draggoon::Color<T>::Color(T _r, T _g, T _b, bool t_transparent) : m_r(_r), m_g(_g), m_b(_b), m_transparent(t_transparent) {

}

template<class T>
T Draggoon::Color<T>::getR() const {
	return m_r;
}

template<class T>
T Draggoon::Color<T>::getG() const {
	return m_g;
}

template<class T>
T Draggoon::Color<T>::getB() const {
	return m_b;
}

template<class T>
bool Draggoon::Color<T>::isTransparent() const {
	return m_transparent;
}

template<class T>
std::tuple<T, T, T> Draggoon::Color<T>::getRGB() const {
	return {m_r, m_g, m_b};
}


Draggoon::Color<float> Draggoon::COLOR_F_WHITE(1.0f, 1.0f, 1.0f);
Draggoon::Color<float> Draggoon::COLOR_F_RED(1.0f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_GREEN(0.0f, 1.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_BLUE(0.0f, 0.0f, 1.0f);
Draggoon::Color<float> Draggoon::COLOR_F_GRAY(0.5f, 0.5f, 0.5f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_RED(0.5f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_GREEN(0.0f, 0.5f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_BLUE(0.0f, 0.0f, 0.5f);
Draggoon::Color<float> Draggoon::COLOR_F_BLACK(0.0f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_TRANSPARENT(0.0f, 0.0f, 0.0f, true);