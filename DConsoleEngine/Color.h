#pragma once

#include <tuple>
#include <type_traits>


namespace Draggoon {

	/// <summary>Holds color information.</summary>
	template<class T=float>
	class Color {
	public:
		Color(T t_r=0, T t_g=0, T t_b=0, bool t_transparent=false);
		T getR() const;
		T getG() const;
		T getB() const;
		bool isTransparent() const;
		std::tuple<T, T, T> getRGB() const;
		char getConsoleColor() const;
	protected:
		T m_r;	/// <summary>Red component of the RGB color.</summary>
		T m_g;	/// <summary>Green component of the RGB color.</summary>
		T m_b;	/// <summary>Blue component of the RGB color.</summary>
		bool m_transparent;
	};

	extern Color<float> COLOR_F_WHITE;
	extern Color<float> COLOR_F_RED;
	extern Color<float> COLOR_F_GREEN;
	extern Color<float> COLOR_F_BLUE;
	extern Color<float> COLOR_F_YELLOW;
	extern Color<float> COLOR_F_CYAN;
	extern Color<float> COLOR_F_PURPLE;
	extern Color<float> COLOR_F_GRAY;
	extern Color<float> COLOR_F_DARK_RED;
	extern Color<float> COLOR_F_DARK_GREEN;
	extern Color<float> COLOR_F_DARK_BLUE;
	extern Color<float> COLOR_F_DARK_YELLOW;
	extern Color<float> COLOR_F_DARK_CYAN;
	extern Color<float> COLOR_F_DARK_PURPLE;
	extern Color<float> COLOR_F_BLACK;
	extern Color<float> COLOR_F_TRANSPARENT;

}
