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

template<class T>
Draggoon::Vector2D<T>::Vector2D(const std::tuple<T, T>& t_vec) {
	std::tie(m_x, m_y) = t_vec;
}

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

// ######### ##,    ##  #######   #######  ##,    ## #########
// ##""""""  ###,   ## ##"""""##   "###"   ###,   ## ##""""""
// ##        ##"#,  ## ##           ###    ##"#,  ## ##
// ########  ## "#, ## ##   ###     ###    ## "#, ## ########
// ##"""""   ##  "#,## ##   ""##    ###    ##  "#,## ##"""""
// ##,,,,,,  ##   "### ##     ##   ,###,   ##   "### ##,,,,,,
// ######### ##    "##  #######   #######  ##    "## #########

Draggoon::DConsoleEngine::DConsoleEngine() :
m_engineThread(nullptr),
m_asyncRunning(false),
m_framerate(60),
m_appName(L"DConsoleEnging Default name"),
m_consoleHandle(nullptr),
m_pixelsBuffer(nullptr),
m_bufferWidth(0),
m_bufferHeight(0),
m_bufferSize(0),
m_desiredWidth(100),
m_desiredHeight(50),
m_actualWidth(0),
m_actualHeight(0),
m_clearScreenBeforeUpdate(true),
m_runEngine(false)
{
}

Draggoon::DConsoleEngine::~DConsoleEngine() {
	if (m_pixelsBuffer != nullptr)
		delete[] m_pixelsBuffer;
}


void Draggoon::DConsoleEngine::onDestroy() {

}

void Draggoon::DConsoleEngine::initConsole() {
	m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_consoleHandle == INVALID_HANDLE_VALUE)
		throw "Draggoon::DConsoleEngine::initConsole: Unable to get console handle.";
	m_originalScreenBuffer = m_consoleHandle;
	m_screenBuffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, 0, NULL, 0, NULL);

	bool r = SetConsoleActiveScreenBuffer(m_screenBuffer);
	if (!r) {
		throw "Draggoon::DConsoleEngine::initConsole: Unable to set the new screenBuffer.";
	}

	onResizeConsole({m_desiredWidth, m_desiredHeight});

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(m_screenBuffer, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(m_screenBuffer, &cursorInfo);
}

void Draggoon::DConsoleEngine::onResizeConsole(const Draggoon::Vector2D<int>& newSize) {
	CONSOLE_SCREEN_BUFFER_INFO oldScreenInfo;
	GetConsoleScreenBufferInfo(m_screenBuffer, &oldScreenInfo);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	fontInfo.nFont = 0;
	fontInfo.dwFontSize.X = 8;	// desiredPixelWidth
	fontInfo.dwFontSize.Y = 8;	// desiredPixelHeight
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;


	wcscpy_s(fontInfo.FaceName, L"Consolas");
	bool r = SetCurrentConsoleFontEx(m_screenBuffer, false, &fontInfo);
	if (!r) {
		throw "Draggoon::DConsoleEngine::onResizeConsole: Unable to set the font.";
	}

	SMALL_RECT size;
	size.Top = 0;
	size.Bottom = 1;
	size.Left = 0;
	size.Right = 1;
	r = SetConsoleWindowInfo(m_screenBuffer, TRUE, &size);
	if (!r) {
		throw "Draggoon::DConsoleEngine::onResizeConsole: Unable to set console window size to 0.";
	}

	COORD coord;
	coord.X = newSize.getX();
	coord.Y = newSize.getY();
	r = SetConsoleScreenBufferSize(m_screenBuffer, coord);
	if (!r) {
		throw "Draggoon::DConsoleEngine::onResizeConsole: Unable to set screen buffer size.";
	}


	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(m_screenBuffer, &screenInfo);
	if (newSize.getX() > screenInfo.dwMaximumWindowSize.X || newSize.getY() > screenInfo.dwMaximumWindowSize.Y)
		throw "Draggoon::DConsoleEngine::onResizeConsole: New size is greater than the maximum allowed.";

	if (newSize.getX() <= 0 || newSize.getY() <= 0) {
		throw "Draggoon::DConsoleEngine::onResizeConsole: New size is 0";
	}

	m_actualWidth = newSize.getX();
	m_actualHeight = newSize.getY();


	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = m_actualHeight-1;
	rect.Left = 0;
	rect.Right = m_actualWidth-1;
	r = SetConsoleWindowInfo(m_screenBuffer, TRUE, &rect);
	if (!r)
		throw "Draggoon::DConsoleEngine::onResizeConsole: Unable to set the console window height and width.";

	if (m_pixelsBuffer != nullptr)
		delete[] m_pixelsBuffer;
	m_bufferSize = m_actualHeight*m_actualWidth;
	m_pixelsBuffer = new CHAR_INFO[m_bufferSize];
	if (m_pixelsBuffer == nullptr)
		throw "Draggoon::DConsoleEngine::onResizeConsole: Memory allocation error.";
	memset(m_pixelsBuffer, 0, m_bufferSize);

	m_bufferWidth = m_actualWidth;
	m_bufferHeight = m_actualHeight;

}

