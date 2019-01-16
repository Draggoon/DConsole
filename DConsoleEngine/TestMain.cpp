/*
*	Test fram for DConsoleEngine
*
*/

#include "TestMain.h"

using namespace std::chrono_literals;

bool TestEngine::onCreate() {
	m_appName = L"Test engine";
	m_framerate = 60;


	return true;
}

bool TestEngine::onUpdate(const std::chrono::duration<float> &elapsedTimeSeconds) {
	//std::cout << "elapsed: " << elapsedTimeSeconds.count() << std::endl;
	Draggoon::Vector2D<int> size = getSize();
	for (int j(0); j<size.getY(); ++j) {
		for (int i(0); i< size.getX(); ++i) {
			if(i%2 == 0 && j%2 == 0 || i%2 != 0 && j%2 != 0)
				setPixel({i,j},Draggoon::C_FULL_BLOCK, Draggoon::F_WHITE);
			else if(i%2 != 0 && j%2 == 0 || i%2 == 0 && j%2 != 0)
				setPixel({i,j}, Draggoon::C_FULL_BLOCK, Draggoon::F_BLACK);
		}	
	}
	drawString({1,1}, "hello", Draggoon::F_GRAY);
	return true;
}
void TestEngine::onDestroy() {

}


int main(char argc, char** argv) {
	TestEngine engine;
	try {
		engine.start();
	}
	catch (const char* s) {
		std::cerr << s << std::endl;
	}

	return 0;
}
