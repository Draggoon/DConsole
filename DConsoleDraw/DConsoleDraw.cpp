#include "DConsoleDraw.h"



DConsoleDraw::DConsoleDraw() :
m_drawSize(16, 16),
m_colors{Draggoon::COLOR_F_WHITE,
	Draggoon::COLOR_F_GRAY,
	Draggoon::COLOR_F_BLACK,
	Draggoon::COLOR_F_DARK_GRAY,
	Draggoon::COLOR_F_RED,
	Draggoon::COLOR_F_DARK_RED,
	Draggoon::COLOR_F_YELLOW,
	Draggoon::COLOR_F_DARK_YELLOW,
	Draggoon::COLOR_F_GREEN,
	Draggoon::COLOR_F_DARK_GREEN,
	Draggoon::COLOR_F_CYAN,
	Draggoon::COLOR_F_DARK_CYAN,
	Draggoon::COLOR_F_BLUE,
	Draggoon::COLOR_F_DARK_BLUE,
	Draggoon::COLOR_F_MAGENTA,
	Draggoon::COLOR_F_DARK_MAGENTA},
m_selectedColor(0),
m_randomGenerator((int)std::chrono::high_resolution_clock::now().time_since_epoch().count()),
m_drawBuffer(nullptr),
m_drawAreaChanged(true)
{
	m_drawBuffer = new char[m_drawSize.getArea()];
	memset(m_drawBuffer, 0, m_drawSize.getArea());
}

DConsoleDraw::~DConsoleDraw() {
	delete[] m_drawBuffer;
}

bool DConsoleDraw::onCreate() {
	m_appName = L"Draw";

	m_framerate = 60;
	m_clearScreenBeforeUpdate = false;

	m_stretchOnResize = true;
	setDrawSize(m_drawSize);
	return true;
}

bool DConsoleDraw::onInitialized() {
	clearScreen();

	return true;
}

