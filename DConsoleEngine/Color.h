#pragma once

#include <tuple>
#include <type_traits>

#include <windows.h>

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
		/// <summary>Returns the 4 bit conlor format of the Windows console (16 possible values)</summary>
		char getConsoleColor() const;
		/// <summary>Returns a character and the color of the front and background (TODO calc possible values)</summary>
		CHAR_INFO getConsoleChar() const;
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
	extern Color<float> COLOR_F_MAGENTA;
	extern Color<float> COLOR_F_GRAY;
	extern Color<float> COLOR_F_DARK_RED;
	extern Color<float> COLOR_F_DARK_GREEN;
	extern Color<float> COLOR_F_DARK_BLUE;
	extern Color<float> COLOR_F_DARK_YELLOW;
	extern Color<float> COLOR_F_DARK_CYAN;
	extern Color<float> COLOR_F_DARK_MAGENTA;
	extern Color<float> COLOR_F_DARK_GRAY;
	extern Color<float> COLOR_F_BLACK;
	extern Color<float> COLOR_F_TRANSPARENT;


	extern short C_FULL_BLOCK;		// █
	extern short C_DARK_SHADE;		// ▓
	extern short C_MEDIUM_SHADE;	// ▒
	extern short C_LIGHT_SHADE;		// ░
	extern short C_SPACE;			//  

}
