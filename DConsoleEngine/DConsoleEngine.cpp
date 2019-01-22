#include "DConsoleEngine.h"


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
	m_enableStats(false),
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
	m_bufferSize(0, 0),
	m_actualSize(0, 0),
	m_mousePosition(0, 0),
	m_cerrFile("errorLog.txt", std::ios_base::out | std::ios_base::trunc),
	m_origCerr(nullptr)
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

	if (m_cerrFile.is_open())
		m_origCerr = std::cerr.rdbuf(m_cerrFile.rdbuf());

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


	std::chrono::time_point<std::chrono::high_resolution_clock> tStartLoop(std::chrono::high_resolution_clock::now()),
																tInputProcessed(std::chrono::high_resolution_clock::now()),
																tResized(std::chrono::high_resolution_clock::now()),
																tScreenCleaned(std::chrono::high_resolution_clock::now()),
																tUserDelta(std::chrono::high_resolution_clock::now()),
																tBufferUpdated(std::chrono::high_resolution_clock::now()),
																tScreenRefreshed(std::chrono::high_resolution_clock::now()),
																tFramerateAdapted(std::chrono::high_resolution_clock::now());

	wchar_t title[256];

	// Main loop (Input processing, drawing, framerate limiting)
	while (m_runEngine) {
		tStartLoop = std::chrono::high_resolution_clock::now();

		// Process inputs
		for (int i(0); i<KEY_COUNT; ++i) {
			m_key[i].setState((GetAsyncKeyState(i) & 0x8000) != 0);	// GetAsyncKeyState returns a short, MSB set = key is down. 
																	// (LSB set = key pressed since last call, but not reliable)
		}
		INPUT_RECORD record;
		DWORD nbToRead, nbRead;
		bool mouseBtnDownTemp[MOUSE_BTN_COUNT];
		for (int i(0); i<MOUSE_BTN_COUNT; ++i) {
			mouseBtnDownTemp[i] = m_mouseBtn[i].isDown();
		}
		// Must check if events are available, otherwise ReadConsoleInput hangs until an event arrives.
		GetNumberOfConsoleInputEvents(m_inputHandle, &nbToRead);
		while (nbToRead>0) {
			ReadConsoleInput(m_inputHandle, &record, 1, &nbRead);
			switch (record.EventType) {
			case MOUSE_EVENT: {
				m_mousePosition = {record.Event.MouseEvent.dwMousePosition.X, record.Event.MouseEvent.dwMousePosition.Y};
				for(int i(0); i<MOUSE_BTN_COUNT; ++i)
					mouseBtnDownTemp[i] = (record.Event.MouseEvent.dwButtonState & (1<<i)) != 0;
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
		for (int i(0); i<MOUSE_BTN_COUNT; ++i)
			m_mouseBtn[i].setState(mouseBtnDownTemp[i]);

		if (!onInputUpdate(m_key, KEY_COUNT, m_mouseBtn, MOUSE_BTN_COUNT)) {
			m_runEngine = false;
		}
		tInputProcessed = std::chrono::high_resolution_clock::now();

		// Check Window size
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
		tResized = std::chrono::high_resolution_clock::now();

		if (m_clearScreenBeforeUpdate)
			clearScreen();
		tScreenCleaned = std::chrono::high_resolution_clock::now();

		// Let the user update the screen
		std::chrono::duration<float> userDelta = tScreenCleaned-tUserDelta;
		tUserDelta = tScreenCleaned;
		if (!onScreenUpdate(userDelta)) {
			m_runEngine = false;
			break;
		}
		tBufferUpdated = std::chrono::high_resolution_clock::now();

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

		tScreenRefreshed = std::chrono::high_resolution_clock::now();

		// Calculate and display framerate, and wait (if fps limit is on)
		std::chrono::duration<float> framerateDelta = tScreenRefreshed - tFramerateAdapted;
		if (m_framerate > 0) {
			if (framerateDelta.count() < 1.0/m_framerate) {
				std::this_thread::sleep_for(std::chrono::microseconds( (long)( (1.0/m_framerate - framerateDelta.count())*1000000 )) );
			}
			swprintf_s(title, 256, L"%s - %dx%d - %3.2f/%d fps", m_appName.c_str(), m_actualSize.getX(), m_actualSize.getY(), 1.0/userDelta.count(), m_framerate);
		}
		else
			swprintf_s(title, 256, L"%s - %dx%d - %3.2f fps", m_appName.c_str(), m_actualSize.getX(), m_actualSize.getY(), 1.0/userDelta.count());

		SetConsoleTitle(title);
		tFramerateAdapted = std::chrono::high_resolution_clock::now();
		if (m_enableStats) {
			float dInput(std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tInputProcessed-tStartLoop).count());
			float dResize(std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tResized-tInputProcessed).count());
			float dClean(std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tScreenCleaned-tResized).count());
			float dUpdate(std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tBufferUpdated-tScreenCleaned).count());
			float dRefresh(std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tScreenRefreshed-tBufferUpdated).count());
			float dFramerate(std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tFramerateAdapted-tScreenRefreshed).count());
			float dTotal(dInput+dResize+dRefresh+dClean+dUpdate+dRefresh+dFramerate);

			swprintf_s(m_lastFrameStats, 256,
				L"Input Processing: %5.3f, (%3.f%%)\n"
				"Resizing: %13.3f, (%3.f%%)\n"
				"Screen cleaning: %6.3f, (%3.f%%)\n"
				"User update: %10.3f, (%3.f%%)\n"
				"Screen refresh: %7.3f, (%3.f%%)\n"
				"Framerate cap: %8.3f, (%3.f%%)\n",
				dInput, dInput/dTotal*100.0f,
				dResize, dResize/dTotal*100.0f,
				dClean, dClean/dTotal*100.0f,
				dUpdate, dUpdate/dTotal*100.0f,
				dRefresh, dRefresh/dTotal*100.0f,
				dFramerate, dFramerate/dTotal*100.0f);
		}
	}

	onDestroy();

	if (m_origCerr != nullptr) {
		std::cerr.rdbuf(m_origCerr);
		m_cerrFile.close();
	}

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