bool DConsoleDraw::onInputUpdate(const Draggoon::Key * keys, const size_t & keyCount, const Draggoon::Key * mouseBtn, const size_t & mouseBtnCount) {
	if (keys[VK_ESCAPE].isPressed())
		internalStop();

	if (keys[VK_CONTROL].isDown()) {
		if (keys['O'].isPressed()) {
			OPENFILENAMEA ofna{0};
			ofna.lStructSize = sizeof(ofna);
			const int size(1024);
			char buffer[size]{0};
			ofna.lpstrFile = buffer;
			ofna.nMaxFile = size;
			ofna.lpstrTitle = "Open drawing";
			if (GetOpenFileNameA(&ofna) != 0) {
				std::string fn(buffer);
				m_filename.assign(fn.length(), L' ');
				std::copy(fn.begin(), fn.end(), m_filename.begin());
				loadFromFile();
			}
			else {
				MessageBoxA(NULL, "Unable to open file", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		if (keys['S'].isPressed()) {
			OPENFILENAMEA ofna{0};
			ofna.lStructSize = sizeof(ofna);
			const int size(1024);
			char buffer[size]{0};
			ofna.lpstrFile = buffer;
			ofna.nMaxFile = size;
			ofna.lpstrTitle = "Save drawing";
			if (GetSaveFileNameA(&ofna) != 0) {
				std::string fn(buffer);
				m_filename.assign(fn.length(), L' ');
				std::copy(fn.begin(), fn.end(), m_filename.begin());
				saveTofile();
			}
			else {
				MessageBoxA(NULL, "Unable to save file", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		if (keys['R'].isPressed()) {
			std::uniform_int_distribution<int> dist(0, 0x0F);
			for (int y(0); y<m_drawSize.getY(); ++y)
				for (int x(0); x < m_drawSize.getX(); ++x)
					m_drawBuffer[y*m_drawSize.getX() + x] = dist(m_randomGenerator);

			m_drawAreaChanged = true;
		}
	}

	if (keys['1'].isPressed())
		if(keys[VK_SHIFT].isDown())
			m_selectedColor = 1;
		else
			m_selectedColor = 0;
	if (keys['2'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 3;
		else
			m_selectedColor = 2;
	if (keys['3'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 5;
		else
			m_selectedColor = 4;
	if (keys['4'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 7;
		else
			m_selectedColor = 6;
	if (keys['5'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 9;
		else
			m_selectedColor = 8;
	if (keys['6'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 11;
		else
			m_selectedColor = 10;
	if (keys['7'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 13;
		else
			m_selectedColor = 12;
	if (keys['8'].isPressed())
		if (keys[VK_SHIFT].isDown())
			m_selectedColor = 15;
		else
			m_selectedColor = 14;

	if (mouseBtn[0].isDown()) {
		int mx(getMousePosition().getX()), my(getMousePosition().getY());
		if (mx > 1 && mx <= m_drawSize.getX()+1 && my > 4 && my <= m_drawSize.getY()+4) {
			m_drawBuffer[(my-5) * m_drawSize.getX() + mx-2] = m_colors[m_selectedColor].getConsoleColor();
			m_drawAreaChanged = true;
		}
		else if (mx > 7 && mx <= 7+COLOR_COUNT && my == 1) {
			m_selectedColor = mx-8;
		}
	}
	
	if (mouseBtn[1].isDown()) {
		int mx(getMousePosition().getX()), my(getMousePosition().getY());
		if (mx > 1 && mx <= m_drawSize.getX()+1 && my > 4 && my <= m_drawSize.getY()+4) {
			m_drawBuffer[(my-5) * m_drawSize.getX() + mx-2] = m_colors[COLOR_COUNT-1].getConsoleColor();
			m_drawAreaChanged = true;
		}
	}

	return true;
}

void DConsoleDraw::setDrawSize(Draggoon::Vector2D<int> newSize) {
	if (newSize.getX() > 0 && newSize.getY() > 0) {
		setPixelSize({12,12});

		if (m_drawSize.getX()+4 < 8+COLOR_COUNT+1)
			setSize({COLOR_COUNT+8+1 ,m_drawSize.getY()+7});
		else
			setSize({m_drawSize.getX()+4,m_drawSize.getY()+7});

		if (m_drawBuffer != nullptr)
			delete[] m_drawBuffer;
		m_drawSize = newSize;
		m_drawBuffer = new char[m_drawSize.getArea()];
		memset(m_drawBuffer, 0, m_drawSize.getArea());

		m_drawAreaChanged = true;
	}
}

void DConsoleDraw::loadFromFile() {
	std::ifstream fstr(m_filename, std::ios::binary | std::ios::in | std::ios::ate);
	if (fstr.is_open()) {
		int fSize = (int)fstr.tellg();
		fstr.seekg(0, std::ios::beg);
		if (fSize < 8)
			throw "DConsoleDraw::loadFromFile: Error while reading file (file too short).";
		char sizeHeader[8];
		fstr.read(sizeHeader, 8);
		Draggoon::Vector2D<int> newSize(sizeHeader[0] | (sizeHeader[1] << 8) | (sizeHeader[2] << 16) | (sizeHeader[3] << 24),
			sizeHeader[4] | (sizeHeader[5] << 8) | (sizeHeader[6] << 16) | (sizeHeader[7] << 24));
		if(newSize.getArea() != fSize - 8)
			throw "DConsoleDraw::loadFromFile: Error while reading file (file too short).";

		setDrawSize(newSize);

		fstr.read(m_drawBuffer, newSize.getArea());
	}
	else
		throw "DConsoleDraw::loadFromFile: could not open file.";
}

void DConsoleDraw::saveTofile() {
	std::ofstream fstr(m_filename, std::ios::binary | std::ios::out | std::ios::trunc);
	if (fstr.is_open()) {
		char sizeHeader[8];
		sizeHeader[0] = m_drawSize.getX() & 0x0000'00FF;
		sizeHeader[1] = (m_drawSize.getX() & 0x0000'FF00) >> 8;
		sizeHeader[2] = (m_drawSize.getX() & 0x00FF'0000) >> 16;
		sizeHeader[3] = (m_drawSize.getX() & 0xFF00'0000) >> 24;
		sizeHeader[4] = m_drawSize.getY() & 0x0000'00FF;
		sizeHeader[5] = (m_drawSize.getY() & 0x0000'FF00) >> 8;
		sizeHeader[6] = (m_drawSize.getY() & 0x00FF'0000) >> 16;
		sizeHeader[7] = (m_drawSize.getY() & 0xFF00'0000) >> 24;
		fstr.write(sizeHeader, 8);
		fstr.write(m_drawBuffer, m_drawSize.getArea());
	}
	else
		throw "DConsoleDraw::saveTofile: could not open file.";
}

bool DConsoleDraw::onScreenUpdate(const std::chrono::duration<float>& elapsedTime) {
	// UI
	drawRect({1,4}, {m_drawSize.getX()+2, m_drawSize.getY()+5}, Draggoon::COLOR_F_GRAY);
	drawString({8,0}, L"1 2 3 4 5 6 7 8", Draggoon::COLOR_F_GRAY);
	drawString({1,1}, L"Color:", Draggoon::COLOR_F_GRAY);
	for (int i(0); i < COLOR_COUNT; ++i) {
		if (m_selectedColor == i) {
			setChar({8+i, 1}, Draggoon::C_FULL_BLOCK, m_colors[i]);
			setChar({8+i, 2}, '^');
		}
		else {
			setChar({8+i, 1}, Draggoon::C_DARK_SHADE, m_colors[i], Draggoon::COLOR_F_BLACK);
			setChar({8+i, 2}, Draggoon::C_SPACE);
		}
	}

	// Draw area
	if (m_drawAreaChanged) {
		for (int i(0); i<m_drawSize.getArea(); ++i) {
			setPixel({i%m_drawSize.getX() + 2, i/m_drawSize.getX() +5}, m_drawBuffer[i]);
		}
		m_drawAreaChanged = false;
	}

	return true;
}



int main(int argc, char** argv) {
	int x(16), y(16);
	DConsoleDraw d;

	if (argc >= 3) {
		x = std::stoi(argv[1]);
		y = std::stoi(argv[2]);
	}
	if (x >= 8 && y >= 8)
		d.setDrawSize({x,y});
	else
		d.setDrawSize({16,16});


	try {
		d.externalStart();
	}
	catch (const char* e) {
		std::cerr << e << std::endl;
		Sleep(2000);
	}

	return 0;
}