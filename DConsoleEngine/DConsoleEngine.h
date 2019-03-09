#pragma once

#ifdef WIN32
#include <Windows.h>
#else
#include <curses.h>
#endif

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <condition_variable>

#include "Vectors.h"
#include "Color.h"

namespace Draggoon {

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
		// Console access methods (to differ between windows and linux)
		/// <summary>Sets the console size and cursor options.</summary>
		void initConsole();
		/// <summary>Changes the size of the buffer when the console is resized.</summary>
		void resizeConsole(const Draggoon::Vector2D<int>& newSize=Draggoon::Vector2D<int>(-1,-1));
		// <summary>Returns the console size in characters.</summary>
		Draggoon::Vector2D<int> getConsoleSize();
		// <summary>Refreshes the screen of the console, displaying what has been prepared by the user in onScreenUpdate.</summary>
		Draggoon::Vector2D<int> refreshConsoleScreen();


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
		virtual bool onInputUpdate(const Key* keys, const size_t& keyCount, const Key* mouseBtn, const size_t& mouseBtnCount) {
			// This is a default method that can be overloaded by the user
			if (keys[Draggoon::Keys::escape].isPressed()) {
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
		virtual void onResize(const Draggoon::Vector2D<int>& newSize) {
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

		CHAR_INFO getCharInfo(Vector2D<int> t_pos);

		void clearScreen();

		void setPixel(Vector2D<int> t_pix, Color<float> t_color);

		void setPixel(Vector2D<int> t_pix, char t_consoleColor);

		/// <summary>Changes a character on the console.</summary>
		/// <param name="t_pix">The char position on the console screen (top: y=0, left: x=0).</param>
		/// <param name="t_char">The character to display. defaults to a "full block" unicode character.</param>
		/// <param name="t_charColor">The color of the character.</param>
		/// <param name="t_backColor">The color of the cell behind the character.</param>
		void setChar(Draggoon::Vector2D<int> t_pix, short t_char=C_FULL_BLOCK, Color<float> t_charColor=COLOR_F_WHITE, Color<float> t_backColor=COLOR_F_BLACK);

		void setChar(Draggoon::Vector2D<int> t_pix, const CHAR_INFO& t_char);
		
		void setCharAlpha(Draggoon::Vector2D<int> t_pix, short t_char, Color<float> t_charColor=COLOR_F_WHITE);
		
		/// <summary>Displays a string of characters on the console.</summary>
		/// <param name="t_pix">The first char position on the console screen (top: y=0, left: x=0).</param>
		/// <param name="t_char">The characters to display.</param>
		/// <param name="t_charColor">The color of the characters.</param>
		/// <param name="t_backColor">The color of the cell behind the characters.</param>
		void drawString(Draggoon::Vector2D<int> t_pix, const wchar_t* t_str, Color<float> t_charColor=COLOR_F_WHITE, Color<float> t_backColor=COLOR_F_BLACK);

		void drawStringAlpha(Draggoon::Vector2D<int> t_pix, const wchar_t* t_str, Color<float> t_charColor=COLOR_F_WHITE);

		void drawLineChar(Draggoon::Vector2D<int> t_pix1, Draggoon::Vector2D<int> t_pix2, short t_str=C_FULL_BLOCK, Color<float> t_charColor=COLOR_F_WHITE, Color<float> t_backColor=COLOR_F_TRANSPARENT);
		
		void drawLine(Draggoon::Vector2D<int> t_pix1, Draggoon::Vector2D<int> t_pix2, Color<float> t_color=COLOR_F_WHITE);

		void drawRect(Vector2D<int> t_pix1, Vector2D<int> t_pix2, Color<float> t_color=COLOR_F_WHITE);
		void fillRect(Vector2D<int> t_pix1, Vector2D<int> t_pix2, Color<float> t_color=COLOR_F_WHITE);


	protected:
		std::wstring m_appName;
		int m_framerate;
		bool m_clearScreenBeforeUpdate;
		bool m_stretchOnResize;

		bool m_enableStats;
		wchar_t m_lastFrameStats[512];

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
		Draggoon::Vector2D<int> m_bufferSize;
		Draggoon::Vector2D<int> m_actualSize;

		static const int KEY_COUNT = 256;
		Key m_key[KEY_COUNT];
		static const int MOUSE_BTN_COUNT = 5;
		Key m_mouseBtn[MOUSE_BTN_COUNT];
		Draggoon::Vector2D<int> m_mousePosition;

		static std::condition_variable m_stopCV;
		static std::mutex m_stopMutex;
		static bool consoleEventHandler(unsigned long event);

		std::ofstream m_cerrFile;
		std::streambuf* m_origCerr;

		void processInputs();

#ifdef WIN32
		HANDLE m_originalScreenBuffer;
		HANDLE m_screenBuffer;
		HANDLE m_inputHandle;
		CHAR_INFO* m_pixelsBuffer;
#endif

	};
}
