#ifndef WELL_INCLUDED
#define WELL_INCLUDED
#include <vector>
#include <iostream>
#include "UserInterface.h"

class Screen;

class Well {
public:
	Well(int well_size_x, int well_size_y);

	/*A copy constructor is used to initialize a previously uninitialized 
	object from some other object's data.*/
	Well(const Well& old_well);

	/*An assignment operator is used to replace the data of a previously 
	initialized object with some other object's data.*/
	Well& operator=(const Well& old_well);

	~Well();
	void display(Screen& screen, int x, int y);
	char get_well(const int x, const int y) const;
	bool set_well(const char c, const int x, const int y);
	int get_sizeX() const;
	int get_sizeY() const;
private:
	int m_well_size_x;
	int m_well_size_y;
	std::vector<std::vector<char> > m_well;
	std::vector<std::vector<char> > m_well_bounding_box;
};

#endif // WELL_INCLUDED
