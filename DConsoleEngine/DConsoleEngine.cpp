#include "DConsoleEngine.h"


template <class T>
Draggoon::Position2D<T>::Position2D(const T& t_x, const T& t_y) : m_x(t_x), m_y(t_y) {

}

template<class T>
Draggoon::Position2D<T>::Position2D(const Position2D& t_pos) :	m_x(t_pos.m_x),	m_y(t_pos.m_y) {

}

template<class T>
T Draggoon::Position2D<T>::getX() const {
	return m_x;
}

template<class T>
T Draggoon::Position2D<T>::getY() const {
	return m_y;
}

template<class T>
void Draggoon::Position2D<T>::setX(const T& t_x) {
	m_x = t_x;
}

template<class T>
void Draggoon::Position2D<T>::setY(const T& t_y) {
	m_y = t_y;
}


Draggoon::DConsoleEngine::DConsoleEngine() :
m_engineThread(nullptr),
m_asyncRunning(false),
m_framerate(60),
m_appName(L"DConsoleEnging Default name"),
m_consoleHandle(nullptr),
m_screenBuffer(nullptr),
m_bufferWidth(0),
m_bufferHeight(0),
m_desiredWidth(120),
m_desiredHeight(30),
m_actualWidth(0),
m_actualHeight(0),
m_clearScreenBeforeUpdate(true),
m_runEngine(false)
{
}

Draggoon::DConsoleEngine::~DConsoleEngine() {
	if (m_screenBuffer != nullptr)
		delete[] m_screenBuffer;
}


void Draggoon::DConsoleEngine::onDestroy() {

}

void Draggoon::DConsoleEngine::start(const bool & t_nonBlocking) {
	m_asyncRunning = t_nonBlocking;

	m_runEngine = true;

	m_engineThread = new std::thread(&Draggoon::DConsoleEngine::engineFunction, this);

	if (m_asyncRunning)
		return;

	m_engineThread->join();
	delete m_engineThread;
}

void Draggoon::DConsoleEngine::stop() {
	m_runEngine = false;

	if (m_engineThread == nullptr)
		throw "DConsoleEngine::stop: Engine no running";

	m_engineThread->join();
	delete m_engineThread;
}

void Draggoon::DConsoleEngine::engineFunction() {
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD size;
	size.X = m_desiredWidth;
	size.Y = m_desiredHeight;
	SetConsoleScreenBufferSize(m_consoleHandle, size);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_consoleHandle, &info);
	m_actualWidth = info.dwSize.X;
	m_actualHeight = info.dwSize.Y;

	m_screenBuffer = new CHAR_INFO[m_actualHeight*m_actualWidth];

	m_bufferWidth = m_actualWidth;
	m_bufferHeight = m_actualHeight;

	if (!onCreate()) {
		m_runEngine = false;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> t1(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock> t2(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock> framerateT1(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock> framerateT2(std::chrono::high_resolution_clock::now());
	while (m_runEngine) {

		if (GetAsyncKeyState(VK_ESCAPE)) {
			m_runEngine = false;
		}

		if (m_clearScreenBeforeUpdate)
			memset(m_screenBuffer, 0, m_actualHeight*m_actualWidth*sizeof(CHAR_INFO));

		// Let the user update the screen
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> userDelta = t2-t1;
		t1 = t2;
		if (!onUpdate(userDelta)) {
			m_runEngine = false;
			break;
		}

		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(m_consoleHandle, &info);
		m_actualWidth = info.dwSize.X;
		m_actualHeight = info.dwSize.Y;

		// Refresh the screen
		COORD bufferSize;
		bufferSize.X = m_bufferWidth;
		bufferSize.Y = m_bufferHeight;
		SMALL_RECT rect;
		rect.Top = 0;
		rect.Bottom = m_actualHeight;
		rect.Left = 0;
		rect.Right = m_actualWidth;
		WriteConsoleOutput(m_consoleHandle, m_screenBuffer, bufferSize, {0,0}, &rect);

		// Calculate and display framerate, and wait if fps limit is on
		framerateT2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> framerateDelta = framerateT2 - framerateT1;
		if (m_framerate > 0) {
			if (framerateDelta.count() < 1.0/m_framerate) {
				std::this_thread::sleep_for(std::chrono::microseconds( (long)( (1.0/m_framerate - framerateDelta.count())*1000000 )) );
			}
			wchar_t title[256];
			if(m_framerate > 0)
				swprintf(title, 256, L"%s - %dx%d - %3.2f fps / %d", m_appName.c_str(), m_actualWidth, m_actualHeight, 1.0/userDelta.count(), m_framerate);
			else
				swprintf(title, 256, L"%s - %dx%d - %3.2f fps", m_appName.c_str(), m_actualWidth, m_actualHeight, 1.0/userDelta.count());
			SetConsoleTitle(title);
		}
		framerateT1 = std::chrono::high_resolution_clock::now();
	}

	onDestroy();

	return;
}


