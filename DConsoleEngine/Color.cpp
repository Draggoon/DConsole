#include "Color.h"

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

template<class T>
char Draggoon::Color<T>::getConsoleColor() const {
	bool bright(false);
	bool r(false), g(false), b(false);
	if (m_r == m_g && m_r == m_b) {	// Grayscale
		if (std::is_floating_point<T>::value) {
			bright = m_r > 0.75 || (m_r <= 0.50 && m_r > 0.25);
			g = m_r > 0.50;
		}
		else {
			bright = m_r > (3*255/4) || (m_r <= (255/2) && m_r > (255/4));	// BE CAREFUL (Only valid with 0-255; To improve)
			g = m_r > (255/2);
		}

		return ((g?1:0) << 2) | ((g?1:0) << 1) | (g?1:0) | ((bright?1:0) <<3);
	}
	else {	// Color
		if (std::is_floating_point<T>::value) {
			bright = m_r > 0.5 || m_g > 0.5 || m_b > 0.5;
			r = m_r != 0.0f;
			g = m_g != 0.0f;
			b = m_b != 0.0f;
		}
		else {
			bright = m_r > (255/2) || m_g > (255/2) || m_b > (255/2);
			r = m_r != 0;
			g = m_g != 0;
			b = m_b != 0;
		}
		return ((r?1:0) << 2) | ((g?1:0) << 1) | (b?1:0) | ((bright?1:0) <<3);
	}
	
}

template<class T>
CHAR_INFO Draggoon::Color<T>::getConsoleChar() const {
	float r, g, b;
	if (!std::is_floating_point<T>::value) {
		r = (float)m_r / 255.0f;
		g = (float)m_g / 255.0f;
		b = (float)m_b / 255.0f;
	}
	else {
		r = (float)m_r;
		g = (float)m_g;
		b = (float)m_b;
	}

	float max, min;
	if (r > g && r > b) {
		max = r;
		if (b>g)
			min = g;
		else
			min = b;
	}
	else if (g > r && g > b) {
		max = g;
		if (r>b)
			min = b;
		else
			min = r;
	}
	else {
		max = b;
		if (g>r)
			min = r;
		else
			min = g;
	}

	float hue;
	float sat;
	float val(max);

	// Hue
	if(min == max)
		hue = 0.0f;
	else if (min == r)
		hue = 60.0f * (g-b)/(max-min);	// TODO simplify (remove 60 factor)
	else if (min == g)
		hue = 120.0f + 60.0f * (b-r)/(max-min);
	else
		hue = 240.0f + 60.0f * (r-g)/(max-min);

	if (hue < 0.0f)
		hue += 360.0f;

	// Saturation
	if (max == 0)
		sat = 0;
	else
		sat = (max-min) / max;

	// Console color translation
	wchar_t shadeChar;
	char charColor, backColor;
	CHAR_INFO ret;

	if (hue < 30.0f || hue >= 330.0f)
		backColor = 0b0100;	// Red
	else if (hue < 90.0f)
		backColor = 0b0110;	// Yellow
	else if (hue < 150.0f)
		backColor = 0b0010;	// Green
	else if (hue < 210.0f)
		backColor = 0b0011;	// Cyan
	else if (hue < 270.0f)
		backColor = 0b0001;	// Blue
	else
		backColor = 0b0101;	// Magenta


	if (sat < 0.10f) {	// TODO tweak	// GrayScale
		charColor = 0b0000;	// Black shade
		if (val < 1.0f/16.0f) {
			ret.Char.UnicodeChar = C_SPACE;
			backColor = 0b0000;	// Black back
		}
		else if (val < 2.0f/16.0f) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			backColor = 0b0000;	// Black back
			charColor = 0b1000;	// DGray shade
		}
		else if (val < 3.0f/16.0f) {
			ret.Char.UnicodeChar = C_MEDIUM_SHADE;
			backColor = 0b0000;	// Black back
			charColor = 0b1000;	// DGray shade
		}
		else if (val < 4.0f/16.0f) {
			ret.Char.UnicodeChar = C_MEDIUM_SHADE;
			backColor = 0b1000;	// DGray back
			charColor = 0b0000;	// Black shade
		}
		else if (val < 5.0/16.0f) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			backColor = 0b1000;	// DGray back
			charColor = 0b0000;	// Black shade
		}
		else if (val < 6.0f/16.0f) {
			ret.Char.UnicodeChar = C_SPACE;
			backColor = 0b1000;	// DGray back
		}
		else if (val < 7.0f/16.0f) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			backColor = 0b1000;	// DGray back
			charColor = 0b0111;	// LGray shade
		}
		else if (val < 8.0f/16.0f) {
			ret.Char.UnicodeChar = C_MEDIUM_SHADE;
			backColor = 0b1000;	// DGray back
			charColor = 0b0111;	// LGray shade
		}
		else if (val < 9.0f/16.0f) {
			ret.Char.UnicodeChar = C_MEDIUM_SHADE;
			backColor = 0b0111;	// LGray back
			charColor = 0b1000;	// DGray shade
		}
		else if (val < 10.0f/16.0f) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			backColor = 0b0111;	// LGray back
			charColor = 0b1000;	// DGray shade
		}
		else if (val < 11.0f/16.0f) {
			ret.Char.UnicodeChar = C_SPACE;
			backColor = 0b0111;	// LGray back
		}
		else if (val < 12.0f/16.0f) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			backColor = 0b0111;	// LGray back
			charColor = 0b1111;	// White shade
		}
		else if (val < 13.0f/16.0f) {
			ret.Char.UnicodeChar = C_MEDIUM_SHADE;
			backColor = 0b0111;	// LGray back
			charColor = 0b1111;	// White shade
		}
		else if (val < 14.0f/16.0f) {
			ret.Char.UnicodeChar = C_MEDIUM_SHADE;
			backColor = 0b1111;	// White back
			charColor = 0b0111;	// LGray shade
		}
		else if (val < 15.0f/16.0f) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			backColor = 0b1111;	// White back
			charColor = 0b0111;	// LGray shade
		}
		else {
			ret.Char.UnicodeChar = C_SPACE;
			backColor = 0b1111;	// White back
		}
		ret.Attributes = (backColor << 4) | charColor;
		return ret;
	}


	{
		charColor = 0b1111;	// White shade
		if (val < 0.20f) {

		}
		else if (val < 0.40) {

		}
		else if (val < 0.60) {

		}
		else if (val < 0.80) {
			ret.Char.UnicodeChar = C_LIGHT_SHADE;
			charColor = 0b1111;
		}
		else {
			ret.Char.UnicodeChar = C_SPACE;
			backColor = 0b1111;	// White back
		}
		ret.Attributes = ((unsigned short)backColor << 4) | charColor;	// White back
		return ret;
	}


	if (val >= 0.50f) {
		charColor = backColor & 0b0111;	// Dark shade
		backColor |= 0b1000;	// Bright back
	}
	else {
		charColor = 0b0000;	// Black shade (back color stays dark)
	}

	if (sat < 0.40f)
		shadeChar = C_DARK_SHADE;
	else if (sat < 0.60f)
		shadeChar = C_MEDIUM_SHADE;
	else if (sat < 0.80f)
		shadeChar = C_LIGHT_SHADE;
	else
		shadeChar = C_SPACE;

}


