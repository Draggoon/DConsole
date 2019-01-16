#pragma once

#include <Windows.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <tuple>

namespace Draggoon {

	/// <summary>Holds coordinates about a 2D position</summary>
	template<class T>
	class Vector2D {
	public:
		Vector2D(const T& t_x = 0, const T& t_y = 0);
		Vector2D(const std::tuple<T, T>& t_vec);
		T getX() const;
		T getY() const;
		void setX(const T& t_x);
		void setY(const T& t_y);
	protected:
		T m_x;	/// <summary>Horizontal coordinate (left to right)</summary>
		T m_y;	/// <summary>Vertical coordinate (top to bottom)</summary>
	};

	template<class T>
	class Color {
	public:
		Color(T t_r=0, T t_g=0, T t_b=0);
		T getR() const;
		T getG() const;
		T getB() const;
		std::tuple<T, T, T> getRGB() const;
	protected:
		T m_r;
		T m_g;
		T m_b;
	};

	extern Draggoon::Color<float> F_WHITE;
	extern Draggoon::Color<float> F_RED;
	extern Draggoon::Color<float> F_GREEN;
	extern Draggoon::Color<float> F_BLUE;
	extern Draggoon::Color<float> F_GRAY;
	extern Draggoon::Color<float> F_DARK_RED;
	extern Draggoon::Color<float> F_DARK_GREEN;
	extern Draggoon::Color<float> F_DARK_BLUE;
	extern Draggoon::Color<float> F_BLACK;

	extern short C_FULL_BLOCK;		// █
	extern short C_DARK_SHADE;		// ▓
	extern short C_MEDIUM_SHADE;	// ▒
	extern short C_LIGHT_SHADE;		// ░

	class DConsoleEngine{
	public:
		DConsoleEngine();
		~DConsoleEngine();

	public:
		void initConsole();
	private:
		void onResizeConsole(const Draggoon::Vector2D<int>& newSize={-1,-1});

	protected:
		/// Must be implemented by the user
		/// <summary>Called when the engine is starting</summary>
		virtual bool onCreate() = 0;

		/// Must be implemented by the user
		/// <summary>Called periodically to process user input and update the screen</summary>
		virtual bool onUpdate(const std::chrono::duration<float> &elapsedTime) = 0;

		/// Can be implemented by the user
		/// <summary>Called when the engine is shutting down</summary>
		virtual void onDestroy();

	public:
		void start(const bool& t_nonBlocking=false);
		void stop();

	protected:
		Draggoon::Vector2D<int> getSize();

		void setPixel(Vector2D<int> t_pix, short t_char=Draggoon::C_FULL_BLOCK, Color<float> t_charColor=Draggoon::F_WHITE, Color<float> t_backColor=Draggoon::F_BLACK);
		void drawString(Vector2D<int> t_pix, const char* t_str, Color<float> t_charColor=Draggoon::F_WHITE, Color<float> t_backColor=Draggoon::F_BLACK);

	protected:
		std::thread* m_engineThread;
		bool m_asyncRunning;
		int m_framerate;

		std::wstring m_appName;

	private:
		HANDLE m_consoleHandle;
		HANDLE m_originalScreenBuffer;
		HANDLE m_screenBuffer;
		CHAR_INFO* m_pixelsBuffer;
		int m_bufferWidth;
		int m_bufferHeight;
		int m_bufferSize;
		int m_desiredWidth;
		int m_desiredHeight;
		int m_desitedPixelWidth;
		int m_desitedPixelHeight;
		int m_actualWidth;
		int m_actualHeight;

	protected:
		bool m_clearScreenBeforeUpdate;

	protected:
		char m_key[256];
		char m_mouse[5];

	private:
		void engineFunction();
		bool m_runEngine;

	};
}