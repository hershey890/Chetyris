#ifndef WELL_INCLUDED
#define WELL_INCLUDED

class Screen;

class Well {
public:
	Well();
	Well(int well_size_x, int well_size_y);
	void display(Screen& screen, int x, int y);
private:
	int m_well_size_x;
	int m_well_size_y;
};

#endif // WELL_INCLUDED
