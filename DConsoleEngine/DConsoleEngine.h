#pragma once

#include <Windows.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <tuple>

namespace Draggoon {

	/// <summary>Holds coordinates about a 2D position.</summary>
	template<class T=int>
	class Vector2D {
	public:
		Vector2D(const T& t_x = 0, const T& t_y = 0);
		//Vector2D(const std::tuple<T, T>& t_vec);
		T getX() const;
		T getY() const;
		void setX(const T& t_x);
		void setY(const T& t_y);
		T getArea() const;
		bool contains(const Draggoon::Vector2D<T> t_v) const;
		bool isContainedIn(const Draggoon::Vector2D<T> t_v) const;
		bool operator==(const Vector2D<T>& t_v) const;
		bool operator!=(const Vector2D<T>& t_v) const;
	protected:
		T m_x;	/// <summary>Horizontal coordinate (left to right).</summary>
		T m_y;	/// <summary>Vertical coordinate (top to bottom).</summary>
	};

	/// <summary>Holds color information.</summary>
	template<class T=float>
	class Color {
	public:
		Color(T t_r=0, T t_g=0, T t_b=0);
		T getR() const;
		T getG() const;
		T getB() const;
		std::tuple<T, T, T> getRGB() const;
	protected:
		T m_r;	/// <summary>Red component of the RGB color.</summary>
		T m_g;	/// <summary>Green component of the RGB color.</summary>
		T m_b;	/// <summary>Blue component of the RGB color.</summary>
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
	extern short C_SPACE;			//  


	/// <summary>Holds the state of a key</summary>
	class Key {
	public:
		Key();
		bool isPressed() const;
		bool isDown() const;
		bool isReleased() const;
		bool isUp() const;

		void setState(const bool& t_down);

	protected:
		bool m_down;
		bool m_changed;
	};


	/// <summary>Abstract class to draw on the console and process inputs.</summary>
	class DConsoleEngine{
	public:
		DConsoleEngine();
		~DConsoleEngine();

	private:
		/// <summary>Sets the console size and cursor options.</summary>
		void initConsole();
		/// <summary>Changes the size of the buffer when the console is resized.</summary>
		void resizeConsole(const Draggoon::Vector2D<int>& newSize=Draggoon::Vector2D<int>(-1,-1));


	protected:
		/// Can be implemented by the user
		/// <summary>Called when the engine is starting, before initialization of the console.</summary>
		virtual bool onCreate() { return true; }

		/// Can be implemented by the user
		/// <summary>Called when the engine is starting, after console initialization.</summary>
		virtual bool onInitialized() { return true; }

		/// Must be implemented by the user
		/// <summary>Called periodically (depending on the framerate) to update the screen buffer.</summary>
		virtual bool onScreenUpdate(const std::chrono::duration<float> &elapsedTime) = 0;

		/// Can be implemented by the user
		/// <summary>Called periodically (depending on the framerate) to process input.</summary>
		virtual bool onInputUpdate(const Draggoon::Key* keys, const size_t& keyCount, const Draggoon::Key* mouseBtn, const size_t& mouseBtnCount) {
			// This is a default method that can be overloaded by the user
			if (keys[VK_ESCAPE].isPressed()) {
				return false;	// Stop the thread loop
			}
			return true;	// Continue
		};

		/// Can be implemented by the user
		/// <summary>Called when the engine is shutting down.</summary>
		virtual void onDestroy() {
			// This is a default method that can be overloaded by the user
		};

		/// Can be implemented by the user
		/// <summary>Called when the console size changes.</summary>
		/// <param name="newSize">Vector2D containing the new size of the screen.</param>
		virtual void onResize(const Vector2D<int>& newSize) {
			// This is a default method that can be overloaded by the user
		};

		void setSize(Draggoon::Vector2D<int> t_size);
		void setPixelSize(Draggoon::Vector2D<int> t_pixSize);

		Draggoon::Vector2D<int> getMousePosition() const;

	public:
		/// <summary>Launches the game thread.</summary>
		/// <param name="t_nonBlocking">Specifies if the Start should return immediately (true) or wait for the thread to end (false, default).</param>
		void externalStart(const bool& t_nonBlocking=false);

		/// <summary>Stops the thread from the main thread (if externalStart was called whith non_blocking = true).</summary>
		void externalStop();

	protected:
		/// <summary>Stops the thread loop from this thread.</summary>
		/// Exits the thread loop and cleans-up (calling onDestroy).
		void internalStop();

		/// <summary>Returns the current size of the console window.</summary>
		Draggoon::Vector2D<int> getSize() const;

		void clearScreen();

		/// <summary>Changes a character on the console.</summary>
		/// <param name="t_pix">The char position on the console screen (top: y=0, left: x=0).</param>
		/// <param name="t_char">The character to display. defaults to a "full block" unicode character.</param>
		/// <param name="t_charColor">The color of the character.</param>
		/// <param name="t_backColor">The color of the cell behind the character.</param>
		void setPixel(Vector2D<int> t_pix, short t_char=Draggoon::C_FULL_BLOCK, Color<float> t_charColor=Draggoon::F_WHITE, Color<float> t_backColor=Draggoon::F_BLACK);
		
		void setCharAlpha(Vector2D<int> t_pix, short t_char, Draggoon::Color<float> t_charColor=Draggoon::F_WHITE);
		
		/// <summary>Displays a string of characters on the console.</summary>
		/// <param name="t_pix">The first char position on the console screen (top: y=0, left: x=0).</param>
		/// <param name="t_char">The characters to display.</param>
		/// <param name="t_charColor">The color of the characters.</param>
		/// <param name="t_backColor">The color of the cell behind the characters.</param>
		void drawString(Vector2D<int> t_pix, const wchar_t* t_str, Color<float> t_charColor=Draggoon::F_WHITE, Color<float> t_backColor=Draggoon::F_BLACK);

		void drawStringAlpha(Vector2D<int> t_pix, const wchar_t* t_str, Color<float> t_charColor=Draggoon::F_WHITE);

		void drawLine(Vector2D<int> t_pix1, Vector2D<int> t_pix2, short t_str=C_FULL_BLOCK, Color<float> t_charColor=F_WHITE, Color<float> t_backColor=F_BLACK);


	protected:
		std::wstring m_appName;
		int m_framerate;
		bool m_clearScreenBeforeUpdate;
		bool m_stretchOnResize;

	private:
		Draggoon::Vector2D<int> m_desiredSize;
		Draggoon::Vector2D<int> m_desiredPixelSize;
		Draggoon::Vector2D<int> m_desiredWindowSize;
		Draggoon::Vector2D<int> m_actualWindowSize;
		int engineFunction();
		static std::thread* m_engineThread;
		static volatile bool m_runEngine;
		bool m_asyncRunning;
		bool m_windowCreated;
		HANDLE m_originalScreenBuffer;
		HANDLE m_screenBuffer;
		HANDLE m_inputHandle;
		CHAR_INFO* m_pixelsBuffer;
		Draggoon::Vector2D<int> m_bufferSize;
		Draggoon::Vector2D<int> m_actualSize;

		static const int KEY_COUNT = 256;
		Draggoon::Key m_key[KEY_COUNT];
		static const int MOUSE_BTN_COUNT = 5;
		Draggoon::Key m_mouseBtn[MOUSE_BTN_COUNT];
		Draggoon::Vector2D<int> m_mousePosition;

		static std::condition_variable m_stopCV;
		static std::mutex m_stopMutex;
		static BOOL consoleEventHandler(DWORD event);

	};
}


