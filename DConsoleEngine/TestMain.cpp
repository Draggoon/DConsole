/*
*	Test fram for DConsoleEngine
*
*/

#include "TestMain.h"

using namespace std::chrono_literals;

using namespace Draggoon;

bool TestEngine::onCreate() {
	m_appName = L"Test";
	m_framerate = 0;
	m_stretchOnResize = true;
	setSize({60,60});
	setPixelSize({15,15});

	m_clearScreenBeforeUpdate = false;

	m_sumDeltaTime = 0.0f;
	m_count = 0.0f;
	m_frameCount = 0;
	m_turnCCW = false;
	m_turnCW = false;
	m_turnFast = false;


	return true;
}

bool TestEngine::onInitialized() {
	clearScreen();
	return true;
}

bool TestEngine::onInputUpdate(const Draggoon::Key * keys, const size_t & keyCount, const Draggoon::Key * mouseBtn, const size_t & mouseBtnCount) {

	if (keys[VK_ESCAPE].isPressed()) {
		return false;
	}

	m_turnCCW = keys['A'].isDown();

	m_turnCW = keys['D'].isDown();

	m_turnFast = keys[VK_SHIFT].isDown();

	//if (keys[VK_SPACE].isPressed())
	//	clearScreen();

	for (int i(0); i<5; ++i) {
		m_flag[i] = mouseBtn[i].isDown();
	}

	return true;
}


bool TestEngine::onScreenUpdate(const std::chrono::duration<float> &elapsedTimeSeconds) {
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
		//*
		for (int j(0); j<size.getY(); ++j) {
			for (int i(0); i< size.getX(); ++i) {
				if (i%2 == 0 && j%2 == 0 || i%2 != 0 && j%2 != 0)
					setPixel({i,j}, ' ', F_WHITE, F_GRAY);
				else
					setPixel({i,j}, ' ', F_BLACK, F_WHITE);
			}
		}
		//*/
		int centerX(size.getX()/2);
		int centerY(size.getY()/2);
		float outX(centerX+15*cos(6.28f*20*m_count/100.0f));
		float outY(centerY+15*sin(6.28f*20*m_count/100.0f));
		drawLine({centerX,centerY}, {(int)outX,(int)outY}, C_FULL_BLOCK, F_RED);
		drawLine({1,10}, {1,10+10}, C_SPACE, F_WHITE, F_BLUE);

		swprintf_s(str, 64, L"Time (s): %4.2f, Frame: %6d, x:%3.2f, y:%3.2f", m_sumDeltaTime, m_frameCount, outX, outY);
		drawStringAlpha({0,0}, str, Draggoon::F_BLACK);
		swprintf_s(str, 64, L"%3d, %3d", getMousePosition().getX(), getMousePosition().getY());
		drawStringAlpha({0,2}, str, Draggoon::F_BLACK);

		for (int i(0); i<5; ++i) {
			if (m_flag[i])
				setPixel({i,3}, C_SPACE, F_WHITE, F_GREEN);
		}
	}
	catch (const char* s) {
#ifdef _DEBUG
		std::cerr << s << std::endl;
#endif
	}

	return true;
}

void TestEngine::onDestroy() {

}


int main(char argc, char** argv) {
	TestEngine engine;
	try {
		engine.externalStart();
	}
	catch (const char* s) {
		std::cerr << s << std::endl;
	}
	std::cout << std::endl;
	return 0;
}

