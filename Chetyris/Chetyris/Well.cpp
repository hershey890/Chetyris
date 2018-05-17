#include "Well.h"
#include "UserInterface.h"
#include "Game.h" //used to get the well size

//Well::Well() : m_well_size_x(0), m_well_size_y(0) {}

Well::Well(int well_size_x, int well_size_y)
: m_well_size_x(well_size_x), m_well_size_y(well_size_y), m_well(m_well_size_x, std::vector<char>(m_well_size_y)),
  m_well_bounding_box(m_well_size_x, std::vector<char>(m_well_size_y))
{
	for (int i = 0; i < m_well_size_x; i++) {
		for (int j = 0; j < m_well_size_y; j++) {
			m_well[i][j] = '_'; //'_' is the default blank value of the well
		}
	}

	for (int i = 0; i < m_well_size_x; i++) {
		for (int j = 0; j < m_well_size_y; j++) {
			m_well_bounding_box[i][j] = '_'; //'_' is the default blank value of the well
		}
	}
}

Well::Well(const Well& old_well) {
	this->m_well_size_x = old_well.m_well_size_x;
	this->m_well_size_y = old_well.m_well_size_y;
	for (int i = 0; i < m_well_size_x; i++) {
		for (int j = 0; j < m_well_size_y; j++) {
			this->m_well[i][j] = old_well.m_well[i][j];
		}
	}
}

Well& Well::operator=(const Well& old_well) {
	this->m_well_size_x = old_well.m_well_size_x;
	this->m_well_size_y = old_well.m_well_size_y;
	for (int i = 0; i < m_well_size_x; i++) {
		for (int j = 0; j < m_well_size_y; j++) {
			this->m_well[i][j] = old_well.m_well[i][j];
		}
	}
	return *this;
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
	for (int i = 0; i < m_well_size_x; i++) {
		screen.gotoXY(i, m_well_size_y - 1);
		screen.printChar('@');
		m_well[i][m_well_size_y - 1] = '@';
	}
	for (int i = 0; i < m_well_size_y; i++) {
		screen.gotoXY(m_well_size_x - 1, i);
		screen.printChar('@');
		m_well[m_well_size_x - 1][i] = '@';
	}
}

char Well::get_well(const int x, const int y) const {
	return m_well[x][y];
}

bool Well::set_well(const char c, const int x, const int y) {
	if ((x > 0 && x < m_well_size_x - 1) && (y >= 0 && y < m_well_size_y - 1)) {
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


/*
0
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16					  x
17					  x
18 1 2 3 4 5 7 8 9 10 11 
 0
*/