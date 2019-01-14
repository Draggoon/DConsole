#pragma once

#include <Windows.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>

namespace Draggoon {

	/// <summary>Holds coordinates about a 2D position</summary>
	template<class T>
	class Position2D {
	public:
		Position2D(const T& t_x = 0, const T& t_y = 0);
		Position2D(const Position2D& t_pos);
		T getX() const;
		T getY() const;
		void setX(const T& t_x);
		void setY(const T& t_y);
	protected:
		T m_x;	/// <summary>Horizontal coordinate (left to right)</summary>
		T m_y;	/// <summary>Vertical coordinate (top to bottom)</summary>
	};

	class DConsoleEngine{
	public:
		DConsoleEngine();
		~DConsoleEngine();

		/// Must be implemented by the user
		/// <summary>Called when the engine is starting</summary>
		virtual bool onCreate() = 0;

		/// Must be implemented by the user
		/// <summary>Called periodically to process user input and update the screen</summary>
		virtual bool onUpdate(const std::chrono::duration<float> &elapsedTime) = 0;

		/// Can be implemented by the user
		/// <summary>Called when the engine is shutting down</summary>
		virtual void onDestroy();

		void start(const bool& t_nonBlocking=false);
		void stop();

	protected:
		std::thread* m_engineThread;
		bool m_asyncRunning;
		int m_framerate;

		std::wstring m_appName;

		HANDLE m_consoleHandle;
		HANDLE m_originalScreenBuffer;
		CHAR_INFO* m_screenBuffer;
		int m_bufferWidth;
		int m_bufferHeight;
		int m_desiredWidth;
		int m_desiredHeight;
		int m_actualWidth;
		int m_actualHeight;
		bool m_clearScreenBeforeUpdate;

		char m_key[256];
		char m_mouse[5];

	private:
		void engineFunction();
		bool m_runEngine;

	};
}