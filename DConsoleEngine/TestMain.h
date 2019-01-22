#pragma once

#include "DConsoleEngine.h"


class TestEngine : public Draggoon::DConsoleEngine {
protected:
	bool onCreate();
	bool onInitialized();
	bool onInputUpdate(const Draggoon::Key* keys, const size_t& keyCount, const Draggoon::Key* mouseBtn, const size_t& mouseBtnCount);
	bool onScreenUpdate(const std::chrono::duration<float> &elapsedTimeSeconds);
	void onDestroy();
	float m_sumDeltaTime;
	int m_frameCount;
	float m_count;
	bool m_turnCCW;
	bool m_turnCW;
	bool m_turnFast;

	bool m_flag[5];
};

