/*
*	Test frame for DConsoleEngine
*
*/

#include "TestMain.h"


using namespace std::chrono_literals;

using namespace Draggoon;

bool TestUser::TestEngine::onCreate() {
	m_appName = L"Test";
	m_framerate = 144;
	m_stretchOnResize = true;
	setSize({256,256});
	setPixelSize({4,4});

	m_clearScreenBeforeUpdate = false;

	m_sumDeltaTime = 0.0f;
	m_count = 0.0f;
	m_frameCount = 0;
	m_turnCCW = false;
	m_turnCW = false;
	m_turnFast = false;

	m_mouseMoved = false;

	m_tampon = {0};
	m_tamponSet = false;
	m_picture = nullptr;
	m_pictureSize ={0,0};




	return true;
}

bool TestUser::TestEngine::onInitialized() {
	clearScreen();
	return true;
}

bool TestUser::TestEngine::onInputUpdate(const Key * keys, const size_t & keyCount, const Key * mouseBtn, const size_t & mouseBtnCount) {

	if (keys[VK_ESCAPE].isPressed()) {
		return false;
	}

	m_turnCCW = keys['A'].isDown();

	m_turnCW = keys['D'].isDown();

	m_turnFast = keys[VK_SHIFT].isDown();

	if (keys[VK_SPACE].isPressed())
		clearScreen();

	if (keys[VK_F1].isPressed() && m_framerate != 0)
		m_framerate = 0;
	else if (keys[VK_F1].isPressed())
		m_framerate = 144;

	if (keys[VK_F2].isPressed() && m_clearScreenBeforeUpdate)
		m_clearScreenBeforeUpdate = false;
	else if (keys[VK_F2].isPressed())
		m_clearScreenBeforeUpdate = true;

	if (keys[VK_F3].isPressed() && m_enableStats)
		m_enableStats = false;
	else if (keys[VK_F3].isPressed())
		m_enableStats = true;

	if (mouseBtn[0].isPressed()) {
		m_btnPressLocation = getMousePosition();
	}
	m_btnDown = mouseBtn[0].isDown();
	if (mouseBtn[0].isReleased()) {
		if (m_btnPressLocation == getMousePosition()) {
			m_tampon = getCharInfo(m_btnPressLocation);
			m_tamponSet = true;
		}
		m_btnReleaseLocation = getMousePosition();
	}

	if (keys[VK_SPACE].isPressed()) {
		m_tamponSet = false;
	}

	if (keys[VK_CONTROL].isDown()) {
		if (keys['O'].isPressed()) {
			std::wstring fname;
			OPENFILENAMEA ofna{0};
			ofna.lStructSize = sizeof(ofna);
			const int size(1024);
			char buffer[size]{0};
			ofna.lpstrFile = buffer;
			ofna.nMaxFile = size;
			ofna.lpstrTitle = "Open drawing";
			if (GetOpenFileNameA(&ofna) != 0) {
				std::string fn(buffer);
				fname.assign(fn.length(), L' ');
				std::copy(fn.begin(), fn.end(), fname.begin());

				unsigned char headerSize[4];
				unsigned char rgb[3];
				std::ifstream f(fname, std::ios::in | std::ios::binary);
				f.seekg(0x0A, f.beg);
				f.read((char*)headerSize, 4);
				int32_t offset = headerSize[0] | (headerSize[1] << 8) | (headerSize[2] << 16) | (headerSize[3] << 24);
				f.seekg(offset, f.beg);
				if (m_picture != nullptr)
					delete[] m_picture;
				m_picture = new Draggoon::Color<float>[256*256];
				m_pictureSize ={256,256};
				if (f.fail()) {
					MessageBoxA(NULL, "FILE READ ERROR", "Error", MB_OK | MB_ICONEXCLAMATION);
				}
				for (int i(0); i<m_pictureSize.getArea(); ++i) {
					try {
						f.read((char*)rgb, 3);
					}
					catch (std::ios_base::failure& e) {
						std::string s(e.what());
						std::cout << s.c_str();
					}
					if (f.fail()) {
						char s[256];
						strerror_s(s, 256, errno);
						MessageBoxA(NULL, "FILE READ ERROR", "Error", MB_OK | MB_ICONEXCLAMATION);
						std::cout << s;
						break;
					}
					m_picture[i] = Draggoon::Color<float>(rgb[2]/255.0f, rgb[1]/255.0f, rgb[0]/255.0f);
				}


			}
			else {
				MessageBoxA(NULL, "Unable to open file", "Error", MB_OK | MB_ICONEXCLAMATION);
			}

		}
	}


	return true;
}