void Draggoon::DConsoleEngine::setPixel(Vector2D<int> t_pix, Color<float> t_color) {
	setChar(t_pix, C_SPACE, COLOR_F_TRANSPARENT, t_color);
}

void Draggoon::DConsoleEngine::setChar(Draggoon::Vector2D<int> t_pix, short t_char, Draggoon::Color<float> t_charColor, Draggoon::Color<float> t_backColor) {
	if (t_pix.isContainedIn(m_bufferSize)) {

		int bufferPos(t_pix.getY() * m_bufferSize.getX() + t_pix.getX());
		if(!t_charColor.isTransparent())
			m_pixelsBuffer[bufferPos].Char.UnicodeChar = t_char;

		bool brightChar(false), brightBack(false);
		brightChar = t_charColor.getR()>0.5f || t_charColor.getG()>0.5f || t_charColor.getB()>0.5f;
		brightBack = t_backColor.getR()>0.5f || t_backColor.getG()>0.5f || t_backColor.getB()>0.5f;

		if(!t_charColor.isTransparent()) {
			m_pixelsBuffer[bufferPos].Attributes = (m_pixelsBuffer[bufferPos].Attributes & 0xFFF0) |
				(t_charColor.getR()>0.0f?1:0) << 2 |
				(t_charColor.getG()>0.0f?1:0) << 1 |
				(t_charColor.getB()>0.0f?1:0) |
				(brightChar?1:0) << 3;
		}
		if (!t_backColor.isTransparent()) {
			m_pixelsBuffer[bufferPos].Attributes = (m_pixelsBuffer[bufferPos].Attributes & 0xFF0F) |
				(t_backColor.getR()>0.0f?1:0) << 6 |
				(t_backColor.getG()>0.0f?1:0) << 5 |
				(t_backColor.getB()>0.0f?1:0) << 4 |
				(brightBack?1:0) << 7;
		}
	}
	//else
	//	throw "Tried to draw outside of screen.";
}

void Draggoon::DConsoleEngine::setCharAlpha(Draggoon::Vector2D<int> t_pix, short t_char, Draggoon::Color<float> t_charColor) {
	setChar(t_pix, t_char, t_charColor, COLOR_F_TRANSPARENT);
	return;
	if (t_pix.isContainedIn(m_bufferSize)) {
		int bufferPos(t_pix.getY() * m_bufferSize.getX() + t_pix.getX());
		m_pixelsBuffer[bufferPos].Char.UnicodeChar = t_char;

		bool brightChar(false);
		brightChar = t_charColor.getR()>0.5f || t_charColor.getG()>0.5f || t_charColor.getB()>0.5f;

		m_pixelsBuffer[bufferPos].Attributes = (m_pixelsBuffer[bufferPos].Attributes & 0xFFF0) |
			(t_charColor.getR()>0.0f?1:0) << 2 |
			(t_charColor.getG()>0.0f?1:0) << 1 |
			(t_charColor.getB()>0.0f?1:0) |
			(brightChar?1:0) << 3;
	}
	//else
	//	throw "Tried to draw outside of screen.";
}

void Draggoon::DConsoleEngine::drawString(Draggoon::Vector2D<int> t_pix, const wchar_t * t_str, Draggoon::Color<float> t_charColor, Draggoon::Color<float> t_backColor) {
	int i(0), x(t_pix.getX()), y(t_pix.getY());
	while (t_str[i]  != '\0') {
		if (t_str[i] == '\n' || x >= m_bufferSize.getX()) {
			x = t_pix.getX();
			y += 1;
		}
		if(t_str[i] != '\n') {
			if (m_bufferSize.contains({x,y}))
				setChar({x, y}, t_str[i], t_charColor, t_backColor);
			//else
			//	throw "Drawing string outside of screen.";
			x+=1;
		}
		++i;
	}
}

