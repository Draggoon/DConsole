#include "DConsoleEngine.h"


// #,     ,# #########  ######,  #########  ,#####,  #######,
// "#     #" ##""""""  ##"""""## """###""" ,#"""""#, ##"""""#,
//  ##   ##  ##        ##           ###    ##     ## ##,,,,,#"
//   #, ,#   ########  ##           ###    ##     ## #######"
//   "#,#"   ##"""""   ##           ###    ##     ## ##"""##,
//    "#"    ##,,,,,,  ##,,,,,##    ###    "#,,,,,#" ##   "##,
//     #     #########  ######"     ###     "#####"  ##    "##

template <class T>
Draggoon::Vector2D<T>::Vector2D(const T& t_x, const T& t_y) : m_x(t_x), m_y(t_y) {

}
/*
template<class T>
Draggoon::Vector2D<T>::Vector2D(const std::tuple<T, T>& t_vec) {
	std::tie(m_x, m_y) = t_vec;
}
*/

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
T Draggoon::Vector2D<T>::getArea() const {
	return m_x * m_y;
}

template<class T>
bool Draggoon::Vector2D<T>::contains(const Draggoon::Vector2D<T> t_v) const {
	bool x(false),y(false);
	if (m_x >= 0 && t_v.m_x >= 0) {
		if (t_v.m_x <= m_x)
			x = true;
	}
	else if (m_x <= 0 && t_v.m_x <= 0) {
		if (t_v.m_x >= m_x)
			x = true;
	}
	if (m_y >= 0 && t_v.m_y >= 0) {
		if (t_v.m_y <= m_y)
			y = true;
	}
	else if (m_y <= 0 && t_v.m_y <= 0) {
		if (t_v.m_y >= m_y)
			y = true;
	}
	return x && y;
}

