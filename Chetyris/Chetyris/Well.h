#ifndef WELL_INCLUDED
#define WELL_INCLUDED
#include <vector>
#include <iostream>
#include "UserInterface.h"

class Screen;

class Well {
public:
	Well(int well_size_x, int well_size_y);
	~Well();
	void	display(Screen& screen, int x, int y);
	bool	get_well(char& ch, const int& x, const int& y) const;
	bool	set_well(const char c, const int x, const int y);
	int		get_sizeX() const;
	int		get_sizeY() const;
	void	reset();
private:
	int		m_well_size_x;
	int		m_well_size_y;
	std::vector<std::vector<char> > m_well;
};

#endif // WELL_INCLUDED