void Draggoon::DConsoleEngine::drawStringAlpha(Draggoon::Vector2D<int> t_pix, const wchar_t* t_str, Draggoon::Color<float> t_charColor) {
	int i(0), x(t_pix.getX()), y(t_pix.getY());
	while (t_str[i]  != '\0') {
		if (t_str[i] == '\n' || x >= m_bufferSize.getX()) {
			x = t_pix.getX();
			y += 1;
		}
		if (t_str[i] != '\n') {
			if (m_bufferSize.contains({x,y}))
				setCharAlpha({x, y}, t_str[i], t_charColor);
			//else
			//	throw "Drawing string outside of screen.";
			x+=1;
		}
		++i;
	}
}

void Draggoon::DConsoleEngine::drawLineChar(Draggoon::Vector2D<int> t_pix1, Draggoon::Vector2D<int> t_pix2, short t_char, Draggoon::Color<float> t_charColor, Draggoon::Color<float> t_backColor) {
	int x(t_pix1.getX());
	int y(t_pix1.getY());
	int dx = t_pix2.getX() - t_pix1.getX();
	int dy = t_pix2.getY() - t_pix1.getY();
	if (t_pix1 == t_pix2) {
		setChar({x,y}, t_char, t_charColor, t_backColor);
	}
	else if (abs(dx)>=abs(dy)) {
		float slope((float)dy/dx);
		float err(0.0f);
		if (dx>=0)
			if (dy>=0)
				while (x<=t_pix2.getX()) {
					if (err>0.5f) {
						++y;
						err -= 1.0f;
					}
					setChar({x,y}, t_char, t_charColor, t_backColor);
					err+=slope;
					++x;
				}
			else
				while (x<=t_pix2.getX()) {
					if (err>0.5f) {
						--y;
						err -= 1.0f;
					}
					setChar({x,y}, t_char, t_charColor, t_backColor);
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
					setChar({x,y}, t_char, t_charColor, t_backColor);
					err-=slope;
					--x;
				}
			else
				while (x>=t_pix2.getX()) {
					if (err>0.5f) {
						--y;
						err -= 1.0f;
					}
					setChar({x,y}, t_char, t_charColor, t_backColor);
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
					setChar({x,y}, t_char, t_charColor, t_backColor);
					err+=slope;
					++y;
				}
			else
				while (y>=t_pix2.getY()) {
					if (err>0.5f) {
						++x;
						err -= 1.0f;
					}
					setChar({x,y}, t_char, t_charColor, t_backColor);
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
					setChar({x,y}, t_char, t_charColor, t_backColor);
					err-=slope;
					++y;
				}
			else
				while (y>=t_pix2.getY()) {
					if (err>0.5f) {
						--x;
						err -= 1.0f;
					}
					setChar({x,y}, t_char, t_charColor, t_backColor);
					err+=slope;
					--y;
				}
	}
}

void Draggoon::DConsoleEngine::drawLine(Draggoon::Vector2D<int> t_pix1, Draggoon::Vector2D<int> t_pix2, Color<float> t_color) {
	drawLineChar(t_pix1, t_pix2, C_SPACE, COLOR_F_TRANSPARENT, t_color);
}

void Draggoon::DConsoleEngine::drawRect(Vector2D<int> t_pix1, Vector2D<int> t_pix2, Color<float> t_color) {
	drawLine(t_pix1, {t_pix1.getX(), t_pix2.getY()}, t_color);
	drawLine({t_pix1.getX(), t_pix2.getY()}, t_pix2, t_color);
	drawLine(t_pix2, {t_pix2.getX(), t_pix1.getY()}, t_color);
	drawLine({t_pix2.getX(), t_pix1.getY()}, t_pix1, t_color);
}

void Draggoon::DConsoleEngine::fillRect(Vector2D<int> t_pix1, Vector2D<int> t_pix2, Color<float> t_color) {
	if (t_pix1.getY() > t_pix2.getY()) {
		for (int i(0); i<t_pix1.getY()-t_pix2.getY()+1; ++i) {
			drawLine({t_pix2.getX(), t_pix2.getY()+i}, {t_pix1.getX(), t_pix2.getY()+i}, t_color);
		}
	}
	else if (t_pix2.getY() > t_pix1.getY()) {
		for (int i(0); i<t_pix2.getY()-t_pix1.getY()+1; ++i) {
			drawLine({t_pix1.getX(), t_pix1.getY()+i}, {t_pix2.getX(), t_pix1.getY()+i}, t_color);
		}
	}
	else
		drawLine(t_pix1, t_pix2, t_color);
}
