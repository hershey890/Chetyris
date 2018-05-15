#include "Well.h"
#include "UserInterface.h"
#include "Game.h" //used to get the well size

Well::Well(): m_well_size_x(0), m_well_size_y(0) {}
Well::Well(int well_size_x, int well_size_y) : m_well_size_x(well_size_x), m_well_size_y(well_size_y) {}

void Well::display(Screen& screen, int x, int y)
{
	screen.clear();

	/*print the leftmost vertical column*/
	//the exact number of rows or columns is prob off by one
	for (int i = 0; i < m_well_size_y; i++) {
		screen.gotoXY(0, i);
		screen.printChar('@');
	}
	for (int i = 0; i < m_well_size_x + 2; i++) {
		screen.gotoXY(i, m_well_size_y);
		screen.printChar('@');
	}
	for (int i = 0; i < m_well_size_y; i++) {
		screen.gotoXY(m_well_size_x + 1, i);
		screen.printChar('@');
	}
}