bool TestUser::TestEngine::onScreenUpdate(const std::chrono::duration<float> &elapsedTimeSeconds) {
	wchar_t str[64];
	m_sumDeltaTime += elapsedTimeSeconds.count();
	++m_frameCount;

	if (m_turnCCW) {
		if(m_turnFast)
			m_count -= elapsedTimeSeconds.count()*5.0f;
		else
			m_count -= elapsedTimeSeconds.count()*0.25f;
	}

	if (m_turnCW) {
		if (m_turnFast)
			m_count += elapsedTimeSeconds.count()*5.0f;
		else
			m_count += elapsedTimeSeconds.count()*0.25f;
	}

	Vector2D<int> size = getSize();
	try {
		
		//for (int j(0); j<size.getY(); ++j) {
		//	for (int i(0); i< size.getX(); ++i) {
		//		if (i%2 == 0 && j%2 == 0 || i%2 != 0 && j%2 != 0)
		//			setChar({i,j}, ' ', COLOR_F_WHITE, COLOR_F_GRAY);
		//		else
		//			setChar({i,j}, ' ', COLOR_F_BLACK, COLOR_F_WHITE);
		//	}
		//}






		int centerX(10);// size.getX()/2);
		int centerY(size.getY()/2);
		float outX(centerX+15*cos(6.28f*20*m_count/100.0f));
		float outY(centerY+15*sin(6.28f*20*m_count/100.0f));

		drawRect({centerX,centerY}, {(int)outX,(int)outY}, COLOR_F_RED);
		drawLine({8,8}, {8,8});


		if (m_btnDown) {
			if (m_btnPressLocation != getMousePosition() || m_mouseMoved) {
				if(m_mouseMoved)
					fillRect(m_btnPressLocation, getMousePosition());
				m_mouseMoved = true;
			}
		}
		else
			m_mouseMoved = false;

		if (m_picture != nullptr) {
			for (int y(0); y<m_pictureSize.getY(); ++y) {
				for (int x(0); x<m_pictureSize.getX(); ++x) {
					setChar({x,m_pictureSize.getY()-y-1}, m_picture[y*m_pictureSize.getX()+x].getConsoleChar());
				}
			}
		}

		//int i(1);
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());
		//setChar({i++,26}, Draggoon::Color<float>((i-1)/16.0f, (i-1)/16.0f, (i-1)/16.0f).getConsoleChar());

		//for (int j(28); j<30; ++j) {
		//	i = 1;
		//	setChar({i++,j}, C_SPACE, COLOR_F_TRANSPARENT, COLOR_F_WHITE);

		//	setChar({i++,j}, C_LIGHT_SHADE, COLOR_F_GRAY, COLOR_F_WHITE);
		//	setChar({i++,j}, C_MEDIUM_SHADE, COLOR_F_GRAY, COLOR_F_WHITE);
		//	setChar({i++,j}, C_MEDIUM_SHADE, COLOR_F_WHITE, COLOR_F_GRAY);
		//	setChar({i++,j}, C_LIGHT_SHADE, COLOR_F_WHITE, COLOR_F_GRAY);

		//	setChar({i++,j}, C_SPACE, COLOR_F_TRANSPARENT, COLOR_F_GRAY);

		//	setChar({i++,j}, C_LIGHT_SHADE, COLOR_F_DARK_GRAY, COLOR_F_GRAY);
		//	setChar({i++,j}, C_MEDIUM_SHADE, COLOR_F_DARK_GRAY, COLOR_F_GRAY);
		//	setChar({i++,j}, C_MEDIUM_SHADE, COLOR_F_GRAY, COLOR_F_DARK_GRAY);
		//	setChar({i++,j}, C_LIGHT_SHADE, COLOR_F_GRAY, COLOR_F_DARK_GRAY);

		//	setChar({i++,j}, C_SPACE, COLOR_F_TRANSPARENT, COLOR_F_DARK_GRAY);

		//	setChar({i++,j}, C_LIGHT_SHADE, COLOR_F_BLACK, COLOR_F_DARK_GRAY);
		//	setChar({i++,j}, C_MEDIUM_SHADE, COLOR_F_BLACK, COLOR_F_DARK_GRAY);
		//	setChar({i++,j}, C_MEDIUM_SHADE, COLOR_F_DARK_GRAY, COLOR_F_BLACK);
		//	setChar({i++,j}, C_LIGHT_SHADE, COLOR_F_DARK_GRAY, COLOR_F_BLACK);

		//	setChar({i++,j}, C_SPACE, COLOR_F_TRANSPARENT, COLOR_F_BLACK);
		//}
		//drawString({1,30}, L"00000000001111111");
		//drawString({1,31}, L"01234567890123456");

		//setChar({1,32}, C_SPACE, COLOR_F_WHITE, COLOR_F_WHITE);
		//setChar({2,32}, C_LIGHT_SHADE, COLOR_F_WHITE, COLOR_F_WHITE);
		//setChar({3,32}, C_MEDIUM_SHADE, COLOR_F_WHITE, COLOR_F_WHITE);
		//setChar({4,32}, C_DARK_SHADE, COLOR_F_WHITE, COLOR_F_WHITE);
		//setChar({5,32}, C_SPACE, COLOR_F_GRAY, COLOR_F_WHITE);
		//setChar({6,32}, C_LIGHT_SHADE, COLOR_F_GRAY, COLOR_F_WHITE);
		//setChar({7,32}, C_MEDIUM_SHADE, COLOR_F_GRAY, COLOR_F_WHITE);
		//setChar({8,32}, C_DARK_SHADE, COLOR_F_GRAY, COLOR_F_WHITE);
		//setChar({9,32}, C_SPACE, COLOR_F_DARK_GRAY, COLOR_F_WHITE);
		//setChar({10,32}, C_LIGHT_SHADE, COLOR_F_DARK_GRAY, COLOR_F_WHITE);
		//setChar({11,32}, C_MEDIUM_SHADE, COLOR_F_DARK_GRAY, COLOR_F_WHITE);
		//setChar({12,32}, C_DARK_SHADE, COLOR_F_DARK_GRAY, COLOR_F_WHITE);
		//setChar({13,32}, C_SPACE, COLOR_F_BLACK, COLOR_F_WHITE);
		//setChar({14,32}, C_LIGHT_SHADE, COLOR_F_BLACK, COLOR_F_WHITE);
		//setChar({15,32}, C_MEDIUM_SHADE, COLOR_F_BLACK, COLOR_F_WHITE);
		//setChar({16,32}, C_DARK_SHADE, COLOR_F_BLACK, COLOR_F_WHITE);

		//setChar({1,33}, C_SPACE, COLOR_F_WHITE, COLOR_F_GRAY);
		//setChar({2,33}, C_LIGHT_SHADE, COLOR_F_WHITE, COLOR_F_GRAY);
		//setChar({3,33}, C_MEDIUM_SHADE , COLOR_F_WHITE, COLOR_F_GRAY);
		//setChar({4,33}, C_DARK_SHADE, COLOR_F_WHITE, COLOR_F_GRAY);
		//setChar({5,33}, C_SPACE, COLOR_F_GRAY, COLOR_F_GRAY);
		//setChar({6,33}, C_LIGHT_SHADE, COLOR_F_GRAY, COLOR_F_GRAY);
		//setChar({7,33}, C_MEDIUM_SHADE, COLOR_F_GRAY, COLOR_F_GRAY);
		//setChar({8,33}, C_DARK_SHADE, COLOR_F_GRAY, COLOR_F_GRAY);
		//setChar({9,33}, C_SPACE, COLOR_F_DARK_GRAY, COLOR_F_GRAY);
		//setChar({10,33}, C_LIGHT_SHADE, COLOR_F_DARK_GRAY, COLOR_F_GRAY);
		//setChar({11,33}, C_MEDIUM_SHADE, COLOR_F_DARK_GRAY, COLOR_F_GRAY);
		//setChar({12,33}, C_DARK_SHADE, COLOR_F_DARK_GRAY, COLOR_F_GRAY);
		//setChar({13,33}, C_SPACE, COLOR_F_BLACK, COLOR_F_GRAY);
		//setChar({14,33}, C_LIGHT_SHADE, COLOR_F_BLACK, COLOR_F_GRAY);
		//setChar({15,33}, C_MEDIUM_SHADE, COLOR_F_BLACK, COLOR_F_GRAY);
		//setChar({16,33}, C_DARK_SHADE, COLOR_F_BLACK, COLOR_F_GRAY);

		//setChar({1,34}, C_SPACE, COLOR_F_WHITE, COLOR_F_DARK_GRAY);
		//setChar({2,34}, C_LIGHT_SHADE, COLOR_F_WHITE, COLOR_F_DARK_GRAY);
		//setChar({3,34}, C_MEDIUM_SHADE, COLOR_F_WHITE, COLOR_F_DARK_GRAY);
		//setChar({4,34}, C_DARK_SHADE, COLOR_F_WHITE, COLOR_F_DARK_GRAY);
		//setChar({5,34}, C_SPACE, COLOR_F_GRAY, COLOR_F_DARK_GRAY);
		//setChar({6,34}, C_LIGHT_SHADE, COLOR_F_GRAY, COLOR_F_DARK_GRAY);
		//setChar({7,34}, C_MEDIUM_SHADE, COLOR_F_GRAY, COLOR_F_DARK_GRAY);
		//setChar({8,34}, C_DARK_SHADE, COLOR_F_GRAY, COLOR_F_DARK_GRAY);
		//setChar({9,34}, C_SPACE, COLOR_F_DARK_GRAY, COLOR_F_DARK_GRAY);
		//setChar({10,34}, C_LIGHT_SHADE, COLOR_F_DARK_GRAY, COLOR_F_DARK_GRAY);
		//setChar({11,34}, C_MEDIUM_SHADE, COLOR_F_DARK_GRAY, COLOR_F_DARK_GRAY);
		//setChar({12,34}, C_DARK_SHADE, COLOR_F_DARK_GRAY, COLOR_F_DARK_GRAY);
		//setChar({13,34}, C_SPACE, COLOR_F_BLACK, COLOR_F_DARK_GRAY);
		//setChar({14,34}, C_LIGHT_SHADE, COLOR_F_BLACK, COLOR_F_DARK_GRAY);
		//setChar({15,34}, C_MEDIUM_SHADE, COLOR_F_BLACK, COLOR_F_DARK_GRAY);
		//setChar({16,34}, C_DARK_SHADE, COLOR_F_BLACK, COLOR_F_DARK_GRAY);

		//setChar({1,35}, C_SPACE, COLOR_F_WHITE, COLOR_F_BLACK);
		//setChar({2,35}, C_LIGHT_SHADE, COLOR_F_WHITE, COLOR_F_BLACK);
		//setChar({3,35}, C_MEDIUM_SHADE, COLOR_F_WHITE, COLOR_F_BLACK);
		//setChar({4,35}, C_DARK_SHADE, COLOR_F_WHITE, COLOR_F_BLACK);
		//setChar({5,35}, C_SPACE, COLOR_F_GRAY, COLOR_F_BLACK);
		//setChar({6,35}, C_LIGHT_SHADE, COLOR_F_GRAY, COLOR_F_BLACK);
		//setChar({7,35}, C_MEDIUM_SHADE, COLOR_F_GRAY, COLOR_F_BLACK);
		//setChar({8,35}, C_DARK_SHADE, COLOR_F_GRAY, COLOR_F_BLACK);
		//setChar({9,35}, C_SPACE, COLOR_F_DARK_GRAY, COLOR_F_BLACK);
		//setChar({10,35}, C_LIGHT_SHADE, COLOR_F_DARK_GRAY, COLOR_F_BLACK);
		//setChar({11,35}, C_MEDIUM_SHADE, COLOR_F_DARK_GRAY, COLOR_F_BLACK);
		//setChar({12,35}, C_DARK_SHADE, COLOR_F_DARK_GRAY, COLOR_F_BLACK);
		//setChar({13,35}, C_SPACE, COLOR_F_BLACK, COLOR_F_BLACK);
		//setChar({14,35}, C_LIGHT_SHADE, COLOR_F_BLACK, COLOR_F_BLACK);
		//setChar({15,35}, C_MEDIUM_SHADE, COLOR_F_BLACK, COLOR_F_BLACK);
		//setChar({16,35}, C_DARK_SHADE, COLOR_F_BLACK, COLOR_F_BLACK);

		if (m_enableStats) {
			swprintf_s(str, 64, L"Time (s): %4.2f\nFrame: %6d, x:%3.2f, y:%3.2f", m_sumDeltaTime, m_frameCount, outX, outY);
			drawStringAlpha({2,0}, str, COLOR_F_GRAY);
			swprintf_s(str, 64, L"(%03d,%03d); Pressed(%03d,%03d); Released(%03d,%03d)",
				getMousePosition().getX(), getMousePosition().getY(),
				m_btnPressLocation.getX(), m_btnPressLocation.getY(),
				m_btnReleaseLocation.getX(), m_btnReleaseLocation.getY());
			drawStringAlpha({2,2}, str, COLOR_F_GRAY);
			drawStringAlpha({2,4}, m_lastFrameStats, COLOR_F_GRAY);

			setPixel(getMousePosition(), COLOR_F_GRAY);
		}

		if(m_tamponSet)
			setChar(getMousePosition(), m_tampon);

	}
	catch (const char* s) {
#ifdef _DEBUG
		std::cerr << s << std::endl;
#endif
	}

	return true;
}

void TestUser::TestEngine::onDestroy() {
	if(m_picture != nullptr)
		delete[] m_picture;
	m_pictureSize ={0,0};
}


int main(char argc, char** argv) {
	TestUser::TestEngine engine;
	try {
		engine.externalStart();
	}
	catch (const char* s) {
		std::cerr << s << std::endl;
	}
	std::cout << std::endl;
	return 0;
}
