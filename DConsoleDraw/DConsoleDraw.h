#pragma once

#include "DConsoleEngine.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <random>

class DConsoleDraw : public Draggoon::DConsoleEngine {
public:
	DConsoleDraw();
	~DConsoleDraw();

	virtual bool onScreenUpdate(const std::chrono::duration<float>& elapsedTime) override;

	bool onCreate();

	bool onInitialized();

	bool onInputUpdate(const Draggoon::Key * keys, const size_t & keyCount, const Draggoon::Key * mouseBtn, const size_t & mouseBtnCount);

	void setDrawSize(Draggoon::Vector2D<int> newSize);

	void loadFromFile();
	void saveTofile();

protected:
	std::wstring m_filename;

	Draggoon::Vector2D<int> m_drawSize;

	static const int COLOR_COUNT = 16;
	Draggoon::Color<float> m_colors[COLOR_COUNT];
	int m_selectedColor;

	char* m_drawBuffer;
	bool m_drawAreaChanged;

	std::default_random_engine m_randomGenerator;
};

