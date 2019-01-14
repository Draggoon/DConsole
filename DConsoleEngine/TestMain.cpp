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
	for (int j(0); j<m_bufferHeight; ++j) {
		for (int i(0); i< m_bufferWidth; ++i) {
			m_screenBuffer[j*m_bufferHeight + i].Char.UnicodeChar = '0'+(j*m_bufferHeight+i)%10;
			m_screenBuffer[j*m_bufferHeight + i].Attributes = 0x0007;
		}	
	}
	return true;
}
void TestEngine::onDestroy() {

}


int main(char argc, char** argv) {
	TestEngine engine;

	engine.start();

	return 0;
}