void Draggoon::DConsoleEngine::start(const bool & t_nonBlocking) {
	m_asyncRunning = t_nonBlocking;


	try {
		initConsole();
	}
	catch (const char* s) {
		SetConsoleActiveScreenBuffer(m_originalScreenBuffer);
		std::cerr << s << " (Last error = " << GetLastError() << ")" << std::endl;
		throw "Draggoon::DConsoleEngine::start: Console initialization failed.";
	}

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
		throw "DConsoleEngine::stop: Engine not running";

	m_engineThread->join();
	delete m_engineThread;

	SetConsoleActiveScreenBuffer(m_originalScreenBuffer);
}

void Draggoon::DConsoleEngine::engineFunction() {
	/*try {
		initConsole();
	}
	catch (const char* s) {
		std::cerr << "Unable to init console: " << std::endl << "\t" << s << std::endl;
		m_runEngine = false;
	}*/

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

		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(m_screenBuffer, &info);
		int width = info.srWindow.Right - info.srWindow.Left + 1;
		int height = info.srWindow.Bottom - info.srWindow.Top + 1;
		if (width != m_actualWidth || height != m_actualHeight) {
			try {
				onResizeConsole(Draggoon::Vector2D<int>(width, height));
			}
			catch (const char* s) {
				std::cerr << s << std::endl;
			}
		}

		if (m_clearScreenBeforeUpdate)
			memset(m_pixelsBuffer, 0, m_actualHeight*m_actualWidth*sizeof(CHAR_INFO));

		// Let the user update the screen
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> userDelta = t2-t1;
		t1 = t2;
		if (!onUpdate(userDelta)) {
			m_runEngine = false;
			break;
		}


		// Refresh the screen
		COORD bufferSize;
		bufferSize.X = m_bufferWidth;
		bufferSize.Y = m_bufferHeight;
		SMALL_RECT rect;
		rect.Top = 0;
		rect.Bottom = m_actualHeight;
		rect.Left = 0;
		rect.Right = m_actualWidth;
		WriteConsoleOutput(m_screenBuffer, m_pixelsBuffer, bufferSize, {0,0}, &rect);

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

// #######,  #######,     ###    #       #
// ##"""""#, ##"""""#,   ##"##   #,  ,  ,#
// ##     ## ##,,,,,#"  ##   ##  "#  #  #"
// ##     ## #######"  ##,,,,,##  # ,#, #
// ##     ## ##"""##,  #########  "## ##"
// ##,,,,,#" ##   "##, ##"""""##  "#" "#"
// #######"  ##    "## ##     ##   #   #

Draggoon::Vector2D<int> Draggoon::DConsoleEngine::getSize() {
	return {m_actualWidth, m_actualHeight};
}

void Draggoon::DConsoleEngine::setPixel(Vector2D<int> t_pix, short t_char, Color<float> t_charColor, Color<float> t_backColor) {
	int bufferPos(t_pix.getY() * m_bufferWidth + t_pix.getX());
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
		(t_backColor.getR()>0.0f?1:0) << 7 |
		(t_backColor.getG()>0.0f?1:0) << 6 |
		(t_backColor.getB()>0.0f?1:0) << 5 |
		brightBack << 8;

}

void Draggoon::DConsoleEngine::drawString(Vector2D<int> t_pix, const char * t_str, Color<float> t_charColor, Color<float> t_backColor) {
	int i(0);
	while (t_str[i]  != '\0')
		setPixel({t_pix.getX()+i, t_pix.getY()}, t_str[i++], t_charColor, t_backColor);
}