Draggoon::Color<float> Draggoon::COLOR_F_WHITE(1.0f, 1.0f, 1.0f);
Draggoon::Color<float> Draggoon::COLOR_F_RED(1.0f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_GREEN(0.0f, 1.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_BLUE(0.0f, 0.0f, 1.0f);
Draggoon::Color<float> Draggoon::COLOR_F_YELLOW(1.0f, 1.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_CYAN(0.0f, 1.0f, 1.0f);
Draggoon::Color<float> Draggoon::COLOR_F_MAGENTA(1.0f, 0.0f, 1.0f);
Draggoon::Color<float> Draggoon::COLOR_F_GRAY(0.75f, 0.75f, 0.75f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_RED(0.5f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_GREEN(0.0f, 0.5f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_BLUE(0.0f, 0.0f, 0.5f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_YELLOW(0.5f, 0.5f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_CYAN(0.0f, 0.5f, 0.5f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_MAGENTA(0.5f, 0.0f, 0.5f);
Draggoon::Color<float> Draggoon::COLOR_F_DARK_GRAY(0.5f, 0.5f, 0.5f);
Draggoon::Color<float> Draggoon::COLOR_F_BLACK(0.0f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::COLOR_F_TRANSPARENT(0.0f, 0.0f, 0.0f, true);



short Draggoon::C_FULL_BLOCK = L'\u2588';	// █
short Draggoon::C_DARK_SHADE = L'\u2593';	// ▓
short Draggoon::C_MEDIUM_SHADE = L'\u2592';	// ▒
short Draggoon::C_LIGHT_SHADE = L'\u2591';	// ░
short Draggoon::C_SPACE = L' ';				//  