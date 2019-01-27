#include "DCellularAutomata.h"



DCellularAutomata::DCellularAutomata() : m_arraySize(100,80), m_turnLength(1.0f), m_sumOfElapsedTime(0.0f), m_runAutomata(false), m_randomGenerator((int)std::chrono::high_resolution_clock::now().time_since_epoch().count()){
	m_cell = new std::pair<bool, int>[m_arraySize.getArea()];
}


DCellularAutomata::~DCellularAutomata() {
}

bool DCellularAutomata::onCreate() {
	m_appName = L"Cellular Automata";
	m_clearScreenBeforeUpdate = false;
	m_framerate = 144;
	m_stretchOnResize = true;

	setSize(m_arraySize);
	setPixelSize({12,12});

	return true;
}


bool DCellularAutomata::onInputUpdate(const Draggoon::Key* keys, const size_t& keyCount, const Draggoon::Key* mouseBtn, const size_t& mouseBtnCount) {
	if (keys[VK_ESCAPE].isPressed()) {
		return false;	// Stop the thread loop
	}

	if (keys[VK_SPACE].isPressed()) {
		m_runAutomata = !m_runAutomata;
	}

	if (keys[VK_ADD].isPressed()) {
		m_turnLength /= 2.0f;
	}

	if (keys[VK_SUBTRACT].isPressed()) {
		m_turnLength *= 2.0f;
	}

	if (keys[VK_DELETE].isPressed()) {
		for (int y(0); y<m_arraySize.getY(); ++y) {
			for (int x(0); x<m_arraySize.getX(); ++x) {
				killCell({x,y});
			}
		}
	}


	if (mouseBtn[0].isDown()) {
		if (m_mouseLastPos != getMousePosition()) {
			toggleCell(getMousePosition());
			m_mouseLastPos = getMousePosition();
		}
	}
	if (mouseBtn[0].isReleased()) {
		m_mouseLastPos = {-1,-1};
	}

	if (mouseBtn[1].isDown()) {
		killCell(getMousePosition());
	}


	if (keys['R'].isPressed()) {
		std::uniform_int_distribution<int> dist(0, 1);
		for (int y(0); y<m_arraySize.getY(); ++y)
			for (int x(0); x < m_arraySize.getX(); ++x)
				if (dist(m_randomGenerator)!=0)
					generateCell({x,y});
				else
					killCell({x,y});
	}



	return true;	// Continue
};

bool DCellularAutomata::onScreenUpdate(const std::chrono::duration<float>& elapsedTime) {
	m_sumOfElapsedTime += elapsedTime.count();
	if (m_sumOfElapsedTime > m_turnLength && m_runAutomata) {
		m_sumOfElapsedTime = 0;
		bool* newCell = new bool[m_arraySize.getArea()];
		for (int y(0); y < m_arraySize.getY(); ++y) {
			for (int x(0); x<m_arraySize.getX(); ++x) {
				int neighbours(0);
				for(int iy(-1); iy<=1; ++iy)
					for(int ix(-1); ix<=1; ++ix)
						if(ix!=0 || iy!=0)
							neighbours+=isAlive({x+ix,y+iy})?1:0;

				if (neighbours < 2 || neighbours > 3)
					newCell[y*m_arraySize.getX() + x] = false;
				else if (neighbours == 3)
					newCell[y*m_arraySize.getX() + x] = true;
				else
					newCell[y*m_arraySize.getX() + x] = m_cell[y*m_arraySize.getX() + x].first;
			}
		}

		for (int y(0); y < m_arraySize.getY(); ++y) {
			for (int x(0); x<m_arraySize.getX(); ++x) {
				ageCell({x,y});
				if (newCell[y*m_arraySize.getX() + x] != isAlive({x,y}))
					toggleCell({x,y});
			}
		}
		for (int i(0); i<m_arraySize.getArea(); ++i) {
		}
		delete[] newCell;
	}

	for (int y(0); y < m_arraySize.getY(); ++y) {
		for (int x(0); x < m_arraySize.getX(); ++x) {
			if (isAlive({x,y}))
				setPixel({x,y}, Draggoon::COLOR_F_WHITE);
			else
				setPixel({x,y}, Draggoon::COLOR_F_BLACK);
		}
	}


	return true;
}

bool DCellularAutomata::onInitialized() {
	clearScreen();
	return true;
}

void DCellularAutomata::toggleCell(Draggoon::Vector2D<int> pos) {
	std::pair<bool, int>& p(m_cell[pos.getY()*m_arraySize.getX() + pos.getX()]);
	p.first = !p.first;
	p.second = 0;
}

void DCellularAutomata::killCell(Draggoon::Vector2D<int> pos) {
	std::pair<bool, int>& p(m_cell[pos.getY()*m_arraySize.getX() + pos.getX()]);
	p.first = false;
	p.second = 0;
}

void DCellularAutomata::generateCell(Draggoon::Vector2D<int> pos) {
	std::pair<bool, int>& p(m_cell[pos.getY()*m_arraySize.getX() + pos.getX()]);
	p.first = true;
	p.second = 0;
}

void DCellularAutomata::ageCell(Draggoon::Vector2D<int> pos) {
	++m_cell[pos.getY()*m_arraySize.getX() + pos.getX()].second;
}

bool DCellularAutomata::isAlive(Draggoon::Vector2D<int> pos) {
	return getCell(pos).first;
}

std::pair<bool, int> DCellularAutomata::getCell(Draggoon::Vector2D<int> pos) {
	if (pos.getX()>=0 && pos.getX()<m_arraySize.getX() && pos.getY()>=0 && pos.getY()<m_arraySize.getY())
		return m_cell[pos.getY()*m_arraySize.getX() + pos.getX()];
	else
		return std::pair<bool, int>(false, 0);	// Outside of array is dead
}



int main(int argc, char** argv) {
	DCellularAutomata d;

	try {
		d.externalStart();
	}
	catch (const char* s) {
		std::cerr << s << std::endl;
	}

	return 0;
}