template<class T>
bool Draggoon::Vector2D<T>::isContainedIn(const Draggoon::Vector2D<T> t_v) const {
	bool x(false), y(false);
	if (m_x >= 0 && t_v.m_x >= 0) {
		if (t_v.m_x >= m_x)
			x = true;
	}
	else if (m_x <= 0 && t_v.m_x <= 0) {
		if (t_v.m_x <= m_x)
			x = true;
	}
	if (m_y >= 0 && t_v.m_y >= 0) {
		if (t_v.m_y >= m_y)
			y = true;
	}
	else if (m_y <= 0 && t_v.m_y <= 0) {
		if (t_v.m_y <= m_y)
			y = true;
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


//  ######,   ,#####,  ##         ,#####,  #######,
// ##"""""## ,#"""""#, ##        ,#"""""#, ##"""""#,
// ##        ##     ## ##        ##     ## ##,,,,,#"
// ##        ##     ## ##        ##     ## #######"
// ##        ##     ## ##        ##     ## ##"""##,
// ##,,,,,## "#,,,,,#" ##,,,,,,, "#,,,,,#" ##   "##,
//  ######"   "#####"  #########  "#####"  ##    "##

template<class T>
Draggoon::Color<T>::Color(T _r, T _g, T _b) : m_r(_r), m_g(_g), m_b(_b) {

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
std::tuple<T, T, T> Draggoon::Color<T>::getRGB() const {
	return {m_r, m_g, m_b};
}


Draggoon::Color<float> Draggoon::F_WHITE(1.0f, 1.0f, 1.0f);
Draggoon::Color<float> Draggoon::F_RED(1.0f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::F_GREEN(0.0f, 1.0f, 0.0f);
Draggoon::Color<float> Draggoon::F_BLUE(0.0f, 0.0f, 1.0f);
Draggoon::Color<float> Draggoon::F_GRAY(0.5f, 0.5f, 0.5f);
Draggoon::Color<float> Draggoon::F_DARK_RED(0.5f, 0.0f, 0.0f);
Draggoon::Color<float> Draggoon::F_DARK_GREEN(0.0f, 0.5f, 0.0f);
Draggoon::Color<float> Draggoon::F_DARK_BLUE(0.0f, 0.0f, 0.5f);
Draggoon::Color<float> Draggoon::F_BLACK(0.0f, 0.0f, 0.0f);



short Draggoon::C_FULL_BLOCK = L'\u2588';	// █
short Draggoon::C_DARK_SHADE = L'\u2593';	// ▓
short Draggoon::C_MEDIUM_SHADE = L'\u2592';	// ▒
short Draggoon::C_LIGHT_SHADE = L'\u2591';	// ░
short Draggoon::C_SPACE = L' ';				//  


// ##     ## ######### ##,   ,##
// ##   ##   ##""""""   "#, ,#"
// ## ##     ##          "#,#"
// #####     ########     ###
// ##""##    ##"""""      ###
// ##   "##  ##,,,,,,     ###
// ##    "## #########    ###

Draggoon::Key::Key() : m_down(false), m_changed(false) {
}

bool Draggoon::Key::isPressed() const {
	return m_down && m_changed;
}

bool Draggoon::Key::isDown() const {
	return m_down;
}

bool Draggoon::Key::isReleased() const {
	return !m_down && m_changed;
}

bool Draggoon::Key::isUp() const {
	return !m_down;
}

void Draggoon::Key::setState(const bool& t_down) {
	if (m_down != t_down) {
		m_down = t_down;
		m_changed = true;
	}
	else {
		m_changed = false;
	}
}


// ######### ##,    ##  #######   #######  ##,    ## #########
// ##""""""  ###,   ## ##"""""##   "###"   ###,   ## ##""""""
// ##        ##"#,  ## ##           ###    ##"#,  ## ##
// ########  ## "#, ## ##   ###     ###    ## "#, ## ########
// ##"""""   ##  "#,## ##   ""##    ###    ##  "#,## ##"""""
// ##,,,,,,  ##   "### ##     ##   ,###,   ##   "### ##,,,,,,
// ######### ##    "##  #######   #######  ##    "## #########


std::mutex Draggoon::DConsoleEngine::m_stopMutex;
std::condition_variable Draggoon::DConsoleEngine::m_stopCV;
volatile bool Draggoon::DConsoleEngine::m_runEngine(false);
std::thread* Draggoon::DConsoleEngine::m_engineThread(nullptr);

Draggoon::DConsoleEngine::DConsoleEngine() :
m_appName(L"DConsoleEnging Default name"),
m_framerate(60),
m_clearScreenBeforeUpdate(true),
m_stretchOnResize(false),
m_desiredSize(40, 30),
m_desiredPixelSize(8, 8),
m_desiredWindowSize(m_desiredSize.getX()*m_desiredPixelSize.getX(), m_desiredSize.getY()*m_desiredPixelSize.getY()),
m_actualWindowSize(m_desiredWindowSize),
m_asyncRunning(false),
m_windowCreated(false),
m_originalScreenBuffer(nullptr),
m_screenBuffer(nullptr),
m_inputHandle(nullptr),
m_pixelsBuffer(nullptr),
m_bufferSize(0,0),
m_actualSize(0,0),
m_mousePosition(0,0)
{

}

Draggoon::DConsoleEngine::~DConsoleEngine() {
	m_runEngine = false;
	if (m_pixelsBuffer != nullptr)
		delete[] m_pixelsBuffer;
}


void Draggoon::DConsoleEngine::initConsole() {
	m_originalScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_originalScreenBuffer == INVALID_HANDLE_VALUE)
		throw "Draggoon::DConsoleEngine::initConsole: Unable to get console handle.";

	m_inputHandle = GetStdHandle(STD_INPUT_HANDLE);

	SetConsoleMode(m_inputHandle, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	m_screenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, 0, NULL);

	bool r = SetConsoleActiveScreenBuffer(m_screenBuffer);
	if (!r) {
		throw "Draggoon::DConsoleEngine::initConsole: Unable to set the new screenBuffer.";
	}
	if(m_desiredWindowSize.getX() < 50 || m_desiredWindowSize.getY() < 50)
		throw "Draggoon::DConsoleEngine::initConsole: Desired size too small.";
	resizeConsole(m_desiredWindowSize);

	SetConsoleCtrlHandler(Draggoon::DConsoleEngine::consoleEventHandler, TRUE);

	m_windowCreated = true;
}

void Draggoon::DConsoleEngine::resizeConsole(const Draggoon::Vector2D<int>& newSize) {
	Draggoon::Vector2D<int> clippedSize=newSize;
	if (newSize.getX() < 50) {
		clippedSize.setX(50);
	}
	if (newSize.getY() < 50) {
		clippedSize.setY(50);
	}

	Draggoon::Vector2D<int> precedentSize = m_actualSize;

	CONSOLE_SCREEN_BUFFER_INFO oldScreenInfo;
	GetConsoleScreenBufferInfo(m_screenBuffer, &oldScreenInfo);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	if (m_stretchOnResize && m_windowCreated) {
		RECT rect;
		GetClientRect(GetConsoleWindow(), &rect);

		fontInfo.dwFontSize.X = clippedSize.getX() / (m_desiredSize.getX());
		fontInfo.dwFontSize.Y = clippedSize.getY() / (m_desiredSize.getY());
		if (fontInfo.dwFontSize.X < 1)
			fontInfo.dwFontSize.X = 1;
		if (fontInfo.dwFontSize.Y < 1)
			fontInfo.dwFontSize.Y = 1;
	}
	else {
		fontInfo.dwFontSize.X = m_desiredPixelSize.getX();
		fontInfo.dwFontSize.Y = m_desiredPixelSize.getY();
	}
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;


	wcscpy_s(fontInfo.FaceName, L"Consolas");
	bool r = SetCurrentConsoleFontEx(m_screenBuffer, false, &fontInfo);
	if (!r) {
		throw "Draggoon::DConsoleEngine::resizeConsole: Unable to set the font size.";
	}


	// Can't change the size of the screenBuffer to a value smaller than the size of the window.
	// Workaround: change the size of the window to a minimum, change the screenbuffer size, then adapt the window.
	SMALL_RECT size;
	size.Top = 0;
	size.Bottom = 1;
	size.Left = 0;
	size.Right = 1;
	r = SetConsoleWindowInfo(m_screenBuffer, TRUE, &size);
	if (!r) {
		throw "Draggoon::DConsoleEngine::resizeConsole: Unable to set console window size to 0.";
	}

	COORD coord;
	if (m_stretchOnResize) {
		coord.X = m_desiredSize.getX();
		coord.Y = m_desiredSize.getY();
	}
	else {
		coord.X = clippedSize.getX()/m_desiredPixelSize.getX();
		coord.Y = clippedSize.getY()/m_desiredPixelSize.getY();
	}
	r = SetConsoleScreenBufferSize(m_screenBuffer, coord);
	if (!r) {
		//resizeConsole(m_actualWindowSize);
		throw "Draggoon::DConsoleEngine::resizeConsole: Unable to set screen buffer size.";
	}


	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(m_screenBuffer, &screenInfo);
	if (coord.X > screenInfo.dwMaximumWindowSize.X || coord.Y > screenInfo.dwMaximumWindowSize.Y)
		throw "Draggoon::DConsoleEngine::resizeConsole: New size is greater than the maximum allowed.";

	if (clippedSize.getX() <= 0 || clippedSize.getY() <= 0) {
		throw "Draggoon::DConsoleEngine::resizeConsole: New size is 0";
	}

	m_actualSize.setX(coord.X);
	m_actualSize.setY(coord.Y);


	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = m_actualSize.getY()-1;
	rect.Left = 0;
	rect.Right = m_actualSize.getX()-1;
	r = SetConsoleWindowInfo(m_screenBuffer, TRUE, &rect);
	if (!r)
		throw "Draggoon::DConsoleEngine::resizeConsole: Unable to set the console window height and width.";

	m_actualWindowSize = clippedSize;

	if (precedentSize != m_actualSize) {
		m_bufferSize.setX(m_actualSize.getX());
		m_bufferSize.setY(m_actualSize.getY());

		if (m_pixelsBuffer != nullptr)
			delete[] m_pixelsBuffer;
		int bufferSize = m_bufferSize.getArea();
		m_pixelsBuffer = new CHAR_INFO[bufferSize];
		if (m_pixelsBuffer == nullptr)
			throw "Draggoon::DConsoleEngine::resizeConsole: Memory allocation error.";
		memset(m_pixelsBuffer, 0, bufferSize);


		onResize(m_bufferSize);
	}

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(m_screenBuffer, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(m_screenBuffer, &cursorInfo);
}

void Draggoon::DConsoleEngine::setSize(Draggoon::Vector2D<int> t_size) {
	if (t_size.getX() <= 0 || t_size.getY() <= 0)
		throw "Draggoon::DConsoleEngine::setSize: Tried to set a size <= 0.";
	m_desiredSize = t_size;
	m_desiredWindowSize.setX(m_desiredPixelSize.getX() * m_desiredSize.getX());
	m_desiredWindowSize.setY(m_desiredPixelSize.getY() * m_desiredSize.getY());
}

void Draggoon::DConsoleEngine::setPixelSize(Draggoon::Vector2D<int> t_pixSize) {
	if (t_pixSize.getX() <= 0 || t_pixSize.getY() <= 0)
		throw "Draggoon::DConsoleEngine::setPixelSize: Tried to set a pixel size <= 0.";
	m_desiredPixelSize = t_pixSize;
	m_desiredWindowSize.setX(m_desiredPixelSize.getX() * m_desiredSize.getX());
	m_desiredWindowSize.setY(m_desiredPixelSize.getY() * m_desiredSize.getY());
}

Draggoon::Vector2D<int> Draggoon::DConsoleEngine::getMousePosition() const {
	return m_mousePosition;
}

void Draggoon::DConsoleEngine::externalStart(const bool & t_nonBlocking) {
	m_asyncRunning = t_nonBlocking;


	m_runEngine = true;

	m_engineThread = new std::thread(&Draggoon::DConsoleEngine::engineFunction, this);

	if (m_asyncRunning)
		return;

	m_engineThread->join();
	delete m_engineThread;
}

void Draggoon::DConsoleEngine::externalStop() {
	m_runEngine = false;

	if (m_engineThread == nullptr)
		throw "DConsoleEngine::externalStop: Engine not running";

	m_engineThread->join();
	delete m_engineThread;

	SetConsoleActiveScreenBuffer(m_originalScreenBuffer);
}

void Draggoon::DConsoleEngine::internalStop() {
	m_runEngine = false;
}

int Draggoon::DConsoleEngine::engineFunction() {

	if (!onCreate()) {
		m_runEngine = false;
	}

	try {
		initConsole();
	}
	catch (const char* s) {
		SetConsoleActiveScreenBuffer(m_originalScreenBuffer);
		std::cerr << s << " (Last error = " << GetLastError() << ")" << std::endl;
		throw "Draggoon::DConsoleEngine::externalStart: Console initialization failed.";
	}

	if (!onInitialized()) {
		m_runEngine = false;
	}


	std::chrono::time_point<std::chrono::high_resolution_clock> t1(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock> t2(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock> framerateT1(std::chrono::high_resolution_clock::now());
	std::chrono::time_point<std::chrono::high_resolution_clock> framerateT2(std::chrono::high_resolution_clock::now());

	wchar_t title[256];

	// Main loop (Input processing, drawing, framerate limiting)
	while (m_runEngine) {

		for (int i(0); i<KEY_COUNT; ++i) {
			m_key[i].setState((GetAsyncKeyState(i) & 0x8000) != 0);	// GetAsyncKeyState returns a short, MSB set = key is down. 
																	// (LSB set = key pressed since last call, not reliable)
		}

		// Clear "changed" flag
		for (int i(0); i<MOUSE_BTN_COUNT; ++i)
			m_mouseBtn[i].setState(m_mouseBtn[i].isDown());
		INPUT_RECORD record;
		DWORD nbToRead, nbRead;
		// Must check if events are available, otherwise ReadConsoleInput hangs until an event arrives.
		GetNumberOfConsoleInputEvents(m_inputHandle, &nbToRead);
		while (nbToRead>0) {
			ReadConsoleInput(m_inputHandle, &record, 1, &nbRead);
			switch (record.EventType) {
			case MOUSE_EVENT: {
				m_mousePosition = {record.Event.MouseEvent.dwMousePosition.X, record.Event.MouseEvent.dwMousePosition.Y};
				for(int i(0); i<MOUSE_BTN_COUNT; ++i)
					m_mouseBtn[i].setState((record.Event.MouseEvent.dwButtonState & (1<<i)) != 0);
				break;
			}
			case KEY_EVENT:
				break;
			case FOCUS_EVENT:
				break;
			case MENU_EVENT:
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				break;
			default:;
			}
			nbToRead -= nbRead;
		}

		if (!onInputUpdate(m_key, KEY_COUNT, m_mouseBtn, MOUSE_BTN_COUNT)) {
			m_runEngine = false;
		}


		RECT winRect;
		GetClientRect(GetConsoleWindow(), &winRect);
		int width(winRect.right-winRect.left), height(winRect.bottom-winRect.top);
		if (width != m_actualWindowSize.getX() || height != m_actualWindowSize.getY()) {
			try {
				resizeConsole(Draggoon::Vector2D<int>(width, height));
			}
			catch (const char* s) {
#ifdef _DEBUG
				std::cerr << s << std::endl;
#endif
			}
		}

		if (m_clearScreenBeforeUpdate)
			clearScreen();

		// Let the user update the screen
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> userDelta = t2-t1;
		t1 = t2;
		if (!onScreenUpdate(userDelta)) {
			m_runEngine = false;
			break;
		}


		// Refresh the screen
		COORD bufferSize;
		bufferSize.X = m_bufferSize.getX();
		bufferSize.Y = m_bufferSize.getY();
		SMALL_RECT rect;
		rect.Top = 0;
		rect.Bottom = m_actualSize.getY();
		rect.Left = 0;
		rect.Right = m_actualSize.getX();
		WriteConsoleOutput(m_screenBuffer, m_pixelsBuffer, bufferSize, {0,0}, &rect);

		// Calculate and display framerate, and wait if fps limit is on
		framerateT2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> framerateDelta = framerateT2 - framerateT1;
		if (m_framerate > 0) {
			if (framerateDelta.count() < 1.0/m_framerate) {
				std::this_thread::sleep_for(std::chrono::microseconds( (long)( (1.0/m_framerate - framerateDelta.count())*1000000 )) );
			}
			swprintf_s(title, 256, L"%s - %dx%d - %3.2f fps / %d", m_appName.c_str(), m_actualSize.getX(), m_actualSize.getY(), 1.0/userDelta.count(), m_framerate);
		}
		else
			swprintf_s(title, 256, L"%s - %dx%d - %3.2f fps", m_appName.c_str(), m_actualSize.getX(), m_actualSize.getY(), 1.0/userDelta.count());

		SetConsoleTitle(title);
		framerateT1 = std::chrono::high_resolution_clock::now();
	}

	onDestroy();

	if (m_pixelsBuffer != nullptr)
		delete[] m_pixelsBuffer;
	m_pixelsBuffer = nullptr;

	m_stopCV.notify_all();
	return 0;
}

BOOL Draggoon::DConsoleEngine::consoleEventHandler(DWORD event) {
	if (event == CTRL_CLOSE_EVENT || event == CTRL_C_EVENT || event == CTRL_LOGOFF_EVENT || event == CTRL_SHUTDOWN_EVENT) {
		m_runEngine = false;
		using namespace std::literals;
		std::unique_lock lock(m_stopMutex);
		m_stopCV.wait_for(lock, 5s);
		return TRUE;
	}

	return FALSE;
}

Draggoon::Vector2D<int> Draggoon::DConsoleEngine::getSize() const {
	return m_actualSize;
}


// #######,  #######,     ###    #       #
// ##"""""#, ##"""""#,   ##"##   #,  ,  ,#
// ##     ## ##,,,,,#"  ##   ##  "#  #  #"
// ##     ## #######"  ##,,,,,##  # ,#, #
// ##     ## ##"""##,  #########  "## ##"
// ##,,,,,#" ##   "##, ##"""""##  "#" "#"
// #######"  ##    "## ##     ##   #   #

void Draggoon::DConsoleEngine::clearScreen() {
	memset(m_pixelsBuffer, 0, (unsigned long long)m_bufferSize.getArea()*sizeof(CHAR_INFO));
}

void Draggoon::DConsoleEngine::setPixel(Draggoon::Vector2D<int> t_pix, short t_char, Draggoon::Color<float> t_charColor, Draggoon::Color<float> t_backColor) {
	if (t_pix.isContainedIn(m_bufferSize)) {

		int bufferPos(t_pix.getY() * m_bufferSize.getX() + t_pix.getX());
		m_pixelsBuffer[bufferPos].Char.UnicodeChar = t_char;

		int brightChar(0), brightBack(0);
		if (t_charColor.getR()>0.5f || t_charColor.getG()>0.5f || t_charColor.getB()>0.5f)
			brightChar = 1;
		if (t_backColor.getR()>0.5f || t_backColor.getG()>0.5f || t_backColor.getB()>0.5f)
			brightBack = 1;

		m_pixelsBuffer[bufferPos].Attributes =
			(t_charColor.getR()>0.0f?1:0) << 2 |
			(t_charColor.getG()>0.0f?1:0) << 1 |
			(t_charColor.getB()>0.0f?1:0) |
			brightChar << 3 |
			(t_backColor.getR()>0.0f?1:0) << 6 |
			(t_backColor.getG()>0.0f?1:0) << 5 |
			(t_backColor.getB()>0.0f?1:0) << 4 |
			brightBack << 7;
	}
	else
		throw "Tried to draw outside of screen.";
}

void Draggoon::DConsoleEngine::setCharAlpha(Draggoon::Vector2D<int> t_pix, short t_char, Draggoon::Color<float> t_charColor) {
	if (t_pix.isContainedIn(m_bufferSize)) {
		int bufferPos(t_pix.getY() * m_bufferSize.getX() + t_pix.getX());
		m_pixelsBuffer[bufferPos].Char.UnicodeChar = t_char;

		int brightChar(0);
		if (t_charColor.getR()>0.5f || t_charColor.getG()>0.5f || t_charColor.getB()>0.5f)
			brightChar = 1;

		m_pixelsBuffer[bufferPos].Attributes = (m_pixelsBuffer[bufferPos].Attributes & 0xFFF0) |
			(t_charColor.getR()>0.0f?1:0) << 2 |
			(t_charColor.getG()>0.0f?1:0) << 1 |
			(t_charColor.getB()>0.0f?1:0) |
			brightChar << 3;
	}
	else
		throw "Tried to draw outside of screen.";
}

void Draggoon::DConsoleEngine::drawString(Draggoon::Vector2D<int> t_pix, const wchar_t * t_str, Draggoon::Color<float> t_charColor, Draggoon::Color<float> t_backColor) {
	int i(0);
	// TODO check if t_str goes outside or allow line break
	while (t_str[i]  != '\0') {
		setPixel({t_pix.getX()+i, t_pix.getY()}, t_str[i], t_charColor, t_backColor);
		++i;
	}
}

void Draggoon::DConsoleEngine::drawStringAlpha(Draggoon::Vector2D<int> t_pix, const wchar_t* t_str, Draggoon::Color<float> t_charColor) {
	int i(0);
	// TODO check if t_str goes outside or allow line break
	while (t_str[i]  != '\0') {
		setCharAlpha({t_pix.getX()+i, t_pix.getY()}, t_str[i], t_charColor);
		++i;
	}
}

void Draggoon::DConsoleEngine::drawLine(Draggoon::Vector2D<int> t_pix1, Draggoon::Vector2D<int> t_pix2, short t_char, Draggoon::Color<float> t_charColor, Draggoon::Color<float> t_backColor) {
	//if (m_actualSize.contains(t_pix1) && m_actualSize.contains(t_pix2)) {
		int x(t_pix1.getX());
		int y(t_pix1.getY());
		int dx = t_pix2.getX() - t_pix1.getX();
		int dy = t_pix2.getY() - t_pix1.getY();
		if (abs(dx)>=abs(dy)) {
			float slope((float)dy/dx);
			float err(0.0f);
			if (dx>=0)
				if (dy>=0)
					while (x<=t_pix2.getX()) {
						if (err>0.5f) {
							++y;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err+=slope;
						++x;
					}
				else
					while (x<=t_pix2.getX()) {
						if (err>0.5f) {
							--y;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err-=slope;
						++x;
					}
			else
				if (dy>=0)
					while (x>=t_pix2.getX()) {
						if (err>0.5f) {
							++y;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err-=slope;
						--x;
					}
				else
					while (x>=t_pix2.getX()) {
						if (err>0.5f) {
							--y;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err+=slope;
						--x;
					}
		}
		else {
			float slope((float)dx/dy);
			float err(0.0f);
			if (dx>=0)
				if (dy>=0)
					while (y<=t_pix2.getY()) {
						if (err>0.5f) {
							++x;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err+=slope;
						++y;
					}
				else
					while (y>=t_pix2.getY()) {
						if (err>0.5f) {
							++x;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err-=slope;
						--y;
					}
			else
				if (dy>=0)
					while (y<=t_pix2.getY()) {
						if (err>0.5f) {
							--x;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err-=slope;
						++y;
					}
				else
					while (y>=t_pix2.getY()) {
						if (err>0.5f) {
							--x;
							err -= 1.0f;
						}
						setPixel({x,y}, t_char, t_charColor, t_backColor);
						err+=slope;
						--y;
					}
		}
	//}
	//else
	//	throw "Drawing line outside of screen.";
}
