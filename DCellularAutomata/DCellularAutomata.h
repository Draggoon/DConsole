#pragma once

#include <DConsoleEngine.h>
#include <random>

class DCellularAutomata : public Draggoon::DConsoleEngine {
public:
	DCellularAutomata();
	~DCellularAutomata();

	virtual bool onCreate() override;
	virtual bool onInputUpdate(const Draggoon::Key* keys, const size_t& keyCount, const Draggoon::Key* mouseBtn, const size_t& mouseBtnCount) override;
	virtual bool onScreenUpdate(const std::chrono::duration<float>& elapsedTime) override;
	virtual bool onInitialized() override;

	void toggleCell(Draggoon::Vector2D<int> pos);
	void killCell(Draggoon::Vector2D<int> pos);
	void generateCell(Draggoon::Vector2D<int> pos);
	void ageCell(Draggoon::Vector2D<int> pos);

	bool isAlive(Draggoon::Vector2D<int> pos);
	std::pair<bool, int> getCell(Draggoon::Vector2D<int> pos);

protected:

	std::pair<bool, int>* m_cell;
	Draggoon::Vector2D<int> m_arraySize;

	Draggoon::Vector2D<int> m_mouseLastPos;

	float m_turnLength;
	float m_sumOfElapsedTime;
	bool m_runAutomata;
	std::default_random_engine m_randomGenerator;

};

