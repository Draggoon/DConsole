#pragma once

#include "DConsoleEngine.h"


class TestEngine : public Draggoon::DConsoleEngine {
	bool onCreate();
	bool onUpdate(const std::chrono::duration<float> &elapsedTimeSeconds);
	void onDestroy();
};

