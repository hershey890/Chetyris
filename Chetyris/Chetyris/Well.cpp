#include "Well.h"
#include "UserInterface.h"
#include "Game.h" //used to get the well size

Well::Well() : m_well_size_x(0), m_well_size_y(0) {}

Well::Well(int well_size_x, int well_size_y) : m_well_size_x(well_size_x), m_well_size_y(well_size_y), m_well(m_well_size_x + 2, std::vector<char>(m_well_size_y + 2)) {
	for (int i = 0; i < m_well_size_x + 2; i++) {
		for (int j = 0; j < m_well_size_y + 2; j++) {
			/*access elemts with m_well[y*sizeX + x]*/
			m_well[i][j] = ' ';
		}
	}
}

Well::~Well() {}

void Well::display(Screen& screen, int x, int y)
{
	screen.clear();

	/*print the leftmost vertical column*/
	for (int i = 0; i < m_well_size_y; i++) {
		screen.gotoXY(0, i);
		screen.printChar('@');
		m_well[0][i] = '@';
	}
	for (int i = 0; i < m_well_size_x + 2; i++) {
		screen.gotoXY(i, m_well_size_y);
		screen.printChar('@');
		m_well[i][m_well_size_y + 1] = '@';
	}
	for (int i = 0; i < m_well_size_y; i++) {
		screen.gotoXY(m_well_size_x + 1, i);
		screen.printChar('@');
		m_well[m_well_size_x + 1][i] = '@';
	}
}

char Well::get_well(const int x, const int y) {
	return m_well[x][y];
}

bool Well::set_well(const char c, const int x, const int y) {
	if ((x > 0 && x <= m_well_size_x) && (y > 0 && y <= m_well_size_y)) {
		m_well[x][y] = c;
		return true;
	}
	return false;
}

int Well::get_sizeX() const{
	return m_well_size_x;
}

int Well::get_sizeY() const {
	return m_well_size_y;
}

