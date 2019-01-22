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
	setSize({60,40});
	setPixelSize({15,15});

	m_clearScreenBeforeUpdate = true;

	m_sumDeltaTime = 0.0f;
	m_count = 0.0f;
	m_frameCount = 0;
	m_turnCCW = false;
	m_turnCW = false;
	m_turnFast = false;

	m_mouseMoved = false;


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
		m_btnReleaseLocation = getMousePosition();
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
		//drawLineChar({1,10}, {1,10+10}, C_SPACE, COLOR_F_WHITE, COLOR_F_BLUE);

		drawLineChar({12,30}, {18,28}, L'@', COLOR_F_RED, COLOR_F_BLUE);
		//drawLineChar({12,31}, {16,31}, C_SPACE, COLOR_F_RED, COLOR_F_RED);

		drawRect({centerX,centerY}, {(int)outX,(int)outY}, COLOR_F_RED);
		drawLine({8,8}, {8,8});

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


		if (m_btnDown) {
			if (m_btnPressLocation != getMousePosition() || m_mouseMoved) {
				if(m_mouseMoved)
					fillRect(m_btnPressLocation, getMousePosition());
				m_mouseMoved = true;
			}
		}
		else
			m_mouseMoved = false;
	}
	catch (const char* s) {
#ifdef _DEBUG
		std::cerr << s << std::endl;
#endif
	}

	return true;
}

void TestUser::TestEngine::onDestroy() {

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
